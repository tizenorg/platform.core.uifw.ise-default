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
#include <string.h>
#include <Evas.h>
#include "mcftypes.h"
#include "mcfconfig.h"

#ifndef __MCF_WINDOWS_H__
#define __MCF_WINDOWS_H__

#ifdef __cplusplus
//MCF_BEGIN_DECLS
#endif

namespace mcf
{
#define MAX_POPUP_WINDOW 1
#define MAX_ZORDER_NUM ( MAX_POPUP_WINDOW + 1 ) /* Popup + Base */

#define MCF_WINDOW_Z_TOP 0

/**@brief  window context structure */
typedef struct _McfWindowContext {
    mcfboolean isVirtual;
    mcfwindow window;
    mcfshort layout;
    MCFPopupType popuptype;
    mcfboolean hidden;

    mcfwindow parentWnd;
    mcfint parentKey;

    mcfint x, y, width, height;
    mcfint imgOffsetx, imgOffsety;

    mcfint timeout;

    void* etcInfoPtr;
} McfWindowContext;


/**@brief  message box type definition */
typedef enum _MCFMsgBoxType {
    MCF_MSG_BOX_TYPE_INFO	 = 0, /**< for information */
    MCF_MSG_BOX_TYPE_ERROR, /**< for error */
    MCF_MSG_BOX_TYPE_WARNING, /**< for warning */
    MAX_MSG_BOX_TYPE/* maximum number of button state */
}MCFMsgBoxType;

/**
 * @brief The base class to work as a soft-based keyboard
 *
 * This class implements all functions for working as a soft-based keyboard
 * In side of ISE developer, they can modify it by their requirements.
 */
class CMCFWindowsImpl
{
public :
    virtual mcfwindow create_base_window(const mcfwindow parentWnd, McfWindowContext *winCtx,
    		                             const mcf16 width, const mcf16 height) = 0;
    virtual mcfwindow create_window(const mcfwindow parentWnd, McfWindowContext *winCtx,
    		                        const mcf16 width, const mcf16 height) = 0;
    virtual mcfwindow create_context_popup_window(const mcfwindow parentWnd) = 0;
    virtual Evas_Object* get_context_popup_window() = 0;
    virtual void destroy_context_popup_window() = 0;
    virtual mcfwindow create_dim_window(const mcfwindow parentWnd, McfWindowContext *winCtx,
    		                            const mcf16 width, const mcf16 height) = 0;
    virtual bool destroy_window(mcfwindow window) = 0;
    virtual void set_parent(const mcfwindow parentWindow, const mcfwindow window) = 0;
    virtual void show_window(const mcfwindow window, mcfboolean queue) = 0;
    virtual void hide_window(const mcfwindow window,  const mcfboolean fForce = FALSE) = 0;
    virtual void move_window(const mcfwindow window, const mcf16 x, const mcf16 y) = 0;
    virtual void move_resize_window(const mcfwindow window, const mcf16 x, const mcf16 y,
    		                        const mcf16 width, const mcf16 height) = 0;
    virtual void resize_window(const mcfwindow window, const mcf16 width, const mcf16 height) = 0;
    virtual void update_window(const mcfwindow window, const mcf16 x = 0, const mcf16 y = 0,
    		                   const mcf16 width = 0, const mcf16 height = 0) = 0;
    virtual void show_message_box(const mcfwindow parentWnd, const mcf8 msgType, mcfchar* title, mcfchar* msg) = 0;
    virtual mcfboolean get_window_rect(const mcfwindow window, McfRectangle *rect) = 0;

    virtual void set_keep_above(const mcfwindow window, const mcfboolean keepabove) = 0;
};

class CMCFWindows
{
private:
    static CMCFWindows* sInstance; /* For singleton */
    CMCFWindows();
public :
    ~CMCFWindows();

    static CMCFWindows* get_instance();

