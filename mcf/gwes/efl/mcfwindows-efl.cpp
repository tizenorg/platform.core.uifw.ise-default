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

#include "mcfwindows-efl.h"
#include "mcfdebug.h"
#include "mcfcontroller.h"
#include "mcfcontext.h"
#include "mcfresourcecache.h"
#include "mcfuibuilder.h"
#include "mcfwindows.h"

#include <Elementary.h>
#include <Ecore_X.h>
#include <malloc.h>
#include <X11/Xlib.h>

using namespace mcf;

Ecore_X_Atom ATOM_WM_CLASS = 0;

void release_all(Evas_Object *win);

#include "mcfgraphics-efl.h"
#include <utilX.h>
#ifdef TEST_NEWBACKEND
#include <Ecore_Evas.h>
#include <Ecore.h>
#include <vector>
extern std::vector<ImageCache> g_ImageCache;
extern std::vector<TextCache> g_TextCache;
#else
#endif

/**
 * Constructor
 */
CMCFWindowsImplEfl::CMCFWindowsImplEfl()
{
    MCF_DEBUG();
    mcf_context_window = NULL;
    /* Initializes all window resources */
    ATOM_WM_CLASS = ecore_x_atom_get("WM_CLASS");
}

/**
 * De-constructor
 */
CMCFWindowsImplEfl::~CMCFWindowsImplEfl()
{
    MCF_DEBUG();
}


/**
 * Create a content window and binds it into given parent window as a child
 */
mcfwindow
CMCFWindowsImplEfl::create_base_window(const mcfwindow parentWnd, McfWindowContext *winCtx, const mcf16 width, const mcf16 height)
{
    MCF_DEBUG();

    mcfwindow ret = MCFWINDOW_INVALID;

    if(winCtx) {
        winCtx->etcInfoPtr = NULL;
        winCtx->window = parentWnd;

        ecore_x_icccm_name_class_set(elm_win_xwindow_get(static_cast<Evas_Object*>(parentWnd)), "Virtual Keyboard", "ISF" );

        set_window_accepts_focus(parentWnd, FALSE);

        ret = winCtx->window;
    }

    return ret;
}

mcfwindow
CMCFWindowsImplEfl::create_context_popup_window(const mcfwindow parentWnd)
{
    CMCFWindows *windows = CMCFWindows::get_instance();
    MCF_DEBUG();
    int w,h;
    mcf_context_window = elm_win_add(NULL, "MCFContextPopup", ELM_WIN_BASIC);
    Ecore_X_Display* dpy;
    dpy = ecore_x_display_get();
    elm_win_borderless_set(mcf_context_window, EINA_TRUE);
    elm_win_alpha_set(mcf_context_window, EINA_TRUE);
    ecore_x_window_size_get(ecore_x_window_root_first_get(), &w, &h);
    ecore_x_icccm_name_class_set(elm_win_xwindow_get(static_cast<Evas_Object*>(mcf_context_window)), "ISF Context Popup", "ISF");
    utilx_set_window_effect_state((Display*)dpy, elm_win_xwindow_get(static_cast<Evas_Object*>(mcf_context_window)), 0);
    CMCFContext *context = CMCFContext::get_instance();
    evas_object_resize(mcf_context_window, w, h);
    evas_object_show(mcf_context_window);
    if(!(windows->get_update_pending())) {
      update_window(mcf_context_window);
    }
    return mcf_context_window;
}

Evas_Object*
CMCFWindowsImplEfl::get_context_popup_window ()
{
    return mcf_context_window;
}

void
CMCFWindowsImplEfl::destroy_context_popup_window ()
{
    if(mcf_context_window) {
      evas_object_del(mcf_context_window);
      mcf_context_window = NULL;
    }
    return;
}

/**
 * Creates a window
 */
mcfwindow
CMCFWindowsImplEfl::create_window(const mcfwindow parentWnd, McfWindowContext *winCtx, const mcf16 width, const mcf16 height)
{
    MCF_DEBUG();

    Evas_Object *win = NULL;
    win = elm_win_add(static_cast<Evas_Object*>(parentWnd), "MCFPopup", ELM_WIN_BASIC);
    Ecore_X_Display* dpy;
    dpy = ecore_x_display_get();

    elm_win_borderless_set(win, EINA_TRUE);
    elm_win_alpha_set(win, EINA_TRUE);
#ifndef FULL_SCREEN_TEST
    evas_object_resize(win, width, height);
#endif

    ecore_x_icccm_name_class_set(elm_win_xwindow_get(static_cast<Evas_Object*>(win)), "ISF Popup", "ISF");

    set_window_accepts_focus(win, FALSE);


    utilx_set_window_effect_state((Display*)dpy, elm_win_xwindow_get(static_cast<Evas_Object*>(win)), 0);

    CMCFContext *context = CMCFContext::get_instance();
    return win;
}

