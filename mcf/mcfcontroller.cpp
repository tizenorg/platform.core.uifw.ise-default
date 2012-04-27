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

#include <time.h>
#include <math.h>
#include <Elementary.h>
#include "mcfcontroller.h"
#include "mcfdebug.h"
#include "mcfresourcecache.h"
#include "mcfactionstate.h"
#include "mcfuibuilder.h"
#include "mcfkeydefines.h"
#include "mcffeedback.h"
#include "mcferroradjustment.h"
#include "mcfimageproxy.h"
#include "mcfcore.h"
#include "../src/include/ise.h"

MCFKeyModifier prevModifier;
McfPoint zoomwinpos = {0,};
const mcfint zoomwinrelposy=20;
static mcfboolean isLongKey = false;

using namespace mcf;

CMCFController* CMCFController::sInstance; /* For singleton */

extern CMCFCore *gCore;
extern CMCFContext *gContext;
extern SETTING_INFO _setup_info;

CMCFController::CMCFController()
{
    MCF_DEBUG();

    mEventCallback = NULL;
    mLongkeyDuration = MCF_LONGKEY_DURATION;
    mLongkeyCancelDist = MCF_LONGKEY_CANCEL_DIST;
    mRepeatKeyDuration = MCF_REPEATKEY_DURATION;
    mShortLongkeyDuration = MCF_SHORT_LONGKEY_DURATION;
    mButtonDelayDuration = MCF_BUTTON_MIN_DURATION;
    mRepeatKeySeq = 0;
    mDebugMode = DEBUGMODE_DISABLED;
    mDebugVariable = 0;
    mInputEventsDisabled = FALSE;
 }

CMCFController::~CMCFController()
{
    MCF_DEBUG();
}

CMCFController*
CMCFController::get_instance()
{
    if (!sInstance) {
        sInstance = new CMCFController();
    }
    return (CMCFController*)sInstance;
}

void
CMCFController::init()
{
    MCF_DEBUG();
    CMCFResourceCache::get_instance();
}

#ifdef TEST_NEWBACKEND
#include <Ecore_Evas.h>
#include <Ecore.h>
#include <vector>
typedef enum {
	EFLOBJECT_NONE,
	EFLOBJECT_IMAGE,
	EFLOBJECT_CLIPOBJECT,
	EFLOBJECT_TEXTBLOCK,
} EFLOBJECT_TYPE;

	typedef struct {
		EFLOBJECT_TYPE type;
		McfRectangle position;
		Evas_Object *obj;
		char *etcInfo;
		mcfboolean bExtracted;
		void *data;
	} EFLObject;
#include <Ecore_Evas.h>
#include <Ecore.h>
	typedef struct {
		mcfboolean used;

		Evas_Object *image;
		Evas_Object *clipper;

		mcfwindow window;
		mcfchar imgPath[_POSIX_PATH_MAX];
		mcfint imgPathHash;
		mcfint destX;
		mcfint destY;
		mcfint destWidth;
		mcfint destHeight;
		mcfint srcX;
		mcfint srcY;
		mcfint srcWidth;
		mcfint srcHeight;
		mcfboolean extractImg;
	} ImageCache;

	typedef struct {
		mcfboolean used;

		Evas_Object *text;

		mcfwindow window;
		mcf::McfFontInfo fontinfo;
		McfColor color;
		mcfchar str[_POSIX_PATH_MAX];;
		mcfint strHash;
		mcfint posx;
		mcfint posy;
		mcfint width;
		mcfint height;
		MCFLabelAlignment align;
		mcfbyte paddingx;
		mcfbyte paddingy;
	} TextCache;

extern std::vector<ImageCache> g_ImageCache;
extern std::vector<TextCache> g_TextCache;
#else
#endif

/**
 * Sets the current input mode to the given mode
 */
mcfboolean
CMCFController::process_input_mode_change(const mcfbyte mode, mcfboolean saveMultikeys /* = TRUE */)
{
    MCF_DEBUG();
    mcf_assert_return_false(mode >= 0);
    CMCFContext *context = CMCFContext::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();

    if (context && windows && cache) {
        mcfint loop;
        if (saveMultikeys) {
            /* Save multikey indexes */
            for (loop = 0;loop < MAX_KEY;loop++) {
                McfLayoutKeyConfigure *pCurLayoutKeyConfigure = cache->get_cur_layout_key_configure(windows->get_base_window(), loop);
                McfButtonContext *pCurButtonContext = cache->get_cur_button_context(windows->get_base_window(), loop);
                if (pCurLayoutKeyConfigure && pCurButtonContext) {
                    if (pCurLayoutKeyConfigure->buttonType == BUTTON_TYPE_ROTATION && pCurLayoutKeyConfigure->fValid == USED) {
                        mcfint innerLoop;
                        mcfint empty_index = -1;
                        for (innerLoop = 0;innerLoop < MAX_MULTIKEY_BUFFER && empty_index == -1;innerLoop++) {
                            if (mMultiKeyBuffer[innerLoop][0] == NOT_USED) {
                                empty_index = innerLoop;
                            }
                        }
                        if (empty_index != -1) {
                            mMultiKeyBuffer[empty_index][0] = context->get_base_layout();
                            mMultiKeyBuffer[empty_index][1] = loop;
                            mMultiKeyBuffer[empty_index][2] = pCurButtonContext->multikeyIdx;
                        }
                    }
                }
            }
        }
        if (mode == context->get_input_mode()) { /* BtnContext does not get initialized if we don't call here */
            cache->recompute_layout(windows->get_base_window());
        }

        if (mode == context->get_input_mode()) return FALSE;
        context->set_input_mode(mode);
        context->set_base_layout(mcf_input_mode_configure[mode].layoutId[context->get_display()]);

        mcfwindow window = windows->get_base_window();
        handle_engine_signal(MCF_SIG_INPMODE_CHANGE, window);

#ifdef TEST_NEWBACKEND
		McfWindowContext *winctx = windows->get_window_context(window, FALSE);
		if (winctx) {
			if (winctx->etcInfoPtr) {

				Eina_List *list = (Eina_List*)(winctx->etcInfoPtr);
				Eina_List *iter = NULL;
				Eina_List *iter_next = NULL;
				void *data = NULL;
				int iIndex = 0;

				EINA_LIST_FOREACH_SAFE(list, iter, iter_next, data) {
					if (data) {
						EFLObject *obj = (EFLObject*)(data);
						if(obj) {
							Evas_Object* eo = obj->obj;
							if(obj->bExtracted) {
								void *data = evas_object_image_data_get(eo, 1);
								if(data) {
									free(data);
								}
							}

							mcfint loop;
							for(loop = 0;loop < g_ImageCache.size();loop++) {
								if(g_ImageCache[loop].image == obj->obj) {
									g_ImageCache[loop].used = FALSE;
								}
							}
							for(loop = 0;loop < g_TextCache.size();loop++) {
								if(g_TextCache[loop].text == obj->obj) {
									g_TextCache[loop].used = FALSE;
								}
							}

						if(eo) {
							evas_object_del(eo);
							obj->obj = NULL;
						}
						delete obj;
					}
					list = eina_list_remove_list(list, iter);
				}
				iIndex++;
			}
			winctx->etcInfoPtr = list;
		    }
		}
#endif

        /* Restore multikey indexes */
        mcfshort base_layout = context->get_base_layout();
        for (loop = 0;loop < MAX_MULTIKEY_BUFFER;loop++) {
            if (mMultiKeyBuffer[loop][0] == base_layout) {
                mcfint keyidx = mMultiKeyBuffer[loop][1];
                McfLayoutKeyConfigure *pCurLayoutKeyConfigure = cache->get_cur_layout_key_configure(windows->get_base_window(),
                		                                                                            keyidx);
                McfLayoutKeyProperties *pCurLayoutKeyProperties = cache->get_cur_layout_key_properties(windows->get_base_window(),
                		                                                                               keyidx);
                McfButtonContext *pCurButtonContext = cache->get_cur_button_context(windows->get_base_window(), keyidx);
                if (pCurLayoutKeyConfigure && pCurButtonContext && pCurLayoutKeyProperties) {
                    if (pCurLayoutKeyConfigure->buttonType == BUTTON_TYPE_ROTATION && pCurLayoutKeyConfigure->fValid == USED) {
                        pCurButtonContext->multikeyIdx = mMultiKeyBuffer[loop][2];
                        if (pCurButtonContext->multikeyIdx >= pCurLayoutKeyProperties->keyValueCnt) {
                            pCurButtonContext->multikeyIdx = 0;
                        }
                    }
                }
            }
        }
    }

    return TRUE;
}

void
CMCFController::clear_multikey_buffer()
{
    for (int loop = 0;loop < MAX_MULTIKEY_BUFFER;loop++) {
        mMultiKeyBuffer[loop][0] = NOT_USED;
    }
}

/**
 * Sets the current display mode to the given mode
 */
mcfboolean
CMCFController::process_display_change()
{
    MCF_DEBUG();
    CMCFContext *context = CMCFContext::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();

    if (context && windows) {
        mcf8 mode;
            mode = MCFDISPLAY_PORTRAIT;

        context->set_display(static_cast<MCFDisplay>(mode));
        context->set_base_layout(mcf_input_mode_configure[context->get_input_mode()].layoutId[context->get_display()]);

        mcfwindow window = windows->get_base_window();
        handle_engine_signal(MCF_SIG_DISP_CHANGE, window);
        windows->update_window(window);
    }
    return TRUE;
}

mcfboolean
CMCFController::process_button_pressed_event(mcfwindow window, mcfint x, mcfint y, mcfbyte keyindex, mcftouchdevice touchid)
{
    MCF_DEBUG();
    mcfboolean ret = FALSE;
    mcfboolean redraw = FALSE;

    CMCFContext *context = CMCFContext::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFEvents *events = CMCFEvents::get_instance();
    CMCFUtils *utils = CMCFUtils::get_instance();
    CMCFFeedback *feedback = CMCFFeedback::get_instance();

    McfButtonContext *btncontext = NULL;
    const McfLayoutKeyCoordination *coordination = NULL;
    const McfLayoutKeyConfigure *configure = NULL;
    const McfLayoutKeyProperties *properties = NULL;

    if(context && cache) {
        btncontext = cache->get_cur_button_context(window, keyindex);
        coordination = cache->get_cur_layout_key_coordination(window, keyindex);
        configure = cache->get_cur_layout_key_configure(window, keyindex);
        properties = cache->get_cur_layout_key_properties(window, keyindex);
    }

    static mcfwindow prevwin = MCFWINDOW_INVALID;
    static mcfbyte prevkey = NOT_USED;

    if (context && cache && windows && events && utils && feedback && btncontext && coordination && configure && properties) {
        /* If this button is pressed */
        if (x >= coordination->x - coordination->addHitLeft &&
            x < coordination->x + coordination->width + coordination->addHitRight &&
            y >= coordination->y - coordination->addHitTop &&
            y < coordination->y + coordination->height + coordination->addHitBottom &&
            /* Process the event only if the this item's sublayout id is active one */
            (coordination->subLayoutID == SUBLAYOUTID_NONE || coordination->subLayoutID == context->get_cur_sublayout_id())) {
            /* If newly pressed key has type MULTI_TOUCH_TYPE_EXCLUSIVE, release all existing pressed events */
            if(properties->multitouchType == MCF_MULTI_TOUCH_TYPE_EXCLUSIVE) {
                for(mcfint loop = 0;loop < context->get_multi_touch_context_num();loop++) {
                    McfKeyEventDesc desc;
                    context->get_multi_touch_event(loop, &desc);
                    if(desc.touchid != touchid) {
                        mouse_release(context->get_cur_move_window(desc.touchid),
                            context->get_cur_move_point(desc.touchid).x, context->get_cur_move_point(desc.touchid).y, desc.touchid);
                    }
                }
            }

            /* Make an unique ID for timer */
            const mcf16 uniqId = utils->get_unique_id();

            context->set_cur_pressed_event_id(touchid, uniqId);
            context->set_cur_pressed_key(touchid, keyindex);
            context->set_cur_pressed_window(touchid, window);

            btncontext->state = BUTTON_STATE_PRESSED;

            redraw = TRUE;
            ret = TRUE;

            /* for feedback */
            feedback->button_pressed(window, keyindex);

            /* Special routine for autopopup */
            if (configure->popupType == POPUP_TYPE_AUTO_POPUP) {
                events->create_timer(MCF_TIMER_AUTOPOPUP, mShortLongkeyDuration, uniqId);
            } else {
                /* for long key & repeat key */
                events->create_timer(MCF_TIMER_LONGKEY, mLongkeyDuration, uniqId);
            }

            MCFShiftState shiftidx = context->get_shift_state();
            if(shiftidx < 0 || shiftidx >= MCF_SHIFT_STATE_MAX) {
            	shiftidx = MCF_SHIFT_STATE_OFF;
            }

            McfKeyEventDesc keyEventDesc = {0};
            keyEventDesc.keyValue = properties->keyValue[shiftidx][0];
            keyEventDesc.keyEvent = properties->keyEvent[shiftidx][0];
            keyEventDesc.keyType = configure->keyType;
            keyEventDesc.keyModifier = KEY_MODIFIER_NONE;

            McfPoint curpoint = {x, y};
            keyEventDesc.touchid = touchid;
            keyEventDesc.mousePressedPt = curpoint;
            keyEventDesc.mouseCurrentPt = curpoint;
            keyEventDesc.mouseFarthestPt = curpoint;

            keyEventDesc.touch_event_order = context->get_multi_touch_event_order(touchid);

            McfKeyEventDesc desc = {0};
            desc.keyType = configure->keyType;
            desc.keyValue = properties->keyValue[shiftidx][0];
            desc.keyEvent = properties->keyEvent[shiftidx][0];
            desc.keyModifier = KEY_MODIFIER_NONE;
            prevModifier = KEY_MODIFIER_NONE;

            /* Now process normal behaviours of each button type */
            switch (configure->buttonType) {
            case BUTTON_TYPE_NORMAL:
            case BUTTON_TYPE_GRAB:
            case BUTTON_TYPE_DIRECTION: {
                /* Send click event right away if this button uses repeat key */
                if (properties->useRepeatKey) {
                    if (mEventCallback) {
                        mEventCallback->on_event_key_clicked(keyEventDesc);
                    }
                }
            }
            break;
            case BUTTON_TYPE_MULTITAP: {
            }
            break;
            case BUTTON_TYPE_ROTATION: {
            }
            break;
            case BUTTON_TYPE_DRAG: {
                /* Drag buttons fires click event immediately when they are pressed */
                if (mEventCallback) {
                    mEventCallback->on_event_key_clicked(keyEventDesc);
                }
            }
            break;
            }


            switch (configure->popupType) {
            case POPUP_TYPE_BTN_PRESS_POPUP_DRAG: {
                mcfbyte popupInputMode = configure->popupInputMode[MCF_DRAG_STATE_NONE];
                MCFDisplay display = context->get_display();
                if(mcf_check_arrindex(popupInputMode, MAX_INPUT_MODE_POPUP) &&
                    mcf_check_arrindex(display, MCFDISPLAY_MAX)) {
                    mcfshort popupLayoutId = mcf_input_mode_popup_configure[popupInputMode].layoutId[display];
                    McfRectangle popupRect;
                    McfRectangle baseWndRect;
                    McfLayout *layout = NULL;
                    if(mcf_check_arrindex(popupLayoutId, MAX_LAYOUT)) {
                        layout = &mcf_layout[popupLayoutId];
                    }
                    if(layout) {
                        windows->get_window_rect(windows->get_base_window(), &baseWndRect);
                        popupRect.x = coordination->x + coordination->popXOffset + baseWndRect.x;
                        popupRect.y = coordination->y + coordination->popYOffset + baseWndRect.y;
                        popupRect.width = layout->width;
                        popupRect.height= layout->height;
                        windows->close_all_popups();
                        windows->open_popup(window, keyindex,
                                            popupRect.x,
                                            popupRect.y,
                                            popupRect.width,
                                            popupRect.height,
                                            popupLayoutId,
                                            configure->popupType,
                                            !(mcf_input_mode_popup_configure[popupInputMode].fUseWindow),
                                            mcf_input_mode_popup_configure[popupInputMode].fUseDimWindow,
                                            coordination->popImageX,
                                            coordination->popImageY,
                                            mcf_input_mode_popup_configure[popupInputMode].fTimeout
                                           );
                    }
                }
            }
            break;
            case POPUP_TYPE_BTN_RELEASE_POPUP:
            case POPUP_TYPE_BTN_RELEASE_POPUP_ONCE:
            case POPUP_TYPE_CONTEXT_POPUP:
            case POPUP_TYPE_BTN_LONGPRESS_POPUP:
            case POPUP_TYPE_BTN_LONGPRESS_POPUP_ONCE:
            case POPUP_TYPE_AUTO_POPUP:
            case POPUP_TYPE_NONE:
                /* Nothing to do in here */
                break;
            }
            prevwin = window;
            prevkey = keyindex;
        } else {
            /* BUTTON_TYPE_MULTITAP type button should restore its multikey index when another button is clicked */
            if (configure->buttonType & BUTTON_TYPE_MULTITAP) {
                btncontext->multikeyIdx = 0;
            }
        }

        /* If there is any need for redrawing */
        if (redraw) {
#ifdef DIRECTLY_DRAW_ON_EVENTS
            CMCFUIBuilder *builder = CMCFUIBuilder::get_instance();
            if (builder) {
                builder->draw_button(window, NULL, keyindex, btncontext->state, TRUE);
            }
#else
            CMCFWindows *windows = CMCFWindows::get_instance();
            if (windows) {
                windows->update_window(window, coordination->x, coordination->y, coordination->width, coordination->height);
            }
#endif
        }
    }

    return ret;
}