    mcfwindow open_popup(const mcfwindow parentWnd, const mcfint parentKey, const mcf16 x,
    		             const mcf16 y, const mcf16 width, const mcf16 height, mcfshort layout,
    		             MCFPopupType popuptype, mcfboolean isVirtual, mcfboolean useDimWindow,
    		             mcfint imgOffsetx = 0, mcfint imgOffsety = 0, mcfint timeout = 0);
    mcfwindow open_context_popup(const mcfwindow parentWnd);
    void destroy_context_popup();
    Evas_Object* get_context_popup();
    bool close_popup(mcfwindow window);
    bool close_all_popups(mcfwindow skipwindow = MCFWINDOW_INVALID);

    mcfwindow create_base_window(const mcfwindow parentWnd, const mcf16 width, const mcf16 height);

    void show_window(const mcfwindow window, mcfboolean queue = FALSE);
    void hide_window(const mcfwindow window,  const mcfboolean fForce = FALSE);
    void set_parent(const mcfwindow parentWindow, const mcfwindow window);
    void set_keep_above(const mcfwindow window, const mcfboolean keepabove);

    void move_window(const mcfwindow window, const mcf16 x, const mcf16 y);
    void resize_window(const mcfwindow window, const mcf16 width, const mcf16 height);
    void move_resize_window(const mcfwindow window, const mcf16 x, const mcf16 y,
    		                const mcf16 width, const mcf16 height);
    void update_window(const mcfwindow window, const mcf16 x = 0, const mcf16 y = 0,
    		           const mcf16 width = 0, const mcf16 height = 0);
    void show_message_box(const mcfwindow parentWnd, const mcf8 msgType, mcfchar* title, mcfchar* msg) {
        get_mcf_windows_impl()->show_message_box(parentWnd, msgType, title, msg);
    }
    mcfboolean get_window_rect(const mcfwindow window, McfRectangle *rect);
    mcfwindow get_base_window();
    mcfboolean is_base_window(mcfwindow window);
    mcfwindow find_by_etcinfo(void* etcInfo);
    mcf8 find_popup_window_index(mcfwindow window);
    McfWindowContext* get_window_context(mcfwindow window, mcfboolean fGetPos = FALSE);
    void set_window_context(mcfwindow window, McfWindowContext* context);

    mcfwindow get_nth_window_in_Z_order_list(mcfbyte index);
    mcfwindow get_nth_popup_window(mcfbyte index);
    mcfbyte get_Z_order(mcfwindow window);

    mcfwindow create_dim_window(const mcfwindow parentWnd, McfWindowContext *winCtx,
    		                    const mcf16 width, const mcf16 height);
    mcfwindow get_dim_window();

    void set_update_pending(mcfboolean pend, mcfboolean autoUpdate);
    mcfboolean get_update_pending();

protected :
    CMCFWindowsImpl* get_mcf_windows_impl();

    mcfwindow create_window(const mcfwindow parentWnd, const mcfint parentKey, const mcf16 x,
    		                const mcf16 y, const mcf16 width, const mcf16 height, mcfshort layout,
    		                MCFPopupType popuptype, mcfboolean isVirtual, mcfint imgOffsetx = 0,
    		                mcfint imgOffsety = 0, mcfint timeout = 0);
    bool destroy_window(mcfwindow window);

    void push_window_in_Z_order_list(mcfwindow window);
    void pop_window_in_Z_order_list(mcfwindow window);

private :
    CMCFWindowsImpl* pImp_;
    McfWindowContext mBaseWinCtx;
    McfWindowContext mPopupWinCtx[MAX_POPUP_WINDOW];

    McfWindowContext mDimWinCtx;
    mcfboolean mPendingUpdate;

    mcfwindow mZOrderList[MAX_ZORDER_NUM];
};


} /* End of mcf namespace */

#ifdef __cplusplus
//MCF_END_DECLS
#endif

#endif //__MCF_WINDOWS_H__
