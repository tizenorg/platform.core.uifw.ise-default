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



#include <stdio.h>
#include <stdlib.h>

#include "mcfcore.h"
#include "mcfdebug.h"
#include "mcfresourcecache.h"

using namespace mcf;

CMCFCore::CMCFCore()
{
    MCF_DEBUG();

    CMCFUIBuilder *builder = CMCFUIBuilder::get_instance();
    CMCFController *controller = CMCFController::get_instance();
    if(builder && controller) {
        builder->init(MCFWINDOW_INVALID);
        controller->init();
    }
}

CMCFCore::CMCFCore(mcfwindow parentWnd)
{
    MCF_DEBUG();

    CMCFUIBuilder *builder = CMCFUIBuilder::get_instance();
    CMCFController *controller = CMCFController::get_instance();
    if(builder && controller) {
        builder->init(parentWnd);
        controller->init();
    }
}

CMCFCore::~CMCFCore()
{
    MCF_DEBUG();
}

/**
 * Shows the MCF main window
 * For displaying the MCF UI, you should explicitly call this function after CMCFCore class is created
 */
mcfboolean
CMCFCore::run()
{
    MCF_DEBUG();
    show();
}

/**
 * Shows the MCF main window
 * For displaying the MCF UI, you should explicitly call this function after CMCFCore class is created
 */
void
CMCFCore::show()
{
    MCF_DEBUG();
    mcfboolean ret = FALSE;
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFController *controller = CMCFController::get_instance();
    if(windows && controller) {
        mcfwindow window = windows->get_base_window();
        controller->handle_engine_signal(MCF_SIG_SHOW);
        windows->show_window(window);
    }
}

/**
 * Hides the MCF main window
 * The real hide action does not work about base window because that is child of the active window
 */
void
CMCFCore::hide()
{
    MCF_DEBUG();
    mcfboolean ret = FALSE;
    CMCFController *controller = CMCFController::get_instance();
    if(controller) {
        controller->handle_engine_signal(MCF_SIG_HIDE);
    }
}


/**
 * Regists an event callback function
 * so that the user which uses MCF can recevies all events occuring in running
 */
void
CMCFCore::set_event_callback(IMCFEventCallback *callback)
{
    MCF_DEBUG();
    CMCFController *controller = CMCFController::get_instance();
    if (controller) {
        controller->set_event_callback(callback);
    }
}

/**
 * Sets the current input mode to the given mode
 * @Usage
 * gCore->set_input_mode(INPUT_MODE_SYMBOL);
 */
mcfboolean
CMCFCore::set_input_mode(mcf8 mode, mcfboolean clearEverything /* = FALSE */)
{
    MCF_DEBUG();
    mcfboolean ret = FALSE;
    CMCFController *controller = CMCFController::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    if (controller && windows) {
        if (clearEverything) {
            controller->clear_multikey_buffer();
        }
        ret = controller->process_input_mode_change(mode, !clearEverything);
        windows->update_window(windows->get_base_window());
    }
    return ret;
}

/**
 * Returns the current display mode
 */
mcf8
CMCFCore::get_display_mode()
{
    MCF_DEBUG();
    mcf8 ret = NOT_USED;
    CMCFContext *context = CMCFContext::get_instance();
    if(context) {
        ret = context->get_display();
    }
    return ret;
}

/**
 * Returns the current input mode
 */
mcf8
CMCFCore::get_input_mode()
{
    MCF_DEBUG();
    mcf8 ret = NOT_USED;
    CMCFContext *context = CMCFContext::get_instance();
    if(context) {
        ret = context->get_input_mode();
    }
    return ret;
}


/**
 * Returns a template private key properties using key properties of the given context
 */
void
CMCFCore::clone_keyproperties(McfPrivateKeyProperties* priv, mcfbyte inputModeIdx, mcfbyte layoutIdx, mcfbyte keyIdx)
{
    MCF_DEBUG();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    if(cache) {
        cache->clone_keyproperties(priv, inputModeIdx, layoutIdx, keyIdx);
    }
}

/**
 * Sets a private key to the current context
 *
 * @Usage
 *       McfPrivateKeyProperties privProperties;
 *       gCore->clone_keyproperties(&privProperties, INPUT_MODE_NUMBER, LYT_PORTRAIT_NOW_3x4, 0);
 *       // change
 *       gCore->set_private_key(&privProperties, TRUE);
 */
mcfint
CMCFCore::set_private_key(McfPrivateKeyProperties* properties, mcfboolean fRedraw)
{
    MCF_DEBUG();
    mcfint ret = NOT_USED;
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    if(windows && cache) {
        ret = cache->set_private_key(properties, fRedraw, windows->get_update_pending());
    }
    return ret;
}

/**
 * Sets a private key to the current context
 * The other properties except given parameters will keep to the orginal value.
 * @Usage
 * gCore->set_private_key(INPUT_MODE_NUMBER, LYT_PORTRAIT_NOW_3x4, 0, "private", 999, TRUE);
 *
 * @param fRedraw If true, it will redraw the current key
 */
