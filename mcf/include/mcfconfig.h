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


#ifndef __MCF_CONFIG_H__
#define __MCF_CONFIG_H__

#define NO_SOCKETPLUG


/* Display direction definition */
typedef enum _MCFDisplay {
    MCFDISPLAY_PORTRAIT,  /* portrait display mode */
    MCFDISPLAY_LANDSCAPE,  /* landscape display mode */
    MCFDISPLAY_MAX, /* maximum count of display mode */
}MCFDisplay;

/**@brief  Layout and Input mode Style definition */
typedef enum _MCFLayoutStyle {
    LAYOUT_STYLE_BASE,
    LAYOUT_STYLE_POPUP,
    LAYOUT_STYLE_POPUP_GRAB,
    MAX_LAYOUT_STYLE
}MCFLayoutStyle;

/* Key Define */

/**@brief  Button Type definition */
typedef enum _MCFButtonType {
    BUTTON_TYPE_NORMAL	 = 0, /**< Normal key */
    BUTTON_TYPE_GRAB,  /**< Focus grab */
    /* A button that fires click event when dragged into the button's area */
    BUTTON_TYPE_DRAG,  /**< Drag button */
    /* A button with variant value - ex> [a] -> [b] */
    BUTTON_TYPE_MULTITAP,/**< Multi-tap key */
    /* A multitap with automatic commitment - ex> ([a] -> a[b]) , ([p] -> p[i] -> pi[n] -> pin[g])*/
    BUTTON_TYPE_ROTATION,/**< Rotation key */
    BUTTON_TYPE_DIRECTION,/**< Direction key */
    BUTTON_TYPE_UIITEM, /** Not a button, just for displaying image */
    MAX_BUTTON_TYPE/* maximum number of Key type */
}MCFButtonType;

/* Values used for extraOption field of McfLayoutKeyProperties structure, in case of BUTTON_TYPE_DIRECTION type button */
#define DIRECTION_EXTRA_OPTION_4_DIRECTIONS 0
#define DIRECTION_EXTRA_OPTION_8_DIRECTIONS 1
#define DIRECTION_EXTRA_OPTION_4_DIRECTIONS_WITH_LONG 2
#define DIRECTION_EXTRA_OPTION_8_DIRECTIONS_WITH_LONG 3
#define DIRECTION_EXTRA_OPTION_4_DIRECTIONS_WITH_RETURN 4
#define DIRECTION_EXTRA_OPTION_8_DIRECTIONS_WITH_RETURN 5
#define DIRECTION_EXTRA_OPTION_4_DIRECTIONS_WITH_RETURN_AND_CURVE 6

/**@brief Key Type definition */
typedef enum _MCFKeyType {
    KEY_TYPE_NONE = 0,	       /**< none */
    KEY_TYPE_CHAR,			/**< key that can be displayed independently */
    KEY_TYPE_CONTROL,		/**< key to use as control */
    KEY_TYPE_MODECHANGE,	/**< key to change mode*/
    KEY_TYPE_UNICODE,		/**< key to be displayed as unicode */
    KEY_TYPE_SYMBOL,		/**< key to be displayed as symbols (unicode) */
    KEY_TYPE_COMPOSITION,	/**< Key to request Composition char */
    MAX_KEY_TYPE			/* maximum number of Key type */
}MCFKeyType;

