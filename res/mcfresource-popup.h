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
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_LabelImg_ {{"B09_icon_arrow_left.png", "B09_icon_arrow_left_press.png", "B09_icon_arrow_left.png"}, {"B09_icon_arrow_left.png", "B09_icon_arrow_left_press.png", "B09_icon_arrow_left.png"}, {"B09_icon_arrow_left.png", "B09_icon_arrow_left_press.png", "B09_icon_arrow_left.png"}},
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_LabelImg_ {{"B09_icon_arrow_right.png", "B09_icon_arrow_right_press.png", "B09_icon_arrow_right.png"}, {"B09_icon_arrow_right.png", "B09_icon_arrow_right_press.png", "B09_icon_arrow_right.png"}, {"B09_icon_arrow_right.png", "B09_icon_arrow_right_press.png", "B09_icon_arrow_right.png"}},
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_LabelImg_ {{"B09_icon_arrow_up.png", "B09_icon_arrow_up_press.png", "B09_icon_arrow_up.png"}, {"B09_icon_arrow_up.png", "B09_icon_arrow_up_press.png", "B09_icon_arrow_up.png"}, {"B09_icon_arrow_up.png", "B09_icon_arrow_up_press.png", "B09_icon_arrow_up.png"}},
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_LabelImg_ {{"B09_icon_arrow_down.png", "B09_icon_arrow_down_press.png", "B09_icon_arrow_down.png"}, {"B09_icon_arrow_down.png", "B09_icon_arrow_down_press.png", "B09_icon_arrow_down.png"}, {"B09_icon_arrow_down.png", "B09_icon_arrow_down_press.png", "B09_icon_arrow_down.png"}},
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
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
		_BGImg_ {{"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}, {"B09_bg_Popup.png","B09_key_btn_press.png","B09_key_btn_press.png"}},
		1,
		_KeyVal_ {{".gov"},{".gov"},{".gov"}},
		_KeyEvt_ {{0},{0},{0}},
		"",
		0,
		FALSE,
	},
}

#endif