mcfint
CMCFCore::set_private_key(mcfshort inputModeIdx, mcfbyte layoutIdx, mcfbyte keyIdx, mcfchar* label,
		                  mcfchar* imagelabel[MCF_BUTTON_STATE_MAX], mcfchar* imagebg[MCF_BUTTON_STATE_MAX],
		                  mcfulong keyEvent, mcfchar *keyValue, mcfboolean fRedraw)
{
    MCF_DEBUG();
    mcfint ret = NOT_USED;
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    if(windows && cache) {
        ret = cache->set_private_key(inputModeIdx, layoutIdx, keyIdx, label, imagelabel, imagebg,
                                     keyEvent, keyValue, fRedraw, windows->get_update_pending());
    }
    return ret;
}


/**
* Unsets a private key to the current context
*
* @Usage
*       McfPrivateKeyProperties privProperties;
*       gCore->clone_keyproperties(&privProperties, INPUT_MODE_NUMBER, LYT_PORTRAIT_NOW_3x4, 0);
*       mcfint pkindex = gCore->set_private_key(&privProperties, TRUE);
*       gCore->unset_private_key(pkindex);
*/
void
CMCFCore::unset_private_key(mcfint customID)
{
    MCF_DEBUG();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    if(cache) {
        cache->unset_private_key(customID);
    }
}

/**
* Sets the current theme
*/
mcfboolean
CMCFCore::set_cur_themename( const mcfchar *themename )
{
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    if(cache && windows) {
        cache->set_cur_themename(themename);
        windows->update_window(windows->get_base_window());
        int loop = 0;
        mcfwindow window;
        do {
            window = windows->get_nth_popup_window(loop);
            if (window) {
                windows->update_window(window);
            }
            loop++;
        } while (window);
    }
    return TRUE;
}

/**
* Find appropriate index of the key specified by customID
*/
mcfbyte
CMCFCore::find_keyidx_by_customid( mcfshort inputModeIdx, mcfbyte layoutIdx, mcfshort customID )
{
    mcfbyte ret = 0;
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    if(cache) {
        ret = cache->find_keyidx_by_customid(inputModeIdx, layoutIdx, customID);
    }
    return ret;
}

MCFShiftState
CMCFCore::get_shift_state()
{
    MCFShiftState ret = MCF_SHIFT_STATE_OFF;
    CMCFContext *context = CMCFContext::get_instance();
    if(context) {
        ret = context->get_shift_state();
    }
    return ret;
}

void
CMCFCore::set_shift_state( MCFShiftState state )
{
    CMCFContext *context = CMCFContext::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    if (context && windows) {
        if(state != context->get_shift_state()) {
            context->set_shift_state(state);
            windows->update_window(windows->get_base_window());
        }
    }
}

/**
 * This function will be called by the user which uses MCF when the context of the focus application is changed
 * ISE user should explicitly call this function when the context of application is changed.
 * For instance, focus-changed, application-changed,, and so on.
 * This function will call CMCFController to init the related variables.
 */
void
CMCFCore::notify_app_focus_changed()
{
    MCF_DEBUG();
    CMCFController *controller = CMCFController::get_instance();
    if(controller) {
        controller->handle_engine_signal(MCF_SIG_FOCUS_CHANGE);
    }
}

void
CMCFCore::reset_popup_timeout()
{
    MCF_DEBUG();
    CMCFEvents *events = CMCFEvents::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();

    if(events && windows) {
        events->destroy_timer(MCF_TIMER_POPUP_TIMEOUT);

        mcfbyte index = 0;
        mcfboolean timerset = FALSE;
        mcfwindow window = MCFWINDOW_INVALID;
        McfWindowContext *winctx = NULL;
        do {
            window = windows->get_nth_window_in_Z_order_list(index);
            winctx = windows->get_window_context(window, FALSE);
            if(winctx) {
                if(winctx->timeout != 0) {
                    events->create_timer(MCF_TIMER_POPUP_TIMEOUT, winctx->timeout, 0, TRUE);
                    timerset = TRUE;
                }
                index++;
            }
        } while (index < MAX_ZORDER_NUM && window != MCFWINDOW_INVALID && !timerset);
    }
}

void
CMCFCore::close_all_popups()
{
    MCF_DEBUG();
    CMCFWindows *windows = CMCFWindows::get_instance();
    if(windows) {
        windows->close_all_popups();
    }
}

/**
 * Returns a scale rate (see default screen resolution in mcfconfig.h file)
 */
mcffloat
CMCFCore::get_scale_rate()
{
    mcffloat ret = 0.0f;
    CMCFUtils *utils = CMCFUtils::get_instance();
    if(utils) {
        ret = utils->get_smallest_scale_rate();
    }
    return ret;
}

/**
 * Returns a calculated x value according to the current screen resolution
 */
mcf16
CMCFCore::get_scale_x(mcf16 x)
{
    mcf16 ret = 0;
    CMCFUtils *utils = CMCFUtils::get_instance();
    if(utils) {
        ret = utils->get_scale_x(x);
    }
    return ret;
}

/**
 * Returns a calculated y value according to the current screen resolution
 */