/**@brief Key Modifier definition */
typedef enum _MCFKeyModifier {
    KEY_MODIFIER_NONE = 0,				/**< none */
    KEY_MODIFIER_LONGKEY,				/**< longkey event */
    KEY_MODIFIER_MULTITAP_START,				/**< multitap started */
    KEY_MODIFIER_MULTITAP_REPEAT,				/**< multitap repeated */

    KEY_MODIFIER_DIRECTION_LEFT,			/**< drag left */
    KEY_MODIFIER_DIRECTION_RIGHT,			/**< drag right */
    KEY_MODIFIER_DIRECTION_UP,				/**< drag up */
    KEY_MODIFIER_DIRECTION_DOWN,			/**< drage down */
    KEY_MODIFIER_DIRECTION_UP_LEFT,			/**< drag up left */
    KEY_MODIFIER_DIRECTION_UP_RIGHT,		/**< drag up right */
    KEY_MODIFIER_DIRECTION_DOWN_LEFT,		/**< drag down left */
    KEY_MODIFIER_DIRECTION_DOWN_RIGHT,		/**< drage down right */

    KEY_MODIFIER_DIRECTION_LEFT_LONG,			/**< drag left */
    KEY_MODIFIER_DIRECTION_RIGHT_LONG,			/**< drag right */
    KEY_MODIFIER_DIRECTION_UP_LONG,				/**< drag up */
    KEY_MODIFIER_DIRECTION_DOWN_LONG,			/**< drage down */
    KEY_MODIFIER_DIRECTION_UP_LEFT_LONG,			/**< drag up left */
    KEY_MODIFIER_DIRECTION_UP_RIGHT_LONG,		/**< drag up right */
    KEY_MODIFIER_DIRECTION_DOWN_LEFT_LONG,		/**< drag down left */
    KEY_MODIFIER_DIRECTION_DOWN_RIGHT_LONG,		/**< drage down right */

    KEY_MODIFIER_DIRECTION_LEFT_RETURN,			/**< drag left */
    KEY_MODIFIER_DIRECTION_RIGHT_RETURN,			/**< drag right */
    KEY_MODIFIER_DIRECTION_UP_RETURN,				/**< drag up */
    KEY_MODIFIER_DIRECTION_DOWN_RETURN,			/**< drage down */
    KEY_MODIFIER_DIRECTION_UP_LEFT_RETURN,			/**< drag up left */
    KEY_MODIFIER_DIRECTION_UP_RIGHT_RETURN,		/**< drag up right */
    KEY_MODIFIER_DIRECTION_DOWN_LEFT_RETURN,		/**< drag down left */
    KEY_MODIFIER_DIRECTION_DOWN_RIGHT_RETURN,		/**< drage down right */

    KEY_MODIFIER_DIRECTION_CURVE_UP_LEFT,			/**< drag up left */
    KEY_MODIFIER_DIRECTION_CURVE_UP_RIGHT,		/**< drag up right */
    KEY_MODIFIER_DIRECTION_CURVE_DOWN_LEFT,		/**< drag down left */
    KEY_MODIFIER_DIRECTION_CURVE_DOWN_RIGHT,		/**< drage down right */
    KEY_MODIFIER_DIRECTION_CURVE_LEFT_UP,			/**< drag left up */
    KEY_MODIFIER_DIRECTION_CURVE_LEFT_DOWN,		/**< drag left down */
    KEY_MODIFIER_DIRECTION_CURVE_RIGHT_UP,		/**< drag right up */
    KEY_MODIFIER_DIRECTION_CURVE_RIGHT_DOWN,		/**< drage right down */

    KEY_MODIFIER_MAX			/* maximum number of Key type */
}MCFKeyModifier;


/**@brief  Popup Type definition */
typedef enum _MCFPopupType {
    POPUP_TYPE_NONE = 0,/**< doesn't use popup */
    POPUP_TYPE_BTN_RELEASE_POPUP,/**< to use popup (It is used in extention character sign. After to be selected, this action is continued until selecting another area) */
    POPUP_TYPE_BTN_RELEASE_POPUP_ONCE,/**< to use popup (It is used in extention character sign. After to be selected, this action disappears instantly) */
    POPUP_TYPE_BTN_PRESS_POPUP_DRAG,/**< to use popup (it is used in extention character sign. This action disappears at Drag-Up */
    POPUP_TYPE_BTN_LONGPRESS_POPUP,/**< to use popup (it is used in extention character sign. This action disappears at Drag-Up */
    POPUP_TYPE_BTN_LONGPRESS_POPUP_ONCE,/**< to use popup (it is used in extention character sign. This action disappears at Drag-Up */
    POPUP_TYPE_CONTEXT_POPUP,
    POPUP_TYPE_AUTO_POPUP,/**< to use popup (for multi-tap char) */
    MAX_POPUP_TYPE/* maximum number of popup type */
}MCFPopupType;

/**@brief Candidate position style definition */
typedef enum _MCFCandidatePosition {
    CAND_POS_ABOVE_KEYPAD = 0,
    CAND_POS_BELOW_TEXT,
    CAND_POS_MANUAL,
    MAX_CANDIDATE_POSITION
}MCFCandidatePosition;

/**@brief Candidate style definition */
typedef enum _MCFCandidateStyle {
    CAND_STYLE_DEFAULT = 0,
    CAND_STYLE_A,
    CAND_STYLE_B,
    CAND_STYLE_C,
    MAX_CANDIDATE_STYLE
}MCFCandidateStyle;

/**@brief Preview window position type definition */
typedef enum _MCFPreviewPosition {
    PREVIEW_POS_CENTER_OF_APPL_AREA = 0,
    PREVIEW_POS_ABOVE_SELECTED_BUTTON,
    PREVIEW_POS_MANUAL,
    MAX_PREVIEW_POSITION
}MCFPreviewPosition;

/**@brief Action state definition */
typedef enum _MCFActionState {
    ACTION_STATE_BASE_INIT = 0,
    ACTION_STATE_BASE_PRESS,
    ACTION_STATE_BASE_MOVING,
    ACTION_STATE_BASE_RELEASE,
    ACTION_STATE_BASE_LONGKEY,
    ACTION_STATE_BASE_REPEATKEY,
    ACTION_STATE_POPUP_INIT,
    ACTION_STATE_POPUP_PRESS,
    ACTION_STATE_POPUP_MOVING,
    ACTION_STATE_POPUP_RELEASE,
    ACTION_STATE_POPUP_LONGKEY,
    ACTION_STATE_POPUP_REPEATKEY,
    MAX_ACTION_STATE
}MCFActionState;

