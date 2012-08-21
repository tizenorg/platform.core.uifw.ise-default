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


#define Uses_SCIM_CONFIG_BASE

#include <X11/Xlib.h>
#include <Elementary.h>
#include <scim.h>
#include "isedata.h"
#include "languagesetting.h"
#include <Ecore_IMF.h>

using namespace scim;

SETTING_INFO _setup_info;
std::vector < int >v_lang_list;
int g_currentLanguage = 0;
ConfigPointer _scim_config;

static Evas_Object *_setup_genlist = NULL;

static void load_config(const ConfigPointer & config);
static void save_config(const ConfigPointer & config);
static void reset_config (const ConfigPointer &config);
static bool query_changed(void);

Evas_Object *main_window = NULL;

extern "C" {
	void scim_module_init(void) {
	}

        void scim_module_exit(void) {
	}

	Evas_Object *scim_setup_module_create_ui(Evas_Object * parent,
						 Evas_Object * layout) {
		if (_setup_genlist == NULL)
			_setup_genlist =
			    create_setup_genlist(parent, layout, _setup_info);

		return _setup_genlist;
	}

	String scim_setup_module_get_category(void) {
		return String("Helper");
	}

	String scim_setup_module_get_name(void) {
		return String(ISE_NAME);
	}

	String scim_setup_module_get_description(void) {
		return String("Setup Module of" ISE_NAME ".");
	}

	void scim_setup_module_load_config(const ConfigPointer & config) {
		load_config(config);

		if (_setup_genlist != NULL)
			set_keyboard_option(true, _setup_info);
	}

	void scim_setup_module_save_config(const ConfigPointer & config) {
		_setup_info = get_keyboard_option();
		save_config(config);
	}

	bool scim_setup_module_query_changed(void) {
		return query_changed();
	}

	bool scim_setup_module_key_proceeding(int key_type) {
		return false;
	}
    bool scim_setup_module_option_reset(const ConfigPointer &config)
    {
        reset_config(config);
        return true;
    }
}				/* extern "C" */

static void load_config(const ConfigPointer & config)
{
	if (!config.null()) {

		_setup_info.flag = config->read(CONFIG_SETTING_FLAG, 0x000);
		config->read(CONFIG_LANGUAGE_LIST, &v_lang_list);
		_setup_info.current_language = config->read(CONFIG_CURRENT_LANGUAGE, 0);
		_setup_info.lang_count = config->read(CONFIG_LANGUAGE_COUNT, 1);
		g_currentLanguage = _setup_info.current_language;
		_scim_config = config;
	}
}

static void save_config(const ConfigPointer & config)
{
	if (!config.null()) {
		config->write(CONFIG_SETTING_FLAG, _setup_info.flag);
		config->write(CONFIG_LANGUAGE_LIST, v_lang_list);
		config->write(CONFIG_CURRENT_LANGUAGE, _setup_info.current_language);
		config->write(CONFIG_LANGUAGE_COUNT, _setup_info.lang_count);
		config->flush();
	}
}

static void reset_config (const ConfigPointer &config)
{
    if (!config.null ())
    {
        config->erase (CONFIG_SETTING_FLAG);
        config->erase (CONFIG_LANGUAGE_LIST);
        config->erase (CONFIG_CURRENT_LANGUAGE);
        config->erase (CONFIG_LANGUAGE_COUNT);
        config->flush ();
    }
}
static bool query_changed(void)
{
	return FALSE;
}
