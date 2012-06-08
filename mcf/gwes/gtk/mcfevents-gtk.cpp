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



#include "mcfevents-gtk.h"
#include "mcfdebug.h"
#include "mcfcontroller.h"
#include "mcfgraphics.h"
#include "mcfuibuilder.h"

using namespace mcf;

mcfboolean bMousePressed = FALSE; /* Checks whether mouse is pressed or not */
mcfboolean bSkipMouseEventsUntilPressed = FALSE; /*Explicitly filter out any mouse events until mouse press occurs */

#define MIN_XY_DIFF 14
static mcfint sLatestXPos = 0;
static mcfint sLatestYPos = 0;

/**
 * Constructor
 */
CMCFEventsImplGtk::CMCFEventsImplGtk()
{
    MCF_DEBUG();
    /* Initializes all window resources */
}

/**
 * De-constructor
 */
CMCFEventsImplGtk::~CMCFEventsImplGtk()
{
    MCF_DEBUG();
}


/* We have to remember the previous position of magnifier window, since it gets changed when mouse press occurs */
mcfint magpressposx = 0;
mcfint magpressposy = 0;
/**
* Generates a mouse press event to CMCFController class
*/
bool
mouse_press(GtkWidget *widget, GdkEventButton *event, gpointer userData)
{
    MCF_DEBUG();
    /* pre-condition */
    CMCFController *controller;
    mcfboolean processed = FALSE;

    bSkipMouseEventsUntilPressed = FALSE;

    controller = CMCFController::get_instance();

    CMCFWindows *windows = CMCFWindows::get_instance();
    GtkWidget* magnifier = static_cast<GtkWidget*>(windows->get_magnifier_window());
    gdk_window_raise(magnifier->window);
    if (widget == windows->get_magnifier_window()) {
        McfWindowContext *magctx = windows->get_window_context(widget, FALSE);
        if (magctx) {
            magpressposx = magctx->x;
            magpressposy = magctx->y;
        }
        /* First convert the local coordination to global coordination */
        mcfwindow window = MCFWINDOW_INVALID;
        mcfbyte index = 0;
        McfWindowContext *context = NULL;
        context = windows->get_window_context(static_cast<mcfwindow>(userData), FALSE);

        mcfint x = event->x;
        mcfint y = event->y;

        x = context->x + x;
        y = context->y + y;

        do {
            window = windows->get_nth_window_in_Z_order_list(index);
            context = windows->get_window_context(window, FALSE);

            if (context) {
                if ((x >= context->x && x <= (context->x + context->width)) &&
                    (y >= context->y && y <= (context->y + context->height))) {
                    /* Now convert the global coordination to appropriate local coordination */
                    x = x - context->x;
                    y = y - context->y;

                    if (controller) {
                        controller->mouse_press(window, x, y);
                        bMousePressed = TRUE;
                        processed = TRUE;
                    }
                }
            }

            index++;
        } while (index < MAX_ZORDER_NUM && window != MCFWINDOW_INVALID);
    } else if (widget == windows->get_dim_window()) {
        controller->mouse_press(static_cast<mcfwindow>(userData), event->x, event->y);
        processed = TRUE;
        bSkipMouseEventsUntilPressed = TRUE;
    }

    controller = CMCFController::get_instance();
    if (controller && !bMousePressed && !processed) {
        controller->mouse_press(static_cast<mcfwindow>(userData), event->x, event->y);
        bMousePressed = TRUE;
    }

    sLatestXPos = event->x;
    sLatestYPos = event->y;

    /* post-condition */
    return TRUE;
}

/**
 * Generates a mouse release event to CMCFController class
 */
bool
mouse_release(GtkWidget *widget, GdkEventButton *event, gpointer userData)
{
    MCF_DEBUG();
    /* pre-condition */
    CMCFController *controller;
    CMCFWindows *windows;

    controller = CMCFController::get_instance();
    windows = CMCFWindows::get_instance();

    mcfint x = event->x;
    mcfint y = event->y;

    if (bMousePressed) {
        mcfboolean processed = FALSE;
        mcfbyte index = 0;
        mcfwindow window = MCFWINDOW_INVALID;
        McfWindowContext *context = NULL;

        /* First convert the local coordination to global coordination */
        context = windows->get_window_context(static_cast<mcfwindow>(userData), FALSE);
        if (widget == windows->get_magnifier_window()) {
            /* Sometimes the event struct contains incorrect mouse position, when magnifier window gets moved by clicking on it */
            GtkWidget* widget = static_cast<GtkWidget*>(windows->get_base_window());
            gdk_window_get_pointer(widget->window, &x, &y, NULL);
            context = windows->get_window_context(windows->get_base_window(), FALSE);
        }

        x = context->x + x;
        y = context->y + y;

        do {
            window = windows->get_nth_window_in_Z_order_list(index);
            context = windows->get_window_context(window, FALSE);

            if (context) {
                if ((x >= context->x && x <= (context->x + context->width)) &&
                    (y >= context->y && y <= (context->y + context->height))) {
                    /* Now convert the global coordination to appropriate local coordination */
                    x = x - context->x;
                    y = y - context->y;

                    if (controller) {
                        controller->mouse_release(window, x, y);
                        processed = TRUE;
                    }
                }
            }

            index++;
        } while (index < MAX_ZORDER_NUM && window != MCFWINDOW_INVALID);

        /* No window exists on that point */
        if (!processed) {
            if (controller) {
                controller->mouse_release(static_cast<mcfwindow>(userData), event->x, event->y);
            }
        }
    } else if (!bSkipMouseEventsUntilPressed) {
        mcf_assert((FALSE && "Mouse released but bMousePressed is FALSE"));
        if (controller) {
            controller->mouse_release(static_cast<mcfwindow>(userData), event->x, event->y);
        }
    }

    bMousePressed = FALSE;
    /* post-condition */
    return TRUE;
}