mcfboolean
CMCFController::process_button_long_pressed_event(mcfwindow window, mcfbyte keyindex, mcftouchdevice touchid)
{
    MCF_DEBUG();

    mcfboolean ret = FALSE;

    CMCFContext *context = CMCFContext::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFActionState *state = CMCFActionState::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();

    if(context && cache && windows && state) {
        const McfLayoutKeyCoordination* coordination = cache->get_cur_layout_key_coordination(window, keyindex);
        const McfLayoutKeyProperties *properties = cache->get_cur_layout_key_properties(window, keyindex);
        const McfLayoutKeyConfigure *configure= cache->get_cur_layout_key_configure(window, keyindex);

        /* Should return FALSE if this key does not have any longkey related property */
        if (properties && configure && coordination) {
            if(configure->popupType == POPUP_TYPE_BTN_LONGPRESS_POPUP ||
               configure->popupType == POPUP_TYPE_BTN_LONGPRESS_POPUP_ONCE ) {
                    McfRectangle popupRect;
                    McfRectangle baseWndRect;
                    windows->get_window_rect(windows->get_base_window(), &baseWndRect);
                    popupRect.x = coordination->x + coordination->popXOffset + baseWndRect.x;
                    popupRect.y = coordination->y + coordination->popYOffset + baseWndRect.y;
                    mcfbyte popupInputMode = configure->popupInputMode[MCF_DRAG_STATE_NONE];
                    MCFDisplay display = context->get_display();
                    if(mcf_check_arrindex(popupInputMode, MAX_INPUT_MODE_POPUP) &&
                       mcf_check_arrindex(display, MCFDISPLAY_MAX)) {
                        McfLayout *layout = NULL;
                        mcfshort popupLayoutId = mcf_input_mode_popup_configure[popupInputMode].layoutId[display];
                        if(mcf_check_arrindex(popupLayoutId, MAX_LAYOUT)) {
                            layout = &mcf_layout[popupLayoutId];
                        }
                        if(layout) {
                            popupRect.width = layout->width;
                            popupRect.height= layout->height;

                            windows->open_popup(window,
                            		            keyindex,
                                                popupRect.x,
                                                popupRect.y,
                                                popupRect.width,
                                                popupRect.height,
                                                popupLayoutId,
                                                configure->popupType,
                                                !(mcf_input_mode_popup_configure[popupInputMode].fUseWindow),
                                                mcf_input_mode_popup_configure[popupInputMode].fUseDimWindow,
                                                coordination->popImageX,
                                                coordination->popImageY,
                                                mcf_input_mode_popup_configure[popupInputMode].fTimeout
                                                );
                        }
                    }
            } else if (properties->longKeyValue) {
                if (strlen(properties->longKeyValue) > 0) {
                    McfPoint ptMoving = context->get_cur_move_point(touchid);
                        if (windows->is_base_window(window)) {
                            state->set_cur_action_state(ACTION_STATE_BASE_LONGKEY);
                        } else {
                            state->set_cur_action_state(ACTION_STATE_POPUP_LONGKEY);
                        }
                        ret = TRUE;

                        if (mEventCallback) {
                            McfKeyEventDesc keyEventDesc = {0};
                            keyEventDesc.keyValue = properties->longKeyValue;
                            keyEventDesc.keyEvent = properties->longKeyEvent;
                            keyEventDesc.keyType = configure->keyType;
                            keyEventDesc.keyModifier = KEY_MODIFIER_LONGKEY;

                            keyEventDesc.touchid = touchid;
                            keyEventDesc.mousePressedPt = context->get_cur_pressed_point(touchid);
                            keyEventDesc.mouseCurrentPt = context->get_cur_move_point(touchid);
                            keyEventDesc.mouseFarthestPt = context->get_farthest_move_point(touchid);

                            keyEventDesc.touch_event_order = context->get_multi_touch_event_order(touchid);

                            mEventCallback->on_event_key_clicked(keyEventDesc);
                        }
                }
            }
        }
    }
    isLongKey = true;
    /* Longkey processing in here */
    return ret;
}

mcfboolean
CMCFController::process_button_repeat_pressed_event(mcfwindow window, mcfbyte keyindex, mcftouchdevice touchid)
{
    MCF_DEBUG();
    CMCFContext *context = CMCFContext::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();

    if(context && cache && windows) {
        const McfLayoutKeyConfigure *configure = cache->get_cur_layout_key_configure(window, keyindex);
        const McfLayoutKeyProperties *properties = cache->get_cur_layout_key_properties(window, keyindex);

        MCFShiftState shiftidx = context->get_shift_state();
        if(shiftidx < 0 || shiftidx >= MCF_SHIFT_STATE_MAX) shiftidx = MCF_SHIFT_STATE_OFF;

        if(configure && properties) {
            switch (configure->buttonType) {
                case BUTTON_TYPE_NORMAL:
                case BUTTON_TYPE_GRAB:
                case BUTTON_TYPE_DIRECTION: {
                    /* This is for enabling backspace key in search layout*/
                    if ((configure->keyType != KEY_TYPE_MODECHANGE && configure->keyType != KEY_TYPE_COMPOSITION)
                            || properties->keyEvent[0][0] == MVK_BackSpace) {
                        if (mEventCallback) {
                            mcfulong repeatKeyEvent = properties->keyEvent[shiftidx][0];

                            /* In case of Delete key, Change from Char deletion to Word deletion
                               when the input accelation speed is reached to Max */
                            if (mcf_default_configure.fUseWordDeletion) {
                                mcflong interval = mRepeatKeyDuration - (mRepeatKeySeq * MCF_REPEATKEY_ACCELERATION);
                                if (repeatKeyEvent == MVK_BackSpace &&
                                	interval <= MCF_REPEATKEY_WORD_DELETION_START_DURATION) {
                                    repeatKeyEvent = MVK_3270_DeleteWord;
                                }
                            }

                            McfKeyEventDesc keyEventDesc = {0};
                            keyEventDesc.keyValue = properties->keyValue[shiftidx][0];
                            keyEventDesc.keyEvent = repeatKeyEvent;
                            keyEventDesc.keyType = configure->keyType;
                            keyEventDesc.keyModifier = KEY_MODIFIER_NONE;

                            keyEventDesc.touchid = touchid;
                            keyEventDesc.mousePressedPt = context->get_cur_pressed_point(touchid);
                            keyEventDesc.mouseCurrentPt = context->get_cur_move_point(touchid);
                            keyEventDesc.mouseFarthestPt = context->get_farthest_move_point(touchid);

                            keyEventDesc.touch_event_order = context->get_multi_touch_event_order(touchid);

                            mEventCallback->on_event_key_clicked(keyEventDesc);

                            McfKeyEventDesc desc = {0};
                            desc.keyType = configure->keyType;
                            desc.keyValue = properties->keyValue[shiftidx][0];
                            desc.keyEvent = properties->keyEvent[shiftidx][0];
                            desc.keyModifier = KEY_MODIFIER_NONE;
                            prevModifier = KEY_MODIFIER_NONE;
                        }
                    }
                }
                break;
            }
        }
    }

    /* Longkey processing in here */
    return TRUE;
}

mcfboolean
CMCFController::process_button_move_event(mcfwindow window, mcfint x, mcfint y, mcfbyte keyindex, mcftouchdevice touchid)
{
    MCF_DEBUG();

    mcfboolean ret = FALSE;

    CMCFUtils *utils = CMCFUtils::get_instance();
    CMCFEvents *events = CMCFEvents::get_instance();
    CMCFContext *context = CMCFContext::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFFeedback *feedback = CMCFFeedback::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();

    McfButtonContext *btncontext = NULL;

    const McfLayoutKeyCoordination *coordination = NULL;
    const McfLayoutKeyConfigure *configure = NULL;
    const McfLayoutKeyProperties *properties = NULL;

    if(cache) {
        coordination = cache->get_cur_layout_key_coordination(window, keyindex);
        configure = cache->get_cur_layout_key_configure(window, keyindex);
        properties = cache->get_cur_layout_key_properties(window, keyindex);
        btncontext = cache->get_cur_button_context(window, keyindex);
    }

    if(btncontext && coordination && configure && properties &&
       feedback && utils && context && cache && events && windows) {
       /* If this key is the key previously pressed, add threshold value for avoiding unintended moving */
        mcfint thresholdX = 0;
        mcfint thresholdY = 0;
        if (context->get_cur_pressed_window(touchid) == window && context->get_cur_pressed_key(touchid) == keyindex) {
            thresholdX = utils->get_scale_x(MCF_MOUSE_BUTTON_CHANGE_THRESHOLD_X);
            thresholdY = utils->get_scale_y(MCF_MOUSE_BUTTON_CHANGE_THRESHOLD_Y);
        }

        if (x >= coordination->x - coordination->addHitLeft  - thresholdX &&
            x < coordination->x + coordination->width + coordination->addHitRight + thresholdX &&
            y >= coordination->y - coordination->addHitTop - thresholdY &&
            y < coordination->y + coordination->height + coordination->addHitBottom + thresholdY &&
            /* Process the event only if the this item's sublayout id is active one */
            (coordination->subLayoutID == SUBLAYOUTID_NONE || coordination->subLayoutID == context->get_cur_sublayout_id())) {
            ret = TRUE;

            /* for testing */
            mcfwindow pressedWindow = context->get_cur_pressed_window(touchid);
            mcf8 pressedKey = context->get_cur_pressed_key(touchid);
            if (keyindex != pressedKey || window != pressedWindow ) {
                McfButtonContext *pressedContext = cache->get_cur_button_context(pressedWindow, pressedKey);
                const McfLayoutKeyConfigure *pressedConfigure = cache->get_cur_layout_key_configure(pressedWindow, pressedKey);

                /* When the focus has moved to another button, destroy all the timers */
                events->destroy_all_timer();

                if (check_event_transition_enabled(pressedConfigure, configure)) {
                    const McfLayout* layout = cache->get_cur_layout(windows->get_base_window());
                    if(layout) {
                        McfPoint pos = {0,};
                        mcffloat scale_rate_x, scale_rate_y;
                        if(layout->displayType == MCFDISPLAY_PORTRAIT) {
                            scale_rate_x = utils->get_scale_rate_x();
                            scale_rate_y = utils->get_scale_rate_y();
                        } else {
                            scale_rate_x = utils->get_scale_rate_y();
                            scale_rate_y = utils->get_scale_rate_x();
                        }

                        /* calculates y position to be set */
                        mcfint scnWidth, scnHeight;
                        utils->get_screen_resolution(&scnWidth, &scnHeight);

                        const mcf16 uniqId = utils->get_unique_id();
                        context->set_cur_pressed_event_id(touchid, uniqId);
                        /* Special routine for autopopup */
                        if (configure->popupType == POPUP_TYPE_AUTO_POPUP) {
                            events->create_timer(MCF_TIMER_AUTOPOPUP, mShortLongkeyDuration, uniqId);
                        } else {
                            /* for long key & repeat key */
                            events->create_timer(MCF_TIMER_LONGKEY, mLongkeyDuration, uniqId);
                        }

                        context->set_cur_pressed_window(touchid, window);
                        context->set_cur_pressed_key(touchid, keyindex);

                        MCFShiftState shiftidx = context->get_shift_state();
                        if(shiftidx < 0 || shiftidx >= MCF_SHIFT_STATE_MAX) shiftidx = MCF_SHIFT_STATE_OFF;
                        if (configure->keyType != KEY_TYPE_CONTROL &&
                            !(configure->keyType == KEY_TYPE_SYMBOL && strlen(properties->label[0][0]) == 0) && /* For the case of emoticons */
                            configure->keyType != KEY_TYPE_MODECHANGE &&
                            configure->keyType != KEY_TYPE_NONE) {
                        }

                        /* for feedback */
                        feedback->button_moved(window, keyindex);

                        btncontext->state = BUTTON_STATE_PRESSED;
                        if (pressedContext) {
                            pressedContext->state = BUTTON_STATE_NORMAL;
                        }
                        /* If the window doesn't get exposed before corresponding release event,
                        * the inverted state of a button will never be drawn onto screen.
                        * To prevent such a case, we draw the inverted state of button forcefully and directly,
                        * without waiting for expose event */

                        switch (configure->buttonType) {
                        case BUTTON_TYPE_DRAG: {
                            if (mEventCallback) {
                                McfKeyEventDesc keyEventDesc = {0};
                                keyEventDesc.keyValue = properties->keyValue[shiftidx][0];
                                keyEventDesc.keyEvent = properties->keyEvent[shiftidx][0];
                                keyEventDesc.keyType = configure->keyType;
                                keyEventDesc.keyModifier = KEY_MODIFIER_NONE;

                                keyEventDesc.touchid = touchid;
                                keyEventDesc.mousePressedPt = context->get_cur_pressed_point(touchid);
                                keyEventDesc.mouseCurrentPt = context->get_cur_move_point(touchid);
                                keyEventDesc.mouseFarthestPt = context->get_farthest_move_point(touchid);

                                keyEventDesc.touch_event_order = context->get_multi_touch_event_order(touchid);

                                mEventCallback->on_event_key_clicked(keyEventDesc);
                            }
                            if (!(windows->is_base_window(window))) {
                                /* When press event occured in popup window, reset POPUP_TIMEOUT timer */
                                McfWindowContext *winctx = windows->get_window_context(window, FALSE);
                                if(winctx) {
                                    if (winctx->timeout > 0) {
                                        events->destroy_timer(MCF_TIMER_POPUP_TIMEOUT);
                                        events->create_timer(MCF_TIMER_POPUP_TIMEOUT, winctx->timeout, 0, TRUE);
                                    }
                                }
                            }
                        }
                        break;
                        }

#ifdef DIRECTLY_DRAW_ON_EVENTS
                        CMCFUIBuilder *builder = CMCFUIBuilder::get_instance();
                        if (builder) {
                            if (btncontext) {
                                builder->draw_button(window, NULL, keyindex, btncontext->state);
                            }
                            if (pressedContext) {
                                builder->draw_button(pressedWindow, NULL, pressedKey, pressedContext->state, TRUE);
                            }
                        }
#else
		                        windows->update_window(window,
		                                               coordination->x, coordination->y,
		                                               coordination->width, coordination->height);

		                        const McfLayoutKeyCoordination *pressedCoordination =
		                            cache->get_cur_layout_key_coordination(pressedWindow, pressedKey);
		                        if (pressedCoordination) {
		                            windows->update_window(pressedWindow, pressedCoordination->x, pressedCoordination->y,
		                                                   pressedCoordination->width, pressedCoordination->height);
                        }
#endif
                    }

                }
            }
        }
    }

    return ret;
}

