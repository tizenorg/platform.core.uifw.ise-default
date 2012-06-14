/*
 * Copyright 2012  Samsung Electronics Co., Ltd
 *
 * Licensed under the Flora License, Version 1.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.tizenopensource.org/license
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
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

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <scim.h>
#include <X11/Xlib.h>

#include <glib.h>

#include <Elementary.h>

#include "ise.h"
#include "isedata.h"
#include "strtbl.h"
#include "window_slide_animator.h"

using namespace scim;

#include <Ecore_X.h>
#include <Ecore_IMF.h>
#include <Elementary.h>

#include <heynoti.h>
#include <vconf.h>
#include <vconf-keys.h>
#include <ise_context.h>

#define SETTING_LANG			"file/menu_widget/language"

extern ConfigPointer _scim_config;
extern SETTING_INFO _setup_info;
extern std::vector < int >v_lang_list;
extern mcfint privateId;
extern Evas_Object* ctxspopup;
/* Function declaration */
static void run(const String &display);

static int main_window_xpos = 0;
static int main_window_ypos = 0;
static int screen_width;
static int screen_height;

static int candidate_height = 79;
static int ise_height = 410;

/* Global variables */
extern Evas_Object *main_window;
extern HelperAgent helper_agent;
extern unsigned int gExplicitLanguageSetting;
extern bool gFHiddenState;
static int gLastIC = 0;
extern bool IseLangDataSelectState[MAX_LANG_NUM];

extern int init(const String &display);
extern void fini();

static HelperInfo helper_info(String(ISEUUID), ISENAME, "", "",
#ifndef DEBUG_MODE
			      SCIM_HELPER_STAND_ALONE |
			      SCIM_HELPER_NEED_SCREEN_INFO |
			      SCIM_HELPER_AUTO_RESTART);
#else
			      SCIM_HELPER_STAND_ALONE |
			      SCIM_HELPER_NEED_SCREEN_INFO);
#endif

void save_config_ise(const ConfigPointer &config);

extern "C" {
#ifndef SETUP_BUILD
	void scim_module_init(void)
	{
		DBG("%s\n", __func__);
		helper_info.name = String(ISENAME);
		helper_info.description =
		    String("An on-screen ise to input characters.");
	}
	void scim_module_exit(void)
	{
		DBG("%s\n", __func__);
	}
#endif
	unsigned int scim_helper_module_number_of_helpers(void)
	{
		DBG("%s\n", __func__);
		return 1;
	}

	bool scim_helper_module_get_helper_info(unsigned int idx,
						HelperInfo &info)
	{
		DBG("%s\n", __func__);
		if (idx == 0) {
			info = helper_info;
			return true;
		}
		return false;
	}

	String scim_helper_module_get_helper_language(unsigned int idx)
	{
		DBG("%s\n", __func__);
		std::vector < String > langlist;
		langlist.push_back(SUPPORTED_LANGUAGES);
		return scim_combine_string_list(langlist);
	}

	void scim_helper_module_run_helper(const String &uuid,
					   const ConfigPointer &config,
					   const String &display)
	{
		DBG("%s\n", __func__);

		if (!config.null()) {
			_scim_config = config;
			_setup_info.flag = config->read(CONFIG_SETTING_FLAG, 0);
			config->read(CONFIG_LANGUAGE_LIST, &v_lang_list);
			_setup_info.current_language = config->read(CONFIG_CURRENT_LANGUAGE, 0);
			_setup_info.lang_count = config->read(CONFIG_LANGUAGE_COUNT, 1);

			for (unsigned int loop = 0; loop < MAX_LANG_NUM; loop++) {
				IseLangDataSelectState[loop] = false;
			}

			for (unsigned int i = 0; i < v_lang_list.size(); i++)
				IseLangDataSelectState[v_lang_list[i]] = true;
		}
		DBG("ise:>>>> Done with assigning scim config \n\n");

		if (uuid == ISEUUID) {
			run(display);
		}
	}
}

