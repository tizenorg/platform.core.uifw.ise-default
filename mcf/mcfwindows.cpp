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


#include "mcfwindows.h"
#ifdef  __WIN32__
#include "mcfwindows-win32.h"
#elif defined(__EFL__)
#include "mcfwindows-efl.h"
#else
#include "mcfwindows-gtk.h"
#endif
#include "mcfdebug.h"
#include "mcfevents.h"
#include "mcfresourcecache.h"
#include "mcfcontroller.h"
#include "mcfactionstate.h"

using namespace mcf;

CMCFWindows* CMCFWindows::sInstance = NULL; /* For singleton */
mcfwindow ctx_window;

CMCFWindows::CMCFWindows()
{
    MCF_DEBUG();

    int loop;

    pImp_ = NULL;

    memset(&mBaseWinCtx, 0x00, sizeof(McfWindowContext));
    memset(&mDimWinCtx, 0x00, sizeof(McfWindowContext));
    memset(mPopupWinCtx, 0x00, sizeof(McfWindowContext) * MAX_POPUP_WINDOW);

    mPendingUpdate = FALSE;

    for (loop = 0;loop < MAX_ZORDER_NUM;loop++) {
        mZOrderList[loop] = MCFWINDOW_INVALID;
    }
}

CMCFWindows::~CMCFWindows()
{
    MCF_DEBUG();
    if (MCFWINDOW_INVALID != mBaseWinCtx.window) {
        get_mcf_windows_impl()->destroy_window(mBaseWinCtx.window);
    }

    if (MCFWINDOW_INVALID != mDimWinCtx.window) {
        get_mcf_windows_impl()->destroy_window(mDimWinCtx.window);
    }

    for (int loop = 0;loop < MAX_POPUP_WINDOW;loop++) {
        if (mPopupWinCtx[loop].window != MCFWINDOW_INVALID) {
            if (!mPopupWinCtx[loop].isVirtual) {
                get_mcf_windows_impl()->destroy_window(mPopupWinCtx[loop].window);
            }
        }
    }

}

CMCFWindowsImpl*
CMCFWindows::get_mcf_windows_impl()
{
    if (pImp_ == 0) {
#ifdef  __WIN32__
        pImp_ = new CMCFWindowsImplWin32;
#elif defined(__EFL__)
        pImp_ = new CMCFWindowsImplEfl;
#else
        pImp_ = new CMCFWindowsImplGtk;
#endif
    }
    return pImp_;
}

CMCFWindows*
CMCFWindows::get_instance()
{
    if (!sInstance) {
        sInstance = new CMCFWindows();
    }
    return (CMCFWindows*)sInstance;
}