MCFKeyModifier
CMCFController::get_drag_key_modifier(mcfint deltax, mcfint deltay, mcffloat dist, mcfboolean check_farthest,
                                      mcftouchdevice touchid, mcfbyte extraOption)
{
    typedef struct {
        double lowerbound;
        double upperbound;
        MCFKeyModifier modifier;
    } DIRECTIONINFO;

    CMCFContext *context = CMCFContext::get_instance();
    MCFKeyModifier keyModifier = KEY_MODIFIER_NONE;

    if(context) {
        double theta = atan2(deltay , (deltax ? deltax : 1)); /* Avoid divide by 0 exception */
        mcffloat ratio = fabs((mcffloat)deltay / (deltax ? deltax : 1));
        MCFDragState curDragState = context->get_cur_drag_state(touchid);
        if (extraOption == DIRECTION_EXTRA_OPTION_8_DIRECTIONS ||
            extraOption == DIRECTION_EXTRA_OPTION_8_DIRECTIONS_WITH_LONG ||
            extraOption == DIRECTION_EXTRA_OPTION_8_DIRECTIONS_WITH_RETURN) { /* 8 directions */
                /* If the theta is below 0, the direction is upward since the y coordination grows downward */
                DIRECTIONINFO info[] = {
                    {-8 * (M_PI / 8), -7 * (M_PI / 8), KEY_MODIFIER_DIRECTION_LEFT},
                    {-7 * (M_PI / 8), -5 * (M_PI / 8), KEY_MODIFIER_DIRECTION_UP_LEFT},
                    {-5 * (M_PI / 8), -2.7 * (M_PI / 8), KEY_MODIFIER_DIRECTION_UP},
                    {-2.7 * (M_PI / 8), -1.5 * (M_PI / 8), KEY_MODIFIER_DIRECTION_UP_RIGHT},
                    {-1.5 * (M_PI / 8),  1 * (M_PI / 8), KEY_MODIFIER_DIRECTION_RIGHT},
                    { 1 * (M_PI / 8),  3 * (M_PI / 8), KEY_MODIFIER_DIRECTION_DOWN_RIGHT},
                    { 3 * (M_PI / 8),  5 * (M_PI / 8), KEY_MODIFIER_DIRECTION_DOWN},
                    { 5 * (M_PI / 8),  7 * (M_PI / 8), KEY_MODIFIER_DIRECTION_DOWN_LEFT},
                    { 7 * (M_PI / 8),  8 * (M_PI / 8), KEY_MODIFIER_DIRECTION_LEFT},
                };
                for (mcfint loop = 0;loop < sizeof(info) / sizeof(DIRECTIONINFO);loop++) {
                    if (theta >= info[loop].lowerbound && theta <= info[loop].upperbound) {
                        keyModifier = info[loop].modifier;
                    }
                }
        } else { /* 4 directions */
            /* If the state was dragging to one of 4 directions and the final release point is
            * far enough from inital press point, and the angle is in between out predefined angle value */
            if(extraOption == DIRECTION_EXTRA_OPTION_4_DIRECTIONS_WITH_RETURN_AND_CURVE &&
               curDragState != MCF_DRAG_STATE_NONE && curDragState != MCF_DRAG_STATE_INVALID &&
               dist > MCF_DRAG_CURVE_RECOG_DIST &&
               ratio > (1 / MCF_DRAG_CURVE_FINAL_ANGLE_VALUE) &&
               ratio < MCF_DRAG_CURVE_FINAL_ANGLE_VALUE) {
                    if(curDragState == MCF_DRAG_STATE_DOWN) {
                        if(deltax > 0) {
                        	keyModifier = KEY_MODIFIER_DIRECTION_CURVE_DOWN_RIGHT;
                        }
                        else {
                        	keyModifier = KEY_MODIFIER_DIRECTION_CURVE_DOWN_LEFT;
                        }
                    }
                    if(curDragState == MCF_DRAG_STATE_UP) {
                        if(deltax > 0) {
                        	keyModifier = KEY_MODIFIER_DIRECTION_CURVE_UP_RIGHT;
                        }
                        else {
                        	keyModifier = KEY_MODIFIER_DIRECTION_CURVE_UP_LEFT;
                        }
                    }
                    if(curDragState == MCF_DRAG_STATE_LEFT) {
                        if(deltay > 0) {
                        	keyModifier = KEY_MODIFIER_DIRECTION_CURVE_LEFT_DOWN;
                        }
                        else {
                        	keyModifier = KEY_MODIFIER_DIRECTION_CURVE_LEFT_UP;
                        }
                    }
                    if(curDragState == MCF_DRAG_STATE_RIGHT) {
                        if(deltay > 0) {
                        	keyModifier = KEY_MODIFIER_DIRECTION_CURVE_RIGHT_DOWN;
                        }
                        else {
                        	keyModifier = KEY_MODIFIER_DIRECTION_CURVE_RIGHT_UP;
                        }
                    }
            } else {
                DIRECTIONINFO info[] = {
                    {-4 * (M_PI / 4), -3 * (M_PI / 4), KEY_MODIFIER_DIRECTION_LEFT},
                    {-3 * (M_PI / 4), -1 * (M_PI / 4), KEY_MODIFIER_DIRECTION_UP},
                    {-1 * (M_PI / 4),  1 * (M_PI / 4), KEY_MODIFIER_DIRECTION_RIGHT},
                    { 1 * (M_PI / 4),  3 * (M_PI / 4), KEY_MODIFIER_DIRECTION_DOWN},
                    { 3 * (M_PI / 4),  4 * (M_PI / 4), KEY_MODIFIER_DIRECTION_LEFT},
                };
                for (mcfint loop = 0;loop < sizeof(info) / sizeof(DIRECTIONINFO);loop++) {
                    if (theta >= info[loop].lowerbound && theta <= info[loop].upperbound) {
                        keyModifier = info[loop].modifier;
                    }
                }
            }
        }

        if(extraOption == DIRECTION_EXTRA_OPTION_8_DIRECTIONS_WITH_LONG ||
           extraOption == DIRECTION_EXTRA_OPTION_4_DIRECTIONS_WITH_LONG) {
                if(keyModifier >= KEY_MODIFIER_DIRECTION_LEFT &&
                   keyModifier <= KEY_MODIFIER_DIRECTION_DOWN_RIGHT) {
                        keyModifier = (MCFKeyModifier)(keyModifier + 8); // Add LONG attribute;
                }
        }
        if(check_farthest || context->get_cur_drag_state(touchid) == MCF_DRAG_STATE_RETURN) {
            if(keyModifier >= KEY_MODIFIER_DIRECTION_LEFT &&
               keyModifier <= KEY_MODIFIER_DIRECTION_DOWN_RIGHT) {
                    keyModifier = (MCFKeyModifier)(keyModifier + 16); // Add RETURN attribute;
            }
        }
    }

    return keyModifier;
}