static void set_transient_for_app_window(Evas_Object *keypad_win)
{
	/* Set a transient window for window stack */
	/* Gets the current XID of the active window into the
	 * root window property  */
	Atom type_return;
	unsigned long nitems_return;
	unsigned long bytes_after_return;
	int format_return;
	unsigned char *data;
	Ecore_X_Window xAppWindow;
	Ecore_X_Window xKeypadWin = elm_win_xwindow_get(keypad_win);
	gint ret = 0;

	ret = XGetWindowProperty((Display *) ecore_x_display_get(),
					ecore_x_window_root_get(xKeypadWin),
					ecore_x_atom_get("_ISF_ACTIVE_WINDOW"), 0,
					G_MAXLONG, False, ((Atom) 33), &type_return,
					&format_return, &nitems_return,
					&bytes_after_return, &data);

	if (ret == Success) {
		if ((type_return == ((Atom) 33)) && (format_return == 32) && (data)) {
			xAppWindow = *(Window *) data;
			ecore_x_icccm_transient_for_set(xKeypadWin, xAppWindow);
			if (data)
				XFree(data);
		}
	}
}

static void slot_exit(const HelperAgent *, int ic, const String &uuid)
{
	DBG("%s\n", __func__);

	ise_hide(TRUE);

	elm_exit();
}

static void
slot_update_screen(const HelperAgent *, int ic,
	   	const String &uuid, int screen)
{
	DBG("%s\n", __func__);
	return;
}

static void
slot_trigger_property(const HelperAgent *agent, int ic, const String &uuid,
		      const String &property)
{
	DBG("%s\n", __func__);
}

static bool bind_ise_window_to_im()
{
	DBG("%s\n", __func__);
	if (main_window == NULL) {
		scim_global_config_update();
		int socId = scim_global_config_read("/IMWindowSocketID", socId);
	}
	return TRUE;
}

static void slot_focus_out(const HelperAgent *, int ic, const String &uuid)
{
	DBG("isemain.cpp  slot_focus_out \n\n");
	ise_focus_out();
}

static void slot_focus_in(const HelperAgent *, int ic, const String &uuid)
{
	DBG("\n================\n%s\n============\n", __func__);

	ise_set_mode();
	ise_focus_in();
}

static int get_root_window_degree()
{
	Atom type_return;
	unsigned long nitems_return;
	unsigned long bytes_after_return;
	int format_return;
	unsigned char *data;
	gint ret = 0;
	int retVal = 0;
	ret =
	    XGetWindowProperty((Display *) ecore_x_display_get(),
				ecore_x_window_root_get(elm_win_xwindow_get(main_window)),
				ecore_x_atom_get("_E_ILLUME_ROTATE_ROOT_ANGLE"),
				0, G_MAXLONG, False, ((Atom) 6), &type_return,
				&format_return, &nitems_return,
				&bytes_after_return, &data);

	if (ret == Success) {
		if ((type_return == ((Atom) 6)) && (format_return == 32)
			&& (data)) {
			retVal = *(unsigned int *)data;
			if (data)
				XFree(data);
		}
	}

	return retVal;
}

static void slot_ise_show(const HelperAgent *, int ic, char *buf, size_t &len)
{
	DBG("\n-------------------\n%s\n-------------------, %d\n",
		   __func__, ic);
	unsigned int i;

	if (main_window) {
		evas_object_show(main_window);
		/*elm_win_raise (main_window);*/
	}

	/* Set a transient window for window stack */
	set_transient_for_app_window(main_window);

	static const char *UUID = KEYBD_ISE_UUID;
	helper_agent.set_keyboard_ise_by_uuid(UUID);

	/* Check if effect is enabled */
	static Ise_Context iseContext;

	if (len >= sizeof(Ise_Context)) {
		/* for EFL application*/
		memcpy(&iseContext, buf, sizeof(iseContext));

		ise_reset_context();	/* reset ISE*/

		ise_explicitly_set_language(iseContext.language);

		ise_set_layout(iseContext.layout);

		ise_set_return_key_type(iseContext.return_key_type);

		ise_set_return_key_disable(iseContext.return_key_disabled);

		ise_set_caps_mode(iseContext.caps_mode);

	} else {
		DBG("\n-=-=-=-=-=-=-= WARNING WARNING WARNING  - slot_ise_show :\
				buf %p len %d size %d =-=-=-=-=-=-=-=-=-=-\n",
		buf, len, sizeof(iseContext));
	}

	ise_show(ic);
	gLastIC = ic;
}

