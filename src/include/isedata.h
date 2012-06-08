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



#ifndef __ISEDATA_H_INCLUDED__
#define __ISEDATA_H_INCLUDED__

#include "ise.h"
#include "ise-default-setting.h"
#include <libintl.h>

#define ISEUUID "12aa3425-f88d-45f4-a509-cee8dfe904e3"
#define ISENAME "Tizen keyboard"
#define KEYBD_ISE_UUID "d75857a5-4148-4745-89e2-1da7ddaf7999"

#define INITIAL_KEYPAD_MODE FALSE

#define SUPPORTS_MULTIPLE_LANGUAGE_SELECTION

#define SUPPORTS_LAYOUT_STYLE_NORMAL
#define SUPPORTS_LAYOUT_STYLE_NUMBER
#define SUPPORTS_LAYOUT_STYLE_EMAIL
#define SUPPORTS_LAYOUT_STYLE_URL
#define SUPPORTS_LAYOUT_STYLE_PHONENUMBER
#define SUPPORTS_LAYOUT_STYLE_IP
#define SUPPORTS_LAYOUT_STYLE_MONTH
#define SUPPORTS_LAYOUT_STYLE_NUMBERONLY

#ifdef SUPPORTS_HELP_PAGE
#undef SUPPORTS_HELP_PAGE
#endif

#define SUPPORTED_LANGUAGES "nl_NL,en_US,fr_FR,de_DE,it_IT,pt_PT,es_ES,tr_TR,el_GR,ru_RU"

#define LANGUAGE_ENGLISH_UNITED_STATES		"en_US"
#define LANGUAGE_DUTCH_STANDARD			"nl_NL"
#define LANGUAGE_FRENCH_STANDARD		"fr_FR"
#define LANGUAGE_GERMAN_STANDARD		"de_DE"
#define LANGUAGE_ITALIAN_STANDARD		"it_IT"
#define LANGUAGE_PORTUGUESE_STANDARD		"pt_PT"
#define LANGUAGE_SPANISH			"es_ES"
#define LANGUAGE_TURKISH			"tr_TR"
#define LANGUAGE_GREEK				"el_GR"
#define LANGUAGE_RUSSIAN			"ru_RU"


#define MAX_LANG_NUM 10
typedef enum {
    LANGUAGE_ID_ENGLISH,
    LANGUAGE_ID_DUTCH,
    LANGUAGE_ID_FRENCH,
    LANGUAGE_ID_GERMAN,
    LANGUAGE_ID_ITALIAN,
    LANGUAGE_ID_PORTUGUESE,
    LANGUAGE_ID_SPANISH,
    LANGUAGE_ID_TURKISH,
    LANGUAGE_ID_GREEK,
    LANGUAGE_ID_RUSSIAN,
    MAX_NUM
} ISE_LANGUAGES;
#define PRIMARY_LANGUAGE_INDEX 0

static const ISEDefaultValueTable IseDefaultValue[MAX_ISE_LAYOUT_CNT]  = {
	{ISE_LAYOUT_STYLE_NORMAL,			INPUT_MODE_NATIVE,			KEYPAD_QWERTY,	INPUTMODE_AMBIGUOUS, 	SHIFTMODE_OFF,	OFF,	COMPLETION_MULTITAB_LOWERCASE_STRING, SINGLE_COMMIT_OFF,FALSE, SUBLAYOUTID_DEFAULT},
	{ISE_LAYOUT_STYLE_NUMBER,			INPUT_MODE_QTY_SYM_1,			KEYPAD_QWERTY,  INPUTMODE_AMBIGUOUS,    SHIFTMODE_OFF,	OFF,	COMPLETION_NUMERIC_STRING,SINGLE_COMMIT_OFF, FALSE, SUBLAYOUTID_DEFAULT},
	{ISE_LAYOUT_STYLE_EMAIL,			INPUT_MODE_QTY_EMAIL,			KEYPAD_QWERTY,	INPUTMODE_AMBIGUOUS ,	SHIFTMODE_OFF,	OFF,	COMPLETION_QWERTY_STRING, SINGLE_COMMIT_ON,TRUE, SUBLAYOUTID_EMAIL},
	{ISE_LAYOUT_STYLE_URL,				INPUT_MODE_QTY_URL,			KEYPAD_QWERTY,	INPUTMODE_AMBIGUOUS, 	SHIFTMODE_OFF,	OFF,	COMPLETION_QWERTY_STRING,SINGLE_COMMIT_ON, TRUE, SUBLAYOUTID_URL},
	{ISE_LAYOUT_STYLE_PHONENUMBER,			INPUT_MODE_QTY_SYM_1,		    	KEYPAD_QWERTY,  INPUTMODE_AMBIGUOUS,	SHIFTMODE_OFF,	OFF,	COMPLETION_NUMERIC_STRING, SINGLE_COMMIT_OFF,FALSE, SUBLAYOUTID_DEFAULT},
	{ISE_LAYOUT_STYLE_IP,				INPUT_MODE_QTY_SYM_1,		    	KEYPAD_QWERTY,  INPUTMODE_AMBIGUOUS,	SHIFTMODE_OFF,	OFF,	COMPLETION_NUMERIC_STRING,SINGLE_COMMIT_OFF, FALSE, SUBLAYOUTID_DEFAULT},
	{ISE_LAYOUT_STYLE_MONTH,			INPUT_MODE_QTY_SYM_1,			KEYPAD_QWERTY,  INPUTMODE_AMBIGUOUS,	SHIFTMODE_OFF,	OFF,	COMPLETION_NUMERIC_STRING,SINGLE_COMMIT_OFF, FALSE, SUBLAYOUTID_DEFAULT},
	{ISE_LAYOUT_STYLE_NUMBERONLY,			INPUT_MODE_QTY_SYM_1,		   	KEYPAD_QWERTY,  INPUTMODE_AMBIGUOUS,	SHIFTMODE_OFF,	OFF,	COMPLETION_NUMERIC_STRING,SINGLE_COMMIT_OFF, FALSE, SUBLAYOUTID_DEFAULT},
};

