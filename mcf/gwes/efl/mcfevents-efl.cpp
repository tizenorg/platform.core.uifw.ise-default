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


#include "mcfevents-efl.h"
#include "mcfdebug.h"
#include "mcfcontroller.h"
#include "mcfgraphics.h"
#include "mcfuibuilder.h"
#include "mcferroradjustment.h"
#include "mcfresource.h"
#include "mcfresourcecache.h"

#include <Elementary.h>
#include <Ecore_X.h>

using namespace mcf;

mcfboolean bMousePressed = FALSE; /* Checks whether mouse is pressed or not */
mcfboolean bSkipMouseEventsUntilPressed = FALSE; /*Explicitly filter out any mouse events until mouse press occurs */
mcfwindow pressedWindow = MCFWINDOW_INVALID;

#define MIN_XY_DIFF 14
static mcfint sLatestXPos;
static mcfint sLatestYPos;

Eina_Bool mouse_press(void *data, int type, void *event_info);
Eina_Bool mouse_move (void *data, int type, void *event_info);
Eina_Bool mouse_release (void *data, int type, void *event_info);

/**
 * Constructor
 */
CMCFEventsImplEfl::CMCFEventsImplEfl()
{
    MCF_DEBUG();
    /* Initializes all window resources */
    mMouseDownHandler = ecore_event_handler_add(ECORE_EVENT_MOUSE_BUTTON_DOWN, mouse_press, NULL);
    mMouseMoveHandler = ecore_event_handler_add(ECORE_EVENT_MOUSE_MOVE, mouse_move, NULL);
    mMouseUpHandler = ecore_event_handler_add(ECORE_EVENT_MOUSE_BUTTON_UP, mouse_release, NULL);
}

/**
 * De-constructor
 */
CMCFEventsImplEfl::~CMCFEventsImplEfl()
{
    MCF_DEBUG();

    if(mMouseDownHandler) {
    	ecore_event_handler_del(mMouseDownHandler);
    }
    if(mMouseMoveHandler) {
    	ecore_event_handler_del(mMouseMoveHandler);
    }
    if(mMouseUpHandler) {
    	ecore_event_handler_del(mMouseUpHandler);
    }
}

mcfboolean get_window_rect(const mcfwindow window, McfRectangle *rect)
{
    MCF_DEBUG();
    mcfboolean bRet = FALSE;
    CMCFUtils *utils = CMCFUtils::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFContext *context = CMCFContext::get_instance();
    if(windows && context && utils && rect) {
        McfWindowContext *winctx = windows->get_window_context(window);
        mcfint scr_w, scr_h;
        /* get window size */
        utils->get_screen_resolution(&scr_w, &scr_h);
        if(winctx) {
            switch(context->get_rotation_degree()) {
                case 90: {
                        rect->height = winctx->width;
                        rect->width = winctx->height;
                        rect->y = scr_h - rect->height - winctx->x;
                        rect->x = winctx->y;
                    }
                    break;
                case 180: {
                        rect->width = winctx->width;
                        rect->height = winctx->height;
                        rect->x = scr_w - winctx->x - rect->width;
                        rect->y = scr_h - winctx->y - rect->height;
                    }
                    break;
                case 270: {
                        rect->height = winctx->width;
                        rect->width = winctx->height;
                        rect->y = winctx->x;
                        rect->x= scr_w - winctx->y - rect->width;
                    }
                    break;
                default: {
                        rect->x = winctx->x;
                        rect->y = winctx->y;
                        rect->width = winctx->width;
                        rect->height = winctx->height;
                    }
                    break;
            }
            bRet = TRUE;
        } else {
            rect->x = rect->y = rect->width = rect->height = 0;
        }
    }
    return bRet;
}

/**  Here x and y contains "actual" x and y position relative to portrait root window,
     and winctx->width,height contains the window's orientation dependant width and height */
McfPoint get_rotated_local_coords(mcfint x, mcfint y, mcfint degree, McfRectangle *rect)
{
    McfPoint ret = {0, 0};

    if(rect) {
        switch(degree) {
            case 90: {
                    ret.x = (rect->y + rect->width) - y;
                    ret.y = x - rect->x;
                }
                break;
            case 180: {
                    ret.x = (rect->x + rect->width) - x;
                    ret.y = (rect->y + rect->height) - y;
                }
                break;
            case 270: {
                    ret.x = y - rect->y;
                    ret.y = (rect->x + rect->height) - x;
                }
                break;
            default: {
                    ret.x = x - rect->x;
                    ret.y = y - rect->y;
                }
                break;
        }
    }
    return ret;
}

