/*
 * Copyright (c) 2012 - 2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <scl.h>

#include "ise.h"
#include "config.h"

#ifndef __OPTION_H__
#define __OPTION_H__

#define LANGUAGE		gettext("IDS_IME_BODY_WRITING_LANGUAGES")
#define PREDICTION		gettext("IDS_IME_BODY_PREDICTION")
#define KEYPAD			gettext("IDS_IME_BODY_KEYBOARD_TYPE")
#define KEYPAD_3X4		gettext("IDS_IME_OPT_3_X_4_KEYBOARD")
#define KEYPAD_QTY		gettext("IDS_IME_OPT_QWERTY")
#define OPTIONS			gettext("IDS_IME_BODY_KEYBOARD_SETTINGS")

#define MSG_NONE_SELECTED			gettext("IDS_IME_BODY_YOU_MUST_SELECT_AT_LEAST_ONE_LANGUAGE_IN_KEYBOARD_SETTINGS")

#define PORTRAIT_DEGREE_0			0
#define PORTRAIT_DEGREE_180			180
#define LANDSCAPE_DEGREE_270		270
#define LANDSCAPE_DEGREE_90			90

#define ITEM_DATA_STRING_LEN 64
struct ITEMDATA
{
    sclchar main_text[ITEM_DATA_STRING_LEN];
    sclchar sub_text[ITEM_DATA_STRING_LEN];
    sclint mode;
    ITEMDATA()
    {
        memset(main_text, 0, sizeof(sclchar)*ITEM_DATA_STRING_LEN);
        memset(sub_text, 0, sizeof(sclchar)*ITEM_DATA_STRING_LEN);
        mode = 0;
    }
};

class ILanguageOption {
public:
    virtual void on_create_option_main_view(Evas_Object *genlist, Evas_Object *naviframe) {}
    virtual void on_destroy_option_main_view() {}
};

class LanguageOptionManager {
public:
    static void add_language_option(ILanguageOption *language_option);
    static scluint get_language_options_num();
    static ILanguageOption* get_language_option_info(scluint index);
private:
    static std::vector<ILanguageOption*> language_option_vector;
};

/**
 * Shows the options window when options key is pressed in keypad.
	@param[in] parent			Parent elementary widget
	@param[in] degree 			Rotation angle
	@return 					Nothing.
 * 
 **/
void
open_option_window(Evas_Object *parent, sclint degree);

/**
 * Closes option window
 	@return 					Nothing.
 * 
 **/
void
close_option_window();

void
read_options();

void
write_options();

#endif
