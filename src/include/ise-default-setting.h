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

#ifndef __ISE_SETTING_H__
#define __ISE_SETTING_H__


#ifdef __cplusplus
MCF_BEGIN_DECLS
#endif

using namespace mcf;

/**
*@brief  context
*/
typedef struct _Context {
	mcf8 mcfInputMode;
	mcf8 KeypadMode;
	mcf8 InputMode;
	mcf8 ShiftMode;
	mcf8 OnOff;
	mcf8 CompletionMode;
	mcf8 Language;
	mcf8 SingleCommit;
}Context;

static const int CMD_KEYPAD    = 0x10; /* 16 */
static const int CMD_LANGUAGE  = 0x20;  /* 32 */
static const int CMD_SHIFTMODE = 0x30; /* 48 */
static const int CMD_STATE   = 0x40;  /* 64 */
static const int CMD_INPUTMODE = 0x50; /* 80 */
static const int CMD_COMPLETION  = 0x60; /* 96 */
static const int CMD_SINGLECOMMIT = 0x70; /* 112 */
static const int CMD_FLUSH = 0x90; /* 144 */



typedef enum _KEYPADMODE {
	KEYPAD_QWRTY_REGIONAL, /* Regional Qwerty */
	KEYPAD_QWERTY, /* Descrite Qwerty */
	KEYPAD_3X4, /* 3x4 keypad */
	KEYPAD_5X4, /* 5x4 keypad */
	MAX_KEYPAD
}KEYPADMODE;

typedef enum _INPUTMODE {
	INPUTMODE_AMBIGUOUS,
	INPUTMODE_MULTITAB,
	MAX_INPUTMODE
}INPUTMODE;

typedef enum _SHIFTMODE {
	SHIFTMODE_OFF,
	SHIFTMODE_ON,
	SHIFTMODE_LOCK,
	MAX_SHIFTMODE
}SHIFTMODE;

/**@brief  Keyboard database options */
typedef enum _CompletionMode {
	COMPLETION_MULTITAB_LOWERCASE_STRING, /* abc */
	COMPLETION_MULTITAB_CAMELCASE_STRING, /* Abc */
	COMPLETION_MULTITAB_UPPERCASE_STRING, /* ABC */
	COMPLETION_AMBIGUOUS_LOWERCASE_STRING, /* abc */
	COMPLETION_AMBIGUOUS_CAMELCASE_STRING, /* Abc */
	COMPLETION_AMBIGUOUS_UPPERCASE_STRING, /* ABC */
	COMPLETION_NUMERIC_STRING,
	COMPLETION_3x4_STRING,
	COMPLETION_5x4_STRING,
	COMPLETION_QWERTY_STRING,
	COMPLETION_HWR_STRING,
	MAX_COMPLETION
}CompletionMode;

