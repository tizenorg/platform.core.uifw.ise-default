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


#include "mcfdebug.h"
#include "isedata.h"
#include "ise-default-setting.h"
#include "languagesetting.h"
#include <Elementary.h>
#include <Ecore_X.h>
#include <Ecore_IMF.h>
#include <X11/Xlib.h>
#include <gmodule.h>
#include <stdlib.h>
#include <string.h>
#include <sensor.h>

/* For ISE Interface. The below defines should be included in the file
   for processing a controlling data between active appl and this ISE  */
#define Uses_SCIM_UTILITY
#define Uses_SCIM_OBJECT
#define Uses_SCIM_POINTER
#define Uses_SCIM_EVENT
#define Uses_SCIM_HELPER
#define Uses_SCIM_CONFIG_BASE

#include <scim.h>
#include <scim_config_path.h>

#define ISF_UUID "ff110940-b8f0-4062-9ff6-a84f4f3setup"
#define ISF_DISPLAY ":13"

using namespace scim;

typedef struct _Keypad_Data {
	Elm_Object_Item *keypad_item;
	Elm_Object_Item *lang_item;
	Evas_Object *keypad_rdg;
} Keypad_Data;

#define ITEM_DATA_STRING_LEN 32

#define LANGUAGE_ID				0
#define KEYBOARD_ID				1
#define MAX_ID 					2

#define LAYOUT_EDJ_PATH "/usr/share/isf/ise/"ISE_NAME"/edje"
#define LAYOUT_EDJ_NAME LAYOUT_EDJ_PATH"/"ISE_NAME"_layout.edj"

struct ItemData {
	char text[ITEM_DATA_STRING_LEN];
	char sub_text[ITEM_DATA_STRING_LEN];
	int mode;
};

ItemData itemdata[MAX_ID] = { 0 };

extern int g_currentLanguage;
extern SETTING_INFO _setup_info;
extern std::vector < int >v_lang_list;
static Keypad_Data keypad_subdata;

Evas_Object *option_list = NULL;


static char *Lang_count_List[] =
	{ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
		"11", "12", "13","14", "15", "16", "17", "18", "19",
		"20", "21", "22", "23", "24", "25", "26", "27", "28",
		"29","30", "31", "32", "33", "34", "35"
};

void (*callback) (SETTING_INFO) = NULL;

static mcfu32 lang_num_index;

typedef struct _IseSettings Ise_Settings;

/*Array to keep track of the current selected languages*/
bool IseLangDataSelectState[MAX_LANG_NUM] = { false };

struct _IseSettings {
	Evas_Object *parent;
	Evas_Object *option_window;
	Evas_Object *option_layout;
	int rotation_angle;
	Elm_Object_Item *First_item;
	Evas_Object *naviframe;
	Evas_Object *effect_ly;
	char *SelLang;
	int SelKeypad;
	bool openedViaGadget;
};



static Ise_Settings ad;
extern ConfigPointer _scim_config;
extern Evas_Object *main_window;
static Ecore_Event_Handler *evtHandler = NULL;

static Evas_Object *create_main_window();
static Evas_Object *create_bg(Evas_Object *window);
static Evas_Object *create_option_mainview(Evas_Object *parent);
static Evas_Object *create_scroller(Evas_Object *parent);
static Evas_Object *create_naviframe(Evas_Object *ly);
static Evas_Object *create_layout(Evas_Object *parent);
static Evas_Object *create_dialogue_layout(Evas_Object *parent);
static Evas_Object *create_list_view(Evas_Object *parent);
Evas_Object *create_radio_item(Evas_Object *parent, Evas_Object **RadioButton, const char *Title);
static Evas_Object *create_radio_button(Evas_Object *parent);
static Evas_Object *create_check_button(Evas_Object *parent);

static Evas_Object *gl4_onoff_get(void *data, Evas_Object *obj,const char *part);
static Eina_Bool gl4_state_get(void *data, Evas_Object *obj, const char *part);

static void gl4_del(void *data, Evas_Object *obj);
static void gl4_sel(void *data, Evas_Object *obj, void *event_info);