static const mcf8 internalLangToLang[MAX_LANG_NUM] = {
	ISE_DEFAULT_LANGUAGE_ENGLISH, /* English_United_States*/
	ISE_DEFAULT_LANGUAGE_DUTCH, /* Dutch_Standard*/
	ISE_DEFAULT_LANGUAGE_FRENCH, /* French_Standard*/
	ISE_DEFAULT_LANGUAGE_GERMAN, /* German_Standard*/
	ISE_DEFAULT_LANGUAGE_ITALIAN, /* Italian_Standard*/
	ISE_DEFAULT_LANGUAGE_PORTUGUESE, /* Portuguese_Standard*/
	ISE_DEFAULT_LANGUAGE_SPANISH, /* Spanish*/
	ISE_DEFAULT_LANGUAGE_TURKISH, /* Turkish*/
	ISE_DEFAULT_LANGUAGE_GREEK, /* Greek*/
	ISE_DEFAULT_LANGUAGE_RUSSIAN/*russian*/
};
static const char *language_text_name[MAX_LANG_NUM] = {
	"IDS_IME_OPT_LANG_ENGLISH",
	"IDS_IME_OPT_LANG_DUTCH",
	"IDS_IME_OPT_LANG_FRENCH",
	"IDS_IME_OPT_LANG_GERMAN",
	"IDS_IME_POP_LANG_ITALIAN",
	"IDS_IME_POP_LANG_PORTUGUESE",
	"IDS_IME_OPT_LANG_SPANISH",
	"IDS_IME_OPT_LANG_TURKISH",
	"IDS_IME_OPT_LANG_GREEK",
	"IDS_IME_OPT_LANG_RUSSIAN"
};
static ISELanguageTable lang_table[] = {
    {LANGUAGE_ENGLISH_UNITED_STATES, LANGUAGE_ID_ENGLISH},
    {LANGUAGE_DUTCH_STANDARD, LANGUAGE_ID_DUTCH},
    {LANGUAGE_FRENCH_STANDARD, LANGUAGE_ID_FRENCH},
    {LANGUAGE_GERMAN_STANDARD, LANGUAGE_ID_GERMAN},
    {LANGUAGE_ITALIAN_STANDARD, LANGUAGE_ID_ITALIAN},
    {LANGUAGE_PORTUGUESE_STANDARD, LANGUAGE_ID_PORTUGUESE},
    {LANGUAGE_SPANISH, LANGUAGE_ID_SPANISH},
    {LANGUAGE_TURKISH, LANGUAGE_ID_TURKISH},
    {LANGUAGE_GREEK, LANGUAGE_ID_GREEK},
    {LANGUAGE_RUSSIAN, LANGUAGE_ID_RUSSIAN},
};