typedef enum _LDBLanguage {
	ISE_DEFAULT_LANGUAGE_ENGLISH = 0,
	ISE_DEFAULT_LANGUAGE_FRENCH,
	ISE_DEFAULT_LANGUAGE_SPANISH,
	ISE_DEFAULT_LANGUAGE_DUTCH,
	ISE_DEFAULT_LANGUAGE_GERMAN,
	ISE_DEFAULT_LANGUAGE_GREEK,
	ISE_DEFAULT_LANGUAGE_ITALIAN,
	ISE_DEFAULT_LANGUAGE_PORTUGUESE,
	ISE_DEFAULT_LANGUAGE_TURKISH,
	ISE_DEFAULT_LANGUAGE_ARABIC,
	ISE_DEFAULT_LANGUAGE_RUSSIAN,
	ISE_DEFAULT_LANGUAGE_AFRIKAANS,
	//ISE_DEFAULT_LANGUAGE_ALBANIAN,
	ISE_DEFAULT_LANGUAGE_ASSAMESE,
	ISE_DEFAULT_LANGUAGE_AZERBAIJANI,
	ISE_DEFAULT_LANGUAGE_BASQUE,
	ISE_DEFAULT_LANGUAGE_BENGALI,
	ISE_DEFAULT_LANGUAGE_BULGARIAN,
	ISE_DEFAULT_LANGUAGE_CATALAN,
	ISE_DEFAULT_LANGUAGE_CZECH,
	ISE_DEFAULT_LANGUAGE_DANISH,
	ISE_DEFAULT_LANGUAGE_ESTONIAN,
	ISE_DEFAULT_LANGUAGE_FINNISH,
	ISE_DEFAULT_LANGUAGE_GALICIAN,
	ISE_DEFAULT_LANGUAGE_GEORGIAN,
	ISE_DEFAULT_LANGUAGE_GUJARATI,
	ISE_DEFAULT_LANGUAGE_HAUSA,
	ISE_DEFAULT_LANGUAGE_HEBREW,
	ISE_DEFAULT_LANGUAGE_HINDI,
	ISE_DEFAULT_LANGUAGE_HUNGARIAN,
	ISE_DEFAULT_LANGUAGE_ICELANDIC,
	ISE_DEFAULT_LANGUAGE_IRISH,
	ISE_DEFAULT_LANGUAGE_KANNADA,
	ISE_DEFAULT_LANGUAGE_KAZAKH,
	ISE_DEFAULT_LANGUAGE_LATVIAN,
	ISE_DEFAULT_LANGUAGE_LITHUANIAN,
	ISE_DEFAULT_LANGUAGE_MACEDONIAN,
	ISE_DEFAULT_LANGUAGE_MALAYALAM,
	ISE_DEFAULT_LANGUAGE_MALAY,
	ISE_DEFAULT_LANGUAGE_MARATHI,
	ISE_DEFAULT_LANGUAGE_MONGOLIAN,
	ISE_DEFAULT_LANGUAGE_NEPALI,
	ISE_DEFAULT_LANGUAGE_NORWEGIAN,
	ISE_DEFAULT_LANGUAGE_POLISH,
	ISE_DEFAULT_LANGUAGE_PUNJABI,
	ISE_DEFAULT_LANGUAGE_ROMANIAN,
	ISE_DEFAULT_LANGUAGE_SERBIAN,
	ISE_DEFAULT_LANGUAGE_SINHALA,
	ISE_DEFAULT_LANGUAGE_SLOVAK,
	ISE_DEFAULT_LANGUAGE_SLOVENIAN,
	ISE_DEFAULT_LANGUAGE_SWEDISH,
	ISE_DEFAULT_LANGUAGE_TAMIL,
	ISE_DEFAULT_LANGUAGE_TELUGU,
	ISE_DEFAULT_LANGUAGE_THAI,
	ISE_DEFAULT_LANGUAGE_UKRAINIAN,
	ISE_DEFAULT_LANGUAGE_URDU,
	ISE_DEFAULT_LANGUAGE_UZBEK,
	ISE_DEFAULT_LANGUAGE_VIETNAMESE,
	ISE_DEFAULT_LANGUAGE_WELSH,
	ISE_DEFAULT_LANGUAGE_XHOSA,
	ISE_DEFAULT_LANGUAGE_YORUBA,
	ISE_DEFAULT_LANGUAGE_ZULU,
    MAX_LANGUAGE
}LDBLanguage;

typedef enum _ONOFF {
	OFF = 0,
	ON,
	MAX_ONOFF
}ONOFF;

typedef enum _SingleCommitONOFF {
	SINGLE_COMMIT_OFF = 0,
	SINGLE_COMMIT_ON,
	MAX_SINGLE_COMMIT_ONOFF
}SingleCommitONOFF;

void
hide();

void
reset();

void
change_completion_option(unsigned int kdbMode);

void
change_ldb_option(unsigned int ldbLanguage);

void
change_prediction_onoff(bool fEnable);

void
change_inputmode(unsigned int inputMode);

void
set_shiftmode(unsigned int shiftMode);

void
change_shiftmode(unsigned int shiftMode);

void
change_keypad(unsigned int keypad);

void
set_single_commit(unsigned int single);

Context
get_Ise_default_context();

void send_flush (void);

void
mode_syncronize();

#ifdef __cplusplus
MCF_END_DECLS
#endif

#endif
