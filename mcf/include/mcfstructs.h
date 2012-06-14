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



#include "mcftypes.h"
#include "mcfconfig.h"

#ifndef __MCF_STRUCTS_H__
#define __MCF_STRUCTS_H__

//MCF_BEGIN_DECLS

/**
  *@brief MCF Layout structure, which is a physical keyboard representation defined by a group of McfLayoutKeyCoordinations
  */
typedef struct _McfLayout {
    mcfboolean fValid;		/* Indicates whether this information is in use or not */
    MCFLayoutStyle style;	/* Desribes the style of this layout */
    mcfchar* name;			/* Name of this layout */
    mcfshort x;				/* X coordination where this layout should be located */
    mcfshort y;				/* Y coordination where this layout should be located */
    mcfshort width;			/* Width of this layout */
    mcfshort height;		/* Height of this layout*/
    mcfchar* imgPath[MCF_BUTTON_STATE_MAX];/* Background image of each button state */
    mcfboolean fUseSWBtn;	/* Whether to draw buttons by SW or not */
    mcfboolean fUseMagnifierWnd;	/* Whether to use magnifier window or not */
    mcfboolean fUseZoomWnd;	/* Whether to use zoom window or not */
    mcfboolean fExtractBG;	/* Whether to display only some part of background image */
    mcfbyte linkedLayoutId;	/* Indicates the ID of linked layout when rotated */

    MCFDisplay displayType;	/* Indicates whether this layout is for portrait mode or landscape display */

    mcfboolean fUseSWbg;	/* Whether to draw background by SW or not */
    McfColor bgColor;		/* If drawn by SW, describes the background color */
    mcfdouble bgLineWidth;	/* If drawn by SW, describes the background border width */
    McfColor bgLineColor;	/* If drawn by SW, describes the background border color */

    mcfshort addGrabLeft;	/* If this layout's style is POPUP_GRAB, desribes the left pixel for grab area */
    mcfshort addGrabRight;	/* If this layout's style is POPUP_GRAB, desribes the right pixel for grab area */
    mcfshort addGrabTop;	/* If this layout's style is POPUP_GRAB, desribes the top pixel for grab area */
    mcfshort addGrabBottom;	/* If this layout's style is POPUP_GRAB, desribes the bottom pixel for grab area */
}McfLayout;

/**
  *@brief MCF Layout key coordination structure, which represents a key's position-related information
  */
typedef struct _McfLayoutKeyCoordination {
    mcfboolean fValid;		/* Indicates whether this information is in use or not */
    mcfshort x;				/* X coordination of this button */
    mcfshort y;				/* Y coordination of this button */
    mcfshort width;			/* Width of this button */
    mcfshort height;		/* Height of this button */
    mcfshort addHitLeft;	/* Left value for expanded hit area */
    mcfshort addHitRight;	/* Right value for expanded hit area */
    mcfshort addHitTop;		/* Left value for expanded hit area */
    mcfshort addHitBottom;	/* Left value for expanded hit area */
    mcfshort popXOffset;	/* Relative X offset for popup window */
    mcfshort popYOffset;	/* Relative Y offset for popup window */
    mcfshort popImageX;		/* If the popup layout has ExtractBG, this value provides the X offset inside the BG image */
    mcfshort popImageY;		/* If the popup layout has ExtractBG, this value provides the Y offset inside the BG image */
    mcfint subLayoutID;		/* Indicates which sublayout this button belongs to */
}McfLayoutKeyCoordination;

/**
*@brief MCF Layout keyset structure, indicates which keyset should be used for specific inputmode and layout combination
*/
typedef struct _McfLayoutKeyset {
    mcfint inputmodeID;		/* Target inputmode ID */
    mcfint layoutID;		/* Target layout ID */
    mcfint keysetID;		/* Keyset ID that should be selected when the above inputmodeID and layoutID are given */
} McfLayoutKeyset;

/**
  *@brief MCF Layout key configure structure
  */
typedef struct _McfLayoutKeyConfigure {
    mcfboolean fValid;			/* Indicates whether this information is in use or not */
    mcfshort customID;			/* A value for indentifying a button when handling private keys or disable key. */
    MCFButtonType buttonType;	/* Indicates the behavioral type of this button */
    MCFKeyType keyType;			/* Delivered key type when the button event is emitted */
    MCFPopupType popupType;		/* Popup type when this button is activated */
    mcfboolean fUseMagnifier;	/* Indicates whether this button needs to display magnifier window when clicked */
    mcfboolean fUseLongKeyMagnifier;	/* Indicates whether this button needs to display magnifier window when long-pressed */
    mcfboolean fEnable;			/* Indicates whether this button needs to be automatically enabled */
    mcfbyte popupInputMode[MCF_DRAG_STATE_MAX];	/* Popup inputmode IDs depending on the direction of the drag action */
    mcfboolean fIsSideButton;  /* Indicates whether this button is located on the very left or right of the layout */
}McfLayoutKeyConfigure;