/**
 * Generates a update window event to CMCFController class
 */
bool
update_window (GtkWidget *widget, GdkEventExpose *event,gpointer userData)
{
    MCF_DEBUG();
    /* pre-condition */
    CMCFGwes* gwes = CMCFGwes::get_instance();
    CMCFUIBuilder *builder = CMCFUIBuilder::get_instance();

    if (gwes && builder)	{
        mcfwindow window = static_cast<mcfwindow>(widget);

        return builder->show_layout(window);
    }

    /* post-condition */
    return FALSE;
}


/**
 * Generates a mouse move event to CMCFController class
 */
bool
mouse_move(GtkWidget *widget, GdkEventButton *event, gpointer userData)
{
    MCF_DEBUG();
    /* pre-condition */
    CMCFController *controller;
    CMCFWindows *windows;
    CMCFUtils *utils;

    controller = CMCFController::get_instance();
    windows = CMCFWindows::get_instance();
    utils = CMCFUtils::get_instance();

    mcfint x = event->x;
    mcfint y = event->y;

    if(abs(sLatestXPos -x) < utils->get_scale_x(MIN_XY_DIFF) &&
       abs(sLatestYPos -y) < utils->get_scale_x(MIN_XY_DIFF)) {
        return FALSE;
    }else{
        sLatestXPos = x;
        sLatestYPos = y;
    }

    if (bMousePressed) {
        mcfbyte index = 0;
        mcfwindow window = MCFWINDOW_INVALID;
        McfWindowContext *context = NULL;

        /* First convert the local coordination to global coordination */
        context = windows->get_window_context(static_cast<mcfwindow>(userData), FALSE);
        if (widget == windows->get_magnifier_window()) {
            GtkWidget* widget = static_cast<GtkWidget*>(windows->get_base_window());
            gdk_window_get_pointer(widget->window, &x, &y, NULL);
            context = windows->get_window_context(windows->get_base_window(), FALSE);
        }
        x = context->x + x;
        y = context->y + y;

        mcfboolean bProcessed = FALSE;

        do {
            window = windows->get_nth_window_in_Z_order_list(index);
            context = windows->get_window_context(window, FALSE);

            if (context) {
                if ((x >= context->x && x <= (context->x + context->width)) &&
                    (y >= context->y && y <= (context->y + context->height))) {
                    /* Now convert the global coordination to appropriate local coordination */
                    x = x - context->x;
                    y = y - context->y;

                    if (controller) {
                        controller->mouse_move(window, x, y);
                    }
                }
            }

            index++;
        } while (index < MAX_ZORDER_NUM && window != MCFWINDOW_INVALID && !bProcessed);

    } else if (!bSkipMouseEventsUntilPressed) {
        if (controller) {
            controller->mouse_move(static_cast<mcfwindow>(userData), event->x, event->y);
        }
    }
    /* post-condition */
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
CMCFEventsImplGtk::connect_window_events(const mcfwindow wnd, const mcfint evt)
{
    MCF_DEBUG();
    /* pre-condition */
    mcf_assert_return(wnd != NULL);

    GtkWidget* widget = static_cast<GtkWidget*>(wnd);

    gtk_widget_add_events ((GtkWidget*)widget, GDK_BUTTON_PRESS_MASK |
                           GDK_BUTTON_RELEASE_MASK |
                           GDK_BUTTON_MOTION_MASK |
                           GDK_POINTER_MOTION_HINT_MASK);

    if (evt & MCF_EVENT_MOUSE) {
        g_signal_connect(G_OBJECT(widget), "button_press_event", G_CALLBACK(mouse_press), static_cast<gpointer>(wnd));
        g_signal_connect(G_OBJECT(widget), "button_release_event", G_CALLBACK(mouse_release), static_cast<gpointer>(wnd));
        g_signal_connect(G_OBJECT(widget), "motion_notify_event", G_CALLBACK(mouse_move), static_cast<gpointer>(wnd));
    }

    if (evt & MCF_EVENT_EXPOSE) {
        g_signal_connect(G_OBJECT(widget), "expose_event", G_CALLBACK(update_window), static_cast<gpointer>(wnd));
    }

    /* post-condition */
}

/**
 * Generates a timer event to CMCFController class
 */
gboolean
timer_event(gpointer data)
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
CMCFEventsImplGtk::create_timer(const mcf16 id, const mcf32 interval, mcf16 value, mcfboolean addToMap)
{
    MCF_DEBUG();
    mcfint data = MCF_MAKELONG(id, value);
    gint timerId = gtk_timeout_add (static_cast<guint32>(interval), timer_event, (gpointer)data);
    if(addToMap) {
        idMap[id] = timerId;
    }
}

/**
 * Destroys the given ID's timer
 */
void
CMCFEventsImplGtk::destroy_timer(const mcf32 id)
{
    MCF_DEBUG();
    for ( std::map<int, int>::iterator idx = idMap.begin(); idx != idMap.end(); ++idx) {
        if ((*idx).first == id) {
            gtk_timeout_remove ((*idx).second);
            idMap.erase((*idx).first);
            break;
        }
    }
}

/**
 * Destroys all of created timer
 */
void
CMCFEventsImplGtk::destroy_all_timer()
{
    MCF_DEBUG();
    for ( std::map<int, int>::iterator idx = idMap.begin(); idx != idMap.end(); ++idx) {
        gtk_timeout_remove ((*idx).second);
    }
    idMap.clear();
}