/**
 * Creates the dim window
 */
mcfwindow
CMCFWindowsImplEfl::create_dim_window(const mcfwindow parentWnd, McfWindowContext *winCtx,
		                              const mcf16 width, const mcf16 height)
{
    MCF_DEBUG();

    Evas_Object *win = NULL;
    win = elm_win_add(static_cast<Evas_Object*>(parentWnd), "MCFPopup", ELM_WIN_BASIC);
    Ecore_X_Display* dpy;
    dpy = ecore_x_display_get();

    elm_win_borderless_set(win, EINA_TRUE);
    elm_win_alpha_set(win, EINA_TRUE);
    evas_object_resize(win, width, height);

    ecore_x_icccm_name_class_set(elm_win_xwindow_get(static_cast<Evas_Object*>(win)), "ISF Popup", "ISF");

    set_window_accepts_focus(win, FALSE);

    utilx_set_window_effect_state((Display*)dpy, elm_win_xwindow_get(static_cast<Evas_Object*>(win)), 0);

    CMCFContext *context = CMCFContext::get_instance();

    hide_window(win);

    return win;
}

/**
 * Make a window relation between parent and child
 * Caution: Currently, If we use transient_for them the ISE will occure some crash. It needs to check X11
 */
void
CMCFWindowsImplEfl::set_parent(const mcfwindow parentWindow, const mcfwindow window)
{
    MCF_DEBUG();

    ecore_x_icccm_transient_for_set(elm_win_xwindow_get(static_cast<Evas_Object*>(window)),
                                    elm_win_xwindow_get(static_cast<Evas_Object*>(parentWindow)));
}

/**
 * Destroys the given window
 */
bool
CMCFWindowsImplEfl::destroy_window(mcfwindow window)
{
    MCF_DEBUG();

    CMCFWindows *windows = CMCFWindows::get_instance();
    McfWindowContext *winctx = windows->get_window_context(window, FALSE);
    if (winctx) {
        if (winctx->etcInfoPtr) {
            while ((Eina_List*)(winctx->etcInfoPtr)) {
                EFLObject *obj = (EFLObject*)eina_list_data_get((Eina_List*)(winctx->etcInfoPtr));
                if (obj) {
                    Evas_Object* eo = obj->obj;
                    if(obj->bExtracted) {
                        void *data = evas_object_image_data_get(eo, 1);
                        if(data) {
                            free(data);
                        }
                    }
                    if(eo) {
                    evas_object_del(eo);
                        obj->obj = NULL;
                    }
                    if(obj->type == EFLOBJECT_TEXTBLOCK) {
                        Evas_Textblock_Style *st = (Evas_Textblock_Style*)(obj->data);
                        if(st) {
                            evas_textblock_style_free(st);
                        }
#ifdef TEST_NEWBACKEND
						for(mcfint loop = 0;loop < g_TextCache.size();loop++) {
							if(g_TextCache[loop].text == obj->obj) {
								g_TextCache[loop].used = FALSE;
							}
						}
#endif
                    } else if(obj->type == EFLOBJECT_IMAGE) {
#ifdef TEST_NEWBACKEND
						for(mcfint loop = 0;loop < g_ImageCache.size();loop++) {
							if(g_ImageCache[loop].image == obj->obj) {
								g_ImageCache[loop].used = FALSE;
							}
						}
#endif
                    }
                }
                winctx->etcInfoPtr = eina_list_remove_list((Eina_List*)(winctx->etcInfoPtr), (Eina_List*)(winctx->etcInfoPtr));
                delete obj;
            }
            winctx->etcInfoPtr = NULL;
        }

        if(!(winctx->isVirtual)) {
            Evas_Object *win = (Evas_Object*)window;
            evas_object_hide(win);
            evas_object_del(win);
        }
    }

    return TRUE;
}

/**
 * Shows the given window
 */
void
CMCFWindowsImplEfl::show_window(const mcfwindow window, mcfboolean queue)
{
    MCF_DEBUG();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFContext *context = CMCFContext::get_instance();
    if(windows && context && window) {
        McfWindowContext *winctx = windows->get_window_context(window);
        if(!(context->get_hidden_state())) {
            if(winctx) {
                if(!(winctx->isVirtual)) {
                    evas_object_show((Evas_Object*)window);
                }
                if(!(windows->get_update_pending())) {
                    update_window(window);
                }
            }
        }
    }
}

