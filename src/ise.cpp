/*
 * Copyright 2012  Samsung Electronics Co., Ltd
 *
 * Licensed under the Flora License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.tizenopensource.org/license
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#define Uses_SCIM_UTILITY
#define Uses_SCIM_OBJECT
#define Uses_SCIM_POINTER
#define Uses_SCIM_EVENT
#define Uses_SCIM_HELPER
#define Uses_SCIM_CONFIG_BASE

#define USE_EFL

/* use EFL*/
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <scim.h>

#include <Ecore_X.h>
#include <Ecore_IMF.h>
#include <Elementary.h>
#include <mcf.h>

#include <X11/XF86keysym.h>

#include <vconf.h>
#include <vconf-keys.h>

#include "ise.h"
#include "isedata.h"
#include "ise-default-setting.h"
#include "languagesetting.h"
#include "include/strtbl.h"

#include "mcfcore.h"
#include "mcfresource.h"
#include "include/window_slide_animator.h"

using namespace scim;
using namespace mcf;

#ifndef SETUP_BUILD
SETTING_INFO _setup_info;
std::vector < int >v_lang_list;
#endif

Evas_Object *box = NULL;

ConfigPointer _scim_config;

Evas_Object *main_window = NULL;
HelperAgent helper_agent;

CMCFCore *gCore = NULL;

static mcf8 gFKeypadMode = INITIAL_KEYPAD_MODE;
static mcf8 gInitialInputMode = NOT_USED;
static mcf32 gPrevInputContext = -1;
static bool gFLayoutChanged = FALSE;
static bool gFPrivateKeySet = FALSE;
static mcfu32 gIseLayout;
static bool gFLayoutSetting = FALSE;
static mcfu32 gLastIseLayout = -1;
static mcfulong prevKeyEvent = NOT_USED;
int gIseScreenDegree = 0;
static int gIseScreenDegreeBackup;
static Ecore_Timer *release_block_timer_id;
int gExplicitLanguageSetting = NOT_USED;
static ISEPrivateKeyBuffer gPrivateKeyBuffer[ISE_PRIVATE_KEY_BUFFER_SIZE];
int gDisableKeyBuffer[MAX_KEY];
static ISELastInputContext gLatestInputContext;
static mcfboolean gFInputContextSet = FALSE;
static bool gFDisplaySetting = FALSE;
bool gFHiddenState = TRUE;
bool gFEffectEnabled = TRUE;
static int prevInputMode = NOT_USED;
static int prevDispMode = MCFDISPLAY_MAX;
static bool prevPrivkeySet = FALSE;
int gExternalShiftLockMode = FALSE;
MCFKeyModifier gPrevModifier = KEY_MODIFIER_NONE;
mcfchar *gPrevMultitapValue = NULL;
Ecore_Timer *release_block_timer_id_for_shift = NULL;
mcfint privateId = FALSE;

bool email_app_check = FALSE;
Evas_Object* ctxspopup = NULL;

#ifdef HAVE_CONFORMANT_AUTOSCROLL
Virtual_Keyboard_State g_virt_keybd_state = KEYPAD_STATE_UNKNOWN;
#endif

static int create_main_window();

int g_currentLanguage = 0;
int g_cursor_position = 0;

extern bool IseLangDataSelectState[MAX_LANG_NUM];

/**
*  * This callback class will receive all response events from MCF
*   * So you should perform desired tasks in this class.
*    */
class CEventCallback:public IMCFEventCallback {
 public:mcfboolean on_event_key_clicked(McfKeyEventDesc keyEventDesc);
	mcfboolean on_event_drag_state_changed(McfKeyEventDesc keyEventDesc) {
		return FALSE;
	}
	mcfboolean on_event_notification(MCFNotiType notiType, mcfint etcInfo);
};

CKesslerISEDecorator decorator;

static CEventCallback callback;

static void _on_input_mode_changed(mcfchar * keyValue, mcfulong keyEvent, MCFKeyType keyType);

static void _on_process_user_defined_keys(mcfchar * keyValue, mcfulong keyEvent, MCFKeyType keyType);

static void ise_send_string(mcfchar * keyValue);
static void ise_update_preedit_string(mcfchar * str);
static void ise_send_event(mcfulong keyEvent, mcfulong keyMask = 0);
static void ise_forward_key_event(mcfulong keyEvent);

static void _set_prediction_private_key();
static void _set_shift_private_key();

static void ise_set_screen_rotation(int degree);

static void write_config(void)
{
	_scim_config->write(CONFIG_SETTING_FLAG, _setup_info.flag);
	_scim_config->write(CONFIG_LANGUAGE_LIST, v_lang_list);
	_scim_config->write(CONFIG_CURRENT_LANGUAGE, _setup_info.current_language);
	_scim_config->write(CONFIG_LANGUAGE_COUNT, _setup_info.lang_count);
	_scim_config->flush();
}

static void setting_window_cb(SETTING_INFO info)
{
#ifdef SUPPORTS_MULTIPLE_LANGUAGE_SELECTION
	int current_index = g_currentLanguage;
	int count = 0;
	while (!IseLangDataSelectState[current_index] && count < MAX_LANG_NUM) {
		current_index++;
		if (current_index >= MAX_LANG_NUM) {
			current_index = 0;
		}
		count++;
	}

	if (current_index != g_currentLanguage) {
		ise_set_language(current_index);
	}
#endif

	_setup_info.current_language = g_currentLanguage;

	write_config();

}

#ifdef HAVE_CONFORMANT_AUTOSCROLL
static void _ise_get_size(struct rectinfo &info)
{
	int x, y, width, height;

	info.pos_x = 0;
	info.pos_y = 0;
	info.width = 0;
	info.height = 0;
	if (main_window) {
		ecore_evas_geometry_get(ecore_evas_ecore_evas_get
					(evas_object_evas_get(main_window)), &x,
					&y, NULL, NULL);
		gCore->get_window_size(&width, &height);

		mcfint win_w, win_h;
		if (gCore) {
			gCore->get_screen_resolution(&win_w, &win_h);
		}

		if (gIseScreenDegree == 90 || gIseScreenDegree == 270) {
			Evas_Coord temp = win_w;
			win_w = win_h;
			win_h = temp;
		}

		info.pos_x = (win_w - width) / 2;

		if (gFHiddenState) {
			info.pos_y = win_h;
		} else {
			info.pos_y = win_h - (height);
		}

		info.width = width;
		info.height = (height);
	}
}

void _send_keypad_geom_atom_info(Evas_Object * window, Virtual_Keyboard_State kbd_state)
{
	Ecore_X_Window zone, xwin;
	struct rectinfo info;

	if ((kbd_state == KEYPAD_STATE_UNKNOWN) || (kbd_state == g_virt_keybd_state))
		return;

	g_virt_keybd_state = kbd_state;
	xwin = elm_win_xwindow_get(window);
	zone = ecore_x_e_illume_zone_get(xwin);
	memset(&info, 0, sizeof(struct rectinfo));

	_ise_get_size(info);
	if (kbd_state == KEYPAD_STATE_ON) {
		ecore_x_e_virtual_keyboard_state_set(zone,
							ECORE_X_VIRTUAL_KEYBOARD_STATE_ON);
		ecore_x_e_illume_keyboard_geometry_set(zone, info.pos_x,
							info.pos_y, info.width,
							info.height);
	} else {
		ecore_x_e_virtual_keyboard_state_set(zone,
							ECORE_X_VIRTUAL_KEYBOARD_STATE_OFF);
		ecore_x_e_illume_keyboard_geometry_set(zone, info.pos_x,
							info.pos_y, 0, 0);
	}
}
#endif