/**
*@brief Language<->Input mode table
*/
static const mcf8 mcfInputModeByLanguage[MAX_KEYPAD][MAX_LANG_NUM] = {
	/*Qwerty Regional*/
	{},
	/*Qwerty*/
	{
		INPUT_MODE_QTY_ENGLISH, /* English_United_States*/
		INPUT_MODE_QTY_DUTCH, /* Dutch_Standard*/
		INPUT_MODE_QTY_FRENCH, /* French_Standard*/
		INPUT_MODE_QTY_GERMAN, /* German_Standard*/
		INPUT_MODE_QTY_ITALIAN, /* Italian_Standard*/
		INPUT_MODE_QTY_PORTUGUESE, /* Portuguese_Standard*/
		INPUT_MODE_QTY_SPANISH, /* Spanish*/
		INPUT_MODE_QTY_TURKISH, /* Turkish*/
		INPUT_MODE_QTY_GREEK, /* Greek*/
		INPUT_MODE_QTY_RUSSIAN,
	},
	/*3x4*/
	{
		INPUT_MODE_4X4_ENGLISH, /* English_United_States*/
		INPUT_MODE_4X4_DUTCH, /* Dutch_Standard*/
		INPUT_MODE_4X4_FRENCH, /* French_Standard*/
		INPUT_MODE_4X4_GERMAN, /* German_Standard*/
		INPUT_MODE_4X4_ITALIAN, /* Italian_Standard*/
		INPUT_MODE_4X4_PORTUGUESE, /* Portuguese_Standard*/
		INPUT_MODE_4X4_SPANISH, /* Spanish*/
		INPUT_MODE_4X4_TURKISH, /* Turkish*/
		INPUT_MODE_4X4_GREEK, /* Greek*/
		INPUT_MODE_4X4_RUSSIAN, /* russian*/
	},
	/*5x4*/
	{}
};

/**
*@brief Language<->Input mode table
*/
static const mcf8 mcfLanguageByInputMode[MAX_INPUT_MODE] = {
	ISE_DEFAULT_LANGUAGE_ENGLISH,		/* INPUT_MODE_3X4_ENGLISH */
	NOT_USED,					/* INPUT_MODE_4X4_NUM */
	NOT_USED,					/* INPUT_MODE_4X4_SYM_1 */
	NOT_USED,					/* INPUT_MODE_4X4_SYM_2 */
	NOT_USED,					/* INPUT_MODE_4X4_SYM_3 */
	NOT_USED,					/* INPUT_MODE_4X4_SYM_4 */
	NOT_USED,					/* INPUT_MODE_4X4_SYM_5 */
	NOT_USED,					/* INPUT_MODE_4X4_TELEPHONE */
	ISE_DEFAULT_LANGUAGE_ENGLISH,		/* INPUT_MODE_4X4_URL */
	ISE_DEFAULT_LANGUAGE_ENGLISH,		/* INPUT_MODE_4X4_EMAIL */
	NOT_USED,					/* INPUT_MODE_4X4_PASSWD */
	NOT_USED,					/* INPUT_MODE_4X4_IPv6_123 */
	NOT_USED,					/* INPUT_MODE_4X4_IPv6_ABC */
	NOT_USED,					/* INPUT_MODE_4X4_NUMONLY */
	NOT_USED,					/* INPUT_MODE_4X4_MONTH */

	ISE_DEFAULT_LANGUAGE_FRENCH,		/* INPUT_MODE_3X4_FRENCH */
	ISE_DEFAULT_LANGUAGE_SPANISH,		/* INPUT_MODE_3X4_SPANISH */
	ISE_DEFAULT_LANGUAGE_ITALIAN,		/* INPUT_MODE_3X4_ITALIAN */
	ISE_DEFAULT_LANGUAGE_DUTCH,			/* INPUT_MODE_3X4_DUTCH */
	ISE_DEFAULT_LANGUAGE_PORTUGUESE,	/* INPUT_MODE_3X4_PORTUGUESE */
	ISE_DEFAULT_LANGUAGE_GERMAN,		/* INPUT_MODE_3X4_GERMAN */

	ISE_DEFAULT_LANGUAGE_TURKISH,		/* INPUT_MODE_3X4_TURKISH */
	ISE_DEFAULT_LANGUAGE_GREEK,			/* INPUT_MODE_3X4_GREEK */
	ISE_DEFAULT_LANGUAGE_RUSSIAN,		/* INPUT_MODE_3X4_RUSSIAN */

	ISE_DEFAULT_LANGUAGE_ENGLISH,		/* INPUT_MODE_QTY_ENGLISH */
	ISE_DEFAULT_LANGUAGE_ENGLISH,		/* INPUT_MODE_QTY_URL */
	ISE_DEFAULT_LANGUAGE_ENGLISH,		/* INPUT_MODE_QTY_EMAIL */
	NOT_USED,					/* INPUT_MODE_QTY_SYM_1 */
	NOT_USED,					/* INPUT_MODE_QTY_SYM_2 */
	NOT_USED,					/* INPUT_MODE_QTY_SYM_3 */
	NOT_USED,					/* INPUT_MODE_QTY_SYM_4 */


	ISE_DEFAULT_LANGUAGE_FRENCH,		/* INPUT_MODE_AZT_FRENCH */
	ISE_DEFAULT_LANGUAGE_SPANISH,		/* INPUT_MODE_QTY_SPANISH */
	ISE_DEFAULT_LANGUAGE_ITALIAN,		/* INPUT_MODE_QTY_ITALIAN */
	ISE_DEFAULT_LANGUAGE_DUTCH,			/* INPUT_MODE_QTY_DUTCH */
	ISE_DEFAULT_LANGUAGE_PORTUGUESE,	/* INPUT_MODE_QTY_PORTUGUESE */
	ISE_DEFAULT_LANGUAGE_GERMAN,		/* INPUT_MODE_QTY_GERMAN */

	ISE_DEFAULT_LANGUAGE_TURKISH,		/* INPUT_MODE_QTY_TURKISH */
	ISE_DEFAULT_LANGUAGE_GREEK,			/* INPUT_MODE_QTY_GREEK */
	ISE_DEFAULT_LANGUAGE_RUSSIAN,		/* INPUT_MODE_QTY_RUSSIAN */
};

