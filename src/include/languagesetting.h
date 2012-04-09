/*
Copyright (c) 2000-2012 Samsung Electronics Co., Ltd All Rights Reserved

This file is part of ise-default
Written by Ashish Malhotra <a.malhotra@samsung.com>

PROPRIETARY/CONFIDENTIAL

This software is the confidential and proprietary information of
SAMSUNG ELECTRONICS ("Confidential Information"). You shall not
disclose such Confidential Information and shall use it only in
accordance with the terms of the license agreement you entered
into with SAMSUNG ELECTRONICS.

SAMSUNG make no representations or warranties about the suitability
of the software, either express or implied, including but not limited
to the implied warranties of merchantability, fitness for a particular
purpose, or non-infringement. SAMSUNG shall not be liable for any
damages suffered by licensee as a result of using, modifying or
distributing this software or its derivatives.
*/

#include "ise.h"
#include "mcf.h"
#include "ise-default-setting.h"
#include <gmodule.h>

#define Uses_SCIM_UTILITY
#define Uses_SCIM_OBJECT
#define Uses_SCIM_POINTER
#define Uses_SCIM_EVENT
#define Uses_SCIM_HELPER
#define Uses_SCIM_CONFIG_BASE

#ifndef __ISE_LANGUAGESETTING_H__
#define __ISE_LANGUAGESETTING_H__

#ifdef __cplusplus
MCF_BEGIN_DECLS
#endif

#define _EDJ(x) elm_layout_edje_get(x)

#define LANGUAGE	dgettext(GETTEXT_PACKAGE, "IDS_IME_BODY_KEYBOARD_LANGUAGES")	//gettext("IDS_IME_BODY_KEYBOARD_LANGUAGES")
#define OPTIONS		dgettext(GETTEXT_PACKAGE, "IDS_IME_BODY_KEYBOARD_SETTINGS")	//gettext("IDS_IME_BODY_KEYBOARD_SETTINGS")

#define BASE_THEME_WIDTH 720.0f
#define BASE_THEME_HEIGHT 1280.0f

typedef struct _SETTING_INFO
{
	unsigned char flag;
	int current_language;
	int lang_count;
} SETTING_INFO;

typedef enum _SoftKeyOptions {
	OPTIONS_WINDOW_BACK,
	LANG_WINDOW_SET,
	LANG_WINDOW_CANCEL,
	KEYBOARD_TYPES_WINDOW_SET,
	KEYBOARD_TYPES_WINDOW_CANCEL,
	MAX_SOFTKEY
}SoftKeyOptions;

typedef struct _ISELangData {
	char* name;
	char* imgPath;
	char* imgPath_for_language_settings;
	int inputMode;
	char displayname[256];
}ISELangData;

SETTING_INFO get_keyboard_option (void);

void set_keyboard_option (bool isEuropeanMode, const SETTING_INFO &info);


/**
 * Create setup genlist when options key is pressed in setting menu.
	@param[in] layout_main 	 Main layout
	@param[in] navi_bar 	 Navigation bar
	@param[in] info 	 	 Setting options
	@return 				Nothing.
 *
 **/
Evas_Object *create_setup_genlist (Evas_Object *layout_main, Evas_Object *navi_bar, SETTING_INFO info);


/**
 * Shows the options window when options key is pressed in setting menu.
	@param[in] parentWidget 	 Mainwindow refernce
	@return 				Nothing.
 *
 **/
void
_show_option_window(Evas_Object * parentWidget);


/**
 * Shows the options window when options key is pressed in keypad.
	@param[in] parentWidget 	 Mainwindow refernce
	@param[in] info 	 	 Setting options
	@param[in] callback      Keyboard setting callback
	@return 				Nothing.
 *
 **/
void
_show_option_window_ise(Evas_Object *parentWidget, SETTING_INFO info, void mainback(SETTING_INFO currentInfo));


/**
 * clean the selection window.
	@return 				Nothing.
 *
 **/
void
clean_up();


#ifdef __cplusplus
MCF_END_DECLS
#endif

#endif