static void slot_ise_hide(const HelperAgent *, int ic, const String &uuid)
{
	DBG("%s\n", __func__);
	CMCFWindows *windows = CMCFWindows::get_instance();
	Evas_Object* win = windows->get_context_popup();
	if(win == NULL)
	  ise_hide(TRUE);
}

static void slot_get_size(const HelperAgent *, struct rectinfo &info)
{
	DBG("%s\n", __func__);

	int x, y, width, height;
	ise_get_size(&x, &y, &width, &height);

	info.pos_x = x;
	info.pos_y = y;
	info.width = width;
	info.height = height;
}

static void slot_set_caps_mode(const HelperAgent *, uint32 &mode)
{
	ise_set_caps_mode(mode);
}

static void slot_set_language(const HelperAgent *, uint32 &language)
{
	ise_explicitly_set_language(language);
}

static void slot_set_layout(const HelperAgent *, uint32 &layout)
{
	ise_set_layout(layout);
}

static void slot_get_layout(const HelperAgent *, uint32 &layout)
{
	layout = ise_get_layout();
}

static void slot_update_cursor_position(const HelperAgent *, int ic, const String &uuid, int position)
{
	ise_update_cursor_position(position);
}

static void slot_get_language_locale(const HelperAgent *, int ic, char **locale)
{
    ise_get_language_locale(locale);
}
static void slot_get_layout_list(const HelperAgent *,
				 std::vector < uint32 > &list)
{
	list.clear();
#ifdef SUPPORTS_LAYOUT_STYLE_NORMAL
	list.push_back(ISE_LAYOUT_STYLE_NORMAL);
#endif
#ifdef SUPPORTS_LAYOUT_STYLE_NUMBER
	list.push_back(ISE_LAYOUT_STYLE_NUMBER);
#endif
#ifdef SUPPORTS_LAYOUT_STYLE_EMAIL
	list.push_back(ISE_LAYOUT_STYLE_EMAIL);
#endif
#ifdef SUPPORTS_LAYOUT_STYLE_URL
	list.push_back(ISE_LAYOUT_STYLE_URL);
#endif
#ifdef SUPPORTS_LAYOUT_STYLE_PHONENUMBER
	list.push_back(ISE_LAYOUT_STYLE_PHONENUMBER);
#endif
#ifdef SUPPORTS_LAYOUT_STYLE_IP
	list.push_back(ISE_LAYOUT_STYLE_IP);
#endif
#ifdef SUPPORTS_LAYOUT_STYLE_MONTH
	list.push_back(ISE_LAYOUT_STYLE_MONTH);
#endif
#ifdef SUPPORTS_LAYOUT_STYLE_NUMBERONLY
	list.push_back(ISE_LAYOUT_STYLE_NUMBERONLY);
#endif
}

static void slot_set_imdata(const HelperAgent *, char *buf, size_t &len)
{

}

static void slot_get_imdata(const HelperAgent *, char **buf, size_t &len)
{

}

/**
* @brief Set return key type slot function for HelperAgent.
*
* @param type The type of return key.
*
* @return void
*/
static void slot_set_return_key_type (const HelperAgent *, uint32 &type)
{
	ise_set_return_key_type(type);
}

/**
* @brief Set return key disable slot function for HelperAgent.
*
* @param disabled The disable flag of return key.
*
* @return void
*/
static void slot_set_return_key_disable (const HelperAgent *, uint32 &disabled)
{
	ise_set_return_key_disable(disabled);
}

static void slot_set_private_key_by_label(const HelperAgent *,
							uint32 &layout_idx,
							uint32 &key_idx,
							char *label, char *value)
{
	std::cout << "<now> slot_set_private_key_by_label \n";
	std::cout << "\tlayout_idx = " << layout_idx << "\n";
	std::cout << "\tkey_idx = " << key_idx << "\n";
	std::cout << "\tlabel = " << label << "\n";
	std::cout << "\tvalue = " << value << "\n";
	ise_set_private_key(key_idx, label, NULL, -1, value);
}