static void option_list_clicked(void *data, Evas_Object *obj,	void *event_info);
static void _show_language_selection_view(Evas_Object *window);
static void list_selected(void *data, Evas_Object *obj, void *event_info);
static void set_the_selected_language(Evas_Object *obj);
static void softkey_clicked	(void *data, Evas_Object *obj, void *event_info);
static void _lang_cancel_cb(void *data, Evas_Object *obj, void *event_info);
static void _lang_set_cb(void *data, Evas_Object *obj, void *event_info);
static void _option_cancel_cb(void *data, Evas_Object *obj, void *event_info);
static void _option_done_cb(void *data, Evas_Object *obj, void *event_info);

int sensor_handle = -1;

void rotation_callback_func(unsigned int event_type,
			    sensor_event_data_t *event, void *data)
{
	const int MAX_MANIPULATED_ANGLES = 4;
	const int manipulated_angles[MAX_MANIPULATED_ANGLES][2] = {
		{ROTATION_EVENT_0, PORTRAIT_DEGREE_0},
		{ROTATION_EVENT_90, LANDSCAPE_DEGREE_270},
		{ROTATION_EVENT_180, PORTRAIT_DEGREE_180},
		{ROTATION_EVENT_270, LANDSCAPE_DEGREE_90},
	};

	int *my_event_data = NULL;

	if (event_type != ACCELEROMETER_EVENT_ROTATION_CHECK || event == NULL) {
		return;
	}
	my_event_data = (int *)(event->event_data);

	if (my_event_data) {
		printf("rotation_callback_func : %d\n", *my_event_data);
		bool bFound = false;
		for (int loop = 0; loop < MAX_MANIPULATED_ANGLES; loop++) {
			if (*my_event_data == manipulated_angles[loop][0]) {
				ad.rotation_angle = manipulated_angles[loop][1];
				bFound = true;
			}
		}

		if (bFound) {
			elm_win_rotation_with_resize_set(ad.option_window,
							 ad.rotation_angle);
		}
	}
}

Evas_Object *create_setup_genlist(Evas_Object *layout_main,
				  Evas_Object *naviframe, SETTING_INFO info)
{
	Evas_Object *genlist = NULL;
	Evas_Object *window = NULL;
	Evas_Object *layout = NULL;

	memset(&ad,0x0,sizeof(Ise_Settings));
	ad.option_layout = layout_main;
	ad.naviframe = naviframe;
	ad.rotation_angle=PORTRAIT_DEGREE_0;
	ad.parent = NULL;
	ad.openedViaGadget = TRUE;

	for (unsigned int loop = 0; loop < MAX_LANG_NUM; loop++) {
		if (IseLangData[KEYPAD_QWERTY][loop].name) {
			scim::String str =	scim::scim_get_language_name_english(IseLangData
									[KEYPAD_QWERTY]
									[loop].name);
			//strncpy(IseLangData[KEYPAD_QWERTY][loop].displayname, str.c_str(), 255);
			strncpy(IseLangData[KEYPAD_QWERTY][loop].displayname, dgettext(GETTEXT_PACKAGE, language_text_name[loop]), 255);
		}
	}

	genlist = create_option_mainview(layout_main);
	option_list = genlist;

	return genlist;
}

SETTING_INFO get_keyboard_option(void)
{
	return _setup_info;
}

void set_keyboard_option(bool isEuropeanMode, const SETTING_INFO &info)
{
	_setup_info = info;

	/*display language  set state */
	if (isEuropeanMode) {
		if (keypad_subdata.lang_item != NULL)
			strncpy(itemdata[LANGUAGE_ID].sub_text,
				gettext(Lang_count_List
					[_setup_info.lang_count]),
					ITEM_DATA_STRING_LEN - 1);
	}

	/*resetting all language state before filling again */
	memset(IseLangDataSelectState, false, MAX_LANG_NUM);

	IseLangDataSelectState[g_currentLanguage] = true;

	/*filling selected language */
	for (unsigned int i = 0; i < v_lang_list.size(); i++)
		IseLangDataSelectState[v_lang_list[i]] = true;

	if (keypad_subdata.lang_item) {
		elm_genlist_item_update(keypad_subdata.lang_item);
	}

	if (keypad_subdata.keypad_item) {
		elm_genlist_item_update(keypad_subdata.keypad_item);
	}
}