int hide_window_timer_event(void *data)
{
    Evas_Object *win = (Evas_Object*)data;
    CMCFUtils *utils = CMCFUtils::get_instance();
    if(utils) {
        utils->log("hide_window_timer_event : %p\n", win);
    }
    if(win) {
        evas_object_hide(win);
    }
    return FALSE;
}
/**
 * Hides the given window
 */
void
CMCFWindowsImplEfl::hide_window(const mcfwindow window,  const mcfboolean fForce)
{
    MCF_DEBUG();
    CMCFWindows *windows = CMCFWindows::get_instance();
    McfWindowContext *winctx = windows->get_window_context(window, FALSE);
    if(winctx) {
        if(!(winctx->isVirtual)) {
            Evas_Object *win = (Evas_Object*)window;
            evas_object_move(win, -10000, -10000);
        }
    }
    if(window == windows->get_dim_window()) {
        if (winctx) {
            if (winctx->etcInfoPtr) {

                Eina_List *list = (Eina_List*)(winctx->etcInfoPtr);
                Eina_List *iter = NULL;
                Eina_List *iter_next = NULL;
                void *data = NULL;
                int iIndex = 0;
#ifdef TEST_NEWBACKEND
#else
                EINA_LIST_FOREACH_SAFE(list, iter, iter_next, data) {
                    if (data) {
                        EFLObject *obj = (EFLObject*)(data);
                        if(obj) {
                            mcfboolean bShouldRemove = FALSE;
                            bShouldRemove = TRUE;
                            if (bShouldRemove) {
                                Evas_Object* eo = obj->obj;
                                if(obj->bExtracted) {
                                    void *data = evas_object_image_data_get(eo, 1);
                                    if(data) {
                                        free(data);
                                    }
                                }
                                if(eo) {
                                    evas_object_del(eo);
                                    obj->obj = NULL;
                                }
                                if(obj->type == EFLOBJECT_TEXTBLOCK) {
                                    Evas_Textblock_Style *st = (Evas_Textblock_Style*)(obj->data);
                                    if(st) {
                                        evas_textblock_style_free(st);
                                    }
#ifdef TEST_NEWBACKEND
                                    for(mcfint loop = 0;loop < g_TextCache.size();loop++) {
                                        if(g_TextCache[loop].text == obj->obj) {
                                            g_TextCache[loop].used = FALSE;
                                        }
                                    }
#endif
                                } else if(obj->type == EFLOBJECT_IMAGE) {
#ifdef TEST_NEWBACKEND
                                    for(mcfint loop = 0;loop < g_ImageCache.size();loop++) {
                                        if(g_ImageCache[loop].image == obj->obj) {
                                            g_ImageCache[loop].used = FALSE;
                                        }
                                    }
#endif
                                }
                                delete obj;
                                list = eina_list_remove_list(list, iter);
                            }
                        }
                        iIndex++;
                    }
                }
                winctx->etcInfoPtr = list;
#endif
            }
        }
    }
    if(window == windows->get_base_window()) {
        elm_cache_all_flush();
        malloc_trim(0);
    }
}

/**
 * Moves the window to the given position
 */
void
CMCFWindowsImplEfl::move_window(const mcfwindow window, const mcf16 x, const mcf16 y)
{
    MCF_DEBUG();
    CMCFUtils *utils = CMCFUtils::get_instance();
    CMCFContext *context = CMCFContext::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();

    if(utils && context && windows) {
        McfWindowContext *winctx = windows->get_window_context(window, FALSE);
        unsigned short win_width = 0;
        unsigned short win_height = 0;
        if(winctx) {
            win_width = winctx->width;
            win_height = winctx->height;
        }

        mcf16 rotatex = x;
        mcf16 rotatey = y;
        mcf16 orgx = x;
        mcf16 orgy = y;

        mcfint scr_w, scr_h;
        /* get window size */
        utils->get_screen_resolution(&scr_w, &scr_h);

        Evas_Object *win = (Evas_Object*)window;
#ifndef FULL_SCREEN_TEST
        evas_object_move(win, rotatex, rotatey);
#endif
    }
}

/**
* Resizes the window to the given metric
*/
void
CMCFWindowsImplEfl::resize_window(const mcfwindow window, const mcf16 width, const mcf16 height)
{
    MCF_DEBUG();

    Evas_Object *win = (Evas_Object*)window;
#ifndef FULL_SCREEN_TEST
    evas_object_resize(win, width, height);
    if(win == CMCFWindows::get_instance()->get_base_window()) {
        CMCFUtils *utils = CMCFUtils::get_instance();
        if(utils) {
            utils->log("RESIZE_BASE_WINDOW : %d %d\n", width, height);
        }
    }
#endif
    CMCFWindows *windows = CMCFWindows::get_instance();
    if(window == windows->get_dim_window()) {
        hide_window(window);
    }
}