Eina_Bool mouse_press(void *data, int type, void *event_info)
{
    MCF_DEBUG();

    CMCFController *controller = CMCFController::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFContext *context = CMCFContext::get_instance();

    Ecore_Event_Mouse_Button *ev = (Ecore_Event_Mouse_Button*)event_info;

    if(controller && windows && context && ev) {
        mcfbyte index = 0;
        mcfboolean processed = FALSE;
        mcfwindow window = MCFWINDOW_INVALID;

        mcfwindow evwin = (mcfwindow)(ev->window);
        mcfboolean bMCFWindow = FALSE;
        if(elm_win_xwindow_get(static_cast<Evas_Object*>(windows->get_base_window())) == ev->window) {
            bMCFWindow = TRUE;
        } else {
            do {
                window = windows->get_nth_window_in_Z_order_list(index);
                McfWindowContext *winctx = windows->get_window_context(window);
                if(winctx) {
                    if(winctx->isVirtual) {
                        bMCFWindow  = TRUE;
                    } else if(elm_win_xwindow_get(static_cast<Evas_Object*>(window)) == ev->window) {
                        bMCFWindow = TRUE;
                    }
                }
                index++;
            } while (index < MAX_ZORDER_NUM && window != MCFWINDOW_INVALID);
            index = 0;
        }
        if(!bMCFWindow) return TRUE;

        McfRectangle rect = {0};
        do {
            window = windows->get_nth_window_in_Z_order_list(index);
            if(window) {
                // Update the position of the target window
                windows->get_window_context(window, TRUE);
                if(get_window_rect(window, &rect)) {
                    int adjustx = ev->root.x;
                    int adjusty = ev->root.y;

                    MCFDisplay currDisplay = context->get_display();
                    CMCFErrorAdjustment *adjustment = CMCFErrorAdjustment::get_instance();
                    if(adjustment && mcf_check_arrindex(currDisplay, MCFDISPLAY_MAX)) {
                        adjustment->apply_touch_offset(mcf_default_configure.touchOffsetLevel[currDisplay],
                        		                       &adjustx, &adjusty);
                    }

                    mcfint winwidth = rect.width;
                    mcfint winheight = rect.height;
                    if(context->get_display() != MCFDISPLAY_PORTRAIT) {
                        rect.height = winwidth;
                        rect.width = winheight;
                    }

                    mcfboolean processEvt = FALSE;
                    if ((adjustx >= rect.x && adjustx <= (rect.x + winwidth)) &&
                        (adjusty >= rect.y && adjusty <= (rect.y + winheight))) {
                            processEvt = TRUE;
                    }
                    if(processEvt)
                    {
                        // Now convert the global coordination to appropriate local coordination
                        McfPoint coords = get_rotated_local_coords(ev->root.x, ev->root.y, context->get_rotation_degree(), &rect);
                        controller->mouse_press(window, coords.x, coords.y, ev->multi.device);
                        bMousePressed = TRUE;
                        processed = TRUE;
                        pressedWindow = window;
                    }
                }
            }

            index++;
        } while (index < MAX_ZORDER_NUM && window != MCFWINDOW_INVALID && !processed);

        if (!processed) {
            window = pressedWindow;
            if(get_window_rect(window, &rect)) {
                if(context->get_rotation_degree() == 90 || context->get_rotation_degree() == 270) {
                    mcfint temp = rect.width;
                    rect.width = rect.height;
                    rect.height = temp;
                }

                // Now convert the global coordination to appropriate local coordination
                McfPoint coords = get_rotated_local_coords(ev->root.x, ev->root.y, context->get_rotation_degree(), &rect);
                controller->mouse_press(window, coords.x, coords.y, ev->multi.device);
                bMousePressed = TRUE;
                processed = TRUE;
            }
        }
    }

    return TRUE;
}