static Eina_Bool release_block_timer_event(void *data)
{
	send_flush();
	return ECORE_CALLBACK_CANCEL;
}

/* For adding delay in Abc Mode */
static Eina_Bool release_block_timer_event_for_shift(void *data)
{
	gCore->set_shift_state(MCF_SHIFT_STATE_OFF);
	change_shiftmode(SHIFTMODE_OFF);
	prevKeyEvent = NOT_USED;
	return 0;
}

mcfboolean CEventCallback::on_event_notification(MCFNotiType notiType,
						 mcfint etcInfo)
{
	if (notiType == NOTITYPE_GESTURE_FLICK) {
		if (etcInfo == DRAG_DOWN) {
			{
				if (gCore->get_debug_mode() != DEBUGMODE_AUTOTEST) {
					ise_hide(TRUE);
				}
			}
		}
	}
}

mcfboolean CEventCallback::on_event_key_clicked(McfKeyEventDesc keyEventDesc)
{
#ifdef DEBUG_MODE
	static bool autotest = FALSE;
	static mcfulong keySeq[3];

	keySeq[0] = keySeq[1];
	keySeq[1] = keySeq[2];
	keySeq[2] = keyEventDesc.keyEvent;

	if (autotest) {
		if (gCore->get_debug_mode() != DEBUGMODE_AUTOTEST) {
			autotest = FALSE;
		}
	} else if (keySeq[0] == MVK_D && keySeq[1] == MVK_B && keySeq[2] == MVK_G) {
		gCore->set_debug_mode(DEBUGMODE_AUTOTEST);
		autotest = TRUE;
	}
#endif

	if (keyEventDesc.keyModifier == KEY_MODIFIER_MULTITAP_START) {
		if (gPrevMultitapValue)
			ise_send_string(gPrevMultitapValue);

		ise_update_preedit_string(keyEventDesc.keyValue);
		gPrevMultitapValue = keyEventDesc.keyValue;
	} else if (keyEventDesc.keyModifier == KEY_MODIFIER_MULTITAP_REPEAT) {
		ise_update_preedit_string(keyEventDesc.keyValue);
		gPrevMultitapValue = keyEventDesc.keyValue;
	} else if (gPrevModifier == KEY_MODIFIER_MULTITAP_START
			|| gPrevModifier == KEY_MODIFIER_MULTITAP_REPEAT) {
		if (gPrevMultitapValue)
			ise_send_string(gPrevMultitapValue);

		ise_update_preedit_string("");
		gPrevMultitapValue = NULL;
	}
	gPrevModifier = keyEventDesc.keyModifier;

	if (keyEventDesc.keyEvent == MVK_space) {
		if (keyEventDesc.keyModifier == KEY_MODIFIER_DIRECTION_LEFT ||
						keyEventDesc.keyModifier == KEY_MODIFIER_DIRECTION_LEFT_RETURN) {
			keyEventDesc.keyEvent = MVK_KP_Left;
		}

		if (keyEventDesc.keyModifier == KEY_MODIFIER_DIRECTION_RIGHT ||
						keyEventDesc.keyModifier == KEY_MODIFIER_DIRECTION_RIGHT_RETURN) {
			keyEventDesc.keyEvent = MVK_KP_Right;
		}
	}

	static mcfchar *prevLongKeySym = 0;
	static mcfint prevLongKeyCode = 0;

	DBG("\non_event_key_clicked : %d\n", keyEventDesc.keyModifier);

	if (keyEventDesc.keyModifier == KEY_MODIFIER_LONGKEY) {
		if (keyEventDesc.keyType == KEY_TYPE_CHAR) {
			keyEventDesc.keyType = KEY_TYPE_UNICODE;
		}
	}

	switch (keyEventDesc.keyType) {
	case KEY_TYPE_UNICODE:
	case KEY_TYPE_SYMBOL:
		if ((gLatestInputContext.keyType == KEY_TYPE_CHAR)
		    || (keyEventDesc.keyEvent == MVK_at)
		    || (keyEventDesc.keyEvent == MVK_slash)) {
			/* It will request flush processing to  ise if the user type
			 * UNICODE after typing CHAR(Composing style) */
			DBG("\n\n Last input context is key type char \n\n");
			send_flush();
		}

        if (keyEventDesc.keyEvent == MVK_period) {
          if (keyEventDesc.keyModifier == KEY_MODIFIER_DIRECTION_LEFT) {
            ise_send_string(",");
          } else if (keyEventDesc.keyModifier == KEY_MODIFIER_DIRECTION_RIGHT) {
            ise_send_string("!");
          } else if (keyEventDesc.keyModifier == KEY_MODIFIER_DIRECTION_UP) {
            ise_send_string("?");
          } else if (keyEventDesc.keyModifier == KEY_MODIFIER_LONGKEY){
            send_flush();
          } else {
            ise_send_string(".");
	      }
        }
        else if (keyEventDesc.keyModifier != KEY_MODIFIER_MULTITAP_START &&
		    keyEventDesc.keyModifier != KEY_MODIFIER_MULTITAP_REPEAT) {
			if (keyEventDesc.keyEvent) {
				ise_forward_key_event(keyEventDesc.keyEvent);
			} else {
				ise_send_string(keyEventDesc.keyValue);
			}
		}
		break;
	case KEY_TYPE_CHAR:
	case KEY_TYPE_CONTROL:
		if (keyEventDesc.keyEvent > UD_MVK_START
		    && keyEventDesc.keyEvent < UD_MVK_END) {
			_on_process_user_defined_keys(keyEventDesc.keyValue,
										keyEventDesc.keyEvent,
										keyEventDesc.keyType);
		} else {
			if(keyEventDesc.keyType == KEY_TYPE_CHAR) {
				ise_send_string(keyEventDesc.keyValue);
			}
			else {
				ise_send_event(keyEventDesc.keyEvent);
			}
		}
		break;
	case KEY_TYPE_MODECHANGE:
		if ((keyEventDesc.keyModifier == KEY_MODIFIER_LONGKEY) &&
				(keyEventDesc.keyEvent != UD_MVK_KEYPAD)) {
			/* Take care of menu for longpress for QTY keyboard only */
			_on_process_user_defined_keys(keyEventDesc.keyValue,
										keyEventDesc.keyEvent,
										keyEventDesc.keyType);
		} else {	/* Take care of change-language in short press */
			_on_input_mode_changed(keyEventDesc.keyValue,
										keyEventDesc.keyEvent,
										keyEventDesc.keyType);
		}
		break;
	default:
		break;
	}

	if (keyEventDesc.keyEvent != UD_MVK_SHIFT) {
		if (get_Ise_default_context().KeypadMode == KEYPAD_QWERTY) {
			if (get_Ise_default_context().ShiftMode == SHIFTMODE_ON) {
				/* Even when a key was pressed, do not turn off
				 * shift mode if external CAPS_MODE is set */
				if (!gExternalShiftLockMode) {
					gCore->set_shift_state(MCF_SHIFT_STATE_OFF);
					helper_agent.update_input_context(ECORE_IMF_INPUT_PANEL_SHIFT_MODE_EVENT,
													ECORE_IMF_INPUT_PANEL_SHIFT_MODE_OFF);
					change_shiftmode(SHIFTMODE_OFF);
				}
			}
		}
	}

	/* Saves the latest context */
	gLatestInputContext.keyValue = keyEventDesc.keyValue;
	gLatestInputContext.keyEvent = keyEventDesc.keyEvent;
	gLatestInputContext.keyType = keyEventDesc.keyType;

	return TRUE;
}