/**
  *@brief MCF Label properties structure
  */
typedef struct _McfLabelProperties {
    mcfboolean fValid;			/* Indicates whether this information is in use or not */
    mcfchar* fontName;			/* Font name of this label property */
    mcfbyte fontSize;			/* Font size of this label property */
    McfColor fontColor[MCF_SHIFT_STATE_MAX][MCF_BUTTON_STATE_MAX];	/* Font color depending on the button and shift state */
    MCFLabelAlignment alignment;	/* Alignment option for this label property */
    mcfbyte paddingX;			/* X padding value for alignment option above */
    mcfbyte paddingY;			/* Y padding value for alignment option above */
    mcfbyte shadowDistance;		/* A distance value for drawing shadow */
    MCFShadowDirection shadowDirection;	/* A direction option for drawing shadown */
    McfColor shadowColor[MCF_SHIFT_STATE_MAX][MCF_BUTTON_STATE_MAX];	/* Shadow color depeding on the button and shift state */
}McfLabelProperties;

/**
  *@brief MCF Layout key properties structure
  */
/* MCF Supports drawing button's background using McfLayout's imgPath - use below macro to bgImgPath for this feature */
#define MCF_BACKGROUND_IMAGE_STRING "BACKGROUND_IMAGE"
#define _Label_
#define _LabelImg_
#define _BGImg_
#define _KeyVal_
#define _KeyEvt_
typedef struct _McfLayoutKeyProperties {
    mcfboolean fValid;			/* Indicates whether this information is in use or not */
    mcfbyte labelCnt;			/* Number of labels for this button (Max value :MAX_SIZE_OF_LABEL_FOR_ONE )*/
    mcfchar* label[MCF_SHIFT_STATE_MAX][MAX_SIZE_OF_LABEL_FOR_ONE]; /* Label strings depending on the shift state */
    mcfshort labelPropId;		/* Label propery ID for specifying the appearance of label strings */
    mcfchar* labelImgPath[MCF_SHIFT_STATE_MAX][MCF_BUTTON_STATE_MAX];	/* ImgPath for the case of image label */
    /** If bgImgPath is NULL, background will not be drawn. If "", layout image will be used instead */
    mcfchar* bgImgPath[MCF_SHIFT_STATE_MAX][MCF_BUTTON_STATE_MAX];	/* ImgPath for the button's background */
    mcfbyte keyValueCnt;		/* Number of key events in case of multitap or rotation button */
    mcfchar* keyValue[MCF_SHIFT_STATE_MAX][MAX_SIZE_OF_MULTITAP_CHAR];	/* A string value delivered to ISE when clicked */
    mcfulong keyEvent[MCF_SHIFT_STATE_MAX][MAX_SIZE_OF_MULTITAP_CHAR];	/* A long int value delivered to ISE when clicked */
    mcfchar* longKeyValue;		/* A string value delivered to ISE when long key clicked */
    mcfulong longKeyEvent;		/* A long int value delivered to ISE when long key clicked */
    mcfboolean useRepeatKey;	/* Indicates whether this button allows repeated key events when pressed for a long time */
    /* optional */
    /* A string of candidate characters that will be shown in an autopopup window */
    mcfchar* autopopupKeys[MCF_SHIFT_STATE_MAX][MAX_SIZE_OF_AUTOPOPUP_STRING];
    /* When set, any events raised from this button will not close opened popup window */
    mcfboolean dontClosePopup;
    /* Extra options for each types of buttons :
        BUTTON_TYPE_ROTATION - Set to 1 if the button should return to initial state when other key is pressed
        BUTTON_TYPE_DIRECTION -
            use definitions below, that are declared in mcfconfig.h file

            #define DIRECTION_EXTRA_OPTION_4_DIRECTIONS 0
            #define DIRECTION_EXTRA_OPTION_8_DIRECTIONS 1
            #define DIRECTION_EXTRA_OPTION_4_DIRECTIONS_WITH_LONG 2
            #define DIRECTION_EXTRA_OPTION_8_DIRECTIONS_WITH_LONG 3
            #define DIRECTION_EXTRA_OPTION_4_DIRECTIONS_WITH_RETURN 4
            #define DIRECTION_EXTRA_OPTION_8_DIRECTIONS_WITH_RETURN 5
            #define DIRECTION_EXTRA_OPTION_4_DIRECTIONS_WITH_RETURN_AND_CURVE 6
    */
    mcfbyte extraOption;
    mcfbyte multitouchType;		/* Indicates what kind of multitouch type this button supports */
    /* If the button should be displayed differently depending on the modifier, describe a decorator and provide its ID here */
    mcfbyte modifierDecorationId;
    /* For the case if this button needs to display multiple labels or different string from the label string */
    mcfchar* magnifierLabel[MCF_SHIFT_STATE_MAX][MAX_SIZE_OF_LABEL_FOR_ONE];
}McfLayoutKeyProperties;