static CompletionMode IseInitialCompletionMode[MAX_INPUT_MODE][2] = { /* On and Off cases each */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_AMBIGUOUS_LOWERCASE_STRING },	/* INPUT_MODE_3X4_ENGLISH */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_MULTITAB_LOWERCASE_STRING },		/* INPUT_MODE_4X4_NUM */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_MULTITAB_LOWERCASE_STRING },		/* INPUT_MODE_4X4_SYM_1 */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_MULTITAB_LOWERCASE_STRING },		/* INPUT_MODE_4X4_SYM_2 */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_MULTITAB_LOWERCASE_STRING },		/* INPUT_MODE_4X4_SYM_3 */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_MULTITAB_LOWERCASE_STRING },		/* INPUT_MODE_4X4_SYM_4 */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_MULTITAB_LOWERCASE_STRING },		/* INPUT_MODE_4X4_SYM_5 */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_MULTITAB_LOWERCASE_STRING },		/* INPUT_MODE_4X4_PHONENUMBER */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_MULTITAB_LOWERCASE_STRING },		/* INPUT_MODE_QTY_URL */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_MULTITAB_LOWERCASE_STRING },		/* INPUT_MODE_QTY_EMAIL*/
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_MULTITAB_LOWERCASE_STRING },		/* INPUT_MODE_4X4_PASSWD*/
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_MULTITAB_LOWERCASE_STRING },		/* INPUT_MODE_4X4_IPv6_123*/
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_MULTITAB_LOWERCASE_STRING },		/* INPUT_MODE_4X4_IPv6_ABC*/
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_MULTITAB_LOWERCASE_STRING },		/* INPUT_MODE_4X4_NUMONLY*/
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_MULTITAB_LOWERCASE_STRING },		/* INPUT_MODE_4X4_MONTH*/

	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_AMBIGUOUS_LOWERCASE_STRING },	/* INPUT_MODE_3X4_FRENCH */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_AMBIGUOUS_LOWERCASE_STRING },	/* INPUT_MODE_3X4_SPANISH */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_AMBIGUOUS_LOWERCASE_STRING },	/* INPUT_MODE_3X4_ITALIAN */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_AMBIGUOUS_LOWERCASE_STRING },	/* INPUT_MODE_3X4_DUTCH */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_AMBIGUOUS_LOWERCASE_STRING },	/* INPUT_MODE_3X4_PORTUGUESE */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_AMBIGUOUS_LOWERCASE_STRING },	/* INPUT_MODE_3X4_GERMAN */

	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_AMBIGUOUS_LOWERCASE_STRING },	/* INPUT_MODE_3X4_TURKEY */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_AMBIGUOUS_LOWERCASE_STRING },	/* INPUT_MODE_3X4_GREEK */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_AMBIGUOUS_LOWERCASE_STRING },	/* INPUT_MODE_3X4_RUSSIAN */

	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_AMBIGUOUS_LOWERCASE_STRING },	/* INPUT_MODE_QTY_ENGLISH */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_MULTITAB_LOWERCASE_STRING },		/* INPUT_MODE_QTY_URL */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_MULTITAB_LOWERCASE_STRING },		/* INPUT_MODE_QTY_EMAIL*/
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_MULTITAB_LOWERCASE_STRING },		/* INPUT_MODE_QTY_SYM1 */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_MULTITAB_LOWERCASE_STRING },		/* INPUT_MODE_QTY_SYM2 */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_MULTITAB_LOWERCASE_STRING },		/* INPUT_MODE_QTY_SYM3 */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_MULTITAB_LOWERCASE_STRING },		/* INPUT_MODE_QTY_SYM4 */

	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_AMBIGUOUS_LOWERCASE_STRING },	/* INPUT_MODE_AZT_FRENCH */

	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_AMBIGUOUS_LOWERCASE_STRING },	/* INPUT_MODE_QTY_SPANISH */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_AMBIGUOUS_LOWERCASE_STRING },	/* INPUT_MODE_QTY_ITALIAN */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_AMBIGUOUS_LOWERCASE_STRING },	/* INPUT_MODE_QTY_DUTCH */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_AMBIGUOUS_LOWERCASE_STRING },	/* INPUT_MODE_QTY_PORTUGUESE */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_AMBIGUOUS_LOWERCASE_STRING },	/* INPUT_MODE_QTY_GERMAN */

	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_AMBIGUOUS_LOWERCASE_STRING },	/* INPUT_MODE_QTY_TURKISH */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_AMBIGUOUS_LOWERCASE_STRING },	/* INPUT_MODE_QTY_GREEK */
	{ COMPLETION_MULTITAB_LOWERCASE_STRING,	COMPLETION_AMBIGUOUS_LOWERCASE_STRING },	/* INPUT_MODE_QTY_RUSSIAN */
};