void _on_input_mode_changed(mcfchar * keyValue, mcfulong keyEvent, MCFKeyType keyType)
{
	mcfboolean bShowCloseWindow = TRUE;
	mcf8 mode = NOT_USED;
	static int email_flag = 0;

	int current_index = g_currentLanguage;

	int count = 0;
	mcfint prevWinWidth, prevWinHeight;
	gCore->get_window_size(&prevWinWidth, &prevWinHeight);

	gCore->set_update_pending(TRUE);

	switch (keyEvent) {
	case UD_MVK_OPTION_WINDOW:
		send_flush();
		if (gCore->get_debug_mode() != DEBUGMODE_AUTOTEST) {
			_setup_info.current_language = g_currentLanguage;
			_show_option_window_ise(main_window, gIseScreenDegree,
						_setup_info, setting_window_cb);
		}
		break;
	case UD_MVK_LANG_CHANGE:
				if(_setup_info.lang_count == 1)
					_show_option_window_ise(main_window, gIseScreenDegree, _setup_info, setting_window_cb);
				else if(_setup_info.lang_count == 2) {
					send_flush();
					do {
						count++;
						current_index++;
						if (current_index >= MAX_LANG_NUM) {
							/*IseLangDataSelectState[0] = true;*/
							current_index = 0;
						}
					} while((!IseLangDataSelectState[current_index]) && (count < MAX_LANG_NUM));

					if(current_index != g_currentLanguage) {
						ise_set_language(current_index);
						g_currentLanguage = current_index;
					}
				}
				helper_agent.update_input_context(ECORE_IMF_INPUT_PANEL_LANGUAGE_EVENT, 0);
		break;
	case UD_MVK_123:
		send_flush();
		ise_send_string(keyValue);
		mode = INPUT_MODE_4X4_NUM;
		if (mcf_check_arrindex(gInitialInputMode, MAX_INPUT_MODE)) {
			if (IseDefaultInputModes[gInitialInputMode][2] != NOT_USED) {
				mode = IseDefaultInputModes[gInitialInputMode][2];
			}
		}
		gCore->set_input_mode(mode);
		break;
	case UD_MVK_SYM:
		ise_send_event(MVK_FLUSH);
		gCore->set_input_mode(mcfInputSymModeKeypad[0][get_Ise_default_context().KeypadMode]);
		break;
	case UD_MVK_ABC:
		send_flush();
		mode = mcfInputModeByLanguage[KEYPAD_QWERTY][g_currentLanguage];
		gCore->set_input_mode(mode);
		break;
	case UD_MVK_NEXTSYM:{

			mcf8 inputmode = gCore->get_input_mode();

			/*removed symbol page 3 according to latest spec*/
			if (inputmode == INPUT_MODE_QTY_SYM_1)
				gCore->set_input_mode(INPUT_MODE_QTY_SYM_2);
			if (inputmode == INPUT_MODE_QTY_SYM_2)
				gCore->set_input_mode(INPUT_MODE_QTY_SYM_1);
		}
		break;
	case UD_MVK_PREVSYM:{

			mcf8 inputmode = gCore->get_input_mode();

			/*removed symbol page 3 according to latest spec*/
			if (inputmode == INPUT_MODE_QTY_SYM_1)
				gCore->set_input_mode(INPUT_MODE_QTY_SYM_2);
			if (inputmode == INPUT_MODE_QTY_SYM_2)
				gCore->set_input_mode(INPUT_MODE_QTY_SYM_1);
		}
		break;
	}

	mcf8 language = NOT_USED;
	if (mode >= 0 && mode < MAX_INPUT_MODE) {
		language = mcfLanguageByInputMode[mode];
	}

	if (get_Ise_default_context().Language != language && language != NOT_USED) {
		change_ldb_option(language);
	}

	if (gExternalShiftLockMode) {
		if (gCore->get_input_mode() == INPUT_MODE_QTY_ENGLISH ||
			gCore->get_input_mode() == INPUT_MODE_QTY_URL ||
			gCore->get_input_mode() == INPUT_MODE_QTY_EMAIL) {
				change_shiftmode(SHIFTMODE_LOCK);
		}
	}

	mcfboolean forceShiftOff = FALSE;

	if (get_Ise_default_context().ShiftMode == SHIFTMODE_ON && !forceShiftOff) {
		gCore->set_shift_state(MCF_SHIFT_STATE_ON);
		helper_agent.update_input_context(ECORE_IMF_INPUT_PANEL_SHIFT_MODE_EVENT,
								ECORE_IMF_INPUT_PANEL_SHIFT_MODE_ON);
	} else if (get_Ise_default_context().ShiftMode == SHIFTMODE_LOCK && !forceShiftOff) {
		gCore->set_shift_state(MCF_SHIFT_STATE_LOCK);
		helper_agent.update_input_context(ECORE_IMF_INPUT_PANEL_SHIFT_MODE_EVENT,
								ECORE_IMF_INPUT_PANEL_SHIFT_MODE_ON);
	} else {
		if (!forceShiftOff)
			change_shiftmode(SHIFTMODE_OFF);
		gCore->set_shift_state(MCF_SHIFT_STATE_OFF);
		helper_agent.update_input_context(ECORE_IMF_INPUT_PANEL_SHIFT_MODE_EVENT,
								ECORE_IMF_INPUT_PANEL_SHIFT_MODE_OFF);
	}

	gCore->set_update_pending(FALSE);
	prevInputMode = gCore->get_input_mode();
	prevDispMode = gCore->get_display_mode();

	if (bShowCloseWindow) {
		int width, height;
		gCore->get_window_size(&width, &height);
		decorator.finish_show_animation(width, height, gIseScreenDegree);
	}

	ise_update_cursor_position(g_cursor_position);

	for (int loop = 0; loop < MAX_KEY; loop++) {
		if (gDisableKeyBuffer[loop] != NOT_USED) {
			gCore->disable_button(gCore->find_keyidx_by_customid(-1, -1, gDisableKeyBuffer[loop]));
		}
	}

	mcfint winWidth, winHeight;
	gCore->get_window_size(&winWidth, &winHeight);

	if (prevWinWidth != winWidth || prevWinHeight != winHeight) {
		helper_agent.update_input_context(ECORE_IMF_INPUT_PANEL_GEOMETRY_EVENT, 0);
#ifdef HAVE_CONFORMANT_AUTOSCROLL
		_send_keypad_geom_atom_info(main_window, KEYPAD_STATE_ON);
#endif
	}
}

