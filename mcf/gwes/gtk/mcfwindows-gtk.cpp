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



#include "mcfwindows-gtk.h"
#include "mcfdebug.h"
#include "mcfcontroller.h"
#include "mcfcontext.h"
#include "mcfresourcecache.h"

using namespace mcf;

/**
 * Constructor
 */
CMCFWindowsImplGtk::CMCFWindowsImplGtk()
{
    MCF_DEBUG();
    /* Initializes all window resources */
}

/**
 * De-constructor
 */
CMCFWindowsImplGtk::~CMCFWindowsImplGtk()
{
    MCF_DEBUG();
}

/**
 * Make a transparent window
 */
static void
make_transparent_window (GtkWidget *window)
{
    MCF_DEBUG();
    GdkScreen *screen;
    GdkColormap *colormap;

    screen = gtk_widget_get_screen (window);
    if (!screen) {
        g_warning ("failed to get window's screen");
        return;
    }

    colormap = gdk_screen_get_rgba_colormap (screen);
    if (!colormap) {
        g_warning ("failed to get RGBA colormap");
        return;
    }

    gtk_widget_set_colormap (window, colormap);
    gtk_widget_set_app_paintable (window, TRUE);
    gtk_widget_realize (window);

}


/**
 * Create a content window and binds it into given parent window as a child
 */
mcfwindow
CMCFWindowsImplGtk::create_base_window(const mcfwindow parentWnd, McfWindowContext *winCtx,
		                               const mcf16 width, const mcf16 height)
{
    MCF_DEBUG();

    GtkWidget* pWindow = NULL;

    /* pre-condition */
    mcf_assert(parentWnd != NULL);
    mcf_assert(winCtx != NULL);
    mcf_assert(winCtx->etcInfoPtr == NULL);

    if (winCtx->window == NULL) {
        pWindow = (GtkWidget*)parentWnd;

        if (GTK_WIDGET_TOPLEVEL(pWindow)) {
            pWindow = (GtkWidget*)parentWnd;
            winCtx->etcInfoPtr = NULL;
            winCtx->window = pWindow;
            make_transparent_window(pWindow);
        } else {
            GtkWidget *drawarea = NULL;
            drawarea = gtk_drawing_area_new();
            gtk_widget_set_size_request(pWindow, width, height);
            gtk_widget_set_size_request(drawarea, width, height);
            gtk_box_pack_start(GTK_BOX(pWindow), drawarea, TRUE, TRUE, 0);
            gtk_widget_show (drawarea);
            gtk_widget_show (pWindow);
            winCtx->etcInfoPtr = static_cast<void*>(drawarea);
            winCtx->window = drawarea;
            make_transparent_window(drawarea);
        }
    }
    /* post-condition */
    return winCtx->window;
}

/**
 * Creates a window
 */
mcfwindow
CMCFWindowsImplGtk::create_window(const mcfwindow parentWnd, McfWindowContext *winCtx,
		                          const mcf16 width, const mcf16 height)
{
    MCF_DEBUG();
    /* pre-condition */
    mcf_assert(parentWnd);
    mcf_assert(winCtx);
    mcf_assert(winCtx->etcInfoPtr == NULL);

    GtkWidget* window = NULL;

    if (winCtx->window == NULL) {
        window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
        gtk_window_set_type_hint(GTK_WINDOW (window), GDK_WINDOW_TYPE_HINT_UTILITY);
        gtk_window_set_accept_focus(GTK_WINDOW(window), FALSE);
#ifdef NO_SOCKETPLUG
        GtkWidget* toplevel = gtk_widget_get_toplevel((GtkWidget*)parentWnd);
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(toplevel));
#endif
        /* Sets a window relation between parent and popup */
        make_transparent_window(window);
        gtk_window_set_default_size(GTK_WINDOW(window), width, height);
#ifndef NO_SOCKETPLUG
        gtk_widget_set_app_paintable(window, TRUE);
#endif
        winCtx->etcInfoPtr = NULL;
        winCtx->window = window;
    }

    return window;
}

/**
 * Creates the dim window
 */
mcfwindow
CMCFWindowsImplGtk::create_dim_window(const mcfwindow parentWnd, McfWindowContext *winCtx,
		                              const mcf16 width, const mcf16 height)
{
    MCF_DEBUG();
    /* pre-condition */
    mcf_assert(parentWnd);
    mcf_assert(winCtx);
    mcf_assert(winCtx->etcInfoPtr == NULL);
    GtkWidget* window = NULL;

    if (winCtx->window == NULL) {
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_type_hint(GTK_WINDOW (window), GDK_WINDOW_TYPE_HINT_UTILITY);

        gtk_widget_set_name(window, "dialog_dim_window");
#ifdef NO_SOCKETPLUG
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(gtk_widget_get_toplevel((GtkWidget*)parentWnd)));
#else
        gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_POPUP_MENU);