static Evas_Object *create_main_window()
{
	Evas_Object *window = NULL;
	Evas_Coord win_w, win_h;

	window = elm_win_add(NULL, "Option window", ELM_WIN_BASIC);
	ecore_x_icccm_name_class_set(elm_win_xwindow_get
				     (static_cast<Evas_Object *>(window)),
				     "Setting Window", "ISF");
	elm_win_rotation_set(window, ad.rotation_angle);
	ecore_x_window_size_get(ecore_x_window_root_first_get(), &win_w,
				&win_h);
	evas_object_move(window, 0, 0);

	if (ad.rotation_angle == 90 || ad.rotation_angle == 270)
		evas_object_resize(window, win_h, win_w);
	else
		evas_object_resize(window, win_w, win_h);

	elm_win_borderless_set(window, 1);
	evas_object_show(window);

	return window;
}

static Evas_Object *create_bg(Evas_Object *window)
{
	Evas_Object *bg=NULL;

	bg = elm_bg_add(window);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(window, bg);
	evas_object_show(bg);

	return bg;
}

static Evas_Object *create_scroller(Evas_Object *parent)
{
	for (unsigned int loop = 0; loop < MAX_LANG_NUM; loop++) {
		if (IseLangData[KEYPAD_QWERTY][loop].name) {
			scim::String str = scim::scim_get_language_name_english(IseLangData
										[KEYPAD_QWERTY]
										[loop].name);
			//strncpy(IseLangData[KEYPAD_QWERTY][loop].displayname, str.c_str(), 255);
			strncpy(IseLangData[KEYPAD_QWERTY][loop].displayname, dgettext(GETTEXT_PACKAGE, language_text_name[loop]), 255);
		}
	}

	Evas_Object *scroller = elm_scroller_add(parent);
	elm_scroller_bounce_set(scroller, EINA_FALSE, EINA_TRUE);
	elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_OFF,
					ELM_SCROLLER_POLICY_AUTO);
	evas_object_show(scroller);

	return scroller;
}

static char *_gl_text_get(void *data, Evas_Object *obj, const char *part)
{
	ItemData *item_data = (ItemData *) data;
	if (!strcmp(part, "elm.text")) {
		return strdup(item_data->text);
	}
	if (!strcmp(part, "elm.text.1")) {
		return strdup(item_data->text);
	}
	if (!strcmp(part, "elm.text.2")) {
		return strdup(item_data->sub_text);
	}
}

static Evas_Object *_gl_content_get(void *data, Evas_Object *obj,
				 const char *part)
{
	Evas_Object *item = NULL;
	ItemData *item_data = (ItemData *) data;
	return item;
}

static void _gl_sel(void *data, Evas_Object *obj, void *event_info)
{

	Elm_Object_Item *item = (Elm_Object_Item *) event_info;

	int id = (int)(data);

	switch (id) {
	case LANGUAGE_ID:{
			elm_genlist_item_selected_set(item, EINA_FALSE);
			option_list_clicked((void *)LANGUAGE, NULL, NULL);
		}
		break;
	}
}

static Eina_Bool _gl_state_get(void *data, Evas_Object *obj, const char *part)
{
	return EINA_FALSE;
}

static void _gl_del(void *data, Evas_Object *obj)
{
	return;
}

static void _gl_con(void *data, Evas_Object *obj, void *event_info)
{
}

static void _gl_selected(void *data, Evas_Object *obj, void *event_info)
{
}

static void _gl_exp(void *data, Evas_Object *obj, void *event_info)
{
}