void _on_process_user_defined_keys(mcfchar * keyValue, mcfulong keyEvent,
			      MCFKeyType keyType)
{
	mcf8 curInputMode = MAX_INPUT_MODE;
	if (gCore) {
		curInputMode = gCore->get_input_mode();
	}

	int current_index = (g_currentLanguage + 1) % MAX_LANG_NUM;
	int count = 0;
	CMCFUtils *utils = CMCFUtils::get_instance();

	switch (keyEvent) {
	case UD_MVK_OPTION_WINDOW:
		send_flush();
		if (gCore->get_debug_mode() != DEBUGMODE_AUTOTEST) {
			_setup_info.current_language = g_currentLanguage;
			_show_option_window_ise(main_window, gIseScreenDegree,
							_setup_info, setting_window_cb);
		}
		break;
	case UD_MVK_SHIFT:
		gCore->set_update_pending(TRUE);

		if (get_Ise_default_context().ShiftMode == SHIFTMODE_OFF) {
			change_shiftmode(SHIFTMODE_ON);
			gCore->set_shift_state(MCF_SHIFT_STATE_ON);
			helper_agent.update_input_context(ECORE_IMF_INPUT_PANEL_SHIFT_MODE_EVENT,
								ECORE_IMF_INPUT_PANEL_SHIFT_MODE_ON);
		} else if (get_Ise_default_context().ShiftMode == SHIFTMODE_ON) {
			change_shiftmode(SHIFTMODE_LOCK);
			gCore->set_shift_state(MCF_SHIFT_STATE_LOCK);
			helper_agent.update_input_context(ECORE_IMF_INPUT_PANEL_SHIFT_MODE_EVENT,
								ECORE_IMF_INPUT_PANEL_SHIFT_MODE_ON);
			if (get_Ise_default_context().CompletionMode == COMPLETION_AMBIGUOUS_LOWERCASE_STRING
				|| get_Ise_default_context().CompletionMode == COMPLETION_AMBIGUOUS_CAMELCASE_STRING)
					change_completion_option(COMPLETION_AMBIGUOUS_UPPERCASE_STRING);

			if (get_Ise_default_context().CompletionMode == COMPLETION_MULTITAB_LOWERCASE_STRING
			    || get_Ise_default_context().CompletionMode == COMPLETION_MULTITAB_CAMELCASE_STRING)
				change_completion_option(COMPLETION_MULTITAB_UPPERCASE_STRING);
		} else {
			change_shiftmode(SHIFTMODE_OFF);
			gCore->set_shift_state(MCF_SHIFT_STATE_OFF);
			helper_agent.update_input_context(ECORE_IMF_INPUT_PANEL_SHIFT_MODE_EVENT,
								ECORE_IMF_INPUT_PANEL_SHIFT_MODE_OFF);
			if (get_Ise_default_context().CompletionMode == COMPLETION_AMBIGUOUS_UPPERCASE_STRING)
				change_completion_option(COMPLETION_AMBIGUOUS_LOWERCASE_STRING);

			if (get_Ise_default_context().CompletionMode == COMPLETION_MULTITAB_UPPERCASE_STRING)
				change_completion_option(COMPLETION_MULTITAB_LOWERCASE_STRING);
		}

		gCore->set_update_pending(FALSE, TRUE);
		break;
	case UD_MVK_LANG_CHANGE:
		send_flush();
		while (!IseLangDataSelectState[current_index]
			&& count < MAX_LANG_NUM) {
			current_index++;

			if (current_index >= MAX_LANG_NUM) {
				current_index = 0;
			}
		}

		if (current_index != g_currentLanguage) {
			ise_set_language(current_index);
		}
		break;
	case UD_MVK_OK:
		/*ise_forward_key_event(keyEvent);*/
		ise_forward_key_event(XF86XK_UserPB);
		break;
	case UD_MVK_MORE:
		/*ise_forward_key_event(keyEvent);*/
		ise_forward_key_event(XF86XK_User1KB);
		break;
	case UD_MVK_HIDE:
		ise_forward_key_event(XF86XK_User2KB);
		ise_hide(TRUE);
		break;
	case UD_MVK_CHANGEISE:
		uint32 withUI = 1;
		break;
	}
}

static void _set_prediction_private_key()
{
	mcf8 mode = gCore->get_input_mode();
	mcf8 layout = -1;
	if (gCore->get_display_mode() == MCFDISPLAY_PORTRAIT) {
		layout = mcf_input_mode_configure[mode].layoutId[0];
	} else {
		layout = mcf_input_mode_configure[mode].layoutId[1];
	}

	/* Make sure prediction option is set off */
	if (get_Ise_default_context().OnOff == OFF) {
		change_prediction_onoff(FALSE);
	} else {
		change_prediction_onoff(TRUE);
	}
}

/*
*	_setup_info update on exit of setting menu
* 	It will be called before launching of keyboard
*/
SETTING_INFO ise_setting_update_on_reload_config(SETTING_INFO _setup_info)
{
	CMCFUtils *utils = CMCFUtils::get_instance();
	SETTING_INFO info;
	info = _setup_info;
	return info;
}

/**
* Send the given string to input framework
*/
static void ise_send_string(mcfchar * keyValue)
{
	helper_agent.commit_string(-1, "", scim::utf8_mbstowcs(keyValue));
}

/**
* Send the preedit string to input framework
*/
static void ise_update_preedit_string(mcfchar * str)
{
	AttributeList temp;
	helper_agent.update_preedit_string(-1, "", scim::utf8_mbstowcs(str), temp);
	if (strlen(str) == 0) {
		helper_agent.hide_preedit_string(-1, "");
	} else {
		helper_agent.show_preedit_string(-1, "");
	}
}

/**
* Send the given event to input framework
*/
static void ise_send_event(mcfulong keyEvent, mcfulong keyMask)
{
	KeyEvent key(keyEvent, keyMask);
	KeyEvent key_release(keyEvent, SCIM_KEY_ReleaseMask);
	helper_agent.send_key_event(-1, "", key);
	helper_agent.send_key_event(-1, "", key_release);
}

static void ise_forward_key_event(mcfulong keyEvent)
{
	KeyEvent key(keyEvent, 0);
	KeyEvent key_release(keyEvent, SCIM_KEY_ReleaseMask);
	helper_agent.forward_key_event(-1, "", key);
	helper_agent.forward_key_event(-1, "", key_release);
}

/**
* At ISE show, it will be called
* In this func,  related variable will set to default for layout
* See header file
*/
void ise_set_layout(mcfu32 layoutIdx)
{
	MCF_DEBUG();
	/* Check if the layoutIdx is in the valid range */
	if (layoutIdx >= MAX_ISE_LAYOUT_CNT)
		layoutIdx = 0;

	if (gLastIseLayout != layoutIdx) {
		gFLayoutChanged = TRUE;
	} else {
		gFLayoutChanged = FALSE;
	}
	gIseLayout = layoutIdx;
	gFLayoutSetting = TRUE;
}

mcfu32 ise_get_layout()
{
	return gIseLayout;
}