static void slot_set_private_key_by_image(const HelperAgent *,
							uint32 &layout_idx,
							uint32 &key_idx,
							char *img_path, char *value)
{
	std::cout << "<now> slot_set_private_key_by_imagel \n";
	std::cout << "\tlayout_idx = " << layout_idx << "\n";
	std::cout << "\tkey_idx = " << key_idx << "\n";
	std::cout << "\tlabel = " << img_path << "\n";
	std::cout << "\tvalue = " << value << "\n";
	ise_set_private_key(key_idx, NULL, img_path, -1, value);
}

static void slot_set_disable_key(const HelperAgent *,
							uint32 &layout_idx,
							uint32 &key_idx, uint32 &disabled)
{
	std::cout << "<now> slot_set_disable_key \n";
	std::cout << "\tlayout_idx = " << layout_idx << "\n";
	std::cout << "\tkey_idx = " << key_idx << "\n";
	std::cout << "\tdisabled = " << disabled << "\n";

	ise_set_disable_key(key_idx, disabled);
}

static void slot_reset_ise_context(const HelperAgent *, int ic,
				   const String &uuid)
{
	DBG("%s\n", __func__);
	ise_reset_context();
}

static void slot_reset_ise_input_context(const HelperAgent *, int ic,
						const String &uuid)
{
	std::cout << "<now> slot_reset_ise_input_context\n";
	ise_reset_input_context();
}

static void slot_set_screen_direction(const HelperAgent *, uint32 &mode)
{
	std::cout << "<now> slot_set_screen_direction" << mode << " \n";
	return;
}

static void slot_update_keyboard_ise(const HelperAgent *, int ic,
						const String &uuid,
						const String &ise_name,
						const String &ise_uuid)
{
	static int failnum = 0;
	static const char *UUID = KEYBD_ISE_UUID;

	if (uuid != UUID) {
		failnum++;
		if (failnum < 5) {
			DBG("***Warning : KEYBOARDISE is not  ISE!!! Retrying ... %d\n", failnum);
			helper_agent.set_keyboard_ise_by_uuid(UUID);
		} else {
			DBG("***Warning : KEYBOARDISE is not  ISE!!! Giving up\n");
		}
	} else {
		failnum = 0;
	}
}

static void slot_update_keyboard_ise_list(const HelperAgent *, int ic,
						const String &uuid,
						const std::vector < String >
						&ise_list)
{

}

static void slot_update_candidate_rect(const HelperAgent *, int ic,
						const String &uuid,
						const rectinfo &info)
{

	std::cout << "    x=" << info.pos_x << " y=" << info.
		pos_y << " width=" << info.width << " height=" << info.
		height << "\n";
	candidate_height = info.height;
}

static void slot_update_spot_location(const HelperAgent *agent, int ic,
				      const String &ic_uuid, int x, int y)
{
	DBG("%s\n", __func__);
}

static Eina_Bool helper_agent_input_handler(void *data,
					    Ecore_Fd_Handler *fd_handler)
{
	DBG("%s\n", __func__);

	if (helper_agent.has_pending_event()) {
		if (!helper_agent.filter_event()) {
			std::cerr <<"helper_agent.filter_event () is failed!!!\n";
			elm_exit();
		}
	} else {
		std::cerr << "helper_agent.has_pending_event () is failed!!!\n";
		elm_exit();
	}

	return ECORE_CALLBACK_RENEW;
}

static void get_screen_size(int &width, int &height)
{
	Display *d = (Display *) ecore_x_display_get();
	if (d == NULL) {
		DBG("ecore_x_display_get () is failed!!!\n");;
		return;
	}

	int screen_num = DefaultScreen(d);
	width = DisplayWidth(d, screen_num);
	height = DisplayHeight(d, screen_num);
	DBG("\n\n width=%d   height=%d \n\n", width, height);
}