mcfboolean
CMCFController::process_button_release_event(mcfwindow window, mcfint x, mcfint y, mcfbyte keyindex, mcftouchdevice touchid)
{
    MCF_DEBUG();

    mcfboolean ret = FALSE;
    mcfboolean redraw = FALSE;
    mcfboolean fireEvt = FALSE;
    MCFKeyModifier keyModifier = KEY_MODIFIER_NONE;

    static mcfwindow lastFiredWin = MCFWINDOW_INVALID;
    static mcfbyte lastFiredKey = NOT_USED;

    CMCFUtils *utils = CMCFUtils::get_instance();
    CMCFFeedback *feedback = CMCFFeedback::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFContext *context = CMCFContext::get_instance();
    CMCFActionState *state = CMCFActionState::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    McfWindowContext *winctx = windows->get_window_context(window);
    McfPoint zoomwinpos = {0,};

    McfButtonContext *btncontext = NULL;
    const McfLayoutKeyCoordination *coordination = NULL;
    const McfLayoutKeyConfigure *configure = NULL;
    const McfLayoutKeyProperties *properties = NULL;

    if(cache) {
        btncontext = cache->get_cur_button_context(window, keyindex);
        coordination = cache->get_cur_layout_key_coordination(window, keyindex);
        configure = cache->get_cur_layout_key_configure(window, keyindex);
        properties = cache->get_cur_layout_key_properties(window, keyindex);
    }

    const McfLayoutKeyConfigure *targetConfigure = NULL;
    const McfLayoutKeyProperties *targetProperties = NULL;

    if (utils && feedback && windows && context && state && cache &&
    	btncontext && coordination && configure && properties) {
        mcf8 savedInputMode = context->get_input_mode();
        MCFShiftState shiftidx = context->get_shift_state();
        if(shiftidx < 0 || shiftidx >= MCF_SHIFT_STATE_MAX) {
        	shiftidx = MCF_SHIFT_STATE_OFF;
        }

        mcfwindow pressedWindow = context->get_cur_pressed_window(touchid);
        mcf8 pressedKey = context->get_cur_pressed_key(touchid);

        /* If this button was pressed, initialize the button context regardless of event  */
        if (btncontext->state == BUTTON_STATE_PRESSED) {
            btncontext->state = BUTTON_STATE_NORMAL;
                redraw = TRUE;
        }

        /* If this key is the key previously pressed, add threshold value for avoiding unintended moving */
        mcfint thresholdX = 0;
        mcfint thresholdY = 0;
        if (context) {
            if (context->get_cur_pressed_window(touchid) == window && context->get_cur_pressed_key(touchid) == keyindex) {
                thresholdX = utils->get_scale_x(MCF_MOUSE_BUTTON_CHANGE_THRESHOLD_X);
                thresholdY = utils->get_scale_y(MCF_MOUSE_BUTTON_CHANGE_THRESHOLD_Y);
            }
        }

        /* Check if the pressed button's type is directional button */
        if (configure->buttonType == BUTTON_TYPE_DIRECTION) {
            if (context) {
                if (context->get_cur_pressed_window(touchid) == window && context->get_cur_pressed_key(touchid) == keyindex) {
                    ret = TRUE;

                    mcfboolean check_farthest = FALSE;
                    mcfint deltax = x - context->get_cur_pressed_point(touchid).x;
                    mcfint deltay = y - context->get_cur_pressed_point(touchid).y;

                    mcffloat dist = utils->get_distance(x, y,
                                                        context->get_cur_pressed_point(touchid).x,
                                                        context->get_cur_pressed_point(touchid).y);
                    mcffloat direction_recog_dist = MCF_DIRECTION_RECOG_DIST;
                    if(configure->fIsSideButton) {
                        direction_recog_dist = MCF_DIRECTION_RECOG_DIST_SIDE;
                    };
                    if(context->get_cur_drag_state(touchid) == MCF_DRAG_STATE_RETURN) {
                        if(properties->extraOption == DIRECTION_EXTRA_OPTION_8_DIRECTIONS_WITH_RETURN ||
                           properties->extraOption == DIRECTION_EXTRA_OPTION_4_DIRECTIONS_WITH_RETURN ||
                           properties->extraOption == DIRECTION_EXTRA_OPTION_4_DIRECTIONS_WITH_RETURN_AND_CURVE) {
                           deltax = context->get_farthest_move_point(touchid).x - context->get_cur_pressed_point(touchid).x;
                           deltay = context->get_farthest_move_point(touchid).y - context->get_cur_pressed_point(touchid).y;
                           dist = utils->get_distance(context->get_farthest_move_point(touchid),
                           context->get_cur_pressed_point(touchid));
                           check_farthest = TRUE;
                        }
                    }
                    if(!isLongKey) {
                    if (dist > direction_recog_dist) {
                        keyModifier = get_drag_key_modifier(deltax, deltay, dist, check_farthest, touchid, properties->extraOption);
                      }
                    }
                    else
                      keyModifier = KEY_MODIFIER_LONGKEY;
                }
            }
        }

        /* Check if the event occured inside this button's rectangle */
        if (x >= coordination->x - coordination->addHitLeft  - thresholdX &&
            x < coordination->x + coordination->width + coordination->addHitRight + thresholdX &&
            y >= coordination->y - coordination->addHitTop - thresholdY &&
            y < coordination->y + coordination->height + coordination->addHitBottom + thresholdY &&
            /* Process the event only if the this item's sublayout id is active one */
            (coordination->subLayoutID == SUBLAYOUTID_NONE || coordination->subLayoutID == context->get_cur_sublayout_id())) {
            ret = TRUE;
        }

        if (ret) {
            /* for feedback */
            feedback->button_released(window, keyindex);

            /* If this button's index is the same as the one initially pressed */
            if (pressedWindow == window && pressedKey == keyindex) {
                fireEvt = TRUE;
                targetConfigure = configure;
                targetProperties = properties;
            } else {
                const McfLayoutKeyConfigure *pressedConfigure = cache->get_cur_layout_key_configure(pressedWindow, pressedKey);
                const McfLayoutKeyProperties *pressedProperties = cache->get_cur_layout_key_properties(pressedWindow, pressedKey);

                if (pressedConfigure && pressedProperties) {
                    if (check_event_transition_enabled(pressedConfigure, configure)) {
                        fireEvt = TRUE;
                        targetConfigure = pressedConfigure;
                        targetProperties = pressedProperties;
                    } else {
                        ret = FALSE;
                    }
                }
            }
        }

        /* Don't fire any events if we're in longkey state */
        if (state->get_cur_action_state() != ACTION_STATE_BASE_LONGKEY &&
            state->get_cur_action_state() != ACTION_STATE_BASE_REPEATKEY &&
            state->get_cur_action_state() != ACTION_STATE_POPUP_LONGKEY &&
            state->get_cur_action_state() != ACTION_STATE_POPUP_REPEATKEY) {
            /* An event occured? */
            if (fireEvt) {
                if (targetConfigure && targetProperties) {
                    McfKeyEventDesc keyEventDesc = {0};
                    keyEventDesc.keyType = targetConfigure->keyType;

                    keyEventDesc.touchid = touchid;
                    keyEventDesc.mousePressedPt = context->get_cur_pressed_point(touchid);
                    keyEventDesc.mouseCurrentPt = context->get_cur_move_point(touchid);
                    keyEventDesc.mouseFarthestPt = context->get_farthest_move_point(touchid);

                    keyEventDesc.touch_event_order = context->get_multi_touch_event_order(touchid);

                    switch (configure->popupType) {
                    case POPUP_TYPE_BTN_RELEASE_POPUP:
                    case POPUP_TYPE_BTN_RELEASE_POPUP_ONCE: {
                        MCFDragState dragstate = context->get_cur_drag_state(touchid);
                        mcfbyte popupInputMode = NOT_USED;
                        if(mcf_check_arrindex(dragstate, MCF_DRAG_STATE_MAX)) {
                            popupInputMode = configure->popupInputMode[dragstate];
                            if(!mcf_check_arrindex(popupInputMode, MAX_INPUT_MODE_POPUP)) {
                                popupInputMode = configure->popupInputMode[MCF_DRAG_STATE_NONE];
                            }
                        }
                        MCFDisplay display = context->get_display();
                        if(mcf_check_arrindex(popupInputMode, MAX_INPUT_MODE_POPUP) &&
                           mcf_check_arrindex(display, MCFDISPLAY_MAX)) {
                            mcfshort popupLayoutId =
                                mcf_input_mode_popup_configure[popupInputMode].layoutId[display];
                            McfLayout *layout = NULL;
                            if(mcf_check_arrindex(popupLayoutId, MAX_LAYOUT)) {
                                layout = &mcf_layout[popupLayoutId];
                            }
                            if(layout) {
                                McfRectangle popupRect;
                                McfRectangle baseWndRect;
                                windows->get_window_rect(windows->get_base_window(), &baseWndRect);
                                popupRect.x = coordination->x + coordination->popXOffset + baseWndRect.x;
                                popupRect.y = coordination->y + coordination->popYOffset + baseWndRect.y;

                                popupRect.width = layout->width;
                                popupRect.height= layout->height;

                                windows->open_popup(window,
                                		            keyindex,
                                                    popupRect.x,
                                                    popupRect.y,
                                                    popupRect.width,
                                                    popupRect.height,
                                                    popupLayoutId,
                                                    configure->popupType,
                                                    !(mcf_input_mode_popup_configure[popupInputMode].fUseWindow),
                                                    mcf_input_mode_popup_configure[popupInputMode].fUseDimWindow,
                                                    coordination->popImageX,
                                                    coordination->popImageY,
                                                    mcf_input_mode_popup_configure[popupInputMode].fTimeout);
                            }
                        }
                    }
                    break;
                    case POPUP_TYPE_CONTEXT_POPUP:
                        zoomwinpos.x = (coordination->x + (coordination->width / 2));
                        mcfint scnWidth, scnHeight;
                        utils->get_screen_resolution(&scnWidth, &scnHeight);
                        if (context->get_display() == MCFDISPLAY_LANDSCAPE) {
                            int  tmp = scnWidth;
                            scnWidth = scnHeight;
                            scnHeight = tmp;
                        }
                        zoomwinpos.y =  coordination->y;
                        if(winctx) {
                            zoomwinpos.x += winctx->x;
                            zoomwinpos.y += winctx->y;
                        }
                        if(_setup_info.lang_count > 2) {
                         windows->open_context_popup(window);
                         _show_language_popup(zoomwinpos.x,zoomwinpos.y);
                        }
                        break;
                    case POPUP_TYPE_AUTO_POPUP:
                    case POPUP_TYPE_BTN_PRESS_POPUP_DRAG:
                    case POPUP_TYPE_NONE:
                        /* Nothing to do in here */
                        break;
                    }

                    switch (targetConfigure->buttonType) {
                    case BUTTON_TYPE_NORMAL:
                    case BUTTON_TYPE_GRAB :
                    case BUTTON_TYPE_DIRECTION : {
                        if (!(targetProperties->useRepeatKey)) {
                            if (mEventCallback) {
                                keyEventDesc.keyValue = properties->keyValue[shiftidx][0];
                                keyEventDesc.keyEvent = properties->keyEvent[shiftidx][0];
                                keyEventDesc.keyModifier = keyModifier;
                                mEventCallback->on_event_key_clicked(keyEventDesc);
                            }
                        }
                    }
                    break;
                    case BUTTON_TYPE_MULTITAP:
                    case BUTTON_TYPE_ROTATION: {
                        if(targetConfigure->buttonType == BUTTON_TYPE_MULTITAP) {
                            if(window == lastFiredWin && keyindex == lastFiredKey) {
                                keyModifier = KEY_MODIFIER_MULTITAP_REPEAT;
                            } else {
                                keyModifier = KEY_MODIFIER_MULTITAP_START;
                            }
                        } else {
                            keyModifier = KEY_MODIFIER_NONE;
                        }
                        if (mEventCallback) {
                            if (btncontext->multikeyIdx < MAX_SIZE_OF_MULTITAP_CHAR) {
                                keyEventDesc.keyValue = properties->keyValue[shiftidx][btncontext->multikeyIdx];
                                keyEventDesc.keyEvent = properties->keyEvent[shiftidx][btncontext->multikeyIdx];
                                keyEventDesc.keyModifier = keyModifier;
                                mEventCallback->on_event_key_clicked(keyEventDesc);
                            }
                        }
                        /* Check if the multikey index is in valid range, and increase by one */
                        if (btncontext->multikeyIdx >= MAX_SIZE_OF_MULTITAP_CHAR - 1) {
                            btncontext->multikeyIdx = 0;
                        } else {
                            mcfbyte orgindex = btncontext->multikeyIdx;
                            btncontext->multikeyIdx = 0;
                            if (targetProperties->keyValue[shiftidx][orgindex + 1]) {
                                if (strlen(targetProperties->keyValue[shiftidx][orgindex + 1]) > 0) {
                                    btncontext->multikeyIdx = orgindex + 1;
                                }
                            }
                        }
                    }
                    break;
                    case BUTTON_TYPE_DRAG : {
                    }
                    break;
                    }
                }

                lastFiredWin = window;
                lastFiredKey = keyindex;
            }
        }

        /* If this button needs to be redrawn */
        if (redraw) {
#ifdef DIRECTLY_DRAW_ON_EVENTS
            CMCFUIBuilder *builder = CMCFUIBuilder::get_instance();
            if (builder) {
                builder->draw_button(window, NULL, keyindex, btncontext->state, TRUE);
            }
#else
            if(savedInputMode == context->get_input_mode()) {
                CMCFWindows *windows = CMCFWindows::get_instance();
                if (windows) {
                    windows->update_window(window, coordination->x, coordination->y,
                    		               coordination->width, coordination->height);
                }
            }

#endif
        }
    }
    isLongKey = false;
    return ret;
}

mcfboolean
CMCFController::mouse_press(mcfwindow window, mcfint x, mcfint y, mcftouchdevice touchid)
{
    MCF_DEBUG();
    mcfboolean ret = FALSE;

    if(mInputEventsDisabled)
	return FALSE;

	prevModifier = KEY_MODIFIER_NONE;

    /* Adjust x,y coordination by touch offset */
    CMCFErrorAdjustment *adjustment = CMCFErrorAdjustment::get_instance();

    CMCFContext *context = CMCFContext::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    CMCFActionState *state = CMCFActionState::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFUtils *utils = CMCFUtils::get_instance();

    mcfint btnIndex = NOT_USED;
    McfWindowContext *winctx = NULL;
    if(windows) {
        winctx = windows->get_window_context(window, FALSE);
        /* If the dim window is virtual and currently active, let's just skip this event */
        if(windows->is_base_window(window)) {
            McfWindowContext *dimctx = windows->get_window_context(windows->get_dim_window());
            if(dimctx) {
                if(dimctx->isVirtual && !(dimctx->hidden)) {
                    window = windows->get_dim_window();
                    winctx = dimctx;
                }
            }
        }
    }

    if (cache && state && windows && context && winctx) {
        MCFDisplay currDisplay = context->get_display();
        adjustment->apply_touch_offset(mcf_default_configure.touchOffsetLevel[currDisplay], &x, &y);

        mcfboolean isSubEvent = FALSE;
        if(context->get_multi_touch_context_num() > 0) {
            McfKeyEventDesc desc;
            context->get_multi_touch_event(0, &desc);
            mcfwindow pressedwindow = context->get_cur_pressed_window(desc.touchid);
            mcf8 pressedkey = context->get_cur_pressed_key(desc.touchid);
            McfLayoutKeyProperties *properties = cache->get_cur_layout_key_properties(pressedwindow, pressedkey);
            if(properties) {
                if(properties->multitouchType == MCF_MULTI_TOUCH_TYPE_GRAB_SUB_EVENTS) {
                    isSubEvent = TRUE;
                }
            }
        }
        context->create_multi_touch_context(touchid, isSubEvent);
        context->set_cur_pressed_window(touchid, window);
        context->set_cur_pressed_point(touchid, x, y);
        context->set_cur_pressed_time(touchid);
        context->set_cur_move_window(touchid, window);
        context->set_cur_move_point(touchid, x, y);
        context->set_last_touch_device_id(touchid);
        context->set_cur_drag_state(touchid, MCF_DRAG_STATE_NONE);
        context->set_cur_key_modifier(touchid, KEY_MODIFIER_NONE);

        /* If there is postponed update of button, update it now */
        CMCFEvents *events = CMCFEvents::get_instance();
        mcfwindow lastWin = context->get_last_pressed_window();
        mcf8 lastKey = context->get_last_pressed_key();
        if (lastWin != MCFWINDOW_INVALID && lastKey != NOT_USED) {
            const McfLayoutKeyCoordination* coords = cache->get_cur_layout_key_coordination(lastWin, lastKey);
            if (coords) {
                windows->update_window(lastWin, coords->x, coords->y, coords->width, coords->height);
            }
        }
        context->set_prev_pressed_window(touchid, MCFWINDOW_INVALID);
        context->set_prev_pressed_key(touchid, NOT_USED);
        context->set_prev_drag_state(touchid, MCF_DRAG_STATE_NONE);

        /* Destroy key related timers */
        events->destroy_timer(MCF_TIMER_BUTTON_DELAY);
        events->destroy_timer(MCF_TIMER_AUTOPOPUP);
        events->destroy_timer(MCF_TIMER_SHORT_LONGKEY);
        events->destroy_timer(MCF_TIMER_LONGKEY);
        events->destroy_timer(MCF_TIMER_REPEATKEY);

        /* Do what has to be done when mouse gets pressed */
        handle_engine_signal(MCF_SIG_MOUSE_PRESS, window);

        if(!isSubEvent) {
            /* Iterate all the buttons and inform the event */
            mcfboolean ended = FALSE;
            for (int loop = 0;loop < MAX_KEY && !ended;loop++) {
                McfButtonContext *btncontext = cache->get_cur_button_context(window, loop);
                const McfLayoutKeyConfigure *configure = cache->get_cur_layout_key_configure(window, loop);
                if (btncontext && configure) {
                    if (!(btncontext->used)) {
                        ended = TRUE;
                    } else if (btncontext->state != BUTTON_STATE_DISABLED &&
                               configure->buttonType != BUTTON_TYPE_UIITEM) {
                        if (process_button_pressed_event(window, x, y, loop, touchid)) {
                            if (windows->is_base_window(window)) {
                                state->set_cur_action_state(ACTION_STATE_BASE_PRESS);
                            } else {
                                state->set_cur_action_state(ACTION_STATE_POPUP_PRESS);
                            }
                            btnIndex = loop;
                            ret = TRUE;
                        }
                    }
                }
            }

            /* For covering a missing area about 1 pixel */
            if (!ret) {
                for (int loop = 0;loop < MAX_KEY;loop++) {
                    McfButtonContext *btncontext = cache->get_cur_button_context(window, loop);
                    const McfLayoutKeyConfigure* configure = cache->get_cur_layout_key_configure(window, loop);
                    if (btncontext && configure) {
                        if (!(btncontext->used)) {
                            break;
                        } else if (btncontext->state != BUTTON_STATE_DISABLED &&
                                   configure->buttonType != BUTTON_TYPE_UIITEM) {
                            if (process_button_pressed_event(window, x+1, y+1, loop, touchid)) {
                                if (windows->is_base_window(window)) {
                                    state->set_cur_action_state(ACTION_STATE_BASE_PRESS);
                                } else {
                                    state->set_cur_action_state(ACTION_STATE_POPUP_PRESS);
                                }
                                btnIndex = loop;
                                break;
                            }
                        }
                    }
                }
            }
        }

        mcfwindow skipwindow = window;
        if (ret && btnIndex != NOT_USED) {
            const McfLayoutKeyProperties *properties = cache->get_cur_layout_key_properties(window, btnIndex);
            const McfLayoutKeyConfigure *configure = cache->get_cur_layout_key_configure(window, btnIndex);
            if (properties && configure) {
                mcfboolean dontClosePopup = FALSE;
                if (properties->dontClosePopup) {
                    dontClosePopup = TRUE;
                }
                /* If the button's popup type is drag type, the opened popup could be the one opened by this press event */
                if (configure->popupType == POPUP_TYPE_BTN_PRESS_POPUP_DRAG) {
                    /* Check the opened popup was opened by this button */
                    mcfwindow popupwin = windows->get_nth_window_in_Z_order_list(MCF_WINDOW_Z_TOP);
                    McfWindowContext *popupctx = windows->get_window_context(popupwin);
                    if (popupctx) {
                        if(popupctx->parentWnd == window && popupctx->parentKey == btnIndex) {
                            dontClosePopup = TRUE;
                        }
                    }
                }
                if (dontClosePopup) {
                    skipwindow = windows->get_nth_window_in_Z_order_list(MCF_WINDOW_Z_TOP);
                }
            }
        }
        windows->close_all_popups(skipwindow);

        /* When press event occured in popup window, reset POPUP_TIMEOUT timer */
        if (!(windows->is_base_window(window))) {
            if (winctx->timeout > 0) {
                events->destroy_timer(MCF_TIMER_POPUP_TIMEOUT);
                events->create_timer(MCF_TIMER_POPUP_TIMEOUT, winctx->timeout, 0, TRUE);
            }
        } else if (skipwindow != window) { /* Or the pressed button has dontClosePopup property, reset POPUP_TIMEOUT timer */
            McfWindowContext *skipwinctx = windows->get_window_context(skipwindow, FALSE);
            if (skipwinctx) {
                if (skipwinctx->timeout > 0) {
                    events->destroy_timer(MCF_TIMER_POPUP_TIMEOUT);
                    events->create_timer(MCF_TIMER_POPUP_TIMEOUT, skipwinctx->timeout, 0, TRUE);
                }
            }
        }
    }

    return TRUE;
}