/**@brief  Button state definition */
typedef enum _MCFButtonState {
    BUTTON_STATE_NORMAL	 = 0, /**< normal state */
    BUTTON_STATE_PRESSED, /**< pressed state */
    BUTTON_STATE_DISABLED, /**< disabled state */
    //BUTTON_STATE_HIGHLIGHT, /**< highlighed state */
    //BUTTON_STATE_LONGKEY, /**< londkey state */
    MCF_BUTTON_STATE_MAX/* maximum number of button state */
}MCFButtonState;


/**@brief  Drag Direction definition */
typedef enum _MCFDragType {
    DRAG_NONE = 0,		/* doesn't use Drag */
    DRAG_LEFT,		/* drag to left side */
    DRAG_RIGHT,		/* drag to right side */
    DRAG_UP,			/* drag to upper side */
    DRAG_DOWN,		/* drage to down side */
    DRAG_LEFT_TOP,		/* drag to left top side */
    DRAG_RIGHT_TOP,		/* drag to right top side */
    DRAG_LEFT_BOTTOM,	/* drag to left bottm side */
    DRAG_RIGHT_BOTTOM,	/* drage to right bottom side */
    MAX_DRAG,		     /* maximum number of drag type */
}MCFDragType;

/**@brief  Label alignment definition */
typedef enum _MCFLabelAlignment {
    LABEL_ALIGN_LEFT_TOP = 0,
    LABEL_ALIGN_CENTER_TOP,
    LABEL_ALIGN_RIGHT_TOP,
    LABEL_ALIGN_LEFT_MIDDLE,
    LABEL_ALIGN_CENTER_MIDDLE,
    LABEL_ALIGN_RIGHT_MIDDLE,
    LABEL_ALIGN_LEFT_BOTTOM,
    LABEL_ALIGN_CENTER_BOTTOM,
    LABEL_ALIGN_RIGHT_BOTTOM,
    MAX_LABEL_ALIGNMENT
}MCFLabelAlignment;

/**@brief  Shadow direction definition */
typedef enum _MCFShadowDirection {
    SHADOW_DIRECTION_NONE = 0,
    SHADOW_DIRECTION_LEFT_TOP ,
    SHADOW_DIRECTION_CENTER_TOP,
    SHADOW_DIRECTION_RIGHT_TOP,
    SHADOW_DIRECTION_LEFT_MIDDLE,
    SHADOW_DIRECTION_CENTER_MIDDLE,
    SHADOW_DIRECTION_RIGHT_MIDDLE,
    SHADOW_DIRECTION_LEFT_BOTTOM,
    SHADOW_DIRECTION_CENTER_BOTTOM,
    SHADOW_DIRECTION_RIGHT_BOTTOM,
    MAX_SHADOW_DIRECTION
}MCFShadowDirection;

/**@brief  timer id */
typedef enum _MCFTimer {
    MCF_TIMER_AUTOPOPUP = 100,
    MCF_TIMER_SHORT_LONGKEY,
    MCF_TIMER_LONGKEY,
    MCF_TIMER_REPEATKEY,
    MCF_TIMER_USERAREA,
    MCF_TIMER_BUTTON_DELAY,
    MCF_TIMER_POPUP_TIMEOUT,
    MCF_TIMER_AUTOTEST,
}MCFTimer;

/**@brief  Touch Offset */
typedef enum _MCFTouchOffsetLevel {
    TOUCH_OFFSET_LEVEL_1,
    TOUCH_OFFSET_LEVEL_2,
    TOUCH_OFFSET_LEVEL_3,
    MAX_TOUCH_OFFSET_LEVEL
}MCFTouchOffsetLevel;

/* Type Limitation definition */
#define MAX_SIZE_OF_LAYOUT_NAME 255
#define MAX_SIZE_OF_MULTITAP_CHAR 10
#define MAX_SIZE_OF_LABEL_FOR_ONE 9
#define MAX_SIZE_OF_LAYOUT 10
#define MAX_SIZE_OF_CANDIDATE 50
#define MAX_PRIVATE_KEY 100
#define MAX_SIZE_OF_AUTOPOPUP_STRING 20