mcfwindow CMCFWindows::open_popup(const mcfwindow parentWnd, const mcfint parentKey, const mcf16 x, const mcf16 y, const mcf16 width, const mcf16 height, mcfshort layout, MCFPopupType popuptype, mcfboolean isVirtual, mcfboolean useDimWindow, mcfint imgOffsetx, mcfint imgOffsety, mcfint timeout)
{
    mcfwindow window = MCFWINDOW_INVALID;

    CMCFEvents *events = CMCFEvents::get_instance();
    CMCFActionState *state = CMCFActionState::get_instance();
    CMCFController *controller = CMCFController::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    CMCFContext *context = CMCFContext::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFUtils *utils = CMCFUtils::get_instance();

    if(events && state && controller && cache && context && windows && utils) {
        for (int loop = 0;loop < MAX_POPUP_WINDOW;loop++) {
            if (mPopupWinCtx[loop].window != MCFWINDOW_INVALID) {
                if (mPopupWinCtx[loop].layout == layout) return MCFWINDOW_INVALID;
            }
        }

        mcfwindow window = create_window(parentWnd, parentKey, x, y, width, height, layout, popuptype,
        		                         isVirtual, imgOffsetx, imgOffsety, timeout);
        events->destroy_timer(MCF_TIMER_POPUP_TIMEOUT);
        if(timeout > 0) {
            events->create_timer(MCF_TIMER_POPUP_TIMEOUT, timeout, layout);
        }

    printf("create window (%p) x: %d, y:%d, width:%d, height:%d , layout:%d, popuptype:%d\n",window, x, y,
    	   width, height, layout, popuptype);

    events->connect_window_events(window, MCF_EVENT_MOUSE | MCF_EVENT_EXPOSE);
    controller->handle_engine_signal(MCF_SIG_POPUP_SHOW, window);

    /* Shows the dim window if it uses the dimwindow */
    if (useDimWindow) {
        mcfwindow dimWindow = get_dim_window();

        /* Currently, get_window_rect does not work normally (need to check X). So I have commented it*/
        McfRectangle rect;
        get_window_rect(get_base_window(), &rect);
        resize_window(dimWindow, rect.width, rect.height);
        move_window(dimWindow, rect.x, rect.y);
        events->connect_window_events(dimWindow, MCF_EVENT_MOUSE);
#ifdef NO_SOCKETPLUG
        /*If we use transient_for them the ISE will occure some crash. It needs to check X11*/
            set_parent(parentWnd, dimWindow);
            McfWindowContext *dimctx = get_window_context(get_dim_window());
            if(dimctx) {
                if(dimctx->isVirtual) {
                    set_parent(parentWnd, window);
                } else {
                    set_parent(dimWindow, window);
                }
            }
#else
#endif
        show_window(dimWindow);
    } else {
#ifdef NO_SOCKETPLUG
        /*If we use transient_for them the ISE will occure some crash. It needs to check X11*/
        set_parent(parentWnd, window);
#else
#endif
    }
    show_window(window);

    push_window_in_Z_order_list(window);

    state->set_cur_action_state(ACTION_STATE_POPUP_INIT);

        const McfLayout *layout = cache->get_cur_layout(window);
        if(layout) {
            /* If the newly opened popup window has POPUP_GRAB style, lets press the nearest button on the new window */
            if(layout->style == LAYOUT_STYLE_POPUP_GRAB) {
                mcfwindow pressedWin = context->get_cur_pressed_window(context->get_last_touch_device_id());
                mcfbyte pressedKey = context->get_cur_pressed_key(context->get_last_touch_device_id());

                const McfLayoutKeyCoordination* coordination = cache->get_cur_layout_key_coordination(pressedWin, pressedKey);
                McfButtonContext *btncontext = cache->get_cur_button_context(pressedWin, pressedKey);

                mcfwindow move_window = context->get_cur_move_window(context->get_last_touch_device_id());
                McfPoint move_point = context->get_cur_move_point(context->get_last_touch_device_id());
                McfWindowContext *move_ctx = windows->get_window_context(move_window);
                McfWindowContext *popup_ctx = windows->get_window_context(window);
                if(move_ctx && popup_ctx) {
                    move_point.x = (move_ctx->x - popup_ctx->x) + move_point.x;
                    move_point.y = (move_ctx->y - popup_ctx->y) + move_point.y;
                }

                /* Find the nearest button on the autopopup window */
                mcfboolean ended = FALSE;
                mcffloat min_dist = (float)((mcfu32)(-1));
                mcfint min_dist_index = NOT_USED;
                for (mcfint loop = 0;loop < MAX_KEY && !ended;loop++) {
                    McfButtonContext *popup_btncontext = cache->get_cur_button_context(window, loop);
                    const McfLayoutKeyConfigure *popup_configure = cache->get_cur_layout_key_configure(window, loop);
                    if (popup_btncontext && popup_configure) {
                        if (!(popup_btncontext->used)) {
                            ended = TRUE;
                        } else if (popup_btncontext->state != BUTTON_STATE_DISABLED &&
                            popup_configure->buttonType != BUTTON_TYPE_UIITEM) {
                                const McfLayoutKeyCoordination *popup_coordination =
                                    cache->get_cur_layout_key_coordination(window, loop);
                                if(popup_coordination) {
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
                }
                /* When we found the nearest button, make it pressed */
                if(min_dist_index != NOT_USED) {
                    const McfLayoutKeyCoordination *popup_coordination =
                        cache->get_cur_layout_key_coordination(window, min_dist_index);
                    if(popup_coordination) {
                        mcfint x = popup_coordination->x + (popup_coordination->width / 2);
                        mcfint y = popup_coordination->y + (popup_coordination->height / 2);
                        controller->mouse_press(window, x, y, context->get_last_touch_device_id());
                    }
                }

                windows->update_window(window, coordination->x, coordination->y, coordination->width, coordination->height);
            }
        }
    }

    return window;
}

mcfwindow CMCFWindows::open_context_popup(const mcfwindow parentWnd)
{
    CMCFEvents *events = CMCFEvents::get_instance();
    CMCFActionState *state = CMCFActionState::get_instance();
    CMCFController *controller = CMCFController::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    CMCFContext *context = CMCFContext::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFUtils *utils = CMCFUtils::get_instance();
    CMCFWindowsImpl* impl = get_mcf_windows_impl();
    ctx_window = impl->create_context_popup_window(parentWnd);
    events->connect_window_events(ctx_window, MCF_EVENT_MOUSE | MCF_EVENT_EXPOSE);
    controller->handle_engine_signal(MCF_SIG_POPUP_SHOW, ctx_window);
    set_parent(parentWnd, ctx_window);
    show_window(ctx_window);
    state->set_cur_action_state(ACTION_STATE_POPUP_INIT);
    return ctx_window;
}

Evas_Object* CMCFWindows::get_context_popup()
{
    CMCFWindowsImpl* impl = get_mcf_windows_impl();
    return impl->get_context_popup_window();
}

void CMCFWindows::destroy_context_popup()
{
    CMCFWindowsImpl* impl = get_mcf_windows_impl();
    impl->destroy_context_popup_window();
    return;
}

bool CMCFWindows::close_popup(mcfwindow window)
{
    printf("close_popup window (%p) \n",window);

    pop_window_in_Z_order_list(window);

    hide_window(get_dim_window());
    hide_window(window);
    return destroy_window(window);
}

bool CMCFWindows::close_all_popups(mcfwindow skipwindow /* = MCFWINDOW_INVALID */)
{
    mcfboolean allClosed = TRUE;
    /* Close all the popup windows except the targetWindow */
    int loop = 0;
    mcfwindow window;
    do {
        window = get_nth_popup_window(loop);
        if (window) {
            if (window != skipwindow) {
                close_popup(window);
            } else {
                allClosed = FALSE;
            }
        }
        loop++;
    } while (window);

    /* If there is a popup still opened, don't destroy POPUP_TIMEOUT timer */
    return allClosed;
}

mcfwindow
CMCFWindows::create_base_window(const mcfwindow parentWnd, const mcf16 width, const mcf16 height)
{
    MCF_DEBUG();

    mBaseWinCtx.hidden = TRUE;
    mBaseWinCtx.width = width;
    mBaseWinCtx.height = height;
    mBaseWinCtx.isVirtual = FALSE;
    mBaseWinCtx.popuptype = POPUP_TYPE_NONE;
    mBaseWinCtx.parentWnd = parentWnd;
    mBaseWinCtx.x = mBaseWinCtx.y = 0;
    mBaseWinCtx.etcInfoPtr = NULL;
    mBaseWinCtx.window = get_mcf_windows_impl()->create_base_window(parentWnd, &mBaseWinCtx, width, height);

    push_window_in_Z_order_list(mBaseWinCtx.window);

    return mBaseWinCtx.window;
}

/**
 * Creates a new top-level window
 *
 * @Code
 *  CMCFGwes* gwes = CMCFGwes::get_instance();
 *  mcfwindow popupWindow = gwes->mWnd->create_window(window, 100, 500, 200, 100, 4, POPUP_TYPE_BTN_RELEASE_POPUP, FALSE);
 *  if(popupWindow != NULL){
 *     gwes->mEvt->connect_window_events(popupWindow, MCF_EVENT_MOUSE | MCF_EVENT_EXPOSE);
 *      cache->recompute_layout(popupWindow);
 *      gwes->mWnd->show_window(popupWindow);
 *  }
 */
mcfwindow
CMCFWindows::create_window(const mcfwindow parentWnd, const mcfint parentKey, const mcf16 x, const mcf16 y, const mcf16 width,
		                   const mcf16 height, mcfshort layout, MCFPopupType popuptype, mcfboolean isVirtual, mcfint imgOffsetx,
		                   mcfint imgOffsety, mcfint timeout)
{
    MCF_DEBUG();

    CMCFWindowsImpl* impl = get_mcf_windows_impl();
    mcfwindow window = MCFWINDOW_INVALID;

    if(impl) {
        for (int loop = 0;loop < MAX_POPUP_WINDOW;loop++) {
            if (mPopupWinCtx[loop].window == MCFWINDOW_INVALID) {
                mPopupWinCtx[loop].hidden = TRUE;
                mPopupWinCtx[loop].parentWnd = parentWnd;
                mPopupWinCtx[loop].parentKey = parentKey;
                mPopupWinCtx[loop].etcInfoPtr = NULL;
                mPopupWinCtx[loop].layout = layout;
                mPopupWinCtx[loop].popuptype = popuptype;
                mPopupWinCtx[loop].isVirtual = isVirtual;

                mPopupWinCtx[loop].x = x;
                mPopupWinCtx[loop].y = y;
                mPopupWinCtx[loop].width = width;
                mPopupWinCtx[loop].height = height;

                mPopupWinCtx[loop].imgOffsetx = imgOffsetx;
                mPopupWinCtx[loop].imgOffsety = imgOffsety;

                mPopupWinCtx[loop].timeout = timeout;

                if (!isVirtual) {
                    window = impl->create_window(parentWnd, &(mPopupWinCtx[loop]), width, height);
                } else {
                    window = reinterpret_cast<mcfwindow>(loop + 1);
                }
                if (window) {
                    mPopupWinCtx[loop].window = window;
                }

                if (!mPopupWinCtx[loop].isVirtual) {
                    impl->move_window(window, x, y);
                }
                break;
            }
        }

        if (window == NULL) {
            printf("Failed to create a new window. The size of window buffer has exeeded.\n");
        }
    }
    return window;
}

mcfwindow
CMCFWindows::create_dim_window(const mcfwindow parentWnd, McfWindowContext *winCtx, const mcf16 width, const mcf16 height)
{
    MCF_DEBUG();

    CMCFWindowsImpl* impl = get_mcf_windows_impl();
    mcfwindow window = MCFWINDOW_INVALID;

    if(impl) {
        if (mDimWinCtx.window == NULL) {
            mDimWinCtx.hidden = TRUE;
            if(mcf_dim_window_configure.fUseWindow) {
                window = impl->create_dim_window(parentWnd, &mDimWinCtx, width, height);
            } else {
                window = reinterpret_cast<mcfwindow>(MCFWINDOW_VIRTUAL_DIM);
                mDimWinCtx.isVirtual = TRUE;
            }
            if (window) {
                mDimWinCtx.window = window;
            }
        } else {
            window = mDimWinCtx.window;
        }

        if (window == NULL) {
            printf("Failed to create a new window. The size of window buffer has exeeded.\n");
        }
    }

    return window;
}

mcfwindow
CMCFWindows::get_dim_window()
{
    MCF_DEBUG();
    return mDimWinCtx.window;
}

bool
CMCFWindows::destroy_window(mcfwindow window)
{
    MCF_DEBUG();

    CMCFWindowsImpl* impl = get_mcf_windows_impl();
    mcfboolean ret = FALSE;

    if(impl) {
        if (window == mBaseWinCtx.window) {
            impl->destroy_window(window);
            memset(&mBaseWinCtx, 0x00, sizeof(McfWindowContext));
            mBaseWinCtx.window = MCFWINDOW_INVALID;
            ret = TRUE;
        } else if (window == mDimWinCtx.window) {
            impl->destroy_window(window);
            memset(&mDimWinCtx, 0x00, sizeof(McfWindowContext));
            mDimWinCtx.window = MCFWINDOW_INVALID;
            ret = TRUE;
        } else {
            for (int loop = 0;loop < MAX_POPUP_WINDOW;loop++) {
                if (mPopupWinCtx[loop].window == window) {
                    impl->destroy_window(window);
                    memset(&mPopupWinCtx[loop], 0x00, sizeof(McfWindowContext));
                    ret = TRUE;
                    mPopupWinCtx[loop].window = MCFWINDOW_INVALID;
                    break;
                }
            }
        }
    }

    return ret;
}

mcfwindow
CMCFWindows::get_base_window()
{
    MCF_DEBUG();

    return mBaseWinCtx.window;
}

mcfboolean
CMCFWindows::is_base_window(mcfwindow window)
{
    MCF_DEBUG();

    mcfboolean ret = FALSE;

    if (window != MCFWINDOW_INVALID) {
        if (window == mBaseWinCtx.window) {
            ret = TRUE;
        }
    }

    return ret;
}

mcfwindow
CMCFWindows::find_by_etcinfo( void* etcInfo )
{
    MCF_DEBUG();

    mcfwindow ret = MCFWINDOW_INVALID;

    if (etcInfo == mBaseWinCtx.etcInfoPtr) {
        ret = mBaseWinCtx.window;
    } else if (etcInfo == mDimWinCtx.etcInfoPtr) {
        ret = mDimWinCtx.window;
    } else {
        for (int loop = 0;loop < MAX_POPUP_WINDOW;loop++) {
            if (etcInfo == mPopupWinCtx[loop].etcInfoPtr) {
                ret = mPopupWinCtx[loop].window;
                break;
            }
        }
    }

    return ret;
}

McfWindowContext*
CMCFWindows::get_window_context(mcfwindow window, mcfboolean fGetPos)
{
    MCF_DEBUG();

    McfWindowContext* ret = NULL;

    if (window == mBaseWinCtx.window) {
        if (fGetPos) {
            McfRectangle rect;
            get_window_rect(window, &rect);
            mBaseWinCtx.x = rect.x;
            mBaseWinCtx.y = rect.y;
        }
        ret = &mBaseWinCtx;
    } else if (window == mDimWinCtx.window) {
        ret = &mDimWinCtx;
    } else {
        for (int loop = 0;loop < MAX_POPUP_WINDOW;loop++) {
            if (mPopupWinCtx[loop].window == window) {
                if (fGetPos) {
                    McfRectangle rect;
                    get_window_rect(window, &rect);
                    mPopupWinCtx[loop].x = rect.x;
                    mPopupWinCtx[loop].y = rect.y;
                }
                ret = &mPopupWinCtx[loop];
                break;
            }
        }
    }

    return ret;
}

void
CMCFWindows::set_window_context(mcfwindow window, McfWindowContext* context)
{
    MCF_DEBUG();

    if (window == mBaseWinCtx.window) {
        memcpy(&mBaseWinCtx,context, sizeof(McfWindowContext));
    } else {
        for (int loop = 0;loop < MAX_POPUP_WINDOW;loop++) {
            if (mPopupWinCtx[loop].window == window) {
                memcpy(&mPopupWinCtx[loop], context, sizeof(McfWindowContext));
                break;
            }
        }
    }
}

mcf8
CMCFWindows::find_popup_window_index(mcfwindow window)
{
    MCF_DEBUG();

    mcf8 ret = NOT_USED;

    for (int loop = 0;loop < MAX_POPUP_WINDOW;loop++) {
        if (mPopupWinCtx[loop].window == window) {
            ret = loop;
            break;
        }
    }

    return ret;
}

void
CMCFWindows::move_window(const mcfwindow window, const mcf16 x, const mcf16 y)
{
    MCF_DEBUG();

    mcfboolean isVirtual = FALSE;
    if (window == mBaseWinCtx.window) {
        mBaseWinCtx.x = x;
        mBaseWinCtx.y = y;
        isVirtual = mBaseWinCtx.isVirtual;
    } else if (window == mDimWinCtx.window) {
        mDimWinCtx.x = x;
        mDimWinCtx.y = y;
        isVirtual = mDimWinCtx.isVirtual;
    } else {
        for (int loop = 0;loop < MAX_POPUP_WINDOW;loop++) {
            if (mPopupWinCtx[loop].window == window) {
                mPopupWinCtx[loop].x = x;
                mPopupWinCtx[loop].y = y;
                isVirtual = mPopupWinCtx[loop].isVirtual;
                break;
            }
        }
    }
    if(!isVirtual) {
        CMCFWindowsImpl* impl = get_mcf_windows_impl();
        if(impl) {
            impl->move_window(window, x, y);
        }
    }
}

void
CMCFWindows::resize_window(const mcfwindow window, const mcf16 width, const mcf16 height)
{
    MCF_DEBUG();

    mcfboolean isVirtual = FALSE;
    if (window == mBaseWinCtx.window) {
        mBaseWinCtx.width = width;
        mBaseWinCtx.height = height;
        isVirtual = mBaseWinCtx.isVirtual;
    } else if (window == mDimWinCtx.window) {
        mDimWinCtx.width = width;
        mDimWinCtx.height = height;
        isVirtual = mDimWinCtx.isVirtual;
    } else {
        for (int loop = 0;loop < MAX_POPUP_WINDOW;loop++) {
            if (mPopupWinCtx[loop].window == window) {
                mPopupWinCtx[loop].width = width;
                mPopupWinCtx[loop].height = height;
                isVirtual = mPopupWinCtx[loop].isVirtual;
                break;
            }
        }
    }
    if(!isVirtual) {
        CMCFWindowsImpl* impl = get_mcf_windows_impl();
        if(impl) {
            impl->resize_window(window, width, height);
        }
    }
}

void
CMCFWindows::move_resize_window(const mcfwindow window, const mcf16 x, const mcf16 y, const mcf16 width, const mcf16 height)
{
    MCF_DEBUG();

    mcfboolean isVirtual = FALSE;
    if (window == mBaseWinCtx.window) {
        mBaseWinCtx.x = x;
        mBaseWinCtx.y = y;
        mBaseWinCtx.width = width;
        mBaseWinCtx.height = height;
        isVirtual = mBaseWinCtx.isVirtual;
    } else if (window == mDimWinCtx.window) {
        mDimWinCtx.x = x;
        mDimWinCtx.y = y;
        mDimWinCtx.width = width;
        mDimWinCtx.height = height;
        isVirtual = mDimWinCtx.isVirtual;
    } else {
        for (int loop = 0;loop < MAX_POPUP_WINDOW;loop++) {
            if (mPopupWinCtx[loop].window == window) {
                mPopupWinCtx[loop].x = x;
                mPopupWinCtx[loop].y = y;
                mPopupWinCtx[loop].width = width;
                mPopupWinCtx[loop].height = height;
                isVirtual = mPopupWinCtx[loop].isVirtual;
                break;
            }
        }
    }
    if(!isVirtual) {
        CMCFWindowsImpl* impl = get_mcf_windows_impl();
        if(impl) {
            impl->move_resize_window(window, x, y, width, height);
        }
    }
}


/* Push given window into a Z-order list */
void CMCFWindows::push_window_in_Z_order_list(mcfwindow window)
{
    MCF_DEBUG();

    mcfint loop;

    for (loop = 0;loop < MAX_ZORDER_NUM;loop++) {
        if (mZOrderList[loop] == MCFWINDOW_INVALID) {
            mZOrderList[loop] = window;
            return;
        }
    }
}

/* Pop given window from a Z-order list */
void CMCFWindows::pop_window_in_Z_order_list(mcfwindow window)
{
    MCF_DEBUG();

    mcfint loop;
    mcfboolean found = FALSE;

    for (loop = 0;loop < MAX_ZORDER_NUM - 1;loop++) {
        if (mZOrderList[loop] == window || found) {
            found = TRUE;
            mZOrderList[loop] = mZOrderList[loop + 1];
        }
    }

    mZOrderList[MAX_ZORDER_NUM - 1] = MCFWINDOW_INVALID;
}

/* Search n-th window in the Z-order stack, starting from the top (TOPMOST window would be the 0 index) */
mcfwindow CMCFWindows::get_nth_window_in_Z_order_list(mcfbyte index)
{
    MCF_DEBUG();

    mcfint loop;

    for (loop = MAX_ZORDER_NUM - 1;loop >= 0;loop--) {
        if (mZOrderList[loop] != MCFWINDOW_INVALID) {
            if (index == 0) {
                return mZOrderList[loop];
            }
            index--;
        }
    }

    return MCFWINDOW_INVALID;
}

mcfwindow CMCFWindows::get_nth_popup_window( mcfbyte index )
{
    MCF_DEBUG();

    mcf_assert_return_null(index >= 0 && index < MAX_POPUP_WINDOW);

    if (index < MAX_POPUP_WINDOW) {
        return mPopupWinCtx[index].window;
    }

    return MCFWINDOW_INVALID;
}

mcfbyte CMCFWindows::get_Z_order(mcfwindow window)
{
    MCF_DEBUG();

    mcfbyte loop;
    for (loop = 0;loop < MAX_ZORDER_NUM;loop++) {
        if (mZOrderList[loop] == window) {
            return loop;
        }
    }
    return NOT_USED;
}

void CMCFWindows::set_parent( const mcfwindow parentWindow, const mcfwindow window )
{
    /* Do not set parent if the window is a virtual window */
    McfWindowContext *winctx = get_window_context(window, FALSE);
    if (winctx) {
        if (!(winctx->isVirtual)) {
            CMCFWindowsImpl* impl = get_mcf_windows_impl();
            if(impl) {
                impl->set_parent(parentWindow, window);
            }
        }
    }
}

void CMCFWindows::set_window_rotation(const mcfwindow window, const mcfint degree)
{
    MCF_DEBUG();

    CMCFWindowsImpl* impl = get_mcf_windows_impl();

    if(impl) {
        if (window == NULL) {
            impl->set_window_rotation(mBaseWinCtx.window, degree);

            if (MCFWINDOW_INVALID != mDimWinCtx.window) {
                /* For indivisual window rotation */
                impl->set_window_rotation(mDimWinCtx.window, degree);
                hide_window(mDimWinCtx.window);
            }
        } else {
            impl->set_window_rotation(window, degree);
            printf("## set_window_rotation : %d \n", degree);
        }
    }

    // Update the position information
    get_window_context(window, TRUE);
}


void
CMCFWindows::show_window(const mcfwindow window, mcfboolean queue /*= FALSE*/)
{
    MCF_DEBUG();

    if (window == mBaseWinCtx.window) {
        mBaseWinCtx.hidden = FALSE;
    } else if (window == mDimWinCtx.window) {
        mDimWinCtx.hidden = FALSE;
    } else {
        for (int loop = 0;loop < MAX_POPUP_WINDOW;loop++) {
            if (mPopupWinCtx[loop].window == window) {
                mPopupWinCtx[loop].hidden = FALSE;
                break;
            }
        }
    }

    CMCFWindowsImpl* impl = get_mcf_windows_impl();
    if(impl) {
        impl->show_window(window, queue);
    }
}

void
CMCFWindows::hide_window(const mcfwindow window, const mcfboolean fForce /*= FALSE*/)
{
    MCF_DEBUG();

    if (window == mBaseWinCtx.window) {
        mBaseWinCtx.hidden = TRUE;
    } else if (window == mDimWinCtx.window) {
        mDimWinCtx.hidden = TRUE;
    } else {
        for (int loop = 0;loop < MAX_POPUP_WINDOW;loop++) {
            if (mPopupWinCtx[loop].window == window) {
                mPopupWinCtx[loop].hidden = TRUE;
                break;
            }
        }
    }

    CMCFWindowsImpl* impl = get_mcf_windows_impl();
    if(impl) {
        impl->hide_window(window, fForce);
    }
}

void
CMCFWindows::set_keep_above(const mcfwindow window, const mcfboolean keepabove)
{
    CMCFWindowsImpl* impl = get_mcf_windows_impl();
    if(impl) {
        impl->set_keep_above(window, keepabove);
    }
}

void
CMCFWindows::set_update_pending(mcfboolean pend, mcfboolean autoUpdate)
{
    mPendingUpdate = pend;
    if(!pend && autoUpdate) {
        update_window(mBaseWinCtx.window);
    }
}

mcfboolean
CMCFWindows::get_update_pending()
{
    return mPendingUpdate;
}

void
CMCFWindows::update_window(const mcfwindow window,
                           const mcf16 x /*= 0*/, const mcf16 y /*= 0*/, const mcf16 width /*= 0*/, const mcf16 height /*= 0*/ )
{
    if(!mPendingUpdate) {
        CMCFWindowsImpl* impl = get_mcf_windows_impl();
        if(impl) {
            impl->update_window(window, x, y, width, height);
        }
    }
}

mcfboolean
CMCFWindows::get_window_rect(const mcfwindow window, McfRectangle *rect) {
    MCF_DEBUG();

    mcfboolean isVirtual = FALSE;
    mcfboolean bRet = FALSE;
    if (window == mBaseWinCtx.window) {
        isVirtual = mBaseWinCtx.isVirtual;
        if(isVirtual) {
            rect->x = mBaseWinCtx.x;
            rect->y = mBaseWinCtx.y;
            rect->width = mBaseWinCtx.width;
            rect->height = mBaseWinCtx.height;
        }
    } else if (window == mDimWinCtx.window) {
        isVirtual = mDimWinCtx.isVirtual;
        if(isVirtual) {
            rect->x = mDimWinCtx.x;
            rect->y = mDimWinCtx.y;
            rect->width = mDimWinCtx.width;
            rect->height = mDimWinCtx.height;
        }
    } else {
        for (int loop = 0;loop < MAX_POPUP_WINDOW;loop++) {
            if (mPopupWinCtx[loop].window == window) {
                isVirtual = mPopupWinCtx[loop].isVirtual;
                if(isVirtual) {
                    rect->x = mPopupWinCtx[loop].x;
                    rect->y = mPopupWinCtx[loop].y;
                    rect->width = mPopupWinCtx[loop].width;
                    rect->height = mPopupWinCtx[loop].height;
                }
                break;
            }
        }
    }
     if(!isVirtual) {
         CMCFWindowsImpl* impl = get_mcf_windows_impl();
         if(impl) {
             bRet = impl->get_window_rect(window, rect);
         }
    }

    return bRet;
}