mcfboolean
CMCFController::mouse_release(mcfwindow window, mcfint x, mcfint y, mcftouchdevice touchid)
{
    MCF_DEBUG();
    mcfboolean ret = FALSE;

    /* Adjust x,y coordination by touch offset */
    CMCFErrorAdjustment *adjustment = CMCFErrorAdjustment::get_instance();

    CMCFContext *context = CMCFContext::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    CMCFActionState *state = CMCFActionState::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFEvents *events = CMCFEvents::get_instance();
    CMCFUtils *utils = CMCFUtils::get_instance();

    mcfint btnIndex = NOT_USED;

    if (cache && state && windows && context && utils && context->find_multi_touch_context(touchid)) {
        mcfwindow skipwindow = MCFWINDOW_INVALID;
        MCFDisplay currDisplay = context->get_display();
        adjustment->apply_touch_offset(mcf_default_configure.touchOffsetLevel[currDisplay], &x, &y);

        context->set_cur_move_window(touchid, MCFWINDOW_INVALID);

        mcfwindow pressedWindow = context->get_cur_pressed_window(touchid);
        mcf8 pressedKey = context->get_cur_pressed_key(touchid);
        McfWindowContext *winctx = windows->get_window_context(window, TRUE);
        /* Adjust event x and y positions as relative position to the virtual window */
        if(winctx) {
            /* If the dim window is virtual and currently active, consider base window's event is occured in dim window */
            if(windows->is_base_window(window)) {
                McfWindowContext *dimctx = windows->get_window_context(windows->get_dim_window());
                if(dimctx) {
                    if(dimctx->isVirtual && !(dimctx->hidden)) {
                        window = windows->get_dim_window();
                        winctx = dimctx;
                    }
                }
            }
        }

        /* Iterate all the buttons and inform the event */
        mcfboolean ended = FALSE;
        CMCFUtils *utils = CMCFUtils::get_instance();


        if(context->get_cur_pressed_window(touchid) == window) {
            if(abs(context->get_cur_pressed_point(touchid).x - x) > utils->get_scale_x(MCF_FLICK_GESTURE_RECOG_THRESHOLD) ||
               abs(context->get_cur_pressed_point(touchid).y - y) > utils->get_scale_y(MCF_FLICK_GESTURE_RECOG_THRESHOLD) ) {
                struct timeval t0 = context->get_cur_pressed_time(touchid);
                struct timeval t1;
                gettimeofday(&t1, NULL);
                float etime;
                etime = ((t1.tv_sec * 1000000 + t1.tv_usec) - (t0.tv_sec * 1000000 + t0.tv_usec))/1000.0;
                if(etime < MCF_FLICK_GESTURE_RECOG_TIME) {
                    mcfint direction = DRAG_NONE;
                    if(x > context->get_cur_pressed_point(touchid).x + utils->get_scale_x(MCF_FLICK_GESTURE_RECOG_THRESHOLD)) {
                        direction = DRAG_RIGHT;
                    }
                    if(x < context->get_cur_pressed_point(touchid).x - utils->get_scale_x(MCF_FLICK_GESTURE_RECOG_THRESHOLD)) {
                        direction = DRAG_LEFT;
                    }
                    if(y > context->get_cur_pressed_point(touchid).y + utils->get_scale_y(MCF_FLICK_GESTURE_RECOG_THRESHOLD)) {
                        direction = DRAG_DOWN;
                    }
                    if(y < context->get_cur_pressed_point(touchid).y - utils->get_scale_y(MCF_FLICK_GESTURE_RECOG_THRESHOLD)) {
                        direction = DRAG_UP;
                    }
                    if (mEventCallback) {
                        if(mEventCallback->on_event_notification(NOTITYPE_GESTURE_FLICK, direction)) {
                            ended = TRUE;
                        }
                    }
                }
            }
        }

        /* FIXME : We should consider this kind of action in general manner, not only specific to autopopup */
        /* And also, this kind of implementation only selects button that was highlighted at least once. */
        /* iPhone supports highlighting autopopup buttons with its direction, even if the pointer never goes up on the button */
        McfWindowContext *pressedCtx = windows->get_window_context(pressedWindow, FALSE);
        if (pressedCtx) {
            utils->log("PRESSED CTX : %p %d %d\n", pressedWindow, pressedCtx->x, pressedCtx->y);
            mcfboolean grab_event = FALSE;
            const McfLayout *layout = cache->get_cur_layout(pressedWindow);
            if(layout) {
                if(layout->style == LAYOUT_STYLE_POPUP_GRAB) {
                    grab_event = TRUE;
                }
                /* If the topmost window has the POPUP_GRAB style, find the nearest button to the mouse pointer */
                if(grab_event) {
                    /* If the layout's addGrab* values are defined, process this event only if the event occured inside grab area */
                    mcfboolean in_grab_area = TRUE;
/*                    if((layout->addGrabLeft != NOT_USED && x < -(layout->addGrabLeft)) ||
                       (layout->addGrabRight != NOT_USED && x > (pressedCtx->width + layout->addGrabRight)) ||
                       (layout->addGrabTop != NOT_USED && y < -(layout->addGrabTop)) ||
                       (layout->addGrabBottom != NOT_USED && y > (pressedCtx->height + layout->addGrabBottom))) {
                        in_grab_area = FALSE;
                    }
*/
                    if(in_grab_area) {
                        McfLayoutKeyCoordination *coord = cache->get_cur_layout_key_coordination(pressedWindow, pressedKey);
                        if(coord) {
                            x = coord->x + (coord->width / 2);
                            y = coord->y + (coord->height / 2);

                            for (int loop = 0;loop < MAX_KEY && !ended;loop++) {
                                McfButtonContext *btncontext = cache->get_cur_button_context(pressedWindow, loop);
                                const McfLayoutKeyConfigure *configure = cache->get_cur_layout_key_configure(window, loop);
                                if (btncontext && configure) {
                                    if (!(btncontext->used)) {
                                        ended = TRUE;
                                    } else if (btncontext->state != BUTTON_STATE_DISABLED &&
                                                configure->buttonType != BUTTON_TYPE_UIITEM) {
                                        if (process_button_release_event(pressedWindow, x, y, loop, touchid)) {
                                            ret = TRUE;
                                            ended = TRUE;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        McfButtonContext *btncontext = cache->get_cur_button_context(pressedWindow, pressedKey);
        const McfLayoutKeyCoordination *coordination = cache->get_cur_layout_key_coordination(pressedWindow, pressedKey);
        const McfLayoutKeyConfigure *configure = cache->get_cur_layout_key_configure(pressedWindow, pressedKey);

        /* FIXME : The rule below would not be a general requirement. A policy is needed regarding this. */
        /* Ignore base window's release event if a popup window is opened */
        if (state->get_cur_action_state() == ACTION_STATE_POPUP_INIT ||
            state->get_cur_action_state() == ACTION_STATE_POPUP_PRESS ||
            state->get_cur_action_state() == ACTION_STATE_POPUP_MOVING ||
            state->get_cur_action_state() == ACTION_STATE_POPUP_RELEASE ||
            state->get_cur_action_state() == ACTION_STATE_POPUP_REPEATKEY ||
            state->get_cur_action_state() == ACTION_STATE_POPUP_LONGKEY) {
            if (windows->is_base_window(window)) {
                ended = TRUE;
                /* In case of direction button, the release event on other window should be processed */
                if(configure && winctx && pressedCtx) {
                    if(configure->buttonType == BUTTON_TYPE_DIRECTION) {
                        mcfint relx = (winctx->x + x) - pressedCtx->x;
                        mcfint rely = (winctx->y + y) - pressedCtx->y;
                        if (process_button_release_event(pressedWindow, relx, rely, pressedKey, touchid)) {
                            btnIndex = pressedKey;
                            ret = TRUE;
                            x = coordination->x + (coordination->width / 2);
                            y = coordination->y + (coordination->height / 2);
                            skipwindow = pressedWindow;
                        }
                    }
                }
            }
        }

        MultiTouchContext *mulctx = context->find_multi_touch_context(touchid);
        if(mulctx) {
            if(!(mulctx->mIsSubEvent)) {
                /* First check if the event occured in pressed key's threshold area */
                if (btncontext && coordination && !ended) {
                    if (btncontext->used && btncontext->state != BUTTON_STATE_DISABLED) {
                        if (process_button_release_event(pressedWindow, x, y, pressedKey, touchid)) {
                            btnIndex = pressedKey;
                            ret = TRUE;
                            x = coordination->x + (coordination->width / 2);
                            y = coordination->y + (coordination->height / 2);
                        }
                    }
                }
                for (int loop = 0;loop < MAX_KEY && !ended;loop++) {
                    McfButtonContext *btncontext = cache->get_cur_button_context(window, loop);
                    const McfLayoutKeyConfigure *configure = cache->get_cur_layout_key_configure(window, loop);
                    if (btncontext && configure) {
                        if (!(btncontext->used)) {
                            ended = TRUE;
                        } else if (btncontext->state != BUTTON_STATE_DISABLED &&
                                    configure->buttonType != BUTTON_TYPE_UIITEM) {
                            if (window != pressedWindow || loop != pressedKey) {
                                if (process_button_release_event(window, x, y, loop, touchid)) {
                                    btnIndex = loop;
                                    ret = TRUE;
                                }
                            }
                        }
                    }
                }
            } else {
                McfKeyEventDesc keyEventDesc = {0};
                keyEventDesc.keyValue = NULL;
                keyEventDesc.keyEvent = NOT_USED;
                keyEventDesc.keyModifier = KEY_MODIFIER_NONE;
                keyEventDesc.touchid = touchid;
                keyEventDesc.mousePressedPt = context->get_cur_pressed_point(touchid);
                keyEventDesc.mouseCurrentPt = context->get_cur_move_point(touchid);
                keyEventDesc.mouseFarthestPt = context->get_farthest_move_point(touchid);

                keyEventDesc.touch_event_order = context->get_multi_touch_event_order(touchid);

                if(mEventCallback) {
                    mEventCallback->on_event_key_clicked(keyEventDesc);
                }
            }
        }

        /* For covering a missing area about 1 pixel */
        if (!ret) {
            ended = FALSE;

            if (state->get_cur_action_state() == ACTION_STATE_POPUP_INIT ||
                state->get_cur_action_state() == ACTION_STATE_POPUP_PRESS ||
                state->get_cur_action_state() == ACTION_STATE_POPUP_MOVING ||
                state->get_cur_action_state() == ACTION_STATE_POPUP_RELEASE ||
                state->get_cur_action_state() == ACTION_STATE_POPUP_REPEATKEY ||
                state->get_cur_action_state() == ACTION_STATE_POPUP_LONGKEY) {
                if (windows->is_base_window(window)) {
                    ended = TRUE;
                }
            }

            for (int loop = 0;loop < MAX_KEY && !ended;loop++) {
                McfButtonContext *btncontext = cache->get_cur_button_context(window, loop);
                if (btncontext && configure) {
                    if (!(btncontext->used)) {
                        ended = TRUE;
                        break;
                    } else if (btncontext->state != BUTTON_STATE_DISABLED &&
                               configure->buttonType != BUTTON_TYPE_UIITEM) {
                        if (process_button_release_event(window, x+1, y+1, loop, touchid)) {
                            btnIndex = loop;
                            ret = TRUE;
                            break;
                        }
                    }
                }
            }
        }

        if (windows->is_base_window(window)) {
            state->set_cur_action_state(ACTION_STATE_BASE_INIT);
        } else {
            state->set_cur_action_state(ACTION_STATE_POPUP_INIT);
        }

        /* Restore previously pressed button's context and redraw it */
        if (btncontext && coordination) {
            btncontext->state = BUTTON_STATE_NORMAL;
         }

        /* If there is postponed update of button, update it now */
        CMCFEvents *events = CMCFEvents::get_instance();
        mcfwindow lastWin = context->get_last_pressed_window();
        mcf8 lastKey = context->get_last_pressed_key();
        if (lastWin != MCFWINDOW_INVALID && lastKey != NOT_USED) {
            const McfLayoutKeyCoordination* coords = cache->get_cur_layout_key_coordination(lastWin, lastKey);
            if (coords) {
                windows->update_window(lastWin, coords->x, coords->y, coords->width, coords->height);
            }
        }

        /* To postpone some of the feedback for releasing */
        context->set_last_pressed_key(context->get_cur_pressed_key(touchid));
        context->set_last_pressed_window(context->get_cur_pressed_window(touchid));

        /* Do what has to be done when mouse gets released */
        mcfboolean signaled = FALSE;
        if (configure) {
            switch (configure->popupType) {
            case POPUP_TYPE_BTN_RELEASE_POPUP:
            case POPUP_TYPE_CONTEXT_POPUP:
            case POPUP_TYPE_BTN_RELEASE_POPUP_ONCE:
            case POPUP_TYPE_BTN_LONGPRESS_POPUP:
            case POPUP_TYPE_BTN_LONGPRESS_POPUP_ONCE:
                /* Fix me : We should consider z-order */
                skipwindow = windows->get_nth_window_in_Z_order_list(MCF_WINDOW_Z_TOP);
                handle_engine_signal(MCF_SIG_MOUSE_RELEASE, skipwindow);
                signaled = TRUE;
                break;
            }
        }
        if (!signaled) {
            McfWindowContext *ctx = windows->get_window_context(window, FALSE);
            if (ctx) {
                if (ctx->popuptype == POPUP_TYPE_BTN_RELEASE_POPUP ||
                    ctx->popuptype == POPUP_TYPE_BTN_LONGPRESS_POPUP) {
                    /* Don't close window if the clicked button is a child of ReleasePopup window */
                    skipwindow = window;
                    handle_engine_signal(MCF_SIG_MOUSE_RELEASE, window);
                    signaled = TRUE;
                }
            }
            if (!signaled) {
                handle_engine_signal(MCF_SIG_MOUSE_RELEASE);
            }
        }

        context->set_cur_pressed_key(touchid, NOT_USED);
        context->set_cur_pressed_window(touchid, MCFWINDOW_INVALID);

        if (ret && btnIndex != NOT_USED) {
            const McfLayoutKeyProperties *properties = cache->get_cur_layout_key_properties(window, btnIndex);
            if (properties) {
                if (properties->dontClosePopup) {
                    skipwindow = windows->get_nth_window_in_Z_order_list(MCF_WINDOW_Z_TOP);
                }
            }
        } else {
            if(pressedWindow == windows->get_nth_window_in_Z_order_list(MCF_WINDOW_Z_TOP)) {
                if(pressedCtx) {
                    if(pressedCtx->popuptype != POPUP_TYPE_BTN_RELEASE_POPUP_ONCE &&
                       pressedCtx->popuptype != POPUP_TYPE_BTN_LONGPRESS_POPUP_ONCE &&
                       pressedCtx->popuptype != POPUP_TYPE_AUTO_POPUP &&
                       pressedCtx->popuptype != POPUP_TYPE_BTN_PRESS_POPUP_DRAG &&
                       pressedCtx->popuptype != POPUP_TYPE_CONTEXT_POPUP) {
                       skipwindow = pressedWindow;
                    }
                }
            }
        }
        windows->close_all_popups(skipwindow);

        /* Destroy key related timers */
        events->destroy_timer(MCF_TIMER_AUTOPOPUP);
        events->destroy_timer(MCF_TIMER_SHORT_LONGKEY);
        events->destroy_timer(MCF_TIMER_LONGKEY);
        events->destroy_timer(MCF_TIMER_REPEATKEY);

        /* To postpone some of the feedback for releasing */
        events->create_timer(MCF_TIMER_BUTTON_DELAY, mButtonDelayDuration, 0);
    }

    if(context) {
        if(touchid == context->get_last_touch_device_id()) {
            context->set_last_touch_device_id(MCFTOUCHDEVICE_INVALID);
        }
        context->destroy_multi_touch_context(touchid);
    }

    return ret;
}

mcfboolean
CMCFController::mouse_move(mcfwindow window, mcfint x, mcfint y, mcftouchdevice touchid)
{
    MCF_DEBUG();
    mcfboolean ret = FALSE;

    if(mInputEventsDisabled)
	return FALSE;

    /* Adjust x,y coordination by touch offset */
    CMCFErrorAdjustment *adjustment = CMCFErrorAdjustment::get_instance();
    /* Iterate all the buttons and inform the event */

    CMCFContext *context = CMCFContext::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    CMCFActionState *state = CMCFActionState::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFEvents *events = CMCFEvents::get_instance();
    CMCFUtils *utils = CMCFUtils::get_instance();

    if (cache && state && windows && context && utils && adjustment) {
        if(!(context->find_multi_touch_context(touchid))) {
        	return FALSE;
        }

        MCFDisplay currDisplay = context->get_display();
        adjustment->apply_touch_offset(mcf_default_configure.touchOffsetLevel[currDisplay], &x, &y);

        McfWindowContext *winctx = windows->get_window_context(window, FALSE);
        /* Adjust event x and y positions as relative position to the virtual window */
        if(winctx) {
            /* If the dim window is virtual and currently active, let's just skip this event */
            if(windows->is_base_window(window)) {
                McfWindowContext *dimctx = windows->get_window_context(windows->get_dim_window());
                if(dimctx) {
                    if(dimctx->isVirtual && !(dimctx->hidden)) {
                        return FALSE;
                    }
                }
            }
            /* If the pressed event was occured in dim window, let's just skip this move event */
            if(context->get_last_pressed_window() == windows->get_dim_window()) {
                return FALSE;
            }
        }

        context->set_cur_move_point(touchid, x, y);
        context->set_cur_move_window(touchid, window);

        /* If in longkey state, do not process, just return */
        if (state->get_cur_action_state() == ACTION_STATE_BASE_LONGKEY ||
            state->get_cur_action_state() == ACTION_STATE_BASE_REPEATKEY ||
            state->get_cur_action_state() == ACTION_STATE_POPUP_LONGKEY ||
            state->get_cur_action_state() == ACTION_STATE_POPUP_REPEATKEY) {
            return FALSE;
        }
        /* FIXME : The rule below would not be a general requirement. A policy is needed regarding this. */
        /* And if the event occured in popup window, don't come back to base window */
        if (state->get_cur_action_state() == ACTION_STATE_POPUP_INIT ||
            state->get_cur_action_state() == ACTION_STATE_POPUP_PRESS ||
            state->get_cur_action_state() == ACTION_STATE_POPUP_MOVING ||
            state->get_cur_action_state() == ACTION_STATE_POPUP_RELEASE ||
            state->get_cur_action_state() == ACTION_STATE_POPUP_REPEATKEY ||
            state->get_cur_action_state() == ACTION_STATE_POPUP_LONGKEY) {
            if (windows->is_base_window(window)) {
                return FALSE;
            }
        }

        /* FIXME : Add a condition to skip this code if longkey timer is not active */
        /* If the mouse has moved out of threshold value of longkey keypress area, destroy longkey timer */
        if(mLongkeyCancelDist > 0) {
            mcffloat dist = utils->get_distance(x, y,
                                                context->get_cur_pressed_point(touchid).x,
                                                context->get_cur_pressed_point(touchid).y);
            if(mLongkeyCancelDist < dist) {
                events->destroy_timer(MCF_TIMER_LONGKEY);
            }
        }

        if (windows->is_base_window(window)) {
            state->set_cur_action_state(ACTION_STATE_BASE_MOVING);
        } else {
            state->set_cur_action_state(ACTION_STATE_POPUP_MOVING);
        }

        /* Iterate all the buttons and inform the event */
        mcfboolean ended = FALSE;
        mcfwindow pressedWindow = context->get_cur_pressed_window(touchid);
        mcf8 pressedKey = context->get_cur_pressed_key(touchid);
        McfButtonContext *btncontext = cache->get_cur_button_context(pressedWindow, pressedKey);
        const McfLayoutKeyCoordination *coordination = cache->get_cur_layout_key_coordination(pressedWindow, pressedKey);

        /* Check farthest move point and update it */
        mcfint originx = x;
        mcfint originy = y;
        if(pressedWindow != window) {
            McfWindowContext *pressedwinctx = windows->get_window_context(pressedWindow, FALSE);
            if(winctx && pressedwinctx) {
                originx = (winctx->x - pressedwinctx->x) + x;
                originy = (winctx->y - pressedwinctx->y) + y;
            }
        }
        mcfint deltax = originx - context->get_cur_pressed_point(touchid).x;
        mcfint deltay = originy - context->get_cur_pressed_point(touchid).y;
        mcffloat dist = utils->get_approximate_distance(originx, originy,
                                                        context->get_cur_pressed_point(touchid).x,
                                                        context->get_cur_pressed_point(touchid).y);
        CMCFUtils *utils = CMCFUtils::get_instance();
        /* Do not check farthest move point if current drag state is MCF_DRAG_STATE_RETURN */
        if(context->get_cur_drag_state(touchid) != MCF_DRAG_STATE_RETURN) {
            if(dist > context->get_farthest_move_dist(touchid)) {
               context->set_farthest_move_point(touchid, originx, originy);
            }
        }

        /* Check if we should recognize drag curve */
        const McfLayoutKeyConfigure *configure = cache->get_cur_layout_key_configure(pressedWindow, pressedKey);
        const McfLayoutKeyProperties *properties = cache->get_cur_layout_key_properties(pressedWindow, pressedKey);
        if(configure && properties) {
            mcfboolean dragStateChanged = FALSE;
            if(configure->buttonType == BUTTON_TYPE_DIRECTION) {
                MCFDragState curDragState = context->get_cur_drag_state(touchid);
                MCFDragState nextDragState = MCF_DRAG_STATE_NONE;
                mcffloat direction_recog_dist = MCF_DIRECTION_RECOG_DIST;
                if(configure->fIsSideButton) {
                    direction_recog_dist = MCF_DIRECTION_RECOG_DIST_SIDE;
                };
                if(curDragState == MCF_DRAG_STATE_RETURN) {
                    direction_recog_dist *= MCF_DRAG_RETURN_RECOG_THRESHOLD_RETURN;
                } else if(curDragState != MCF_DRAG_STATE_NONE) {
                    direction_recog_dist *= MCF_DRAG_RETURN_RECOG_THRESHOLD_OTHER;
                }
                if(dist > direction_recog_dist) {
                    mcffloat ratio = fabs((mcffloat)deltay / (deltax ? deltax : 1));
                    /* If tan(theta) is smaller than our predefined value */
                    if(ratio <= (1 / MCF_DRAG_CURVE_4_DIRECTION_ANGLE_VALUE)) {
                        if(deltax > 0) {
                            nextDragState = MCF_DRAG_STATE_RIGHT;
                        } else {
                            nextDragState = MCF_DRAG_STATE_LEFT;
                        }
                    } /* If tan(theta) is bigger than our predefined value */
                    else if(ratio >= MCF_DRAG_CURVE_4_DIRECTION_ANGLE_VALUE) {
                        if(deltay > 0) {
                            nextDragState = MCF_DRAG_STATE_DOWN;
                        } else {
                            nextDragState = MCF_DRAG_STATE_UP;
                        }
                    } else {
                        nextDragState = MCF_DRAG_STATE_INVALID;
                    }
                    /* Disable longkey if dragging is recognized */
                    events->destroy_timer(MCF_TIMER_LONGKEY);
                }
                if(curDragState != nextDragState) {
                    dragStateChanged = TRUE;
                }
                if(curDragState == MCF_DRAG_STATE_NONE) {
                        curDragState = nextDragState;
                } else if(curDragState != nextDragState) {
                    if(properties->extraOption == DIRECTION_EXTRA_OPTION_4_DIRECTIONS_WITH_RETURN_AND_CURVE &&
                       nextDragState != MCF_DRAG_STATE_NONE) {
                       curDragState = MCF_DRAG_STATE_INVALID;
                    } else {
                        if(nextDragState == MCF_DRAG_STATE_NONE) {
                           curDragState = MCF_DRAG_STATE_RETURN;
                        } else {
                           curDragState = nextDragState;

                            /* Let's save our drag state if the drag state changes to RETURN */
                            if(curDragState == MCF_DRAG_STATE_RETURN) {
                               context->set_prev_drag_state(touchid, context->get_cur_drag_state(touchid));
                            }
                        }
                    }
                }

                /* Let's add one more condition for checking return state - if the drag action goes backward*/
                if(curDragState == context->get_prev_drag_state(touchid) ||
                   context->get_prev_drag_state(touchid) == MCF_DRAG_STATE_NONE) {
                    mcfint distFromFarthest =
                    		utils->get_approximate_distance(context->get_farthest_move_point(touchid).x,
                    				                        context->get_farthest_move_point(touchid).y,
                        context->get_cur_move_point(touchid).x, context->get_cur_move_point(touchid).y);
                    if(distFromFarthest > MCF_DRAG_RETURN_RECOG_DIST_BACKWARD) {
                        mcffloat theta_from_farthest = 0.0f;
                        mcffloat theta_to_farthest = 0.0f;
                        McfPoint delta_from_farthest;
                        McfPoint delta_to_farthest;
                        delta_from_farthest.x =
                            context->get_cur_move_point(touchid).x - context->get_farthest_move_point(touchid).x;
                        delta_from_farthest.y =
                            context->get_cur_move_point(touchid).y - context->get_farthest_move_point(touchid).y;
                        delta_to_farthest.x =
                            context->get_farthest_move_point(touchid).x - context->get_cur_pressed_point(touchid).x;
                        delta_to_farthest.y =
                            context->get_farthest_move_point(touchid).y - context->get_cur_pressed_point(touchid).y;
                        /* For the case of divide by zero situation */
                        if(delta_from_farthest.x == 0) {
                        	delta_from_farthest.x = 1;
                        }
                        if(delta_to_farthest.x == 0) {
                        	delta_to_farthest.x = 1;
                        }
                        theta_from_farthest = atan2((mcffloat)delta_from_farthest.y, (mcffloat)delta_from_farthest.x);
                        theta_to_farthest = atan2((mcffloat)delta_to_farthest.y, (mcffloat)delta_to_farthest.x);

                        /* If the direction is different */
                        const mcffloat threshold_angle = MCF_DRAG_RETURN_RECOG_ANGLE_BACKWARD;
                        if(fabs(theta_to_farthest - theta_from_farthest) > M_PI - threshold_angle &&
                           fabs(theta_to_farthest - theta_from_farthest) < M_PI + threshold_angle) {
                           curDragState = MCF_DRAG_STATE_RETURN;
                           if(context->get_cur_drag_state(touchid) != MCF_DRAG_STATE_RETURN) {
                              context->set_prev_drag_state(touchid, context->get_cur_drag_state(touchid));
                           }
                        }
                    }
                }

                if(context->get_cur_drag_state(touchid) == MCF_DRAG_STATE_RETURN && curDragState != MCF_DRAG_STATE_RETURN) {
                    if(curDragState == context->get_prev_drag_state(touchid)) {
                        if(curDragState == MCF_DRAG_STATE_LEFT) {
                            if(context->get_cur_move_point(touchid).x > context->get_farthest_move_point(touchid).x) {
                               curDragState = MCF_DRAG_STATE_RETURN;
                            }
                        }
                        if(curDragState == MCF_DRAG_STATE_RIGHT) {
                            if(context->get_cur_move_point(touchid).x < context->get_farthest_move_point(touchid).x) {
                               curDragState = MCF_DRAG_STATE_RETURN;
                            }
                        }
                        if(curDragState == MCF_DRAG_STATE_UP) {
                            if(context->get_cur_move_point(touchid).y > context->get_farthest_move_point(touchid).y) {
                               curDragState = MCF_DRAG_STATE_RETURN;
                            }
                        }
                        if(curDragState == MCF_DRAG_STATE_DOWN) {
                            if(context->get_cur_move_point(touchid).y < context->get_farthest_move_point(touchid).y) {
                               curDragState = MCF_DRAG_STATE_RETURN;
                            }
                        }
                    }
                }

                if(context->get_cur_drag_state(touchid) == MCF_DRAG_STATE_RETURN && curDragState != MCF_DRAG_STATE_RETURN) {
                    /* Reset farthest point if the state goes from return to simple drag */
                    context->set_farthest_move_point(touchid,
                        context->get_cur_pressed_point(touchid).x, context->get_cur_pressed_point(touchid).y);
                    if(curDragState != context->get_prev_drag_state(touchid)) {
                        context->set_prev_drag_state(touchid, MCF_DRAG_STATE_NONE);
                    }
                }
                context->set_cur_drag_state(touchid, curDragState);
                mcfboolean check_farthest = FALSE;
                mcfshort display = context->get_display();
                if(!mcf_check_arrindex(display, MCFDISPLAY_MAX)) display = 0;
                mcffloat dist = utils->get_distance(originx, originy,
                    context->get_cur_pressed_point(touchid).x, context->get_cur_pressed_point(touchid).y);
                if(dist < direction_recog_dist && context->get_cur_drag_state(touchid) == MCF_DRAG_STATE_RETURN) {
                    if(properties->extraOption == DIRECTION_EXTRA_OPTION_8_DIRECTIONS_WITH_RETURN ||
                       properties->extraOption == DIRECTION_EXTRA_OPTION_4_DIRECTIONS_WITH_RETURN ||
                       properties->extraOption == DIRECTION_EXTRA_OPTION_4_DIRECTIONS_WITH_RETURN_AND_CURVE) {
                       deltax = context->get_farthest_move_point(touchid).x -
                                context->get_cur_pressed_point(touchid).x;
                       deltay = context->get_farthest_move_point(touchid).y -
                                context->get_cur_pressed_point(touchid).y;
                       dist = utils->get_distance(context->get_farthest_move_point(touchid),
                                context->get_cur_pressed_point(touchid));
                       check_farthest = TRUE;
                    }
                }
                MCFKeyModifier keyModifier = get_drag_key_modifier(deltax, deltay, dist,
                                                                   check_farthest, touchid, properties->extraOption);
                if (dist > direction_recog_dist) {
                    context->set_cur_key_modifier(touchid, keyModifier);
                }
                /* If this button needs to be decorated when dragged */
                if(properties->modifierDecorationId) {
                    const McfModifierDecoration  *decoration = NULL;
                    if(mcf_check_arrindex(properties->modifierDecorationId,
                        sizeof(mcf_modifier_decoration) / sizeof(McfModifierDecoration ))) {
                            decoration = &(mcf_modifier_decoration[properties->modifierDecorationId]);
                    }
                    /* Check if the button really needs to be redrawn (whether it has non-null bgImgPath information */
                    if(decoration) {
                        if(keyModifier != prevModifier) {
                            if(decoration->bgImgPath[display][keyModifier]) {
                                windows->update_window(window,
                                                       coordination->x, coordination->y,
                                                       coordination->width, coordination->height);
                            }
                        }
                    }
                }
                if (dist > direction_recog_dist) {
                    McfKeyEventDesc desc = {0};
                    MCFShiftState shiftidx = context->get_shift_state();
                    desc.keyType = configure->keyType;
                    desc.keyValue = properties->keyValue[shiftidx][0];
                    desc.keyEvent = properties->keyEvent[shiftidx][0];
                    desc.mousePressedPt = context->get_cur_pressed_point(touchid);
                    desc.mouseCurrentPt = context->get_cur_move_point(touchid);
                    desc.mouseFarthestPt = context->get_farthest_move_point(touchid);
                    desc.keyModifier = keyModifier;
                }
            }
        }

        mcfboolean grab_event = FALSE;
        const McfLayout *layout = cache->get_cur_layout(window);
        if(layout) {
            if(layout->style == LAYOUT_STYLE_POPUP_GRAB) {
                grab_event = TRUE;
            }
            /* If the topmost window has the POPUP_GRAB style, find the nearest button to the mouse pointer */
            if(grab_event) {
                /* If the layout's addGrab* values are defined, process this event only if the event occured inside grab area */
                mcfboolean in_grab_area = TRUE;
                if(layout->addGrabLeft != NOT_USED && x < -(layout->addGrabLeft)) {
                    in_grab_area = FALSE;
                }
                if(layout->addGrabRight != NOT_USED && x > (winctx->width + layout->addGrabRight)) {
                    in_grab_area = FALSE;
                }
                if(layout->addGrabTop != NOT_USED && y < -(layout->addGrabTop)) {
                    in_grab_area = FALSE;
                }
                if(layout->addGrabBottom != NOT_USED && y > (winctx->height + layout->addGrabBottom)) {
                    in_grab_area = FALSE;
                }
                if(in_grab_area) {
                    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
                    if(cache) {
                        float min_dist = (float)((unsigned int)(-1));
                        int min_dist_index = NOT_USED;
                        for (int loop = 0;loop < MAX_KEY && !ended && !ret;loop++) {
                            btncontext = cache->get_cur_button_context(window, loop);
                            const McfLayoutKeyConfigure *configure = cache->get_cur_layout_key_configure(window, loop);
                            if (btncontext && configure) {
                                if (!(btncontext->used)) {
                                    ended = TRUE;
                                } else if (btncontext->state != BUTTON_STATE_DISABLED &&
                                           configure->buttonType != BUTTON_TYPE_UIITEM) {
                                        const McfLayoutKeyCoordination *coordination =
                                        		cache->get_cur_layout_key_coordination(window, loop);
                                        float dist = utils->get_approximate_distance(x, y,
                                                                                     coordination->x + (coordination->width / 2),
                                                                                     coordination->y + (coordination->height / 2));
                                        if(dist < min_dist) {
                                            min_dist_index = loop;
                                            min_dist = dist;
                                        }
                                }
                            }
                        }
                        /* When we found the nearest button, generate this event on the button */
                        if(min_dist_index != NOT_USED) {
                            const McfLayoutKeyCoordination *coordination = cache->get_cur_layout_key_coordination(window,
                            		                                                                              min_dist_index);
                            x = coordination->x + (coordination->width / 2);
                            y = coordination->y + (coordination->height / 2);
                            if (process_button_move_event(window, x, y, min_dist_index, touchid)) {
                                ret = TRUE;
                            }
                        }
                    }
                }
            } else {
                MultiTouchContext *mulctx = context->find_multi_touch_context(touchid);
                if(mulctx) {
                    if(!(mulctx->mIsSubEvent)) {
                        /* First check if the event occured in pressed key's threshold area */
                        if (btncontext && coordination) {
                            if (pressedWindow == window) { // Check only when the window is the one initally pressed
                                if (btncontext->used && btncontext->state != BUTTON_STATE_DISABLED) {
                                    if (process_button_move_event(pressedWindow, x, y, pressedKey, touchid)) {
                                        ret = TRUE;
                                        x = coordination->x + (coordination->width / 2);
                                        y = coordination->y + (coordination->height / 2);
                                    }
                                }
                            }
                        }
                        for (int loop = 0;loop < MAX_KEY && !ended && !ret;loop++) {
                            btncontext = cache->get_cur_button_context(window, loop);
                            const McfLayoutKeyConfigure *configure = cache->get_cur_layout_key_configure(window, loop);
                            if (btncontext && configure) {
                                if (!(btncontext->used)) {
                                    ended = TRUE;
                                } else if (btncontext->state != BUTTON_STATE_DISABLED &&
                                           configure->buttonType != BUTTON_TYPE_UIITEM) {
                                    if (window != pressedWindow || loop != pressedKey) {
                                        if (process_button_move_event(window, x, y, loop, touchid)) {
                                            ret = TRUE;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return ret;
}

/**
 * Processes a timer event
 * If return FALSE, the current timer will be stop
 * ID : MCF_LOWORD(data)
 * value : MCF_HIWORD(data)
 */
mcfboolean
CMCFController::timer_event(const mcf32 data)
{
    MCF_DEBUG();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFContext *context = CMCFContext::get_instance();
    CMCFEvents* events = CMCFEvents::get_instance();
    CMCFActionState *state = CMCFActionState::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    CMCFUtils *utils = CMCFUtils::get_instance();

    mcf16 id = MCF_LOWORD(data); /* Timer ID */
    mcf16 value = MCF_HIWORD(data); /* event unique ID */

    switch (id) {
    case MCF_TIMER_AUTOPOPUP: {
        /* Checks whether my event id is availble */
        if (context->get_cur_pressed_event_id(context->get_last_touch_device_id()) != value ||
            state->get_cur_action_state() == ACTION_STATE_BASE_INIT ||
            state->get_cur_action_state() == ACTION_STATE_BASE_RELEASE ||
            state->get_cur_action_state() == ACTION_STATE_BASE_LONGKEY ||
            state->get_cur_action_state() == ACTION_STATE_BASE_REPEATKEY ||
            state->get_cur_action_state() == ACTION_STATE_POPUP_INIT ||
            state->get_cur_action_state() == ACTION_STATE_POPUP_RELEASE ||
            state->get_cur_action_state() == ACTION_STATE_POPUP_LONGKEY ||
            state->get_cur_action_state() == ACTION_STATE_POPUP_REPEATKEY) {
            /* Ignores if the event id is different */
            return FALSE;
        }

        McfRectangle rect = {0,};
        mcfwindow window = context->get_cur_pressed_window(context->get_last_touch_device_id());
        mcfbyte keyIndex = context->get_cur_pressed_key(context->get_last_touch_device_id());

        McfButtonContext *btncontext = cache->get_cur_button_context(window, keyIndex);
        if (btncontext->state == BUTTON_STATE_PRESSED) {
            btncontext->state = BUTTON_STATE_NORMAL;
            CMCFWindows *windows = CMCFWindows::get_instance();
            if (windows) {
                const McfLayoutKeyCoordination *coordination = cache->get_cur_layout_key_coordination(window, keyIndex);
                windows->update_window(window, coordination->x, coordination->y, coordination->width, coordination->height);
            }
        }
        if (configure_autopopup_window(window, keyIndex, &rect)) {
            /* Currently, window does not support virtual window */
            mcfwindow popup_window = windows->open_popup(window,
            		                                     keyIndex,
                                                         rect.x,
                                                         rect.y,
                                                         rect.width,
                                                         rect.height,
                                                         MCF_LAYOUT_AUTOPOPUP,
                                                         POPUP_TYPE_AUTO_POPUP,
                                                         FALSE,
                                                         FALSE);

            const McfLayoutKeyCoordination* coordination = cache->get_cur_layout_key_coordination(window, keyIndex);
            McfButtonContext *btncontext = cache->get_cur_button_context(window, keyIndex);

			mcfwindow move_window = context->get_cur_move_window(context->get_last_touch_device_id());
			McfPoint move_point = context->get_cur_move_point(context->get_last_touch_device_id());
			McfWindowContext *move_ctx = windows->get_window_context(move_window);
			McfWindowContext *popup_ctx = windows->get_window_context(popup_window);
			if(move_ctx && popup_ctx) {
				move_point.x = (move_ctx->x - popup_ctx->x) + move_point.x;
				move_point.y = (move_ctx->y - popup_ctx->y) + move_point.y;
			}
			printf("AUTOPOPUP : %d %d\n", move_point.x, move_point.y);

			/* Find the nearest button on the autopopup window */
			mcfboolean ended = FALSE;
			float min_dist = (float)((unsigned int)(-1));
			int min_dist_index = NOT_USED;
			for (int loop = 0;loop < MAX_KEY && !ended;loop++) {
				McfButtonContext *popup_btncontext = cache->get_cur_button_context(popup_window, loop);
				const McfLayoutKeyConfigure *popup_configure = cache->get_cur_layout_key_configure(popup_window, loop);
				if (popup_btncontext && popup_configure) {
					if (!(popup_btncontext->used)) {
						ended = TRUE;
					} else if (popup_btncontext->state != BUTTON_STATE_DISABLED &&
						       popup_configure->buttonType != BUTTON_TYPE_UIITEM) {
							const McfLayoutKeyCoordination *popup_coordination =
								cache->get_cur_layout_key_coordination(popup_window, loop);
							float dist = utils->get_approximate_distance(move_point.x, move_point.y,
								                                         popup_coordination->x + (popup_coordination->width / 2),
								                                         popup_coordination->y + (popup_coordination->height / 2));
							if(dist < min_dist) {
								min_dist_index = loop;
								min_dist = dist;
							}
					}
				}
			}
			/* When we found the nearest button, make it pressed */
			if(min_dist_index != NOT_USED) {
				const McfLayoutKeyCoordination *popup_coordination =
					cache->get_cur_layout_key_coordination(popup_window, min_dist_index);
				mcfint x = popup_coordination->x + (popup_coordination->width / 2);
				mcfint y = popup_coordination->y + (popup_coordination->height / 2);
				process_button_pressed_event(popup_window, x, y, min_dist_index, context->get_last_touch_device_id());
			}

            context->set_cur_pressed_window(context->get_last_touch_device_id(), popup_window);
            context->set_cur_pressed_key(context->get_last_touch_device_id(), min_dist_index);
            if(btncontext) {
                btncontext->state = BUTTON_STATE_NORMAL;
            }

            CMCFWindows *windows = CMCFWindows::get_instance();
            if (windows) {
                windows->update_window(window, coordination->x, coordination->y, coordination->width, coordination->height);
            }
        }
        events->destroy_timer(id);
        context->set_cur_pressed_window(context->get_last_touch_device_id(), MCFWINDOW_INVALID);
        context->set_cur_pressed_key(context->get_last_touch_device_id(), NOT_USED);
        return FALSE;
    }
    break;

    case MCF_TIMER_LONGKEY: {
        /* Checks whether my event id is availble */
        if (context->get_cur_pressed_event_id(context->get_last_touch_device_id()) != value ||
            state->get_cur_action_state() == ACTION_STATE_BASE_INIT ||
            state->get_cur_action_state() == ACTION_STATE_BASE_RELEASE ||
            state->get_cur_action_state() == ACTION_STATE_BASE_LONGKEY ||
            state->get_cur_action_state() == ACTION_STATE_BASE_REPEATKEY ||
            state->get_cur_action_state() == ACTION_STATE_POPUP_INIT ||
            state->get_cur_action_state() == ACTION_STATE_POPUP_RELEASE ||
            state->get_cur_action_state() == ACTION_STATE_POPUP_LONGKEY ||
            state->get_cur_action_state() == ACTION_STATE_POPUP_REPEATKEY) {
            /* Ignores if the event id is different */
            return FALSE;
        }
        /* Ignores if the event id is different */
        mcfwindow window = context->get_cur_pressed_window(context->get_last_touch_device_id());
        mcfbyte keyindex = context->get_cur_pressed_key(context->get_last_touch_device_id());
        if (process_button_long_pressed_event(window, keyindex, context->get_last_touch_device_id())) {
            /* The button processed long key event, now enter longkey mode not to fire any events before releasing */
            handle_engine_signal(MCF_SIG_MOUSE_LONG_PRESS, window);
        } else {
            /* Start the repeat key timer for NORMAL or GRAB buttons if longkey is not supported */
            const McfLayoutKeyConfigure *configure = cache->get_cur_layout_key_configure(window, keyindex);
            const McfLayoutKeyProperties *properties = cache->get_cur_layout_key_properties(window, keyindex);

            if(configure && properties) {
                /* This is for enabling backspace key in search layout*/
                if (properties->useRepeatKey|| properties->keyEvent[0][0] == MVK_BackSpace) {
                    if (configure->buttonType == BUTTON_TYPE_NORMAL ||
                        configure->buttonType == BUTTON_TYPE_GRAB ||
                        configure->buttonType == BUTTON_TYPE_DIRECTION) {
                        mRepeatKeySeq = 0;
                        events->create_timer(MCF_TIMER_REPEATKEY, mRepeatKeyDuration, value);
                        if (windows->is_base_window(window)) {
                            state->set_cur_action_state(ACTION_STATE_BASE_REPEATKEY);
                        } else {
                            state->set_cur_action_state(ACTION_STATE_POPUP_REPEATKEY);
                        }
                    }
                }
            }
        }
        events->destroy_timer(id);
        return FALSE;
    }
    break;

    case MCF_TIMER_REPEATKEY: {
        /* Checks whether my event id is availble */
        if (context->get_cur_pressed_event_id(context->get_last_touch_device_id()) != value ||
            (state->get_cur_action_state() != ACTION_STATE_BASE_REPEATKEY &&
            state->get_cur_action_state() != ACTION_STATE_POPUP_REPEATKEY)) {
            /* Ignores if the event id is different */
            return FALSE;
        }
        mcfwindow window = context->get_cur_pressed_window(context->get_last_touch_device_id());
        mcfbyte keyIndex = context->get_cur_pressed_key(context->get_last_touch_device_id());
        mcflong interval = mRepeatKeyDuration - (mRepeatKeySeq * MCF_REPEATKEY_ACCELERATION);
        if (interval < MCF_REPEATKEY_MIN_DURATION) {
            interval = MCF_REPEATKEY_MIN_DURATION;
        }
        process_button_repeat_pressed_event(window, keyIndex, context->get_last_touch_device_id());
        events->destroy_timer(id);
        events->create_timer(MCF_TIMER_REPEATKEY, interval, value);
        mRepeatKeySeq++;
        return FALSE;
    }
    break;
    case MCF_TIMER_BUTTON_DELAY: {
        /* If there is postponed update of button, update it now */
        mcfwindow lastWin = context->get_last_pressed_window();
        mcf8 lastKey = context->get_last_pressed_key();

        if (lastWin != MCFWINDOW_INVALID && lastKey != NOT_USED) {
            const McfLayoutKeyCoordination* coords = cache->get_cur_layout_key_coordination(lastWin, lastKey);
            if (coords) {
                windows->update_window(lastWin, coords->x, coords->y, coords->width, coords->height);
            }
        }

        context->set_last_pressed_window(MCFWINDOW_INVALID);
        context->set_last_pressed_key(NOT_USED);
        events->destroy_timer(id);
        return FALSE;
    }
    break;
    case MCF_TIMER_POPUP_TIMEOUT: {
        windows->close_all_popups();

        if (mEventCallback) {
            mEventCallback->on_event_notification(NOTITYPE_POPUP_TIMEOUT, data);
        }
        events->destroy_timer(id);
        return FALSE;
    }
    break;
    case MCF_TIMER_AUTOTEST: {
        CMCFController *controller = CMCFController::get_instance();
        CMCFResourceCache *cache = CMCFResourceCache::get_instance();
        CMCFUtils *utils = CMCFUtils::get_instance();
        MCFDisplay currDisplay =  context->get_display();

        mcfint rnd = rand() % 100;
        mcfint x = (rand() % (cache->get_cur_layout(windows->get_base_window())->width));
        mcfint y = (rand() % (cache->get_cur_layout(windows->get_base_window())->height));

        if(rnd < 80) {
            events->generate_mouse_event(MCF_MOUSE_EVENT_PRESS, x, y);
            events->generate_mouse_event(MCF_MOUSE_EVENT_RELEASE, x, y);
        } else if(rnd < 90) {
            events->generate_mouse_event(MCF_MOUSE_EVENT_MOVE, x, y);
        } else if(rnd < 95) {
            events->generate_mouse_event(MCF_MOUSE_EVENT_PRESS, x, y);
        } else {
            events->generate_mouse_event(MCF_MOUSE_EVENT_RELEASE, x, y);
        }

        mDebugVariable++;
        if (mDebugVariable < MCF_AUTOTEST_NUM) events->create_timer(MCF_TIMER_AUTOTEST, MCF_AUTOTEST_TIMER_INTERVAL, 0, FALSE);
        else mDebugMode = DEBUGMODE_DISABLED;
        return FALSE;
    }
    break;

    default: {
        events->destroy_timer(id);
    }
    break;
    }

    return TRUE;
}

void
CMCFController::set_event_callback(IMCFEventCallback *callback)
{
    MCF_DEBUG();

    mcf_assert_return(callback);

    mEventCallback = callback;
}

/* Handles signals to manage contexts mainly focusing on resetting variables and cleaning up states */
void CMCFController::handle_engine_signal( McfInternalSignal signal, mcfwindow targetWindow )
{
    MCF_DEBUG();

    enum SIGACTIONS {
        SIGACTION_RESIZE_RESOURCES,
        SIGACTION_DESTROY_TIMERS,
        SIGACTION_CLEAR_PRIVATEKEYS,
        SIGACTION_RECOMPUTE_LAYOUT,
        SIGACTION_FREE_IMAGES,
        SIGACTION_CLOSE_POPUP,
        SIGACTION_UNSET_SHIFT,
        SIGACTION_UNPRESS_KEYS,
        SIGACTION_INIT_DISPLAY,
        SIGACTION_INIT_INPUTMODE,

        SIGACTION_MAXNUM
    };
    const mcfboolean SIGNAL_TABLE[SIGACTION_MAXNUM][MCF_SIG_MAXNUM] = {
        //	START,	SHOW,	HIDE,	INPCHNG,	DISPCHNG,	POPUPSHOW,	POPUPHIDE,	MOUSEPRES,	M-LONGPRES,	MOUSEREL,	KEYEVT,	FOCUSCHNG
        // SIGACTION_RESIZE_RESOURCES
        {	TRUE,	0,		0,		0,			0,			0,			0,			0,			0,			0,			0,		0		},
        // SIGACTION_DESTROY_TIMERS
        {	TRUE,	TRUE,	TRUE,	TRUE,		TRUE,		0,			0,			0,			0,			0,			0,		TRUE	},
        // SIGACTION_CLEAR_PRIVATEKEYS
        {	TRUE,	0,		0,		0,			0,			0,			0,			0,			0,			0,			0,		TRUE	},
        // SIGACTION_RECOMPUTE_LAYOUT
        {	0,		TRUE,	0,		TRUE,		TRUE,		TRUE,		0,			0,			0,			0,			0,		TRUE	},
        // SIGACTION_FREE_IMAGES
        {	TRUE,	0,		TRUE,	TRUE,		TRUE,		0,			0,			0,			0,			0,			0,		0		},
        // SIGACTION_CLOSE_POPUP
        {	TRUE,	TRUE,	TRUE,	TRUE,		TRUE,		0,			0,			0,			0,			0,			0,		TRUE	},
        // SIGACTION_UNSET_SHIFT
        {	TRUE,	0	,	0	,	TRUE,		TRUE,		0,			0,			0,			0,			0,			0,		TRUE	},
        // SIGACTION_UNPRESS_KEYS
        {	TRUE,	TRUE,	TRUE,	TRUE,		TRUE,		0,			0,			0,			0,			0,			0,		TRUE	},
        // SIGACTION_INIT_DISPLAY
        {	TRUE,	0,		0,		0,			0,			0,			0,			0,			0,			0,			0,		0		},
        // SIGACTION_INIT_INPUTMODE
        {	TRUE,	0,		0,		0,			0,			0,			0,			0,			0,			0,			0,		TRUE	},
    };

    mcf_assert_return(signal >= 0 && signal < MCF_SIG_MAXNUM);

    CMCFEvents* events = CMCFEvents::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFContext *context = CMCFContext::get_instance();
    CMCFImageProxy *proxy = CMCFImageProxy::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();

    switch(signal) {
        case MCF_SIG_SHOW:
            {
                context->set_hidden_state(FALSE);
            }
            break;
        case MCF_SIG_HIDE:
            {
                context->set_hidden_state(TRUE);
                windows->close_all_popups();
            }
            break;
    }

    int loop = 0;
    for (loop = 0;loop < SIGACTION_MAXNUM;loop++) {
        if (SIGNAL_TABLE[loop][signal] == TRUE) {
            switch (loop) {
            case SIGACTION_RESIZE_RESOURCES:
                break;
            case SIGACTION_DESTROY_TIMERS:
                events->destroy_all_timer();
                break;
            case SIGACTION_CLEAR_PRIVATEKEYS:
                break;
            case SIGACTION_RECOMPUTE_LAYOUT: {
                if (targetWindow != MCFWINDOW_INVALID) {
                    cache->recompute_layout(targetWindow);
                    // EFL testing
                    windows->update_window(targetWindow);
                }
            }
            break;
            case SIGACTION_FREE_IMAGES:
                proxy->free_images();
                break;
            case SIGACTION_CLOSE_POPUP: {
                /* If there is a popup still opened, don't destroy POPUP_TIMEOUT timer */
                if (!(windows->close_all_popups(targetWindow))) {
                    events->destroy_timer(MCF_TIMER_POPUP_TIMEOUT);
                }
            }
            break;
            case SIGACTION_UNSET_SHIFT:
                context->set_shift_state(MCF_SHIFT_STATE_OFF);
                break;
            case SIGACTION_UNPRESS_KEYS:
                context->set_cur_pressed_key(context->get_last_touch_device_id(), NOT_USED);
                context->set_cur_pressed_window(context->get_last_touch_device_id(), MCFWINDOW_INVALID);
                break;
            case SIGACTION_INIT_DISPLAY:
                break;
            case SIGACTION_INIT_INPUTMODE:
                break;
            }
        }
    }
}

/**
 * Sets the duration value for longkey
 * If not set, it will use default longkey duration. see mcfconfig
 */
mcfboolean
CMCFController::set_longkey_duration(mcflong msc)
{
    MCF_DEBUG();
    mcfboolean bRet = FALSE;
    if(msc > 0) {
        mLongkeyDuration = msc;
        bRet = TRUE;
    }
    return bRet;
}

/**
* Sets the distance value for cancel longkey
* If not set, it will use default longkey duration. see mcfconfig
*/
mcfboolean
CMCFController::set_longkey_cancel_dist(mcfshort dist)
{
    MCF_DEBUG();
    mcfboolean bRet = FALSE;
    if(dist > 0) {
        mLongkeyCancelDist = dist;
        bRet = TRUE;
    }
    return bRet;
}

/**
* Sets the duration value for repeatkey
* If not set, it will use default repeatkey duration. see mcfconfig
*/
mcfboolean
CMCFController::set_repeatkey_duration(mcflong msc)
{
    MCF_DEBUG();
    mcfboolean bRet = FALSE;
    if(msc > 0) {
        mRepeatKeyDuration = msc;
        bRet = TRUE;
    }
    return bRet;
}

/**
 * Sets the duration value for short longkey
 * If not set, it will use default short longkey duration. see mcfconfig
 */
mcfboolean
CMCFController::set_short_longkey_duration(mcflong msc)
{
    MCF_DEBUG();
    mcfboolean bRet = FALSE;
    if(msc > 0) {
        mShortLongkeyDuration = msc;
        bRet = TRUE;
    }
    return bRet;
}

/**
 * Sets the amount value for button delay
 * If not set, it will use default button delay amount. see mcfconfig
 */
mcfboolean
CMCFController::set_button_delay_duration(mcflong msc)
{
    MCF_DEBUG();
    mcfboolean bRet = FALSE;
    if(msc > 0) {
        mButtonDelayDuration = msc;
        bRet = TRUE;
    }
    return bRet;
}

/**
 * Configures the variables for auto-popup window
 * It will return rectangle area
 * @return FALSE It's not avaiable popup key
 */
mcfboolean
CMCFController::configure_autopopup_window(mcfwindow window, mcfbyte keyindex, McfRectangle* rect)
{
    MCF_DEBUG();
    mcf_assert_return_false(window);
    mcf_assert_return_false(keyindex >= 0);

    mcfboolean ret = TRUE;

    mcfbyte numKeys, numColumns, numRows;

    CMCFUtils *utils = CMCFUtils::get_instance();
    CMCFContext *context = CMCFContext::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();

    const McfLayoutKeyCoordination *coordination = NULL;
    const McfLayoutKeyProperties *properties = NULL;

    if(cache) {
        coordination = cache->get_cur_layout_key_coordination(window, keyindex);
        properties = cache->get_cur_layout_key_properties(window, keyindex);
    }

    if(utils && context && windows && cache && properties && coordination && rect) {
        MCFShiftState shiftidx = context->get_shift_state();
        if(shiftidx < 0 || shiftidx >= MCF_SHIFT_STATE_MAX) shiftidx = MCF_SHIFT_STATE_OFF;
        if (utils->get_autopopup_window_variables(properties->autopopupKeys[shiftidx],
            &numKeys, &numColumns, &numRows, &rect->width, &rect->height)) {

            /* There is no need for an autopopup window if number of keys are equal to or less than 1 */
            if (!(numKeys > 0)) {
                ret = FALSE;
            }
            /* calculates y position to be set */
            McfRectangle baseWndRect;
            int scrwidth, scrheight;
            const McfLayout *layout = cache->get_cur_layout(windows->get_base_window());
            utils->get_screen_resolution(&scrwidth, &scrheight);

            windows->get_window_rect(windows->get_base_window(), &baseWndRect);
            /* Let the autopopup have its position right above the pressed button, with center alignment) */
            rect->x = baseWndRect.x + coordination->x + (coordination->width / 2) - (rect->width / 2);
            rect->y = (scrheight - layout->height) + coordination->y - rect->height +
            		   utils->get_scale_y(mcf_autopopup_configure.wndDecoSize);
            /* Check if the window goes out of screen boundary */
            if (rect->x + rect->width > scrwidth) rect->x = (scrwidth) - rect->width;
            if (rect->y + rect->height > scrheight) rect->y = scrheight - rect->height;
            if (rect->x < 0 - utils->get_scale_x(mcf_autopopup_configure.wndDecoSize)) rect->x = 0 -
                utils->get_scale_x(mcf_autopopup_configure.wndDecoSize);
            // restrict to 0
            if (rect->x < 0) rect->x = 0;
            if (rect->y < 0) rect->y = 0;
        } else {
            ret = FALSE;
        }
    }
    return ret;
}

/**
 * If the mouse was pressed on the button A and moved to B without releasing,
 * this function decides whether we should allow event transition, meaning
 * button A gets restored to its initial state and B gets pressed instead.
 */
mcfboolean
CMCFController::check_event_transition_enabled(const McfLayoutKeyConfigure *btnFrom, const McfLayoutKeyConfigure *btnTo)
{
    mcfboolean ret = FALSE;
    mcfbyte typeFrom = BUTTON_TYPE_NORMAL; /* To enable event transition even if no button is pressed currently */
    mcfbyte typeTo = MAX_BUTTON_TYPE;

    const mcfboolean TRANSITION_TABLE[MAX_BUTTON_TYPE][MAX_BUTTON_TYPE] = {
        //	NORMAL	GRAB	DRAG	MULTITAP	ROTATION	DIRECTION	UIITEM
        //	From : NORMAL
        {	TRUE,	0,		TRUE,	TRUE,		TRUE,		0,		0	},
        //	From : GRAB
        {	0	,	0,		0,		0	,		0	,		0,		0	},
        //	From : DRAG
        {	TRUE,	0,		TRUE,	TRUE,		TRUE,		0,		0	},
        //	From : MULTITAP
        {	TRUE,	0,		TRUE,	TRUE,		TRUE,		0,		0	},
        //	From : ROTATION
        {	TRUE,	0,		TRUE,	TRUE,		TRUE,		0,		0	},
        //	From : DIRECTION
        {	0	,	0,		0,		0	,		0	,		0,		0	},
        //	From : UIITEM
        {	0	,	0,		0,		0	,		0	,		0,		0	},
    };

    if (btnFrom) typeFrom = btnFrom->buttonType;
    if (btnTo) typeTo = btnTo->buttonType;

    mcf_assert_return_false(typeFrom >= 0 && typeFrom < MAX_BUTTON_TYPE);
    mcf_assert_return_false(typeTo >= 0 && typeTo < MAX_BUTTON_TYPE);

    if (typeFrom < MAX_BUTTON_TYPE && typeTo < MAX_BUTTON_TYPE) {
        ret = TRANSITION_TABLE[typeFrom][typeTo];
    }

    return ret;
}

MCFDebugMode
CMCFController::get_debug_mode()
{
#ifdef MCF_DEBUG_ON
    return mDebugMode;
#else
    return mDebugMode;
    return DEBUGMODE_DISABLED;
#endif
}

void
CMCFController::set_debug_mode(MCFDebugMode mode)
{
    CMCFEvents *events = CMCFEvents::get_instance();
    CMCFUtils *utils = CMCFUtils::get_instance();

    mDebugMode = mode;
    mDebugVariable = 0;

    if (mDebugMode == DEBUGMODE_AUTOTEST) {
        srand(time(NULL));
        if(events && utils) {
            events->create_timer(MCF_TIMER_AUTOTEST, MCF_AUTOTEST_TIMER_INITIAL_INTERVAL, 0, FALSE);
            utils->log("set_debug_mode\n");
        }
    }
}

void
CMCFController::disable_input_events(mcfboolean disabled)
{
    mInputEventsDisabled = disabled;
}