static mcf8 IseDefaultInputModes[MAX_INPUT_MODE][3] = { /* 3x4, QTY, NUM */
	{ INPUT_MODE_4X4_ENGLISH,		INPUT_MODE_QTY_ENGLISH,		INPUT_MODE_4X4_NUM },	/* INPUT_MODE_3X4_ENGLISH */
	{ NOT_USED,						NOT_USED,					INPUT_MODE_4X4_NUM },	/* INPUT_MODE_4X4_NUM */
	{ NOT_USED,						NOT_USED,					INPUT_MODE_4X4_NUM },	/* INPUT_MODE_4X4_SYM_1 */
	{ NOT_USED,						NOT_USED,					INPUT_MODE_4X4_NUM },	/* INPUT_MODE_4X4_SYM_2 */
	{ NOT_USED,						NOT_USED,					INPUT_MODE_4X4_NUM },	/* INPUT_MODE_4X4_SYM_3 */
	{ NOT_USED,						NOT_USED,					INPUT_MODE_4X4_NUM },	/* INPUT_MODE_4X4_SYM_4 */
	{ NOT_USED,						NOT_USED,					INPUT_MODE_4X4_NUM },	/* INPUT_MODE_4X4_SYM_5 */
	{ INPUT_MODE_4X4_TELEPHONE,		INPUT_MODE_4X4_TELEPHONE,	NOT_USED },				/* INPUT_MODE_4X4_TELEPHONE */
	{ INPUT_MODE_4X4_URL,			INPUT_MODE_QTY_URL,			INPUT_MODE_4X4_NUM },	/* INPUT_MODE_4x4_URL */
	{ INPUT_MODE_4X4_EMAIL,			INPUT_MODE_QTY_EMAIL,		INPUT_MODE_4X4_NUM },	/* INPUT_MODE_4x4_EMAIL */
	{ INPUT_MODE_4X4_PASSWD,		INPUT_MODE_4X4_PASSWD,			INPUT_MODE_4X4_NUM},	/* INPUT_MODE_4X4_PASSWD */
	{ INPUT_MODE_4X4_IPv6_123,		INPUT_MODE_4X4_IPv6_123,			INPUT_MODE_4X4_IPv6_123},	/* INPUT_MODE_4X4_IPv6_123 */
	{ INPUT_MODE_4X4_IPv6_ABC,		INPUT_MODE_4X4_IPv6_ABC,			INPUT_MODE_4X4_IPv6_123},	/* INPUT_MODE_4X4_IPv6_ABC */
	{ INPUT_MODE_4X4_NUMONLY,		INPUT_MODE_4X4_NUMONLY,		INPUT_MODE_4X4_NUMONLY},	/* INPUT_MODE_4X4_NUMONLY */
	{ INPUT_MODE_4X4_MONTH,			INPUT_MODE_4X4_MONTH,		INPUT_MODE_4X4_MONTH},	/* INPUT_MODE_4X4_MONTH */

	{ INPUT_MODE_4X4_FRENCH,		INPUT_MODE_QTY_FRENCH,		INPUT_MODE_4X4_NUM },	/* INPUT_MODE_3X4_FRENCH */
	{ INPUT_MODE_4X4_SPANISH,		INPUT_MODE_QTY_SPANISH,		INPUT_MODE_4X4_NUM },	/* INPUT_MODE_3X4_SPANISH */
	{ INPUT_MODE_4X4_ITALIAN,		INPUT_MODE_QTY_ITALIAN,		INPUT_MODE_4X4_NUM },	/* INPUT_MODE_3X4_ITALIAN */
	{ INPUT_MODE_4X4_DUTCH,			INPUT_MODE_QTY_DUTCH,		INPUT_MODE_4X4_NUM },	/* INPUT_MODE_3X4_DUTCH */
	{ INPUT_MODE_4X4_PORTUGUESE,	INPUT_MODE_QTY_PORTUGUESE,	INPUT_MODE_4X4_NUM },	/* INPUT_MODE_3X4_PORTUGUESE */
	{ INPUT_MODE_4X4_GERMAN,		INPUT_MODE_QTY_GERMAN,		INPUT_MODE_4X4_NUM },	/* INPUT_MODE_3X4_GERMAN */

	{ INPUT_MODE_4X4_TURKISH,		INPUT_MODE_QTY_TURKISH,		INPUT_MODE_4X4_NUM },	/* INPUT_MODE_3X4_TURKISH */
	{ INPUT_MODE_4X4_GREEK,			INPUT_MODE_QTY_GREEK,		INPUT_MODE_4X4_NUM },	/* INPUT_MODE_3X4_GREEK */
	{ INPUT_MODE_4X4_RUSSIAN,		INPUT_MODE_QTY_RUSSIAN,		INPUT_MODE_4X4_NUM },	/* INPUT_MODE_3X4_RUSSIAN */


	{ INPUT_MODE_4X4_ENGLISH,		INPUT_MODE_QTY_ENGLISH,		INPUT_MODE_4X4_NUM },	/* INPUT_MODE_QTY_ENGLISH */
	{ INPUT_MODE_4X4_URL,			INPUT_MODE_QTY_URL,			INPUT_MODE_4X4_NUM },	/* INPUT_MODE_QTY_URL */
	{ INPUT_MODE_4X4_EMAIL,			INPUT_MODE_QTY_EMAIL,		INPUT_MODE_4X4_NUM },	/* INPUT_MODE_QTY_URL */
	{ NOT_USED,						NOT_USED,					INPUT_MODE_4X4_NUM },	/* INPUT_MODE_QTY_SYM_1 */
	{ NOT_USED,						NOT_USED,					INPUT_MODE_4X4_NUM },	/* INPUT_MODE_QTY_SYM_2 */
	{ NOT_USED,						NOT_USED,					INPUT_MODE_4X4_NUM },	/* INPUT_MODE_QTY_SYM_3 */
	{ NOT_USED,						NOT_USED,					INPUT_MODE_4X4_NUM },	/* INPUT_MODE_QTY_SYM_4 */


	{ INPUT_MODE_4X4_FRENCH,		INPUT_MODE_QTY_FRENCH,		INPUT_MODE_4X4_NUM },	/* INPUT_MODE_AZT_FRENCH */
	{ INPUT_MODE_4X4_SPANISH,		INPUT_MODE_QTY_SPANISH,		INPUT_MODE_4X4_NUM },	/* INPUT_MODE_QTY_SPANISH */
	{ INPUT_MODE_4X4_ITALIAN,		INPUT_MODE_QTY_ITALIAN,		INPUT_MODE_4X4_NUM },	/* INPUT_MODE_QTY_ITALIAN */
	{ INPUT_MODE_4X4_DUTCH,			INPUT_MODE_QTY_DUTCH,		INPUT_MODE_4X4_NUM },	/* INPUT_MODE_QTY_DUTCH */
	{ INPUT_MODE_4X4_PORTUGUESE,	INPUT_MODE_QTY_PORTUGUESE,	INPUT_MODE_4X4_NUM },	/* INPUT_MODE_QTY_PORTUGUESE */
	{ INPUT_MODE_4X4_GERMAN,		INPUT_MODE_QTY_GERMAN,		INPUT_MODE_4X4_NUM },	/* INPUT_MODE_QTY_GERMAN */

	{ INPUT_MODE_4X4_TURKISH,		INPUT_MODE_QTY_TURKISH,		INPUT_MODE_4X4_NUM },	/* INPUT_MODE_3X4_TURKISH */
	{ INPUT_MODE_4X4_GREEK,			INPUT_MODE_QTY_GREEK,		INPUT_MODE_4X4_NUM },	/* INPUT_MODE_3X4_GREEK */
	{ INPUT_MODE_4X4_RUSSIAN,		INPUT_MODE_QTY_RUSSIAN,		INPUT_MODE_4X4_NUM },	/* INPUT_MODE_3X4_RUSSIAN */
};

