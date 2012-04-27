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

#ifndef _DEFAULTISE_H_
#define _DEFAULTISE_H_

#include <Ecore.h>
#include <Evas.h>
#include <Ecore_Evas.h>
#include "perf_test.h"
#include "mcf.h"
#include "ise-default-setting.h"
#include "languagesetting.h"
#include <Ecore_IMF.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif  /*  */

/*#define DEBUG_MODE*/

/*#define PACKAGE					ISE_NAME*/
#define LOCALEDIR					"/usr/share/locale"

#define CONFIG_SETTING_FLAG		ISE_NAME "/setting_flag"
#define CONFIG_LANGUAGE_LIST		ISE_NAME "/language_list"
#define CONFIG_CURRENT_LANGUAGE	ISE_NAME "/current_language"
#define CONFIG_LANGUAGE_COUNT		ISE_NAME "/lang_count"


#define INPUT_MODE_NATIVE	MAX_INPUT_MODE /* Native mode. It will distinguish to the current user language */

#define ISE_RELEASE_AUTOCOMMIT_BLOCK_INTERVAL	1300
#define ISE_RELEASE_AUTOCOMMIT_BLOCK_INTERVAL_FOR_SHIFT	1

#define WWWCOM_BUTTON_WWW_STR		"www."
#define WWWCOM_BUTTON_COM_STR		".com"

#ifndef DBG
#define DBG printf
#endif

typedef enum _IseLayout {
	ISE_LAYOUT_STYLE_NORMAL = 0,
	ISE_LAYOUT_STYLE_NUMBER,
	ISE_LAYOUT_STYLE_EMAIL,
	ISE_LAYOUT_STYLE_URL,
	ISE_LAYOUT_STYLE_PHONENUMBER,
	ISE_LAYOUT_STYLE_IP,
	ISE_LAYOUT_STYLE_MONTH,
	ISE_LAYOUT_STYLE_NUMBERONLY,
	MAX_ISE_LAYOUT_CNT
}IseLayout;

#ifdef HAVE_CONFORMANT_AUTOSCROLL
typedef enum _Virtual_Keyboard_State
{
	KEYPAD_STATE_UNKNOWN = 0,
	KEYPAD_STATE_OFF,
	KEYPAD_STATE_ON,
} Virtual_Keyboard_State;

void _send_keypad_geom_atom_info(Evas_Object *window, Virtual_Keyboard_State kbd_state);
#endif

/**
*@brief ISE Default value case by ISE layout
*/
typedef struct _ISEDefaultValueTable {
	IseLayout iseLayout;
	mcfu8 mcfInputMode;
	mcf8 KeypadMode;
	mcf8 InputMode;
	mcf8 ShiftMode;
	mcf8 OnOff;
	mcf8 CompletionMode;
	mcf8 SingleCommit;
	mcfboolean resetOnModeChange;
    mcfint subLayoutID;
}ISEDefaultValueTable;

#define ISE_PRIVATE_KEY_BUFFER_SIZE 16
#define ISE_PRIVATE_KEY_LABEL_LEN 16
#define ISE_PRIVATE_KEY_VALUE_LEN 16
#define ISE_PRIVATE_KEY_IMAGE_LEN 256

typedef struct _ISEPrivateKeyBuffer {
	mcfboolean used;
	mcfchar label[ISE_PRIVATE_KEY_LABEL_LEN];
	mcfint valueIdx;
	mcfchar valueStr[ISE_PRIVATE_KEY_VALUE_LEN];
	mcfchar ImgPath[ISE_PRIVATE_KEY_IMAGE_LEN];
	mcfchar *imgPathArr[MCF_BUTTON_STATE_MAX];
	mcfint privateId;
	mcfint layoutIdx;
	mcfint keyIdx;
}ISEPrivateKeyBuffer;


typedef struct _ISELastInputContext {
	mcfchar *keyValue;
	mcfulong keyEvent;
	MCFKeyType keyType;
}ISELastInputContext;

typedef struct {
    const char *language_string;
    const int language_code;
}ISELanguageTable;

void ise_show(int ic);
void ise_set_mode();
void ise_hide(bool fCallHided);
void ise_set_language(unsigned int language);
void ise_set_lang_to_vconf(unsigned int language);
void ise_focus_in();
void ise_new();
void ise_set_mode(unsigned int mode);
void ise_explicitly_set_language(unsigned int language);
void ise_reset_context();
void ise_reset_input_context();
void ise_focus_out();
bool process_option_key_window_events(char *Str);
void ise_set_private_key(int keyIdx, char* label, char* ImgPath, int valueIdx, char* value );
void ise_set_disable_key(int keyIdx, int disabled );
unsigned int ise_get_language();
unsigned int ise_get_mode();
void ise_set_layout(mcfu32 layoutIdx);
mcfu32 ise_get_layout();
void ise_get_size(int *x, int *y, int *width, int *height);
void ise_update_cursor_position(int position);
void ise_update_spot_location(int x, int y);
void ise_set_caps_mode(unsigned int mode);
void ise_set_return_key_type(unsigned int type);
void ise_set_return_key_disable(unsigned int disabled);
void ise_get_language_locale(char **locale);
void _show_language_popup(mcfshort x, mcfshort y);
SETTING_INFO ise_setting_update_on_reload_config(SETTING_INFO _setup_info);
#endif