static void slot_reload_config_callback(const HelperAgent *, int ic,
					const String &uuid)
{
	CMCFCore *gCore = NULL;

	if (!_scim_config.null()) {
		_setup_info.flag = _scim_config->read(CONFIG_SETTING_FLAG, _setup_info.flag);
		_scim_config->read(CONFIG_LANGUAGE_LIST, &v_lang_list);
		_setup_info.current_language = _scim_config->read(CONFIG_CURRENT_LANGUAGE,
				       _setup_info.current_language);
		_setup_info.lang_count = _scim_config->read(CONFIG_LANGUAGE_COUNT, _setup_info.lang_count);

		for (unsigned int loop = 0; loop < MAX_LANG_NUM; loop++) {
			IseLangDataSelectState[loop] = false;
		}

		for (unsigned int i = 0; i < v_lang_list.size(); i++)
			IseLangDataSelectState[v_lang_list[i]] = true;

		ise_set_language(_setup_info.current_language);
		/* for updating _setup_info before launching
		 * keyboard from any application
		   if the values are changed through keyboard further
		   , it will be written through setting_window_cb */
		_setup_info = ise_setting_update_on_reload_config(_setup_info);
	}
}

extern int init_i18n(const char *domain, const char *dir);

void isf_setting_language_changed_cb(keynode_t *key, void *data)
{
	DBG("###################### ISE listen that language\
			changed ###########################\n");
	char clang[256] = { 0 };
	snprintf(clang, sizeof(clang), "%s", vconf_get_str(VCONFKEY_LANGSET));
	DBG("###################### current language is %s \
			##############################\n",clang);

	if (clang[0]) {
		setenv("LANG", clang, 1);
		setlocale(LC_MESSAGES, clang);
		init_i18n(PACKAGE, LOCALEDIR);
	} else {
		setenv("LANG", "en_US.utf8", 1);
		setlocale(LC_MESSAGES, "en_US.utf8");
		init_i18n(PACKAGE, LOCALEDIR);
	}
}

void signalhandler(int sig)
{
	std::cerr << "    Signal=" << sig << "\n";
	elm_exit();
}

static Eina_Bool _client_message_cb(void *data, int type, void *event)
{
	Ecore_X_Event_Client_Message *ev = (Ecore_X_Event_Client_Message *) event;

	int angle;

	if (ev->message_type == ECORE_X_ATOM_E_ILLUME_ROTATE_ROOT_ANGLE) {
		if (!gFHiddenState) {
            CMCFWindows *windows = CMCFWindows::get_instance();
            Evas_Object* win = windows->get_context_popup();
            if(win != NULL) {
              evas_object_del(win);
              evas_object_del(ctxspopup);
            }
			angle = ev->data.l[0];
			Ecore_X_Window activeWin = ev->data.l[1];
			if (activeWin ==
				ecore_x_icccm_transient_for_get(elm_win_xwindow_get(main_window))) {
				ise_show(gLastIC);
			}
		}
	}

	return ECORE_CALLBACK_RENEW;
}

void hibernation_enter_callback(void *data)
{
	vconf_ignore_key_changed(VCONFKEY_LANGSET,
				isf_setting_language_changed_cb);
}

void hibernation_leave_callback(void *data)
{
	vconf_notify_key_changed(VCONFKEY_LANGSET,
				isf_setting_language_changed_cb, NULL);
}