static int create_main_window()
{
	PERF_TEST_START("create_main_window()");
	mcfint win_w, win_h;

	/* Create window and set the attribute of window */
	main_window = elm_win_add(NULL, "ISE_WINDOW", ELM_WIN_BASIC);
	elm_win_alpha_set(main_window, EINA_TRUE);

	elm_win_borderless_set(main_window, EINA_TRUE);

	elm_win_keyboard_win_set(main_window, EINA_TRUE);

	/* Set font
	evas = evas_object_evas_get(main_window);
	evas_font_path_prepend(evas, "/usr/share/SLP/fonts");*/

	box = elm_box_add(main_window);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(main_window, box);

	decorator.create();
	decorator.set_target_window(main_window);

	/* get window size */
	if (gCore) {
		gCore->get_screen_resolution(&win_w, &win_h);
	}

	PERF_TEST_END("create_main_window()");

	return EXIT_SUCCESS;
}

void render_flush_post_cb(void *data, Evas * e, void *event_info)
{
	mcfint width = -1;
	mcfint height = -1;
	gCore->get_window_size(&width, &height);

	if (gFEffectEnabled) {
		decorator.start_show_animation(width, height, gIseScreenDegree);
	} else {
		int posx, posy;

		CWindowSlideAnimator::get_window_pos_by_rotation(width, height,
													gIseScreenDegree,
													&posx, &posy,
													NULL, NULL);
		evas_object_move(main_window, posx, posy);

		helper_agent.update_input_context(ECORE_IMF_INPUT_PANEL_STATE_EVENT,
										ECORE_IMF_INPUT_PANEL_STATE_SHOW);
#ifdef HAVE_CONFORMANT_AUTOSCROLL
		_send_keypad_geom_atom_info(main_window, KEYPAD_STATE_ON);
#endif
	}
	evas_object_show(main_window);

	evas_event_callback_del(e, EVAS_CALLBACK_RENDER_FLUSH_POST, render_flush_post_cb);
}

void ise_show(int ic)
{
	mcfboolean bRotated = FALSE;
	mcfboolean bShouldUpdate = FALSE;

	mcfint width = -1;
	mcfint height = -1;

	if (gCore == NULL)
		ise_new();

	mcf_assert_return(gCore);

	if (ise_get_layout() == ISE_LAYOUT_STYLE_EMAIL) {
		email_app_check = TRUE;
	}

	if (!gFLayoutSetting) {
		gIseLayout = ISE_LAYOUT_STYLE_NORMAL;
		ise_set_layout(gIseLayout);
	}

	gLastIseLayout = gIseLayout;
	mcf8 mcfInputMode = gCore->get_input_mode();
	mcf32 langID = PRIMARY_LANGUAGE_INDEX;
	mcf8 newInputMode = NOT_USED;
	mcf8 language = NOT_USED;

	if (mcf_check_arrindex(gExplicitLanguageSetting, MAX_LANG_NUM)) {
		langID = gExplicitLanguageSetting;
	}

	gCore->set_update_pending(TRUE);

	if (gIseScreenDegreeBackup != gIseScreenDegree) {
		_send_keypad_geom_atom_info(main_window, KEYPAD_STATE_OFF);
		evas_object_move(main_window, -10000, -10000);
		ise_set_screen_rotation(gIseScreenDegree);
		gFDisplaySetting = FALSE;
		bRotated = TRUE;
	}

	if (ic != gPrevInputContext || gFLayoutChanged || !gFInputContextSet
		|| bRotated || (g_currentLanguage != gExplicitLanguageSetting
		&& gExplicitLanguageSetting != NOT_USED)) {
		set_single_commit(IseDefaultValue[gIseLayout].SingleCommit);
		change_inputmode(IseDefaultValue[gIseLayout].InputMode);

		if (IseDefaultValue[gIseLayout].mcfInputMode == INPUT_MODE_NATIVE) {
			mcfInputMode = mcfInputModeByLanguage[IseDefaultValue[gIseLayout].KeypadMode][langID];
		} else if (gIseLayout ==  ISE_LAYOUT_STYLE_URL) {
			mcfInputMode = mcfInputModeByLanguage[IseDefaultValue[gIseLayout].KeypadMode][langID];
		} else if (gIseLayout ==  ISE_LAYOUT_STYLE_EMAIL) {
			mcfInputMode = mcfInputModeByLanguage[IseDefaultValue[gIseLayout].KeypadMode][langID];
		} else {
			mcfInputMode = IseDefaultValue[gIseLayout].mcfInputMode;
		}

		if (gFKeypadMode && gIseScreenDegree != 90
				&& gIseScreenDegree != 270) {
				newInputMode = IseDefaultInputModes[mcfInputMode][0];
		} else {
				newInputMode = IseDefaultInputModes[mcfInputMode][1];
		}
		if (newInputMode != NOT_USED) {
			mcfInputMode = newInputMode;
		}
		if (mcfInputMode >= 0 && mcfInputMode < MAX_INPUT_MODE) {
			g_currentLanguage = langID;
			IseLangDataSelectState[g_currentLanguage] = true;
			change_ldb_option(internalLangToLang[langID]);
			change_keypad((uint32)IseKeypadMode[mcfInputMode]
						[(gCore->get_display_mode() == MCFDISPLAY_LANDSCAPE) ? 1 : 0]);

			if (ic != gPrevInputContext) {
				change_completion_option(IseInitialCompletionMode[mcfInputMode][get_Ise_default_context().OnOff]);
			}

			gCore->set_input_mode(mcfInputMode, TRUE);

			mcf8 subLayoutID = IseDefaultValue[gIseLayout].subLayoutID;
			if(gCore->get_cur_sublayout_id() != subLayoutID)
			{
				gCore->set_cur_sublayout_id(subLayoutID);
				bShouldUpdate = TRUE;
			}

			if (ic != gPrevInputContext) {
				gInitialInputMode = mcfInputMode;
			}

			language = mcfLanguageByInputMode[mcfInputMode];
		}

		if (get_Ise_default_context().Language != language
			&& language != NOT_USED) {
			change_ldb_option(language);
		}

		if (TRUE &&
#ifdef SUPPORTS_LAYOUT_STYLE_MONTH
			gCore->get_input_mode() != INPUT_MODE_4X4_MONTH &&
#endif
#ifdef SUPPORTS_LAYOUT_STYLE_NUMBERONLY
			gCore->get_input_mode() != INPUT_MODE_4X4_NUMONLY &&
#endif
#ifdef SUPPORTS_LAYOUT_STYLE_IP
			gCore->get_input_mode() != INPUT_MODE_4X4_IPv6_123 &&
#endif
			TRUE) {
			_set_prediction_private_key();
		}

		if(gExternalShiftLockMode) {
			change_shiftmode(SHIFTMODE_LOCK);
			helper_agent.update_input_context(ECORE_IMF_INPUT_PANEL_SHIFT_MODE_EVENT,
					ECORE_IMF_INPUT_PANEL_SHIFT_MODE_ON);
			if (gCore->get_shift_state() != MCF_SHIFT_STATE_LOCK) {
				gCore->set_shift_state(MCF_SHIFT_STATE_LOCK);
				bShouldUpdate = TRUE;
			}
		} else {
			change_shiftmode(IseDefaultValue[gIseLayout].ShiftMode);
			helper_agent.update_input_context(ECORE_IMF_INPUT_PANEL_SHIFT_MODE_EVENT,
					(IseDefaultValue[gIseLayout].ShiftMode == SHIFTMODE_OFF)
					? ECORE_IMF_INPUT_PANEL_SHIFT_MODE_OFF : ECORE_IMF_INPUT_PANEL_SHIFT_MODE_ON);
			if (IseDefaultValue[gIseLayout].ShiftMode != gCore->get_shift_state()) {
				gCore->set_shift_state((MCFShiftState)IseDefaultValue[gIseLayout].ShiftMode);
				bShouldUpdate = TRUE;
			}
		}
	}

	gCore->show();
	gCore->disable_input_events(FALSE);

	ise_update_cursor_position(g_cursor_position);

	for (int loop = 0; loop < MAX_KEY; loop++) {
		if (gDisableKeyBuffer[loop] != NOT_USED) {
			gCore->disable_button(gCore->find_keyidx_by_customid(-1, -1, gDisableKeyBuffer[loop]));
		}
	}

	static int prevDispModeBackup;
	prevDispModeBackup = prevDispMode;
	if (prevInputMode != gCore->get_input_mode()
		|| prevDispMode != gCore->get_display_mode() || gFPrivateKeySet
		|| prevPrivkeySet || bShouldUpdate) {
			prevInputMode = gCore->get_input_mode();
			prevDispMode = gCore->get_display_mode();
			gCore->set_update_pending(FALSE);
	} else {
			gCore->set_update_pending(FALSE, FALSE);
	}

	/* Postpone window showing until first frame has been fully flushed */
	if (!bRotated) {
		gCore->get_window_size(&width, &height);

		if (gFEffectEnabled) {
			if (gFHiddenState || prevDispModeBackup != gCore->get_display_mode()) {
				decorator.start_show_animation(width, height, gIseScreenDegree);
			} else {
				decorator.finish_show_animation(width, height, gIseScreenDegree);
			}
		} else {
			int posx, posy;

			CWindowSlideAnimator::get_window_pos_by_rotation(width,
														height,
														gIseScreenDegree,
														&posx,
														&posy,
														NULL,
														NULL);
			evas_object_move(main_window, posx, posy);

			helper_agent.update_input_context(ECORE_IMF_INPUT_PANEL_STATE_EVENT,
											ECORE_IMF_INPUT_PANEL_STATE_SHOW);
#ifdef HAVE_CONFORMANT_AUTOSCROLL
			_send_keypad_geom_atom_info(main_window, KEYPAD_STATE_ON);
#endif
		}

		evas_object_show(main_window);
	} else {
		Evas *evas = evas_object_evas_get(main_window);
		evas_event_callback_add(evas, EVAS_CALLBACK_RENDER_FLUSH_POST,
							render_flush_post_cb, NULL);
	}

	evas_object_show(main_window);

	gFLayoutChanged = FALSE;
	gPrevInputContext = ic;
	gFInputContextSet = TRUE;
	gFHiddenState = FALSE;
	gIseScreenDegreeBackup = gIseScreenDegree;

	gPrevModifier = KEY_MODIFIER_NONE;

	prevPrivkeySet = gFPrivateKeySet;
}

