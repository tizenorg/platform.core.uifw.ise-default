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

#ifdef KEY_CONFIGURE_PHASE
/* KEYSET_PUNCTUATION_POPUP */
{
	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_UNICODE, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},
	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_UNICODE, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},
	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_UNICODE, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},
	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_UNICODE, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},
	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_UNICODE, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},
	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_UNICODE, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},

	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_UNICODE, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},
	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_UNICODE, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},
	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_UNICODE, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},
	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_UNICODE, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},

	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_CONTROL, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},
	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_CONTROL, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},
	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_CONTROL, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},
	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_CONTROL, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},

	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_UNICODE, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},

	{NOT_USED,},
},

/* KEYSET_URL */
{
	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_UNICODE, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},
	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_UNICODE, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},
	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_UNICODE, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},
	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_UNICODE, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},
	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_UNICODE, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},
	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_UNICODE, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},
	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_UNICODE, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},
	{USED, NOT_USED, BUTTON_TYPE_NORMAL, KEY_TYPE_UNICODE, POPUP_TYPE_NONE, TRUE, TRUE, TRUE, {NOT_USED,}},
	{NOT_USED,},
}

#endif

#ifdef KEY_PROPERTIES_PHASE

/* KEYSET_PUNCTUATION_POPUP */
{
	/* fValid, labelCnt, label, labelPropId, imgPath, keyValueCnt, keyValue, keyEvent, longKeyValue, longKeyEvent, userLabel,  userKeyValue,  userKeyEvent, */

	{
		USED, /* 1x1 */
		1,
		_Label_ {{"."},{"."},{"."}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"",""}, {"",""}, {"",""}},
		_BGImg_ {{"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}, {"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}, {"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}},
		1,
		_KeyVal_ {{"."},{"."},{"."}},
		_KeyEvt_ {{0},{0},{0}},
		"",
		0,
		FALSE,
	},
	{
		USED, /* 1x1 */
		1,
		_Label_ {{","},{","},{","}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"",""}, {"",""}, {"",""}},
		_BGImg_ {{"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}, {"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}, {"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}},
		1,
		_KeyVal_ {{","},{","},{","}},
		_KeyEvt_ {{0},{0},{0}},
		"",
		0,
		FALSE,
	},
	{
		USED, /* 1x1 */
		1,
		_Label_ {{"?"},{"?"},{"?"}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"",""}, {"",""}, {"",""}},
		_BGImg_ {{"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}, {"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}, {"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}},
		1,
		_KeyVal_ {{"?"},{"?"},{"?"}},
		_KeyEvt_ {{0},{0},{0}},
		"",
		0,
		FALSE,
	},
	{
		USED, /* 1x1 */
		1,
		_Label_ {{"!"},{"!"},{"!"}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"",""}, {"",""}, {"",""}},
		_BGImg_ {{"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}, {"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}, {"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}},
		1,
		_KeyVal_ {{"!"},{"!"},{"!"}},
		_KeyEvt_ {{0},{0},{0}},
		"",
		0,
		FALSE,
	},
	{
		USED, /* 1x1 */
		1,
		_Label_ {{":"},{":"},{":"}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"",""}, {"",""}, {"",""}},
		_BGImg_ {{"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}, {"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}, {"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}},
		1,
		_KeyVal_ {{":"},{":"},{":"}},
		_KeyEvt_ {{0},{0},{0}},
		"",
		0,
		FALSE,
	},
	{
		USED, /* 1x1 */
		1,
		_Label_ {{"~"},{"~"},{"~"}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"",""}, {"",""}, {"",""}},
		_BGImg_ {{"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}, {"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}, {"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}},
		1,
		_KeyVal_ {{"~"},{"~"},{"~"}},
		_KeyEvt_ {{0},{0},{0}},
		"",
		0,
		FALSE,
	},

	{
		USED, /* 1x1 */
		1,
		_Label_ {{"@"},{"@"},{"@"}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"",""}, {"",""}, {"",""}},
		_BGImg_ {{"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}, {"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}, {"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}},
		1,
		_KeyVal_ {{"@"},{"@"},{"@"}},
		_KeyEvt_ {{0},{0},{0}},
		"",
		0,
		FALSE,
	},
	{
		USED, /* 1x1 */
		1,
		_Label_ {{"/"},{"/"},{"/"}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"",""}, {"",""}, {"",""}},
		_BGImg_ {{"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}, {"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}, {"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}},
		1,
		_KeyVal_ {{"/"},{"/"},{"/"}},
		_KeyEvt_ {{0},{0},{0}},
		"",
		0,
		FALSE,
	},
	{
		USED, /* 1x1 */
		1,
		_Label_ {{"www."},{"www."},{"www."}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"",""}, {"",""}, {"",""}},
		_BGImg_ {{"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}, {"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}, {"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}},
		1,
		_KeyVal_ {{"www."},{"www."},{"www."}},
		_KeyEvt_ {{0},{0},{0}},
		"",
		0,
		FALSE,
	},
	{
		USED, /* 1x1 */
		1,
		_Label_ {{".com"},{".com"},{".com"}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"",""}, {"",""}, {"",""}},
		_BGImg_ {{"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}, {"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}, {"textinput_qwerty_button_nml.png","textinput_qwerty_button_press.png","textinput_qwerty_button_nml.png"}},
		1,
		_KeyVal_ {{".com"},{".com"},{".com"}},
		_KeyEvt_ {{0},{0},{0}},
		"",
		0,
		FALSE,
	},

	{
		USED, /* 1x1 */
		1,
		_Label_ {{""},{""},{""}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"textinput_icon_arrow_left.png","textinput_icon_arrow_left.png"}, {"textinput_icon_arrow_left.png","textinput_icon_arrow_left.png"}, {"textinput_icon_arrow_left.png","textinput_icon_arrow_left.png"}},
		_BGImg_ {{"textinput_qwerty_button_function.png","textinput_qwerty_button_press.png"}, {"textinput_qwerty_button_function.png","textinput_qwerty_button_press.png"}, {"textinput_qwerty_button_function.png","textinput_qwerty_button_press.png"}},
		1,
		_KeyVal_ {{""},{""},{""}},
		_KeyEvt_ {{MVK_KP_Left},{MVK_KP_Left},{MVK_KP_Left}},
		"",
		0,
		TRUE,
		{{""},{""},{""}},
		TRUE,
	},
	{
		USED, /* 1x1 */
		1,
		_Label_ {{""},{""},{""}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"textinput_icon_arrow_right.png","textinput_icon_arrow_right.png"}, {"textinput_icon_arrow_right.png","textinput_icon_arrow_right.png"}, {"textinput_icon_arrow_right.png","textinput_icon_arrow_right.png"}},
		_BGImg_ {{"textinput_qwerty_button_function.png","textinput_qwerty_button_press.png"}, {"textinput_qwerty_button_function.png","textinput_qwerty_button_press.png"}, {"textinput_qwerty_button_function.png","textinput_qwerty_button_press.png"}},
		1,
		_KeyVal_ {{""},{""},{""}},
		_KeyEvt_ {{MVK_KP_Right},{MVK_KP_Right},{MVK_KP_Right}},
		"",
		0,
		TRUE,
		{{""},{""},{""}},
		TRUE,
	},
	{
		USED, /* 1x1 */
		1,
		_Label_ {{""},{""},{""}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"textinput_icon_arrow_up.png","textinput_icon_arrow_up.png"}, {"textinput_icon_arrow_up.png","textinput_icon_arrow_up.png"}, {"textinput_icon_arrow_up.png","textinput_icon_arrow_down.png"}},
		_BGImg_ {{"textinput_qwerty_button_function.png","textinput_qwerty_button_press.png"}, {"textinput_qwerty_button_function.png","textinput_qwerty_button_press.png"}, {"textinput_qwerty_button_function.png","textinput_qwerty_button_press.png"}},
		1,
		_KeyVal_ {{""},{""},{""}},
		_KeyEvt_ {{MVK_KP_Up},{MVK_KP_Up},{MVK_KP_Up}},
		"",
		0,
		TRUE,
		{{""},{""},{""}},
		TRUE,
	},
	{
		USED, /* 1x1 */
		1,
		_Label_ {{""},{""},{""}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"textinput_icon_arrow_down.png","textinput_icon_arrow_down.png"}, {"textinput_icon_arrow_down.png","textinput_icon_arrow_down.png"}, {"textinput_icon_arrow_down.png","textinput_icon_arrow_down.png"}},
		_BGImg_ {{"textinput_qwerty_button_function.png","textinput_qwerty_button_press.png"}, {"textinput_qwerty_button_function.png","textinput_qwerty_button_press.png"}, {"textinput_qwerty_button_function.png","textinput_qwerty_button_press.png"}},
		1,
		_KeyVal_ {{""},{""},{""}},
		_KeyEvt_ {{MVK_KP_Down},{MVK_KP_Down},{MVK_KP_Down}},
		"",
		0,
		TRUE,
		{{""},{""},{""}},
		TRUE,
	},


	{
		USED, /* 1x1 */
		1,
		_Label_ {{""},{""},{""}},
		LABEL_PROPERTIES_QTY_DEFAULT_TYPE,
		_LabelImg_ { {"",""}, {"",""}},
		_BGImg_ { {NULL,NULL}, {NULL,NULL}, {NULL,NULL}},
		1,
		_KeyVal_ {{""},{""},{""}},
		_KeyEvt_ {{0},{0},{0}},
		"",
		0,
		FALSE,
		},
		{NOT_USED,},
},

/* KEYSET_URL_POPUP */
{
	/* fValid, labelCnt, label, labelPropId, imgPath, keyValueCnt, keyValue, keyEvent, longKeyValue, longKeyEvent, userLabel,  userKeyValue,  userKeyEvent, */

	{
		USED, /* 1x1 */
		1,
		_Label_ {{".co.kr"},{".co.kr"},{".co.kr"}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"",""}, {"",""}, {"",""}},
		_BGImg_ {{"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}, {"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}, {"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}},
		1,
		_KeyVal_ {{".co.kr"},{".co.kr"},{".co.kr"}},
		_KeyEvt_ {{0},{0},{0}},
		"",
		0,
		FALSE,
	},
	{
		USED, /* 1x1 */
		1,
		_Label_ {{".or.kr"},{".or.kr"},{".or.kr"}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"",""}, {"",""}, {"",""}},
		_BGImg_ {{"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}, {"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}, {"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}},
		1,
		_KeyVal_ {{".or.kr"},{".or.kr"},{".or.kr"}},
		_KeyEvt_ {{0},{0},{0}},
		"",
		0,
		FALSE,
	},
	{
		USED, /* 1x1 */
		1,
		_Label_ {{".ac.kr"},{".ac.kr"},{".ac.kr"}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"",""}, {"",""}, {"",""}},
		_BGImg_ {{"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}, {"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}, {"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}},
		1,
		_KeyVal_ {{".ac.kr"},{".ac.kr"},{".ac.kr"}},
		_KeyEvt_ {{0},{0},{0}},
		"",
		0,
		FALSE,
	},
	{
		USED, /* 1x1 */
		1,
		_Label_ {{".go.kr"},{".go.kr"},{".go.kr"}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"",""}, {"",""}, {"",""}},
		_BGImg_ {{"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}, {"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}, {"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}},
		1,
		_KeyVal_ {{".go.kr"},{".go.kr"},{".go.kr"}},
		_KeyEvt_ {{0},{0},{0}},
		"",
		0,
		FALSE,
	},
	{
		USED, /* 1x1 */
		1,
		_Label_ {{".net"},{".net"},{".net"}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"",""}, {"",""}, {"",""}},
		_BGImg_ {{"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}, {"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}, {"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}},
		1,
		_KeyVal_ {{".net"},{".net"},{".net"}},
		_KeyEvt_ {{0},{0},{0}},
		"",
		0,
		FALSE,
	},
	{
		USED, /* 1x1 */
		1,
		_Label_ {{".org"},{".org"},{".org"}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"",""}, {"",""}, {"",""}},
		_BGImg_ {{"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}, {"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}, {"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}},
		1,
		_KeyVal_ {{".org"},{".org"},{".org"}},
		_KeyEvt_ {{0},{0},{0}},
		"",
		0,
		FALSE,
	},
	{
		USED, /* 1x1 */
		1,
		_Label_ {{".edu"},{".edu"},{".edu"}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"",""}, {"",""}, {"",""}},
		_BGImg_ {{"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}, {"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}, {"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}},
		1,
		_KeyVal_ {{".edu"},{".edu"},{".edu"}},
		_KeyEvt_ {{0},{0},{0}},
		"",
		0,
		FALSE,
	},
	{
		USED, /* 1x1 */
		1,
		_Label_ {{".gov"},{".gov"},{".gov"}},
		LABEL_PROPERTIES_PUNTUATION_BUTTON_TYPE,
		_LabelImg_ {{"",""}, {"",""}, {"",""}},
		_BGImg_ {{"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}, {"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}, {"textinput_moa_button2_nml.png","textinput_moa_button2_press.png"}},
		1,
		_KeyVal_ {{".gov"},{".gov"},{".gov"}},
		_KeyEvt_ {{0},{0},{0}},
		"",
		0,
		FALSE,
	},
}

#endif