/**
  *@brief MCF private key structure for application, to change the configuration and property of a specific key
  */
typedef struct _McfPrivateKeyProperties {
    mcfboolean fValid;			/* Indicates whether this information is in use or not */
    mcfshort inputModeIdx;		/* Target inputmode index that this private key wants to affect */
    mcfshort layoutIdx;			/* Target layout index that this private key wants to affect */
    mcfbyte keyIdx;				/* Target key index that this private key wants to affect */

    /* Configurations */ /* Has same purpose with the McfLayoutKeyConfiguration's member variable */
    mcfbyte customID;
    MCFButtonType buttonType;
    MCFKeyType keyType;
    MCFPopupType popupType;
    mcfboolean fUseMagnifier;
    mcfboolean fUseLongKeyMagnifier;
    mcfboolean fEnable;
    mcfbyte popupInputMode[MCF_DRAG_STATE_MAX];

    /* Properties */ /* Has same purpose with the McfLayoutKeyProperties' member variable */
    mcfbyte labelCnt;
    mcfchar* label[MCF_SHIFT_STATE_MAX][MAX_SIZE_OF_LABEL_FOR_ONE];
    mcfshort labelPropId;
    mcfchar* labelImgPath[MCF_SHIFT_STATE_MAX][MCF_BUTTON_STATE_MAX];
    mcfchar* bgImgPath[MCF_SHIFT_STATE_MAX][MCF_BUTTON_STATE_MAX];
    mcfbyte keyValueCnt;
    mcfchar* keyValue[MCF_SHIFT_STATE_MAX][MAX_SIZE_OF_MULTITAP_CHAR];
    mcfulong keyEvent[MCF_SHIFT_STATE_MAX][MAX_SIZE_OF_MULTITAP_CHAR];
    mcfchar* longKeyValue;
    mcfulong longKeyEvent;
    mcfboolean useRepeatKey;
    mcfchar* autopopupKeys[MCF_SHIFT_STATE_MAX][MAX_SIZE_OF_AUTOPOPUP_STRING];
    mcfboolean dontClosePopup;
    mcfbyte extraOption;
    mcfbyte multitouchType;
    mcfbyte modifierDecorationId;
}McfPrivateKeyProperties;

/**
  *@brief MCF input mode configuration structure, which indicates a type of SW keyboard
  */
typedef struct _McfInputModeConfigure {
    mcfchar* name;			/* Name of this inputmode */
    mcfboolean fUseScreenRotation;	/* Indicates whether this inputmode supports screen rotation */
    mcfbyte layoutId[MCFDISPLAY_MAX];	/* Each inputmode has 2 physical layouts - one for portrait and one for landscape */
}McfInputModeConfigure;

/**
  *@brief MCF input mode configuration structure, similar with McfInputModeConfigure but for popup windows
  */
typedef struct _McfInputModePopupConfigure {
    mcfchar* name;			/* Name of this popup inputmode */
    MCFLayoutStyle style;	/* The layout style of this popup inputmode */
    mcfboolean fUseRotation;	/* Indicates whether this popup inputmode supports screen rotation */
    mcfbyte layoutId[MCFDISPLAY_MAX];	/* Physical layouts for portrait and landscape mode */
    mcfboolean fUseWindow;	/* Whether to use a new window or make a popup window as a layer of existing base window */
    mcfboolean fUseDimWindow;	/* Whether to use a dim window */
    mcfint fTimeout; /* fTimeout seconds of idle state will close this popup window. Set 0 to disable */
}McfInputModePopupConfigure;

/**
  *@brief MCF S/W button style structure
  */