/**
* Saves screen direction
*/
void ise_set_screen_direction(int degree)
{
	if (degree == 0 || degree == 90 || degree == 180 || degree == 270) {
		if (gIseScreenDegree == degree)
			return;
		if (gCore == NULL)
			ise_new();
		mcf_assert_return(gCore);
		gIseScreenDegree = degree;
		gFDisplaySetting = TRUE;
#ifdef HAVE_CONFORMANT_AUTOSCROLL
		g_virt_keybd_state = KEYPAD_STATE_UNKNOWN;
#endif
	}
}

/**
* Sets screen rotation
*/
static void ise_set_screen_rotation(int degree)
{
	mcfint width = -1;
	mcfint height = -1;

	if (gCore == NULL)
		ise_new();
	mcf_assert_return(gCore);

	gCore->set_display_mode(degree);
	gCore->get_window_size(&width, &height);
	evas_object_resize(main_window, width, height);
}

void ise_hide(bool fCallHided)
{
	CMCFWindows *windows = CMCFWindows::get_instance();
	if(windows) {
		windows->destroy_context_popup();
	}
	if (gCore) {
		gCore->hide();
		gCore->disable_input_events(TRUE);
	}

	helper_agent.candidate_hide();
	send_flush();

	mcfint width = -1;
	mcfint height = -1;

	gCore->get_window_size(&width, &height);
	if (gFEffectEnabled) {
		helper_agent.update_input_context(ECORE_IMF_INPUT_PANEL_STATE_EVENT,
										ECORE_IMF_INPUT_PANEL_STATE_HIDE);
		DBG("-=-=-= update_input_context :\
			ECORE_IMF_INPUT_PANEL_STATE_HIDE\n");
		decorator.start_hide_animation(width, height, gIseScreenDegreeBackup);
	} else {
		if (main_window)
			evas_object_move(main_window, -10000, -10000);

		helper_agent.update_input_context(ECORE_IMF_INPUT_PANEL_STATE_EVENT,
										ECORE_IMF_INPUT_PANEL_STATE_HIDE);
	}

	if (email_app_check)
		email_app_check = FALSE;

	gFHiddenState = TRUE;
#ifdef HAVE_CONFORMANT_AUTOSCROLL
	_send_keypad_geom_atom_info(main_window, KEYPAD_STATE_OFF);
#endif
}

/**
* Set  mode
**/
void ise_set_mode()
{
	mode_syncronize();
}

void ise_set_language(unsigned int language)
{
	mcfint prevWinWidth, prevWinHeight;
	gCore->get_window_size(&prevWinWidth, &prevWinHeight);

	if (mcf_check_arrindex(language, MAX_LANG_NUM)) {
		change_ldb_option(internalLangToLang[language]);
		change_shiftmode(SHIFTMODE_OFF);
		mcf8 mode = IseLangData[get_Ise_default_context().KeypadMode][language].inputMode;
		gCore->set_input_mode(mode);

		if (IseDefaultValue[gIseLayout].mcfInputMode == INPUT_MODE_NATIVE) {
			gInitialInputMode = gCore->get_input_mode();
		}

		/* Save current language setting so that this
		 * language will be displayed next time ISE_SHOW is called */
		gExplicitLanguageSetting = language;

		g_currentLanguage = language;
		_setup_info.current_language = g_currentLanguage;

		write_config();

		IseLangDataSelectState[g_currentLanguage] = true;
		_set_prediction_private_key();
	}

	mcfint winWidth, winHeight;
	gCore->get_window_size(&winWidth, &winHeight);

	if (prevWinWidth != winWidth || prevWinHeight != winHeight) {
		helper_agent.update_input_context(ECORE_IMF_INPUT_PANEL_GEOMETRY_EVENT, 0);
#ifdef HAVE_CONFORMANT_AUTOSCROLL
		_send_keypad_geom_atom_info(main_window, KEYPAD_STATE_ON);
#endif

	}

	for (int loop = 0; loop < MAX_KEY; loop++) {
		if (gDisableKeyBuffer[loop] != NOT_USED) {
			gCore->disable_button(gCore->find_keyidx_by_customid(-1, -1, gDisableKeyBuffer[loop]));
		}
	}
	helper_agent.update_input_context(ECORE_IMF_INPUT_PANEL_LANGUAGE_EVENT, 0);
}

