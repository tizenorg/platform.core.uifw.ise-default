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

#define LANGUAGE	dgettext(GETTEXT_PACKAGE, "IDS_IME_BODY_WRITING_LANGUAGES")
#define OPTIONS		dgettext(GETTEXT_PACKAGE, "IDS_IME_BODY_KEYBOARD_SETTINGS")

#define PORTRAIT_DEGREE_0	0
#define PORTRAIT_DEGREE_180	180
#define LANDSCAPE_DEGREE_270	270
#define LANDSCAPE_DEGREE_90	90

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
	@param[in] degree 	 Rotation angle
	@return 				Nothing.
 *
 **/
void
_show_option_window(Evas_Object * parentWidget, mcfint degree);


/**
 * Shows the options window when options key is pressed in keypad.
	@param[in] parentWidget 	 Mainwindow refernce
	@param[in] degree 	 Rotation angle
	@param[in] info 	 	 Setting options
	@param[in] callback      Keyboard setting callback
	@return 				Nothing.
 *
 **/
void
_show_option_window_ise(Evas_Object *parentWidget, mcfint degree, SETTING_INFO info, void mainback(SETTING_INFO currentInfo));


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
