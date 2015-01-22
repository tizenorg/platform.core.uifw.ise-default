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

#include <string>
#include <vector>

#include <scl.h>

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define ISE_CONFIG_KEYPAD_MODE          "/ise-default/keypad_mode"
#define ISE_CONFIG_PREDICTION_ON        "/ise-default/prediction_on"
#define ISE_CONFIG_ENABLED_LANGUAGES    "/ise-default/enabled_languages"
#define ISE_CONFIG_SELECTED_LANGUAGE    "/ise-default/selected_language"

enum KEYPAD_MODE {
    KEYPAD_MODE_3X4,
    KEYPAD_MODE_QTY,
};

struct CONFIG_VALUES {
    CONFIG_VALUES();

    KEYPAD_MODE keypad_mode;
    sclboolean prediction_on;
    std::string selected_language;
    std::vector<std::string> enabled_languages;
};

/**
 * Reads all option values from SCIM config file
    @return		Nothing.
 * 
 **/
void
read_ise_config_values();

/**
 * Writes all option values to SCIM config file
    @return		Nothing.
 * 
 **/
void
write_ise_config_values();

/**
 * Resets all option values by erasing them
    @return		Nothing.
 *
 **/
void
erase_ise_config_values();


#endif