static void _dismissed_cb(void *data, Evas_Object *obj, void *event_info)
{
	evas_object_del(obj);
	CMCFWindows *windows = CMCFWindows::get_instance();
	windows->destroy_context_popup();
	obj = NULL;
}

static void _ctxpopup_cb(void *data, Evas_Object *obj, void *event_info)
{
	int curLang = (int)data;
	evas_object_del(obj);
	CMCFWindows *windows = CMCFWindows::get_instance();
	windows->destroy_context_popup();
	obj = NULL;
	ise_set_language(curLang);
}

void _show_language_popup(mcfshort x, mcfshort y) {
	CMCFWindows *windows = CMCFWindows::get_instance();
	Evas_Object* win = windows->get_context_popup();
	ctxspopup = elm_ctxpopup_add(win);
	elm_object_scroll_freeze_push(ctxspopup);
	evas_object_smart_callback_add(ctxspopup,"dismissed", _dismissed_cb, (void*)win);
	for (unsigned int loop = 0; loop < MAX_LANG_NUM; loop++) {
		if (IseLangData[KEYPAD_QWERTY][loop].name) {
			scim::String str = scim::scim_get_language_name_english(IseLangData[KEYPAD_QWERTY][loop].name);
			strncpy(IseLangData[KEYPAD_QWERTY][loop].displayname, dgettext(GETTEXT_PACKAGE, language_text_name[loop]), 255);
		}
	}
	for (unsigned int loop = 0; loop < MAX_LANG_NUM; loop++) {
		if (IseLangDataSelectState[loop]) {
			elm_ctxpopup_item_append(ctxspopup,IseLangData[1][loop].displayname, NULL, _ctxpopup_cb, (void*)loop);
		}
	}
	evas_object_size_hint_max_set(ctxspopup, 800, 230);
	evas_object_move(ctxspopup,x,y);
	evas_object_show(win);
	evas_object_show(ctxspopup);
}

void ise_set_lang_to_vconf(unsigned int language)
{
	if (mcf_check_arrindex(language, MAX_LANG_NUM)) {
		change_ldb_option(internalLangToLang[language]);
		g_currentLanguage = language;
		IseLangDataSelectState[g_currentLanguage] = true;

	}
}

void ise_focus_in()
{
	if (gCore == NULL)
		ise_new();

	mcf_assert_return(gCore);
}

void ise_new()
{
	if (gCore == NULL) {
		if (main_window) {
			gCore = new CMCFCore((void *)main_window);
			gCore->set_event_callback(&callback);
			float timeout = elm_config_longpress_timeout_get();

			/* convert timeout second value to milisecond value;*/
			timeout *= 1000;
			gCore->set_longkey_duration(timeout);
		}
	}
}

/**
* It will call when the current ISE lose focus
*/
void ise_focus_out()
{
	if (gCore == NULL)
		ise_new();
	mcf_assert_return(gCore);
	if (main_window != NULL) {
	          send_flush();
	}
	//memset(gPrivateKeyBuffer, 0x00, sizeof(gPrivateKeyBuffer));

	if (gPrevMultitapValue) {
		gPrevMultitapValue = NULL;
	}
}

void ise_set_caps_mode(unsigned int mode)
{
	mcf8 inputmode = gCore->get_input_mode();
	if (gCore == NULL)
		ise_new();
	mcf_assert_return(gCore);

	if (mode) {
		gExternalShiftLockMode = TRUE;
	} else {
		gExternalShiftLockMode = FALSE;
	}

	if (inputmode == INPUT_MODE_QTY_ENGLISH ||
        inputmode >= INPUT_MODE_QTY_FRENCH && inputmode <= INPUT_MODE_QTY_RUSSIAN ||
        inputmode == INPUT_MODE_QTY_URL ||
        inputmode == INPUT_MODE_QTY_EMAIL) {
			change_shiftmode(mode ? SHIFTMODE_LOCK : SHIFTMODE_OFF);
			gCore->set_shift_state(mode ? MCF_SHIFT_STATE_LOCK : MCF_SHIFT_STATE_OFF);
			helper_agent.update_input_context(ECORE_IMF_INPUT_PANEL_SHIFT_MODE_EVENT,
						mode ? ECORE_IMF_INPUT_PANEL_SHIFT_MODE_ON : ECORE_IMF_INPUT_PANEL_SHIFT_MODE_OFF);
	}
}

void ise_explicitly_set_language(unsigned int language)
{
	if (language == ECORE_IMF_INPUT_PANEL_LANG_ALPHABET) {
		/* Force to show PRIMARY_LANGUAGE_INDEX mode*/
		gExplicitLanguageSetting = PRIMARY_LANGUAGE_INDEX;
	}
}

void ise_get_language_locale(char **locale)
{
    if(locale) {
        for(int index = 0;index < (sizeof(lang_table) / sizeof(ISELanguageTable));index++) {
            if(lang_table[index].language_code == g_currentLanguage) {
                *locale = strdup(lang_table[index].language_string);
            }
        }
    }
}
unsigned int ise_get_language()
{
	mcf8 language = 0;
	if (gCore == NULL)
		ise_new();
	mcf_assert_return_false(gCore);

	return language;
}

/**
* Sets the current context to default context
*/
void ise_reset_context()
{
	if (gCore == NULL)
		ise_new();
	mcf_assert_return(gCore);
	send_flush();

	int loop;
	for (loop = 0; loop < ISE_PRIVATE_KEY_BUFFER_SIZE; loop++) {
		if (gPrivateKeyBuffer[loop].used) {
			gCore->unset_private_key(gPrivateKeyBuffer[loop].privateId);
		}
	}

	gCore->set_update_pending(TRUE);
	memset(gPrivateKeyBuffer, 0x00, sizeof(gPrivateKeyBuffer));
	gIseLayout = ISE_LAYOUT_STYLE_NORMAL;
	ise_set_layout(gIseLayout);
	ise_set_screen_direction(0);

	for (loop = 0; loop < MAX_KEY; loop++) {
		gDisableKeyBuffer[loop] = NOT_USED;
	}

	gExternalShiftLockMode = FALSE;
	gFPrivateKeySet = FALSE;
	gCore->set_update_pending(FALSE, FALSE);
}

void ise_reset_input_context()
{
	if (gCore == NULL)
		ise_new();
	mcf_assert_return(gCore);
	send_flush();

	if (gCore) {
		gCore->close_all_popups();
	}
}