mcf16
CMCFCore::get_scale_y(mcf16 y)
{
    mcf16 ret = 0;
    CMCFUtils *utils = CMCFUtils::get_instance();
    if(utils) {
        ret = utils->get_scale_y(y);
    }
    return ret;
}

/**
 * Returns the mcf base window size
 */
void
CMCFCore::get_window_size(mcfint *width, mcfint *height)
{
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    if(cache && windows && width && height) {
        const McfLayout *layout  = cache->get_cur_layout(windows->get_base_window());
        if(layout) {
            *width = layout->width;
            *height = layout->height;
        }
    }
}

/**
* Returns the screen resolution
*/
void
CMCFCore::get_screen_resolution(mcfint *width, mcfint *height)
{
    CMCFUtils *utils = CMCFUtils::get_instance();
    if(utils && width && height) {
        utils->get_screen_resolution(width, height);
    }
}

/**
 * Returns the composed path with the given path
 */
mcfchar*
CMCFCore::get_composed_path(mcfchar* buf, int bufLength, const mcfchar* path)
{
    mcfchar* ret = NULL;
    CMCFUtils *utils = CMCFUtils::get_instance();
    if(utils) {
        ret = utils->get_composed_path(buf, bufLength, path);
    }
    return ret;
}


void
CMCFCore::set_debug_mode(MCFDebugMode mode)
{
    CMCFController *controller = CMCFController::get_instance();
    if(controller) {
        controller->set_debug_mode(mode);
    }
}

MCFDebugMode
CMCFCore::get_debug_mode()
{
    MCFDebugMode ret = DEBUGMODE_DISABLED;
    CMCFController *controller = CMCFController::get_instance();
    if(controller) {
        ret = controller->get_debug_mode();
    }
    return ret;
}

void
CMCFCore::set_update_pending(mcfboolean pend, mcfboolean autoUpdate)
{
    CMCFWindows *windows = CMCFWindows::get_instance();
    if(windows) {
        windows->set_update_pending(pend, autoUpdate);
    }
}

void
CMCFCore::enable_button(mcfbyte keyIdx)
{
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    if(cache) {
        cache->enable_button(keyIdx);
    }
}

void
CMCFCore::disable_button(mcfbyte keyIdx)
{
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    if(cache) {
        cache->disable_button(keyIdx);
    }
}

mcfint
CMCFCore::get_multi_touch_context_num()
{
    mcfboolean ret = FALSE;
    CMCFContext *context = CMCFContext::get_instance();
    if(context) {
        ret = context->get_multi_touch_context_num();
    }
    return ret;
}

mcfboolean
CMCFCore::get_multi_touch_event(mcfint seqorder, McfKeyEventDesc *desc)
{
    mcfboolean ret = FALSE;
    CMCFContext *context = CMCFContext::get_instance();
    if(context) {
        ret = context->get_multi_touch_event(seqorder, desc);
    }
    return ret;
}

mcfboolean
CMCFCore::set_longkey_duration(mcflong msc)
{
    mcfboolean ret = FALSE;
    CMCFController *controller = CMCFController::get_instance();
    if(controller) {
        ret = controller->set_longkey_duration(msc);
    }
    return ret;
}

mcfboolean
CMCFCore::set_longkey_cancel_dist(mcfshort dist)
{
    mcfboolean ret = FALSE;
    CMCFController *controller = CMCFController::get_instance();
    if(controller) {
        ret = controller->set_longkey_cancel_dist(dist);
    }
    return ret;
}

mcfboolean
CMCFCore::set_repeatkey_duration(mcflong msc)
{
    mcfboolean ret = FALSE;
    CMCFController *controller = CMCFController::get_instance();
    if(controller) {
        ret = controller->set_repeatkey_duration(msc);
    }
    return ret;
}

mcfboolean
CMCFCore::set_short_longkey_duration(mcflong msc)
{
    mcfboolean ret = FALSE;
    CMCFController *controller = CMCFController::get_instance();
    if(controller) {
        ret = controller->set_short_longkey_duration(msc);
    }
    return ret;
}

mcfboolean
CMCFCore::set_button_delay_duration(mcflong msc)
{
    mcfboolean ret = FALSE;
    CMCFController *controller = CMCFController::get_instance();
    if(controller) {
        ret = controller->set_button_delay_duration(msc);
    }
    return ret;
}

void
CMCFCore::disable_input_events(mcfboolean disabled)
{
    CMCFController *controller = CMCFController::get_instance();
    if(controller) {
        controller->disable_input_events(disabled);
    }
}

mcfboolean
CMCFCore::set_cur_sublayout_id(mcfint id)
{
    mcfboolean ret = FALSE;
    CMCFContext *context = CMCFContext::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    if(context && windows) {
        if(id != context->get_cur_sublayout_id()) {
            ret = context->set_cur_sublayout_id(id);
            windows->update_window(windows->get_base_window());
        }
    }
    return ret;
}

mcfint
CMCFCore::get_cur_sublayout_id()
{
    mcfint ret = SUBLAYOUTID_NONE;
    CMCFContext *context = CMCFContext::get_instance();
    if(context) {
        ret = context->get_cur_sublayout_id();
    }
    return ret;
}