typedef struct _McfSWButtonStyle {
    mcfdouble lineWidth[MCF_BUTTON_STATE_MAX];	/* A border line width when drawing a SW button */
    McfColor lineColor[MCF_BUTTON_STATE_MAX];	/* A border line color when drawing a SW buttton */
    McfColor bgColor[MCF_BUTTON_STATE_MAX];		/* A background color when drawing a SW button */
    mcfdouble lineCurve;		/* A line curve value for drawing round rectangle. This can be ignored in some platforms */
    mcffloat bgAlpha;			/* A alpha value used when painting background. This can be ignored in some platforms */
}McfSWButtonStyle;

/**
  *@brief MCF input mode configuration structure
  */
typedef struct _McfDefaultConfigure {
    MCFDisplay display;			/* Initial display mode */
    mcfbyte inputMode;			/* Initial input mode */
    mcfboolean fUseMagnifierWnd;	/* A flag for enabling magnifer window */
    mcfboolean fUseAutoPopup;		/* A flag for enabling autopopup window */
    mcfboolean fUseZoomWnd;			/* A flag for enabling zoom window */
    mcfboolean fOnErrorNotiSnd;		/* A flag for playing error sound */
    mcfboolean fUseWordDeletion;	/* A flag for using word deletion mode when repeating delete key */
    mcfbyte swBtnStyleIdx;			/* Describes which SW button style should be used */
    MCFTouchOffsetLevel touchOffsetLevel[MCFDISPLAY_MAX];	/* Touch offset adjustment level description */
    McfPoint touchOffset[MCFDISPLAY_MAX];	/* Touch offset adjustment value description */
}McfDefaultConfigure;

/**
  *@brief MCF dim window configuration structure. This will be displayed between the base window and popup windows
  */
typedef struct _McfDimWndConfigure {
    mcfboolean fUseWindow;			/* Whether to use a new window or make a dim window as a layer of existing base window */
    McfColor dimColor;				/* Color setting for dim window */
}McfDimWndConfigure;

/**
*@brief MCF auto-generated popup configuration structure. Mostly used for umlaut alternate characters in european languages
*/
typedef struct _McfAutoPopupConfigure {
    mcfchar* bgImgPath;				/* File path for background image */
    McfColor bgColor;				/* Background color for SW background */
    mcfdouble bgLineWidth;			/* Background SW border width */
    McfColor bgLineColor;			/* Background SW border color */
    mcfbyte bgPadding;				/* Padding amount between the bg border and buttons */
    mcfchar* btnImgPath[MCF_BUTTON_STATE_MAX];	/* A filepath for each button's background image */
    mcfbyte swBtnStyleIdx;			/* Describes which SW button style should be used */
    mcfshort btnWidth;				/* Each button's width */
    mcfshort btnHeight;				/* Each button's height */
    mcfbyte btnSpacing;				/* Spacing amount between buttons */
    McfLabelProperties labelProp;	/* Label property for each button's label strings */
    mcfchar* wndDecoImgPath[MAX_WND_DECORATOR];	/* A 8-way decotation images for autopopup window */
    mcfbyte wndDecoSize;			/* Size of window decoration images */
    mcfbyte maxColumn;				/* Maximum number of items in a row */

    mcfshort addGrabLeft;			/* If this layout's style is POPUP_GRAB, desribes the left pixel for grab area */
    mcfshort addGrabRight;			/* If this layout's style is POPUP_GRAB, desribes the right pixel for grab area */
    mcfshort addGrabTop;				/* If this layout's style is POPUP_GRAB, desribes the top pixel for grab area */
    mcfshort addGrabBottom;			/* If this layout's style is POPUP_GRAB, desribes the bottom pixel for grab area */
}McfAutoPopupConfigure;

/**
*@brief 9 patch image information struct - image files registered here would be displayed in 9 patch style
*/
typedef struct _McfNinePatchInfo {
    mcfchar *imgPath;			/* File path for identifying nine patch image file */
    mcfshort left;				/* Amount of left pixels for describing area that should not be scaled */
    mcfshort right;				/* Amount of right pixels for describing area that should not be scaled */
    mcfshort top;				/* Amount of top pixels for describing area that should not be scaled */
    mcfshort bottom;			/* Amount of bottom pixels for describing area that should not be scaled */
}McfNinePatchInfo;

/**
*@brief A struct for decorating a button depending on the current modifier state
*/
typedef struct _McfModifierDecoration {
    mcfboolean fExtractBG;		/* Whether to display only some part of background image */
    mcfchar *bgImgPath[MCFDISPLAY_MAX][KEY_MODIFIER_MAX];	/* Background image file for decoration */
}McfModifierDecoration;

//MCF_END_DECLS

#endif //__MCF_STRUCTS_H__