Eina_Bool mouse_release (void *data, int type, void *event_info)
{
    MCF_DEBUG();

    CMCFController *controller = CMCFController::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFContext *context = CMCFContext::get_instance();

    Ecore_Event_Mouse_Button *ev = (Ecore_Event_Mouse_Button*)event_info;

    if(controller && windows && context && ev) {
        mcfbyte index = 0;
        mcfboolean processed = FALSE;
        mcfwindow window = MCFWINDOW_INVALID;
        McfRectangle rect;
        mcfboolean dimwinevent = FALSE;
        McfWindowContext *dimctx = windows->get_window_context(windows->get_dim_window());
        if(dimctx) {
            if(!(dimctx->isVirtual)) {
        if(elm_win_xwindow_get(static_cast<Evas_Object*>(windows->get_dim_window())) == ev->window) {
                    dimwinevent = TRUE;
                }
            }
        }
        if(dimwinevent) {
            controller->mouse_press(windows->get_dim_window(), ev->root.x, ev->root.y, ev->multi.device);
        } else {
            do {
                window = windows->get_nth_window_in_Z_order_list(index);
                if(window) {
                    if(get_window_rect(window, &rect)) {
                        int adjustx = ev->root.x;
                        int adjusty = ev->root.y;
                        MCFDisplay currDisplay = context->get_display();
                        CMCFErrorAdjustment *adjustment = CMCFErrorAdjustment::get_instance();
                        if(adjustment && mcf_check_arrindex(currDisplay, MCFDISPLAY_MAX)) {
                            adjustment->apply_touch_offset(mcf_default_configure.touchOffsetLevel[currDisplay],
                            		                       &adjustx, &adjusty);
                        }

                        mcfint winwidth = rect.width;
                        mcfint winheight = rect.height;
                        if(context->get_display() != MCFDISPLAY_PORTRAIT) {
                            rect.height = winwidth;
                            rect.width = winheight;
                        }

                        mcfboolean processEvt = FALSE;
                        if ((adjustx >= rect.x && adjustx <= (rect.x + winwidth)) &&
                            (adjusty >= rect.y && adjusty <= (rect.y + winheight))) {
                                processEvt = TRUE;
                        }
                        if(processEvt)
                        {
                            /* Now convert the global coordination to appropriate local coordination */
                            McfPoint coords = get_rotated_local_coords(ev->root.x, ev->root.y, context->get_rotation_degree(), &rect);
                            controller->mouse_release(window, coords.x, coords.y, ev->multi.device);
                            processed = TRUE;
                        }
                    }
                }

                index++;
            } while (index < MAX_ZORDER_NUM && window != MCFWINDOW_INVALID && !processed);
        }

        if (!processed) {
            window = pressedWindow;
            if(get_window_rect(window, &rect)) {
                if(context->get_rotation_degree() == 90 || context->get_rotation_degree() == 270) {
                    mcfint temp = rect.width;
                    rect.width = rect.height;
                    rect.height = temp;
                }

                /* Now convert the global coordination to appropriate local coordination */
                McfPoint coords = get_rotated_local_coords(ev->root.x, ev->root.y, context->get_rotation_degree(), &rect);
                controller->mouse_release(window, coords.x, coords.y, ev->multi.device);
                processed = TRUE;
            }
        }

        bMousePressed = FALSE;
    }

    return TRUE;
}

Eina_Bool mouse_move (void *data, int type, void *event_info)
{
    MCF_DEBUG();

    CMCFController *controller = CMCFController::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFContext *context = CMCFContext::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();

    Ecore_Event_Mouse_Move *ev = (Ecore_Event_Mouse_Move*)event_info;


    if(controller && windows && context && cache && ev) {
        mcfbyte index = 0;
        mcfboolean processed = FALSE;
        mcfwindow window = MCFWINDOW_INVALID;
        McfRectangle rect;

        if(context->get_cur_pressed_window(ev->multi.device) != MCFWINDOW_INVALID &&
           get_window_rect(context->get_cur_pressed_window(ev->multi.device), &rect)) {
            mcfint winwidth = rect.width;
            mcfint winheight = rect.height;
            if(context->get_display() != MCFDISPLAY_PORTRAIT) {
                rect.height = winwidth;
                rect.width = winheight;
            }
            McfPoint coords = get_rotated_local_coords(ev->root.x, ev->root.y, context->get_rotation_degree(), &rect);

            controller->mouse_move(context->get_cur_pressed_window(ev->multi.device), coords.x, coords.y, ev->multi.device);
            processed = TRUE;
        } else {
            do {
                window = windows->get_nth_window_in_Z_order_list(index);
                if(window) {
                    if(get_window_rect(window, &rect)) {
                        int adjustx = ev->root.x;
                        int adjusty = ev->root.y;
                        MCFDisplay currDisplay = context->get_display();
                        CMCFErrorAdjustment *adjustment = CMCFErrorAdjustment::get_instance();
                        if(adjustment && mcf_check_arrindex(currDisplay, MCFDISPLAY_MAX)) {
                            adjustment->apply_touch_offset(mcf_default_configure.touchOffsetLevel[currDisplay],
                            		                       &adjustx, &adjusty);
                        }

                        mcfint winwidth = rect.width;
                        mcfint winheight = rect.height;
                        if(context->get_display() != MCFDISPLAY_PORTRAIT) {
                            rect.height = winwidth;
                            rect.width = winheight;
                        }

                        mcfboolean processEvt = FALSE;
                        if ((adjustx >= rect.x && adjustx <= (rect.x + winwidth)) &&
                            (adjusty >= rect.y && adjusty <= (rect.y + winheight))) {
                                processEvt = TRUE;
                        }
                        /* Process this event regardless of the coordination if the top window has the POPUP_GRAB layout style */
                        if(index == MCF_WINDOW_Z_TOP) {
                            const McfLayout *layout = cache->get_cur_layout(window);
                            if(layout) {
                                if(layout->style == LAYOUT_STYLE_POPUP_GRAB) {
                                    processEvt = TRUE;
                                }
                            }
                        }
                        if(processEvt)
                        {
                            /* Now convert the global coordination to appropriate local coordination */
                            McfPoint coords = get_rotated_local_coords(ev->root.x, ev->root.y,
                            		                                   context->get_rotation_degree(), &rect);

                            controller->mouse_move(window, coords.x, coords.y, ev->multi.device);
                            processed = TRUE;
                        }

                        index++;
                    }
                }
            } while (index < MAX_ZORDER_NUM && window != MCFWINDOW_INVALID && !processed);
        }

        if (!processed) {
            window = pressedWindow;
            if(get_window_rect(window, &rect)) {
                /* Now convert the global coordination to appropriate local coordination */
                McfPoint coords = get_rotated_local_coords(ev->root.x, ev->root.y, context->get_rotation_degree(), &rect);
                controller->mouse_move(window, coords.x, coords.y, ev->multi.device);
                processed = TRUE;
            }
        }
    }
    return TRUE;
}