/**
* Resizes the window to the given metric
*/
void
CMCFWindowsImplEfl::move_resize_window(const mcfwindow window, const mcf16 x, const mcf16 y,
		                               const mcf16 width, const mcf16 height)
{
    MCF_DEBUG();
    Evas_Object *win = (Evas_Object*)window;
#ifndef FULL_SCREEN_TEST
    evas_object_move(win, x, y);
    evas_object_resize(win, width, height);
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFUtils *utils = CMCFUtils::get_instance();
    if(windows && utils) {
        if(win == windows->get_base_window()) {
            utils->log("RESIZE_BASE_WINDOW : %d %d\n", width, height);
        }
    }
#endif
}

/**
* Update the window to redraw given area
*/
void
CMCFWindowsImplEfl::update_window(const mcfwindow window, const mcf16 x, const mcf16 y,
		                          const mcf16 width, const mcf16 height)
{
    MCF_DEBUG();

    mcfboolean removeall = FALSE;
    McfRectangle updatearea = {x, y, width, height};
    if (x + y + width + height == 0) {
        removeall = TRUE;
    }

    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFUtils *utils = CMCFUtils::get_instance();
    McfWindowContext *winctx = NULL;

    if(windows) {
        winctx = windows->get_window_context(window, FALSE);
    }
    if (windows && utils && winctx) {
        if(winctx->isVirtual) {
            McfWindowContext *basectx = windows->get_window_context(windows->get_base_window());
            if(basectx) {
                updatearea.x += (winctx->x - basectx->x);
                updatearea.y += (winctx->y - basectx->y);
            }
        }
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
                        mcfboolean bShouldRemove = FALSE;
                        if (removeall || utils->is_rect_overlap(obj->position, updatearea)) {
                            bShouldRemove = TRUE;
                        }
                        if (iIndex == 0 && !removeall) {
                            bShouldRemove = FALSE; // window's background
                        }
                        if (bShouldRemove) {
#ifdef TEST_NEWBACKEND
                            MCF_DEBUG_ELAPASED_TIME_START();
                            if(TRUE) {
                                if(winctx->width != obj->position.width || winctx->height != obj->position.height ||
                                    obj->type == EFLOBJECT_TEXTBLOCK) {
                                    evas_object_hide(obj->obj);
                                }
                            } else {
#else
                            Evas_Object* eo = obj->obj;
                            if(obj->bExtracted) {
                                void *data = evas_object_image_data_get(eo, 1);
                                if(data) {
                                    free(data);
                                }
                            }
                            if(eo) {
                                evas_object_del(eo);
                                obj->obj = NULL;
                            }
                            if(obj->type == EFLOBJECT_TEXTBLOCK) {
                                Evas_Textblock_Style *st = (Evas_Textblock_Style*)(obj->data);
                                if(st) {
                                    evas_textblock_style_free(st);
                                }
#ifdef TEST_NEWBACKEND
                                for(mcfint loop = 0;loop < g_TextCache.size();loop++) {
                                    if(g_TextCache[loop].text == obj->obj) {
                                        g_TextCache[loop].used = FALSE;
                                    }
                                }
#endif
                            } else if(obj->type == EFLOBJECT_IMAGE) {
#ifdef TEST_NEWBACKEND
                                for(mcfint loop = 0;loop < g_ImageCache.size();loop++) {
                                    if(g_ImageCache[loop].image == obj->obj) {
                                        g_ImageCache[loop].used = FALSE;
                                    }
                                }
#endif
                            }
                            delete obj;
                            list = eina_list_remove_list(list, iter);
#endif
#ifdef TEST_NEWBACKEND
                            }
#endif
                        }
                    }
                    iIndex++;
                }
            }
            winctx->etcInfoPtr = list;
        }
    }

    CMCFUIBuilder *builder = CMCFUIBuilder::get_instance();
    builder->show_layout(window, x, y, width, height);
}

/**
 * Returns the position of x,y,width,height of the given window
 */