static KEYPADMODE IseKeypadMode[MAX_INPUT_MODE][2] = { /* PORTRAIT, LANDSCAPE*/
	{KEYPAD_3X4,		KEYPAD_QWERTY}, /*ENGLISH_4X4*/
	{KEYPAD_3X4,		KEYPAD_3X4}, /*NUM_4X4*/
	{KEYPAD_3X4,			KEYPAD_3X4}, /*SYM_4X4_1*/
	{KEYPAD_3X4,			KEYPAD_3X4}, /*SYM_4X4_2*/
	{KEYPAD_3X4,			KEYPAD_3X4}, /*SYM_4X4_3*/
	{KEYPAD_3X4,			KEYPAD_3X4}, /*SYM_4X4_4*/
	{KEYPAD_3X4,			KEYPAD_3X4}, /*SYM_4X4_5*/
	{KEYPAD_3X4,		KEYPAD_3X4}, /*PHONE_4X4*/
	{KEYPAD_3X4,		KEYPAD_QWERTY}, /*URL_4X4*/
	{KEYPAD_3X4,		KEYPAD_QWERTY}, /*EMAIL_4X4*/
	{KEYPAD_3X4,		KEYPAD_3X4}, /*PASSWD_4X4*/
	{KEYPAD_3X4,		KEYPAD_3X4}, /*IPv6_4X4_123*/
	{KEYPAD_3X4,		KEYPAD_3X4}, /*IPv6_4X4_ABC*/
	{KEYPAD_3X4,		KEYPAD_3X4}, /*NUMONLY_4X4*/
	{KEYPAD_3X4,			KEYPAD_3X4}, /*MONTH_4X4*/

	{KEYPAD_3X4,		KEYPAD_QWERTY}, /*FRENCH_4X4*/
	{KEYPAD_3X4,		KEYPAD_QWERTY}, /*SPANISH_4X4*/
	{KEYPAD_3X4,		KEYPAD_QWERTY}, /*ITALIAN_4X4*/
	{KEYPAD_3X4,		KEYPAD_QWERTY}, /*DUTCH_4X4*/
	{KEYPAD_3X4,		KEYPAD_QWERTY}, /*PORTUGUESE_4X4*/
	{KEYPAD_3X4,		KEYPAD_QWERTY}, /*GERMAN_4X4*/

	{KEYPAD_3X4,		KEYPAD_QWERTY}, /*TURKISH_4X4*/
	{KEYPAD_3X4,		KEYPAD_QWERTY}, /*GREEK_4X4*/
	{KEYPAD_3X4,		KEYPAD_QWERTY}, /*RUSSIAN_4X4*/

	{KEYPAD_QWERTY,		KEYPAD_QWERTY}, /*ENGLISH_QTY*/
	{KEYPAD_QWERTY,			KEYPAD_QWERTY}, /*URL_QTY*/
	{KEYPAD_QWERTY,			KEYPAD_QWERTY}, /*EMAIL_QTY*/
	{KEYPAD_QWERTY,		KEYPAD_QWERTY}, /*SYM_QTY_1*/
	{KEYPAD_QWERTY,		KEYPAD_QWERTY}, /*SYM_QTY_2*/
	{KEYPAD_QWERTY,		KEYPAD_QWERTY}, /*SYM_QTY_3*/
	{KEYPAD_QWERTY,		KEYPAD_QWERTY}, /*SYM_QTY_4*/

	{KEYPAD_QWERTY,		KEYPAD_QWERTY}, /*FRENCH_QTY*/
	{KEYPAD_QWERTY,		KEYPAD_QWERTY}, /*SPANISH_QTY*/
	{KEYPAD_QWERTY,		KEYPAD_QWERTY}, /*ITALIAN_QTY*/
	{KEYPAD_QWERTY,		KEYPAD_QWERTY}, /*DUTCH_QTY*/
	{KEYPAD_QWERTY,		KEYPAD_QWERTY}, /*PORTUGUESE_QTY*/
	{KEYPAD_QWERTY,		KEYPAD_QWERTY}, /*GERMAN_QTY*/

	{KEYPAD_QWERTY,		KEYPAD_QWERTY}, /*TURKISH_QTY*/
	{KEYPAD_QWERTY,		KEYPAD_QWERTY}, /*GREEK_QTY*/
	{KEYPAD_QWERTY,		KEYPAD_QWERTY}, /*RUSSIAN_QTY*/
};