#endif
        gtk_window_set_accept_focus(GTK_WINDOW(window), FALSE);
        gtk_widget_set_size_request(window, width, height);
        gtk_window_set_opacity(GTK_WINDOW(window), 0.5);
        gtk_widget_realize(window);
        winCtx->etcInfoPtr = NULL;
        winCtx->window = window;
    }

    return window;
}

/**
 * Make a window relation between parent and child
 * Caution: Currently, If we use transient_for them the ISE will occure some crash. It needs to check X11
 */
void
CMCFWindowsImplGtk::set_parent(const mcfwindow parentWindow, const mcfwindow window)
{
    MCF_DEBUG();
    mcf_assert_return(window);
    mcf_assert_return(parentWindow);
#ifdef NO_SOCKETPLUG
    GtkWidget* widget = static_cast<GtkWidget*>(parentWindow);
    GtkWidget* toplevel = gtk_widget_get_toplevel(widget);
#else
    GtkWindow* gtkParentWindow = static_cast<GtkWindow*>(parentWindow);
#endif
    GtkWindow* gtkWindow = static_cast<GtkWindow*>(window);
#ifdef NO_SOCKETPLUG
    gtk_window_set_transient_for(gtkWindow, GTK_WINDOW(toplevel));
#endif
}

/**
 * Destroys the given window
 */
bool
CMCFWindowsImplGtk::destroy_window(mcfwindow window)
{
    MCF_DEBUG();
    mcf_assert_return_false(window);
    /* add codes for destroying window */
    GtkWidget* widget = static_cast<GtkWidget*>(window);
    gtk_widget_destroy(widget);
    return TRUE;
}

/**
 * Shows the given window
 */
void
CMCFWindowsImplGtk::show_window(const mcfwindow window, mcfboolean queue)
{
    MCF_DEBUG();
    mcf_assert_return(window);
    GtkWidget* widget = static_cast<GtkWidget*>(window);
    gtk_widget_show(widget);

    /*Forcing expose on the magnifier window*/
    if (queue) {
        gtk_widget_queue_draw(widget);
    } else {
        gdk_window_invalidate_rect(widget->window, NULL, TRUE);
        gdk_window_process_updates(widget->window, TRUE);
    }

    /*we will use always on top style when the window is created */
    /*we cannot use keep_above, becase the other toplevel window cannot appear if we use it*/
    /* X team will fix it */
    CMCFWindows *windows = CMCFWindows::get_instance();
    if (windows->get_base_window() != window && windows->get_dim_window() != window) {
#ifndef NO_SOCKETPLUG
        gdk_window_raise(widget->window);
#endif
    } else if (windows->get_dim_window() == window) {
#ifndef NO_SOCKETPLUG
        gdk_window_raise(widget->window);
#endif
    } else if (windows->get_base_window() == window) {
#ifndef NO_SOCKETPLUG
        gdk_window_raise(gtk_widget_get_toplevel(widget)->window);
#endif
    }
}

/**
 * Hides the given window
 */