/**@brief  Window Decorator Images */
typedef enum _MCFWindowDecorator {
    WND_DECORATOR_TOP_LEFT,
    WND_DECORATOR_TOP_CENTER,
    WND_DECORATOR_TOP_RIGHT,
    WND_DECORATOR_MIDDLE_LEFT,
    WND_DECORATOR_MIDDLE_RIGHT,
    WND_DECORATOR_BOTTOM_LEFT,
    WND_DECORATOR_BOTTOM_CENTER,
    WND_DECORATOR_BOTTOM_RIGHT,
    MAX_WND_DECORATOR,
}MCFWindowDecorator;

/**@brief  MCF Notification to ISEs */
typedef enum _MCFNotiType {
    NOTITYPE_POPUP_TIMEOUT, // The layout of popup window will be passed as etcInfo data
    NOTITYPE_GESTURE_FLICK,
    MAX_NOTITYPE,
}MCFNotiType;

/**@brief  Current dragging state */
typedef enum _MCFDragState {
    MCF_DRAG_STATE_NONE,
    MCF_DRAG_STATE_LEFT,
    MCF_DRAG_STATE_RIGHT,
    MCF_DRAG_STATE_UP,
    MCF_DRAG_STATE_DOWN,
    MCF_DRAG_STATE_INVALID,
    MCF_DRAG_STATE_RETURN,
    MCF_DRAG_STATE_MAX,
} MCFDragState;

typedef enum _MCFMultitouchType {
    MCF_MULTI_TOUCH_TYPE_EXCLUSIVE,
    MCF_MULTI_TOUCH_TYPE_COOPERATIVE,
    MCF_MULTI_TOUCH_TYPE_GRAB_SUB_EVENTS,
} MCFMultiTouchType;

/* Shift flag */
typedef enum _MCFShiftState {
    MCF_SHIFT_STATE_OFF,
    MCF_SHIFT_STATE_ON,
    MCF_SHIFT_STATE_LOCK,
    MCF_SHIFT_STATE_MAX,
} MCFShiftState;

/* resolution */
#define MCF_BASE_SCREEN_WIDTH 720
#define MCF_BASE_SCREEN_HEIGHT 1280

/* MCF predefined Identifiers */
#define MCF_LAYOUT_AUTOPOPUP 10000

#define MCF_LABEL_PROPERTY_AUTOPOPUP 10000
#define MCF_LABEL_PROPERTY_CANDIDATE 10001
#define MCF_LABEL_PROPERTY_ZOOMING 10002
#define MCF_LABEL_PROPERTY_PREVIEW 10003

#define MCF_LONGKEY_DURATION 800
#define MCF_LONGKEY_CANCEL_DIST -1
#define MCF_SHORT_LONGKEY_DURATION 500
#define MCF_REPEATKEY_DURATION 150
#define MCF_REPEATKEY_WORD_DELETION_START_DURATION 100
#define MCF_REPEATKEY_ACCELERATION 10
#define MCF_REPEATKEY_MIN_DURATION 50

#define MCF_BUTTON_MIN_DURATION 100

#define MCF_DIRECTION_RECOG_DIST 40
#define MCF_DIRECTION_RECOG_DIST_SIDE 20
#define MCF_DIRECTION_LONG_RECOG_DIST 50

#define MCF_DRAG_CURVE_RECOG_DIST 70
#define MCF_DRAG_CURVE_4_DIRECTION_ANGLE_VALUE 1.0f // ratio between x and y for determining 4 direction, 3 means atan(1/3) degree
#define MCF_DRAG_CURVE_FINAL_ANGLE_VALUE 3.0f // ratio between x and y for determining final drag curve, 3 means atan(1/3) degree

#define MCF_DRAG_RETURN_RECOG_THRESHOLD_RETURN 1.1f // In RETURN drag state, apply this value for checking drag return action
#define MCF_DRAG_RETURN_RECOG_THRESHOLD_OTHER 0.9f // In non-NONE drag state, apply this value for checking drag return action
#define MCF_DRAG_RETURN_RECOG_DIST_BACKWARD 60
#define MCF_DRAG_RETURN_RECOG_ANGLE_BACKWARD M_PI / 8

#define MCF_AUTOTEST_TIMER_INTERVAL 20
#define MCF_AUTOTEST_TIMER_INITIAL_INTERVAL 3000
#define MCF_AUTOTEST_NUM 500

#define MCF_MOUSE_BUTTON_CHANGE_THRESHOLD_X 15
#define MCF_MOUSE_BUTTON_CHANGE_THRESHOLD_Y 20

#define MCF_FLICK_GESTURE_RECOG_THRESHOLD 200
#define MCF_FLICK_GESTURE_RECOG_TIME 500

#define MCF_AUTO_DETECT_PORTRAIT_LANDSCAPE TRUE

typedef enum _MCFDebugMode {
    DEBUGMODE_DISABLED,
    DEBUGMODE_DISPLAY_INTERNAL,
    DEBUGMODE_AUTOTEST,
}MCFDebugMode;

#endif //__MCF_CONFIG_H__