/**
*@brief Keypad<->NumSym mode table
*/
static const mcf8 mcfInputSymModeKeypad[1][MAX_KEYPAD] = { /* as of now only one symbol modes are available */
	{
		INPUT_MODE_QTY_SYM_1,	/* KEYPAD_QWRTY_REGIONAL, Regional Qwerty */
		INPUT_MODE_QTY_SYM_1,	/* KEYPAD_QWERTY,  Descrite Qwerty */
		INPUT_MODE_4X4_SYM_1,	/* KEYPAD_3X4,  3x4 keypad */
		INPUT_MODE_QTY_SYM_1	/* KEYPAD_5X4,  5x4 keypad */
	}
};


static ISELangData IseLangData[MAX_KEYPAD][MAX_LANG_NUM] = {
    /*KEYPAD_QWRTY_REGIONAL*/
	{},
    /*KEYPAD_QWERTY*/
	{
		{LANGUAGE_ENGLISH_UNITED_STATES			,NULL, NULL , INPUT_MODE_QTY_ENGLISH},
		{LANGUAGE_DUTCH_STANDARD				,NULL, NULL , INPUT_MODE_QTY_DUTCH},
		{LANGUAGE_FRENCH_STANDARD				,NULL, NULL , INPUT_MODE_QTY_FRENCH},
		{LANGUAGE_GERMAN_STANDARD				,NULL, NULL , INPUT_MODE_QTY_GERMAN},
		{LANGUAGE_ITALIAN_STANDARD				,NULL, NULL , INPUT_MODE_QTY_ITALIAN},
		{LANGUAGE_PORTUGUESE_STANDARD			,NULL, NULL , INPUT_MODE_QTY_PORTUGUESE},
		{LANGUAGE_SPANISH						,NULL, NULL , INPUT_MODE_QTY_SPANISH},
		{LANGUAGE_TURKISH			            ,NULL, NULL , INPUT_MODE_QTY_TURKISH},
		{LANGUAGE_GREEK			                ,NULL, NULL , INPUT_MODE_QTY_GREEK},
		{LANGUAGE_RUSSIAN			            ,NULL, NULL , INPUT_MODE_QTY_RUSSIAN},
	},
    /*KEYPAD_3X4*/
	{
		{LANGUAGE_ENGLISH_UNITED_STATES			,NULL, NULL , INPUT_MODE_4X4_ENGLISH},
		{LANGUAGE_DUTCH_STANDARD				,NULL, NULL , INPUT_MODE_4X4_DUTCH},
		{LANGUAGE_FRENCH_STANDARD				,NULL, NULL , INPUT_MODE_4X4_FRENCH},
		{LANGUAGE_GERMAN_STANDARD				,NULL, NULL , INPUT_MODE_4X4_GERMAN},
		{LANGUAGE_ITALIAN_STANDARD				,NULL, NULL , INPUT_MODE_4X4_ITALIAN},
		{LANGUAGE_PORTUGUESE_STANDARD			,NULL, NULL , INPUT_MODE_4X4_PORTUGUESE},
		{LANGUAGE_SPANISH						,NULL, NULL , INPUT_MODE_4X4_SPANISH},
		{LANGUAGE_TURKISH			            ,NULL, NULL , INPUT_MODE_4X4_TURKISH},
		{LANGUAGE_GREEK			                ,NULL, NULL , INPUT_MODE_4X4_GREEK},
		{LANGUAGE_RUSSIAN			            ,NULL, NULL , INPUT_MODE_4X4_RUSSIAN},
	},
        /*KEYPAD_5X4*/
	{}
};

#endif