mcfboolean
CMCFWindowsImplEfl::get_window_rect(const mcfwindow window, McfRectangle *rect)
{
    MCF_DEBUG();
    CMCFUtils *utils = CMCFUtils::get_instance();
    CMCFContext *context = CMCFContext::get_instance();

    if(utils && context && rect) {
        Window junkwin;
        Ecore_X_Window_Attributes attrs;
        int x, y, width, height;
        mcfint scr_w, scr_h;
        ecore_x_window_geometry_get(elm_win_xwindow_get(static_cast<Evas_Object*>(window)), &x, &y, &width, &height);
        ecore_x_window_attributes_get(elm_win_xwindow_get(static_cast<Evas_Object*>(window)), &attrs);
        XTranslateCoordinates((Display*)ecore_x_display_get(), (Drawable)elm_win_xwindow_get(static_cast<Evas_Object*>(window)),
                              attrs.root, -attrs.border, -attrs.border, &x, &y, &junkwin);

        utils->log("CMCFWindowsImplEfl::get_window_rect : %p , %d %d %d %d\n", window, x, y, width, height);

        /* get window size */
        utils->get_screen_resolution(&scr_w, &scr_h);
        rect->x = x;
        rect->y = y;
        rect->width = width;
        rect->height = height;
    }

    return TRUE;
}

#include <X11/Xutil.h>

/**
 * Shows a message box
 */
void
CMCFWindowsImplEfl::show_message_box(const mcfwindow parentWnd, const mcf8 msgType, mcfchar* title, mcfchar* msg)
{
    MCF_DEBUG();
}


void
CMCFWindowsImplEfl::set_keep_above(const mcfwindow window, const mcfboolean keepabove)
{
    MCF_DEBUG();
}

void release_all(Evas_Object *win)
{
	MCF_DEBUG();
	evas_object_move(win, -10000, -10000);
	CMCFWindows *windows = CMCFWindows::get_instance();
		McfWindowContext *winctx = windows->get_window_context(win, FALSE);
		if (winctx) {
			if (winctx->etcInfoPtr) {

				Eina_List *list = (Eina_List*)(winctx->etcInfoPtr);
				Eina_List *iter = NULL;
				Eina_List *iter_next = NULL;
				void *data = NULL;
				int iIndex = 0;
#ifdef TEST_NEWBACKEND
#else
                EINA_LIST_FOREACH_SAFE(list, iter, iter_next, data) {
                    if (data) {
                        EFLObject *obj = (EFLObject*)(data);
                        if(obj) {
                            mcfboolean bShouldRemove = FALSE;
                            bShouldRemove = TRUE;
                            if (bShouldRemove) {
                                Evas_Object* eo = obj->obj;
                                if(obj->bExtracted) {
                                    void *data = evas_object_image_data_get(eo, 1);
                                    if(data) {
                                        free(data);
                                    }
                                }
                                if(eo) {
                                    evas_object_del(eo);
                                    obj->obj = NULL;
                                }
                                if(obj->type == EFLOBJECT_TEXTBLOCK) {
                                    Evas_Textblock_Style *st = (Evas_Textblock_Style*)(obj->data);
                                    if(st) {
                                        evas_textblock_style_free(st);
                                    }
#ifdef TEST_NEWBACKEND
                                    for(mcfint loop = 0;loop < g_TextCache.size();loop++) {
                                        if(g_TextCache[loop].text == obj->obj) {
                                            g_TextCache[loop].used = FALSE;
                                        }
                                    }
#endif
                                } else if(obj->type == EFLOBJECT_IMAGE) {
#ifdef TEST_NEWBACKEND
                                    for(mcfint loop = 0;loop < g_ImageCache.size();loop++) {
                                        if(g_ImageCache[loop].image == obj->obj) {
                                            g_ImageCache[loop].used = FALSE;
                                        }
                                    }
#endif
                                }
                                delete obj;
                                list = eina_list_remove_list(list, iter);
                            }
                        }
                        iIndex++;
                    }
                }
                winctx->etcInfoPtr = list;
#endif
			}
		}
}

void CMCFWindowsImplEfl::set_window_accepts_focus(const mcfwindow window, const mcfboolean acceptable)
{
    Eina_Bool accepts_focus;
    Ecore_X_Window_State_Hint initial_state;
    Ecore_X_Pixmap icon_pixmap;
    Ecore_X_Pixmap icon_mask;
    Ecore_X_Window icon_window;
    Ecore_X_Window window_group;
    Eina_Bool is_urgent;

    if(window) {
        ecore_x_icccm_hints_get(elm_win_xwindow_get(static_cast<Evas_Object*>(window)),
                                &accepts_focus, &initial_state, &icon_pixmap, &icon_mask,
                                &icon_window, &window_group, &is_urgent);
        ecore_x_icccm_hints_set(elm_win_xwindow_get(static_cast<Evas_Object*>(window)),
                                acceptable, initial_state, icon_pixmap, icon_mask,
                                icon_window, window_group, is_urgent);
    }
}