static Evas_Object *create_option_mainview(Evas_Object *parent)
{
	ad.option_layout = parent;
	memset(&keypad_subdata, 0x00, sizeof(keypad_subdata));
	static Elm_Genlist_Item_Class itcSeparator;

	Evas_Object *genlist = elm_genlist_add(ad.naviframe);
	elm_object_style_set(genlist, "dialogue");

	static Elm_Genlist_Item_Class itcOnOff;
	static Elm_Genlist_Item_Class itcOnOff2;
	static Elm_Genlist_Item_Class itcTextOnly;

	itcTextOnly.item_style = "dialogue/2text.3";
	itcTextOnly.func.text_get = _gl_text_get;
	itcTextOnly.func.content_get = _gl_content_get;
	itcTextOnly.func.state_get = _gl_state_get;
	itcTextOnly.func.del = _gl_del;

	/* Set item class for dialogue seperator*/
	itcSeparator.item_style = "dialogue/separator/21/with_line";
	itcSeparator.func.text_get = NULL;
	itcSeparator.func.content_get = NULL;
	itcSeparator.func.state_get = NULL;
	itcSeparator.func.del = NULL;

	Elm_Object_Item *separator = elm_genlist_item_append(genlist, &itcSeparator, NULL, NULL,
												ELM_GENLIST_ITEM_NONE, NULL, NULL);
	elm_genlist_item_select_mode_set(separator, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);

#if MAX_LANG_NUM > 2
	strncpy(itemdata[LANGUAGE_ID].text, LANGUAGE, ITEM_DATA_STRING_LEN - 1);
	strncpy(itemdata[LANGUAGE_ID].sub_text,
		gettext(Lang_count_List[_setup_info.lang_count]),
		ITEM_DATA_STRING_LEN - 1);
	itemdata[LANGUAGE_ID].mode = LANGUAGE_ID;
	keypad_subdata.lang_item = elm_genlist_item_append(genlist, &itcTextOnly,
												&itemdata[LANGUAGE_ID], NULL,
												ELM_GENLIST_ITEM_NONE, _gl_sel,
												(void *)(itemdata[LANGUAGE_ID].mode));
#endif

	evas_object_smart_callback_add(genlist, "selected", _gl_selected, genlist);
	evas_object_smart_callback_add(genlist, "expanded", _gl_exp, genlist);
	evas_object_smart_callback_add(genlist, "contracted", _gl_con, genlist);

	return genlist;
}