void ise_set_private_key(int keyIdx, char *label, char *imgPath, int valueIdx, char *valueStr)
{
	if (gCore == NULL)
		ise_new();

	mcf_assert_return(gCore);

	mcf_assert_return(valueStr);

	mcf8 pkIndex = -1;
	mcfint loop;

	for (loop = 0; loop < ISE_PRIVATE_KEY_BUFFER_SIZE; loop++) {
		/* Overwrite the buffer if the given key is already in the buffer list */
		if (gPrivateKeyBuffer[loop].keyIdx == keyIdx) {
			pkIndex = loop;
		}
		if(pkIndex == -1 && !gPrivateKeyBuffer[loop].used) {
			pkIndex = loop;
		}
	}

	if(pkIndex != -1) {
		gPrivateKeyBuffer[pkIndex].used = TRUE;
		gPrivateKeyBuffer[pkIndex].keyIdx = keyIdx;

		if (label) {
			strncpy(gPrivateKeyBuffer[pkIndex].label, label, ISE_PRIVATE_KEY_LABEL_LEN - 1);
			gPrivateKeyBuffer[pkIndex].label[ISE_PRIVATE_KEY_LABEL_LEN - 1] = '\0';
		} else {
			strcpy(gPrivateKeyBuffer[pkIndex].label, "");
		}

		if (valueStr) {
			strncpy(gPrivateKeyBuffer[pkIndex].valueStr, valueStr, ISE_PRIVATE_KEY_VALUE_LEN - 1);
			gPrivateKeyBuffer[pkIndex].valueStr[ISE_PRIVATE_KEY_VALUE_LEN- 1] = '\0';
		} else {
			strcpy(gPrivateKeyBuffer[pkIndex].valueStr, "");
		}

		if (imgPath) {
			strncpy(gPrivateKeyBuffer[pkIndex].ImgPath, imgPath, ISE_PRIVATE_KEY_IMAGE_LEN - 1);
			gPrivateKeyBuffer[pkIndex].ImgPath[ISE_PRIVATE_KEY_IMAGE_LEN - 1] = '\0';
		} else {
			strcpy(gPrivateKeyBuffer[pkIndex].ImgPath,"");
		}

		for(loop = 0; loop < MCF_BUTTON_STATE_MAX; loop++) {
			gPrivateKeyBuffer[pkIndex].imgPathArr[loop] = gPrivateKeyBuffer[pkIndex].ImgPath;
		}

		gCore->set_private_key(-1, -1, gCore->find_keyidx_by_customid(-1, -1, keyIdx),
			gPrivateKeyBuffer[pkIndex].label, gPrivateKeyBuffer[pkIndex].imgPathArr, NULL,
			valueIdx, gPrivateKeyBuffer[pkIndex].valueStr, TRUE);

		gFPrivateKeySet = TRUE;
	}
}

#ifdef SLP_PROF
inline static double cur_time_get(void)
{
	struct timeval timev;

	gettimeofday(&timev, NULL);
	return (double)timev.tv_sec + (((double)timev.tv_usec) / 1000000);
}
#endif

void ise_set_disable_key(int keyIdx, int disabled)
{
	if (gCore == NULL)
		ise_new();
	mcf_assert_return(gCore);
	/* Key index */
	int actualKeyIdx = gCore->find_keyidx_by_customid(-1, -1, keyIdx);
	if (disabled) {
		gCore->disable_button(actualKeyIdx);
	} else {
		gCore->enable_button(actualKeyIdx);
	}

	if (disabled) {
		int iIndex = NOT_USED;
		for (int loop = 0; loop < MAX_KEY; loop++) {
			if (gDisableKeyBuffer[loop] == keyIdx)
				return;
			if (gDisableKeyBuffer[loop] == NOT_USED) {
				iIndex = loop;
			}
		}

		if (iIndex != NOT_USED) {
			gDisableKeyBuffer[iIndex] = keyIdx;
		}
	} else {
		for (int loop = 0; loop < MAX_KEY; loop++) {
			if (gDisableKeyBuffer[loop] == keyIdx) {
				gDisableKeyBuffer[loop] = NOT_USED;
			}
		}
	}

	gFPrivateKeySet = TRUE;
}

void ise_update_cursor_position(int position)
{
	if (gCore == NULL)
		ise_new();
	mcf_assert_return(gCore);

	g_cursor_position = position;

	if (gCore) {
		if(position > 0) {
			ise_set_private_key(CUSTOMID_WWWCOM, WWWCOM_BUTTON_COM_STR, NULL, 0, WWWCOM_BUTTON_COM_STR);
		} else {
			ise_set_private_key(CUSTOMID_WWWCOM, WWWCOM_BUTTON_WWW_STR, NULL, 0, WWWCOM_BUTTON_WWW_STR);
		}
	}
}

void ise_set_return_key_type(unsigned int type)
{
	char buf[64];

	switch (type)
	{
		case ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_DONE:
			sprintf(buf, _("Done"));
		break;
		case ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_GO:
			sprintf(buf, _("Go"));
		break;
		case ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_JOIN:
			sprintf(buf, _("Join"));
		break;
		case ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_LOGIN:
			sprintf(buf, _("Login"));
		break;
		case ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_NEXT:
			sprintf(buf, _("Next"));
		break;
		case ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_SEARCH:
			sprintf(buf, _("Search"));
		break;
		case ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_SEND:
			sprintf(buf, _("Send"));
		break;
	}

	if (type != ECORE_IMF_INPUT_PANEL_RETURN_KEY_TYPE_DEFAULT)
		ise_set_private_key(CUSTOMID_ENTER, buf, NULL, -1, NULL);
}

void ise_set_return_key_disable(unsigned int disabled)
{
	ise_set_disable_key(CUSTOMID_ENTER, disabled);
}

/**
* Update spot location
*/
void ise_update_spot_location(int x, int y)
{

}

int init_i18n(const char *domain, const char *dir)
{
	if (setlocale(LC_ALL, "") == NULL)
		return -1;

	if (bindtextdomain(domain, dir) == NULL)
		return -1;

	if (textdomain(domain) == NULL)
		return -1;

	return 0;
}

/**
* initialize the variables
*/
int init(const String & display)
{
	PERF_TEST_START("\tinit()");

	char **argv = new char*[4];
	int argc = 3;
	int ret;

	main_window = NULL;

	argv[0] = (char *)ISE_NAME;
	argv[1] = (char *)"--display";
	argv[2] = const_cast<char *>(display.c_str());
	argv[3] = 0;

#ifdef SLP_PROF
	start_time = cur_time_get();
#endif

	setenv("DISPLAY", display.c_str(), 1);

	elm_init(argc, argv);
	PERF_TEST_MID("\t\telm_init()");

	ret = init_i18n(PACKAGE, LOCALEDIR);

	create_main_window();

	PERF_TEST_END("\tinit()");
	return EXIT_SUCCESS;
}

int fini_cnt = 0;

void fini()
{
	if (gCore)
		delete(gCore);
	gCore = NULL;

	elm_shutdown();
}

void ise_get_size(int *x, int *y, int *width, int *height)
{
	if (x && y && width && height) {
		*x = 0;
		*y = 0;
		*width = 0;
		*height = 0;

		mcfint win_w, win_h;
		if (gCore) {
			gCore->get_window_size(width, height);
			gCore->get_screen_resolution(&win_w, &win_h);

			if (gCore->get_display_mode() == MCFDISPLAY_LANDSCAPE) {
				mcfint temp = win_w;
				win_w = win_h;
				win_h = temp;
			}
			*x = (win_w - *width) / 2;
			if (gFHiddenState) {
				*y = win_h;
			} else {
				*y = win_h - (*height);
			}
		}
	}
}