void
CMCFWindowsImplGtk::hide_window(const mcfwindow window,  const mcfboolean fForce)
{
    MCF_DEBUG();
    mcf_assert_return(window);
    GtkWidget* widget = static_cast<GtkWidget*>(window);
    CMCFWindows *windows = CMCFWindows::get_instance();
    if (windows->get_magnifier_window() == window && fForce == FALSE) {
        /* Fix me : The below is a temporary code for magnifier speed enhancement */
#ifdef NO_SOCKETPLUG
        gint root_x, root_y;
        gtk_window_get_position (GTK_WINDOW(widget), &root_x, &root_y);
        if (GTK_WIDGET_MAPPED(widget)) {

            /* Window rotation*/
            CMCFContext* context = CMCFContext::get_instance();
            if (context->get_display() == DISPLAY_LANDSCAPE) {
                gtk_window_move(GTK_WINDOW(widget), gdk_screen_get_height(gdk_screen_get_default()) - 1,
                		        gdk_screen_get_width(gdk_screen_get_default())-1);
            } else {
                gtk_window_move(GTK_WINDOW(widget), gdk_screen_get_width(gdk_screen_get_default()) - 1,
                		        gdk_screen_get_height(gdk_screen_get_default())-1);
            }
#else
        if (GTK_WIDGET_VISIBLE(widget)) {
            gtk_window_move(GTK_WINDOW(widget), 0, gdk_screen_get_height(gdk_screen_get_default())+1);
#endif
        }
    } else {
        if (GTK_WIDGET_VISIBLE(widget)) {
            gtk_widget_hide(widget);
        }
    }

}

/**
 * Moves the window to the given position
 */
void
CMCFWindowsImplGtk::move_window(const mcfwindow window, const mcf16 x, const mcf16 y)
{
    MCF_DEBUG();
    mcf_assert_return(window);
    GtkWidget* widget = static_cast<GtkWidget*>(window);
    gtk_window_move(GTK_WINDOW(widget), x, y);
}

/**
* Resizes the window to the given metric
*/
void
CMCFWindowsImplGtk::resize_window(const mcfwindow window, const mcf16 width, const mcf16 height)
{
    MCF_DEBUG();
    mcf_assert_return(window);
    GtkWidget* widget = static_cast<GtkWidget*>(window);
    if (GTK_IS_WINDOW(widget)) {
        gtk_widget_set_size_request(GTK_WIDGET(widget), width, height);
    }
}

/**
* Resizes the window to the given metric
*/
void
CMCFWindowsImplGtk::move_resize_window(const mcfwindow window, const mcf16 x, const mcf16 y,
		                               const mcf16 width, const mcf16 height)
{
    MCF_DEBUG();
    mcf_assert_return(window);
    GtkWidget* widget = static_cast<GtkWidget*>(window);
    gdk_window_move_resize(GDK_WINDOW(widget->window), x, y, width, height);
}


/**
* Update the window to redraw given area
*/
void
CMCFWindowsImplGtk::update_window(const mcfwindow window, const mcf16 x, const mcf16 y,
		                          const mcf16 width, const mcf16 height)
{
    MCF_DEBUG();
    mcf_assert_return(window);
    GtkWidget* widget = static_cast<GtkWidget*>(window);

    if (!GTK_WIDGET_VISIBLE(widget)) gtk_widget_show(widget);

    if (x + y + width + height == 0) {
        gdk_window_invalidate_rect(widget->window, NULL, TRUE);
    } else {
        GdkRectangle rect = { x, y, width, height} ;
        gdk_window_invalidate_rect(widget->window, &rect, TRUE);
    }

    gdk_window_process_updates(widget->window, TRUE);
}

/**
 * Returns the position of x,y,width,height of the given window
 */
mcfboolean
CMCFWindowsImplGtk::get_window_rect(const mcfwindow window, McfRectangle *rect)
{
    MCF_DEBUG();
    mcfboolean ret = FALSE;
    GtkWidget* widget = static_cast<GtkWidget*>(window);
    if (rect && GTK_WIDGET_REALIZED(widget)) {
        CMCFContext *context = CMCFContext::get_instance();
        CMCFWindows *windows = CMCFWindows::get_instance();
        if (context->get_display() == DISPLAY_LANDSCAPE) {
            gdk_window_get_root_origin (GDK_WINDOW(gtk_widget_get_toplevel(widget)->window),  &(rect->x), &(rect->y));
            if (window == windows->get_base_window()) {
                CMCFUtils *utils = CMCFUtils::get_instance();
                CMCFResourceCache *cache = CMCFResourceCache::get_instance();

                mcfint scnWidth, scnHeight;
                utils->get_screen_resolution(&scnWidth, &scnHeight);
                rect->y = scnWidth - cache->get_cur_layout(window)->height;
            }
        } else {
            gdk_window_get_root_origin (GDK_WINDOW(gtk_widget_get_toplevel(widget)->window),  &(rect->x), &(rect->y));
            if (window == windows->get_base_window()) {
                CMCFUtils *utils = CMCFUtils::get_instance();
                CMCFResourceCache *cache = CMCFResourceCache::get_instance();

                mcfint scnWidth, scnHeight;
                utils->get_screen_resolution(&scnWidth, &scnHeight);
                rect->y = scnHeight - cache->get_cur_layout(window)->height;
            }
        }
        gtk_window_get_size (GTK_WINDOW(gtk_widget_get_toplevel(widget)), &(rect->width), &(rect->height));

        ret = TRUE;
    }
    return ret;
}

/**
 * Sets rotation
 */
void
CMCFWindowsImplGtk::set_window_rotation(const mcfwindow window)
{
    MCF_DEBUG();
    GtkWidget* widget = gtk_widget_get_toplevel(static_cast<GtkWidget*>(window));
}


/**
 * Shows a message box
 */
void
CMCFWindowsImplGtk::show_message_box(const mcfwindow parentWnd, const mcf8 msgType, mcfchar* title, mcfchar* msg)
{
    MCF_DEBUG();
    mcf_assert_return(strlen(msg) > 0);

    GtkWidget *dialog = NULL;

    switch (msgType) {
    case MCF_MSG_BOX_TYPE_INFO:
        dialog = gtk_message_dialog_new((GtkWindow*)parentWnd,
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_INFO,
                                        GTK_BUTTONS_OK,
                                        msg, "");
        break;
    case MCF_MSG_BOX_TYPE_ERROR:
        dialog = gtk_message_dialog_new((GtkWindow*)parentWnd,
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_ERROR,
                                        GTK_BUTTONS_OK,
                                        msg);
        break;
    case MCF_MSG_BOX_TYPE_WARNING:
        dialog = gtk_message_dialog_new((GtkWindow*)parentWnd,
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_WARNING,
                                        GTK_BUTTONS_OK,
                                        msg);
        break;
    default:
        break;
    }

    if (dialog) {
        gtk_window_set_title(GTK_WINDOW(dialog), title);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}


void
CMCFWindowsImplGtk::set_keep_above(const mcfwindow window, const mcfboolean keepabove)
{
    MCF_DEBUG();

    gtk_window_set_keep_above(GTK_WINDOW(window), keepabove);
}