static Evas_Object *create_layout(Evas_Object *parent)
{
	Evas_Object *ly;

	/* create a main layout */
	ly = elm_layout_add( parent );
	elm_layout_theme_set(ly, "layout", "application", "default");
	evas_object_size_hint_weight_set( ly, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add( parent, ly );
	evas_object_show( ly );

	return ly;
}

static Evas_Object *create_dialogue_layout(Evas_Object *parent)
{
	Evas_Object *ly;

	/* create a main layout */
	ly = elm_layout_add(parent);
	elm_layout_theme_set(ly, "standard", "window", "integration");
	evas_object_size_hint_weight_set(ly, EVAS_HINT_EXPAND,
					 EVAS_HINT_EXPAND);
	elm_win_resize_object_add(parent, ly);
	evas_object_show(ly);

	edje_object_signal_emit(_EDJ(ly), "elm,state,show,content", "elm");
	edje_object_signal_emit(_EDJ(ly), "elm,bg,show,group_list", "elm");

	return ly;
}

static Evas_Object *create_list_view(Evas_Object *parent)
{
	Evas_Object *lang_list = NULL;

	lang_list = elm_list_add(parent);
	elm_list_mode_set(lang_list, ELM_LIST_COMPRESS);
	evas_object_smart_callback_add(lang_list, "selected", list_selected, NULL);
	return lang_list;
}

Evas_Object *create_radio_item(Evas_Object *parent,
									Evas_Object **RadioButton,
									const char *Title)
{
	Evas_Object *rd=NULL;

	rd = elm_radio_add(parent);
	evas_object_show(rd);
	return rd;
}

static Evas_Object *create_radio_button(Evas_Object *parent)
{
	Evas_Object *rd = NULL;

	rd = elm_radio_add(parent);
	evas_object_show(rd);
	return rd;
}

static Evas_Object *create_check_button(Evas_Object *parent)
{
	Evas_Object *ck = NULL;

	ck = elm_check_add(parent);
	evas_object_show(ck);
	return ck;
}

static Evas_Object *create_naviframe(Evas_Object *ly)
{
	Evas_Object *naviframe = NULL;

	naviframe = elm_naviframe_add(ly);
	evas_object_show(naviframe);

	return naviframe;
}

void clean_up()
{
	if (!ad.openedViaGadget && ad.option_window) {
		if (ad.naviframe)
			ad.naviframe = NULL;
		evas_object_del(ad.option_layout);
		ad.option_layout = NULL;
		evas_object_hide(ad.option_window);
	} else {
		ad.openedViaGadget = FALSE;
	}

	if (evtHandler)
		ecore_event_handler_del(evtHandler);
	evtHandler = NULL;

	if (sensor_handle >= 0) {
		sf_unregister_event(sensor_handle, ACCELEROMETER_EVENT_ROTATION_CHECK);
		if (sensor_handle < 0) {
			printf("t sensor_unregister_cb fail\n");
		}

		sf_stop(sensor_handle);
		sf_disconnect(sensor_handle);
		sensor_handle = -1;
	}
}

static Eina_Bool
_ise_focus_out_cb(void *data, int type, void *event)
{
	set_the_selected_language(NULL);
	clean_up();

	return ECORE_CALLBACK_CANCEL;
}

static void list_selected(void *data, Evas_Object *obj, void *event_info)
{
	Elm_Object_Item *it = (Elm_Object_Item *) elm_list_selected_item_get(obj);
	if (it)
		elm_list_item_selected_set(it, 0);
}

static void set_the_selected_language(Evas_Object *obj)
{
	unsigned int loop = 0, order = 0;
	bool state;
	Elm_Object_Item *NextItem = NULL, *Item = NULL;

#ifdef SUPPORTS_MULTIPLE_LANGUAGE_SELECTION
	Evas_Object *check = NULL;
	if (ad.First_item) {
		NextItem = elm_list_item_next(ad.First_item);
		check = elm_object_item_part_content_get(ad.First_item, "start");
		if (check) {
			state = elm_check_state_get(check);
			IseLangDataSelectState[order] = state;
		}
	}
	do {
		order++;
		if (NextItem) {
			Item = NextItem;
			NextItem = elm_list_item_next(Item);
			check = elm_object_item_part_content_get(Item, "start");
			if (check) {
				state = elm_check_state_get(check);
				IseLangDataSelectState[order] = state;
			}
		}
	} while (NextItem);

	/* saving current values into global structure */
	_setup_info.current_language = g_currentLanguage;

	/* If hard key is pressed for saving current values into global structure
	   and no language is selected update lang_count to 1 */
	if (_setup_info.lang_count == 0)
		_setup_info.lang_count = 1;

	v_lang_list.clear();

	for (unsigned int loop = 0; loop < MAX_LANG_NUM; loop++) {
		if (IseLangDataSelectState[loop]) {
			v_lang_list.push_back(loop);
		}
	}
#else
	Evas_Object *radio = NULL;
	if (ad.First_item) {
		NextItem = elm_list_item_next(ad.First_item);
		radio = elm_list_item_icon_get(ad.First_item);
		if (radio) {
			int selected = elm_radio_value_get(radio);
			if (selected >= 0 && selected < MAX_LANG_NUM) {
				ise_set_language(selected);
				g_currentLanguage = selected;
				/* saving current values into global structure */
				_setup_info.current_language = g_currentLanguage;

				v_lang_list.clear();
				v_lang_list.push_back(g_currentLanguage);
			}
		}
	}
#endif

	if (callback) {
		callback(_setup_info);
		_setup_info.current_language = g_currentLanguage;

	}
#ifdef SUPPORTS_MULTIPLE_LANGUAGE_SELECTION
	else {
		int current_index = g_currentLanguage;
		int count = 0;

		while (!IseLangDataSelectState[current_index]
			&& count < MAX_LANG_NUM) {
			current_index++;

			if (current_index >= MAX_LANG_NUM) {
				current_index = 0;
			}

			count++;
		}

		_setup_info.current_language = current_index;
		g_currentLanguage = current_index;
	}
#else
	clean_up();
#endif
	/* This is necessary to update keyboard options
	 * when options update is made from the settings window */
	set_keyboard_option(true, _setup_info);
}

static void __response_cb(void *data, Evas_Object * obj, void *event_info)
{
	if(!data) return ;
		_scim_config->write(CONFIG_LANGUAGE_COUNT, _setup_info.lang_count);
	if (obj)
		evas_object_del(obj);
}
static int _check_selected_language_count(Evas_Object *list)
{
	const Eina_List *items, *l;
	Evas_Object *check;
	void *it;
	int count = 0;

	items = elm_list_items_get(list);
	EINA_LIST_FOREACH(items, l, it) {
		 check = elm_object_item_part_content_get((Elm_Object_Item *)it, "start");
		if (elm_check_state_get(check))
			count++;
	}
	return count;
}

static void _lang_check_button_toggled(void *data, Evas_Object *obj,
				       void *event_info)
{
	Evas_Object *list = (Evas_Object *) data;
	Evas_Object *button = (Evas_Object *) evas_object_data_get(list, "back_button");
	Evas_Object *lang_popup = NULL;
	_setup_info.lang_count = _check_selected_language_count(list);

	if (_setup_info.lang_count > 0)
		elm_object_disabled_set(button, EINA_FALSE);
	else {
		lang_popup = elm_popup_add(list);
		elm_check_state_set(obj,1);
		_setup_info.lang_count = 1;
		elm_object_text_set(lang_popup, dgettext(GETTEXT_PACKAGE, "IDS_IME_BODY_YOU_MUST_SELECT_AT_LEAST_ONE_LANGUAGE_IN_KEYBOARD_SETTINGS"));
		elm_popup_timeout_set(lang_popup, 3.0);
		evas_object_smart_callback_add(lang_popup, "block,clicked", __response_cb, list);
		evas_object_show(lang_popup);
	}
}

static void _lang_list_sel(void *data, Evas_Object *obj, void *event_info)
{
	Elm_Object_Item *it = (Elm_Object_Item *) event_info;
	Evas_Object *list = (Evas_Object *) data;
	Evas_Object *button = (Evas_Object *) evas_object_data_get(list, "back_button");
	Evas_Object *check;
	Evas_Object *lang_popup = NULL;

	check = elm_object_item_part_content_get(it, "start");
	elm_check_state_set(check, !elm_check_state_get(check));

	_setup_info.lang_count = _check_selected_language_count(list);
	if (_setup_info.lang_count > 0)
		elm_object_disabled_set(button, EINA_FALSE);
	else {
		lang_popup = elm_popup_add(list);
		elm_check_state_set(check,1);
		_setup_info.lang_count = 1;
		elm_object_text_set(lang_popup, dgettext(GETTEXT_PACKAGE, "IDS_IME_BODY_YOU_MUST_SELECT_AT_LEAST_ONE_LANGUAGE_IN_KEYBOARD_SETTINGS"));
		elm_popup_timeout_set(lang_popup, 3.0);
		evas_object_smart_callback_add(lang_popup, "block,clicked", __response_cb, list);
		evas_object_show(lang_popup);
	}
}

static void _lang_cancel_cb(void *data, Evas_Object *obj, void *event_info)
{
	elm_naviframe_item_pop(ad.naviframe);
}

static void _lang_set_cb(void *data, Evas_Object *obj, void *event_info)
{
	set_the_selected_language(obj);

	_scim_config->write(CONFIG_CURRENT_LANGUAGE, _setup_info.current_language);
	_scim_config->write(CONFIG_LANGUAGE_COUNT, _setup_info.lang_count);
	_scim_config->write(CONFIG_LANGUAGE_LIST, v_lang_list);
	_scim_config->reload();

	String display_name = String(ISF_DISPLAY);
	const char *p = getenv("DISPLAY");
	if (p != NULL)
		display_name = String(p);

	HelperAgent helper_agent;
	HelperInfo helper_info(ISF_UUID, "ISF Setting", "", "", SCIM_HELPER_STAND_ALONE);
	int id = helper_agent.open_connection(helper_info, display_name);

	if (id == -1) {
		std::cerr << "    open_connection failed!!!!!!\n";
	} else {
		helper_agent.reload_config();
		helper_agent.close_connection();
	}

	strncpy(itemdata[LANGUAGE_ID].text, LANGUAGE, ITEM_DATA_STRING_LEN - 1);
	strncpy(itemdata[LANGUAGE_ID].sub_text,
		gettext(Lang_count_List[_setup_info.lang_count]),
		ITEM_DATA_STRING_LEN - 1);
	if (keypad_subdata.lang_item) {
		elm_genlist_item_update(keypad_subdata.lang_item);
	}

	ad.First_item = NULL;
}

static void _show_language_selection_view(Evas_Object *navi_frame)
{
	/* selecting writing language */
	Evas_Object *lang_list = NULL;
	Elm_Object_Item *list_item = NULL;
	Evas_Object *rdg = NULL;
	Evas_Object *item = NULL;
	Evas_Object *back_btn = NULL;


	ad.First_item = NULL;
	lang_list = create_list_view(ad.naviframe);

	for (unsigned int loop = 0; loop < MAX_LANG_NUM; loop++) {
		item = create_check_button(lang_list);
		evas_object_propagate_events_set(item, EINA_FALSE);

		if (IseLangDataSelectState[loop])
			elm_check_state_set(item, 1);
		else
			elm_check_state_set(item, 0);
		evas_object_smart_callback_add(item, "changed",
									_lang_check_button_toggled,
									(void *)lang_list);

		list_item = elm_list_item_append(lang_list,
									IseLangData
									[KEYPAD_QWERTY]
									[loop].displayname, item, NULL,
									_lang_list_sel,
									(void *)lang_list);

		if (loop == 0)
			ad.First_item = list_item;
	}

	elm_list_go(lang_list);
	evas_object_show(lang_list);

	Elm_Object_Item *navi_it = elm_naviframe_item_push(ad.naviframe, LANGUAGE, NULL, NULL, lang_list, NULL);
	back_btn = elm_object_item_part_content_get(navi_it, "elm.swallow.prev_btn");

	evas_object_smart_callback_add(back_btn, "clicked", _lang_set_cb, NULL);
	evas_object_data_set(lang_list, "back_button", back_btn);
}

static void option_list_clicked(void *data, Evas_Object *obj, void *event_info)
{
	char *str = (char *)data;
	Evas_Event_Mouse_Up *ev = (Evas_Event_Mouse_Up *) event_info;
	if (ev && ev->event_flags & EVAS_EVENT_FLAG_ON_HOLD) {
		return;
	}

	if (!strcmp(str, LANGUAGE))
		_show_language_selection_view(ad.naviframe);
}

static void _option_cancel_cb(void *data, Evas_Object *obj, void *event_info)
{
	clean_up();
}

static void _option_done_cb(void *data, Evas_Object *obj, void *event_info)
{
	set_the_selected_language(obj);

	clean_up();
}

void _show_option_window(Evas_Object *parentWidget, mcfint degree)
{
	Evas_Object *window = NULL;
	Evas_Object *layout = NULL;
	bool indicator = FALSE;
	Evas_Object *cancel_btn, *done_btn;

	ad.rotation_angle = degree;
	ad.parent = NULL;
	ad.openedViaGadget = TRUE;

	/* push into naviframe */
	cancel_btn = elm_button_add(ad.naviframe);
	evas_object_smart_callback_add(cancel_btn, "clicked", _option_done_cb, NULL);

	elm_naviframe_item_push(ad.naviframe, OPTIONS, cancel_btn, NULL, option_list, NULL);
	elm_object_style_set(cancel_btn, "naviframe/back_btn/default");

	return;
}

static Ecore_X_Window get_isf_active_window()
{
	/* Set a transient window for window stack */
	/* Gets the current XID of the active window into the root window property  */
	Atom type_return;
	unsigned long nitems_return;
	unsigned long bytes_after_return;
	int format_return;
	unsigned char *data;
	Ecore_X_Window xAppWindow = NULL;
	gint ret = 0;

	ret = XGetWindowProperty((Display *) ecore_x_display_get(),
							ecore_x_window_root_get(elm_win_xwindow_get(main_window)),
							ecore_x_atom_get("_ISF_ACTIVE_WINDOW"), 0,
							G_MAXLONG, False, ((Atom) 33), &type_return,
							&format_return, &nitems_return,
							&bytes_after_return, &data);

	if (ret == Success) {
		if ((type_return == ((Atom) 33)) && (format_return == 32)
		    && (data)) {
			xAppWindow = *(Window *) data;
			if (data)
				XFree(data);
		}
	}
	return xAppWindow;
}

static void set_transient_for_app_window(Evas_Object *option_win)
{
	Ecore_X_Window xOptionWin = elm_win_xwindow_get(option_win);
	ecore_x_icccm_transient_for_set(xOptionWin, get_isf_active_window());
}

void
_show_option_window_ise(Evas_Object *parentWidget, mcfint degree,
								SETTING_INFO previnfo,
								void mainback(SETTING_INFO currentInfo))
{
	Evas_Object *option_list = NULL, *window = NULL;
	Evas_Object *layout = NULL, *bg = NULL;
	bool indicator = FALSE;
	callback = mainback;
	ad.openedViaGadget = FALSE;
	_setup_info = previnfo;
	sensor_handle = sf_connect(ACCELEROMETER_SENSOR);
	if (sensor_handle < 0) {
		printf("sensor attach fail\n");

	}
	int state =
	    sf_register_event(sensor_handle, ACCELEROMETER_EVENT_ROTATION_CHECK,
			      NULL, rotation_callback_func, NULL);
	if (state < 0) {
		printf("sensor_register_cb fail\n");

	}

	state = sf_start(sensor_handle, 0);
	if (state < 0) {
		printf("SLP_sensor_start fail\n");
	}
	printf("Start SF done\n");

	/* Do not open option window if our ISE was
	 * called by option window itself ( dictionary) */

	for (unsigned int loop = 0; loop < MAX_LANG_NUM; loop++) {
		if (IseLangData[KEYPAD_QWERTY][loop].name) {
			scim::String str =
			    scim::scim_get_language_name_english(IseLangData
								 [KEYPAD_QWERTY]
								 [loop].name);
			//strncpy(IseLangData[KEYPAD_QWERTY][loop].displayname, str.c_str(), 255);
			strncpy(IseLangData[KEYPAD_QWERTY][loop].displayname, dgettext(GETTEXT_PACKAGE, language_text_name[loop]), 255);
		}
	}

	if (ad.option_window) {
		window = ad.option_window;
		elm_win_rotation_with_resize_set(ad.option_window, degree);
		evas_object_show(ad.option_window);
		indicator = TRUE;
	} else {
		memset(&ad, 0x0, sizeof(Ise_Settings));
		ad.rotation_angle = degree;
		/* create option window */
		window = create_main_window();
		ad.option_window = window;
		elm_win_indicator_mode_set(window, ELM_WIN_INDICATOR_SHOW);
		indicator = FALSE;
	}

	/* create main layout */
	ad.parent = parentWidget;
	bg = create_bg(window);
	layout = create_layout(window);
	ad.option_layout = layout;
	evas_object_show(layout);
	set_transient_for_app_window(window);
	if(window)
		elm_win_raise (window);

	ad.naviframe = create_naviframe(layout);
	elm_object_part_content_set(layout, "elm.swallow.content", ad.naviframe);
	option_list = create_option_mainview(layout);
	/* push into naviframe */
	Evas_Object *cancel_btn;

	cancel_btn = elm_button_add(ad.naviframe);
	evas_object_smart_callback_add(cancel_btn, "clicked", _option_done_cb,
				       NULL);

	elm_naviframe_item_push(ad.naviframe, OPTIONS, cancel_btn, NULL, option_list, NULL);
	elm_object_style_set(cancel_btn, "naviframe/back_btn/default");

	/*For focus-out */
	evtHandler =
	    ecore_event_handler_add(ECORE_X_EVENT_WINDOW_FOCUS_OUT, _ise_focus_out_cb,
				    NULL);

	/* This is necessary to update keyboard options when
	 * keboard layout is changed by long press 123 button */
	set_keyboard_option(true, _setup_info);
}