void run(const String &display)
{
#ifdef SLP_PROF
	DBG("[%s] current time : %10.3f\n", __func__, cur_time_get());
#endif

	DBG("DBG : %s\n", __func__);
	PERF_TEST_START("run()");
	char clang[256] = { 0 };
	int slp_fd;
	int ret1;
	int id = helper_agent.open_connection(helper_info, display);

	if (!id) {
		DBG("\n ERROR: helper_agent open_connection id invalid \n");
	}

	init(display);
	ise_new();
	PERF_TEST_MID("\tinit()");

	get_screen_size(screen_width, screen_height);
	isf_setting_language_changed_cb(NULL, NULL);

	slp_fd = heynoti_init();
	heynoti_subscribe(slp_fd, "HIBERNATION_ENTER",
			  hibernation_enter_callback, NULL);
	heynoti_subscribe(slp_fd, "HIBERNATION_LEAVE",
			  hibernation_leave_callback, NULL);

	vconf_notify_key_changed(VCONFKEY_LANGSET,
				 isf_setting_language_changed_cb, NULL);

	snprintf(clang, sizeof(clang), "%s", vconf_get_str(VCONFKEY_LANGSET));

	for (int loop = 0; loop < MAX_LANG_NUM; loop++) {
		if (!strcmp(clang, scim::scim_get_language_name_english(IseLangData[1][loop].
									name).c_str())) {
			gExplicitLanguageSetting = loop;
			break;
		}
	}

	ret1 = heynoti_attach_handler(slp_fd);

	helper_agent.signal_connect_exit(slot(slot_exit));
	helper_agent.signal_connect_update_screen(slot(slot_update_screen));
	helper_agent.signal_connect_trigger_property(slot(slot_trigger_property));
	helper_agent.signal_connect_focus_out(slot(slot_focus_out));
	helper_agent.signal_connect_focus_in(slot(slot_focus_in));
	helper_agent.signal_connect_ise_show(slot(slot_ise_show));
	helper_agent.signal_connect_ise_hide(slot(slot_ise_hide));
	helper_agent.signal_connect_get_geometry(slot(slot_get_size));
	helper_agent.signal_connect_set_language(slot(slot_set_language));
	helper_agent.signal_connect_set_imdata(slot(slot_set_imdata));
	helper_agent.signal_connect_get_imdata(slot(slot_get_imdata));
	helper_agent.signal_connect_set_layout(slot(slot_set_layout));
	helper_agent.signal_connect_get_layout(slot(slot_get_layout));
	helper_agent.signal_connect_reset_input_context(slot(slot_reset_ise_input_context));
	helper_agent.signal_connect_update_keyboard_ise(slot(slot_update_keyboard_ise));
	helper_agent.signal_connect_update_keyboard_ise_list(slot(slot_update_keyboard_ise_list));
	helper_agent.signal_connect_update_candidate_geometry(slot(slot_update_candidate_rect));
	helper_agent.signal_connect_update_spot_location(slot(slot_update_spot_location));
	helper_agent.signal_connect_set_caps_mode(slot(slot_set_caps_mode));
	helper_agent.signal_connect_update_cursor_position( slot (slot_update_cursor_position));
	helper_agent.signal_connect_get_language_locale( slot (slot_get_language_locale));
	helper_agent.signal_connect_reload_config(slot(slot_reload_config_callback));
	helper_agent.signal_connect_set_return_key_type (slot (slot_set_return_key_type));
	helper_agent.signal_connect_set_return_key_disable (slot (slot_set_return_key_disable));
#ifndef USE_EFL
	helper_agent.signal_connect_get_geometry(slot(slot_get_size));
#endif

	helper_agent.set_candidate_position(-1, -1);

	int fd = helper_agent.get_connection_number();
	if (fd >= 0) {
		Property prop("/EuropeanISE", "EuropeanISE", "",
				"Show/Hide EuropeanISE.");
		PropertyList props;
		props.push_back(prop);
		helper_agent.register_properties(props);
		ecore_main_fd_handler_add(fd, ECORE_FD_READ,
					helper_agent_input_handler, NULL,
					NULL, NULL);
	}

	Ecore_Event_Handler *XClientMsgHandler =
					ecore_event_handler_add(ECORE_X_EVENT_CLIENT_MESSAGE,
					_client_message_cb, NULL);

	PERF_TEST_MID("\tbegin elm_run()");

	PERF_TEST_END("run()");

	signal(SIGQUIT, signalhandler);
	signal(SIGTERM, signalhandler);
	signal(SIGINT, signalhandler);
	signal(SIGHUP, signalhandler);
	elm_run();

	vconf_ignore_key_changed(VCONFKEY_LANGSET,
					isf_setting_language_changed_cb);
	ecore_event_handler_del(XClientMsgHandler);
	heynoti_close(slp_fd);
	DBG("shutdown\n");
	fini();
}