/**
 * Regists a event callback func to given window.
 * In this function, it should call serveral event functions of CMCFController class whenever an event has occured
 * The below list shows what event function should be called.
 * - mouse_press (when the user presses mouse button)
 * - mouse_release (when the user releases mouse button)
 * - mouse_move (when the user drags mouse button)
 * - show_base_layout (when the expost event has occured)
 */
void
CMCFEventsImplEfl::connect_window_events(const mcfwindow wnd, const mcfint evt)
{
    MCF_DEBUG();

}

Eina_Bool timer_event(void *data)
{
    MCF_DEBUG();
    mcf32 sendData = (mcf32)data;
    CMCFController *controller;
    controller = CMCFController::get_instance();
    if (controller) {
        return controller->timer_event(sendData);
    }
    return TRUE;
}

/**
 * Creates a timer
 * In this function, it should call timer_event of CMCFController class
 */
void
CMCFEventsImplEfl::create_timer(const mcf16 id, const mcf32 interval, mcf16 value, mcfboolean addToMap)
{
    MCF_DEBUG();
    mcfint data = MCF_MAKELONG(id, value);
    Ecore_Timer *pTimer = ecore_timer_add((double)interval / 1000.0, timer_event, (void*)data);
    if (pTimer) {
        if (addToMap) {
            idMap[id] = pTimer;
        }
    }
}

/**
 * Destroys the given ID's timer
 */
void
CMCFEventsImplEfl::destroy_timer(const mcf32 id)
{
    MCF_DEBUG();
        std::map<int, Ecore_Timer*>::iterator idx = idMap.find(id);
        if(idx != idMap.end()) {
            ecore_timer_del((*idx).second);
            idMap.erase((*idx).first);
        }
}

/**
 * Destroys all of created timer
 */
void
CMCFEventsImplEfl::destroy_all_timer()
{
    MCF_DEBUG();
    for ( std::map<int, Ecore_Timer*>::iterator idx = idMap.begin(); idx != idMap.end(); ++idx) {
        ecore_timer_del((*idx).second);
    }
    idMap.clear();
}

void
CMCFEventsImplEfl::generate_mouse_event(MCFMouseEvent type, mcf16 x, mcf16 y)
{
    CMCFWindows *windows = CMCFWindows::get_instance();
    McfWindowContext *winctx = NULL;
    if(windows) {
        switch(type) {
            case MCF_MOUSE_EVENT_PRESS: {
                Ecore_Event_Mouse_Button evt;
                evt.window = elm_win_xwindow_get(static_cast<Evas_Object*>(windows->get_base_window()));
                winctx = windows->get_window_context(windows->get_base_window(), FALSE);
                if(winctx) {
                    evt.root.x = x + winctx->x;
                    evt.root.y = y + winctx->y;
                    mouse_press(NULL, 0, &evt);
                }
            }
            break;
            case MCF_MOUSE_EVENT_RELEASE: {
                Ecore_Event_Mouse_Button evt;
                evt.window = elm_win_xwindow_get(static_cast<Evas_Object*>(windows->get_base_window()));
                winctx = windows->get_window_context(windows->get_base_window(), FALSE);
                if(winctx) {
                    evt.root.x = x + winctx->x;
                    evt.root.y = y + winctx->y;
                    mouse_release(NULL, 0, &evt);
                }
            }
            break;
            case MCF_MOUSE_EVENT_MOVE: {
                Ecore_Event_Mouse_Move evt;
                evt.window = elm_win_xwindow_get(static_cast<Evas_Object*>(windows->get_base_window()));
                winctx = windows->get_window_context(windows->get_base_window(), FALSE);
                if(winctx) {
                    evt.root.x = x + winctx->x;
                    evt.root.y = y + winctx->y;
                    mouse_move(NULL, 0, &evt);
                }
            }
            break;
        }
    }
}
