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

#include "mcftypes.h"
#include "mcfconfig.h"

#ifndef __MCF_EVENTS_H__
#define __MCF_EVENTS_H__

#ifdef __cplusplus
//MCF_BEGIN_DECLS
#endif

namespace mcf
{

/**@brief window event definition */
typedef enum _MCFCbEvent {
    MCF_EVENT_MOUSE = 1 << 1, /**< for mouse event */
    MCF_EVENT_EXPOSE = 1 << 2 /**< for expose */
}MCFCbEvent;

/**@brief window event definition */
typedef enum _MCFMouseEvent {
    MCF_MOUSE_EVENT_PRESS,
    MCF_MOUSE_EVENT_MOVE,
    MCF_MOUSE_EVENT_RELEASE,
}MCFMouseEvent;

/**
 * @brief The base class to work as a soft-based keyboard
 *
 * This class implements all functions for working as a soft-based keyboard
 * In side of ISE developer, they can modify it by their requirements.
 */
class CMCFEventsImpl
{
public :
    virtual void connect_window_events(const mcfwindow wnd, const mcfint evt) = 0;
    virtual void create_timer(const mcf16 id, const mcf32 interval, mcf16 value, mcfboolean addToMap) = 0;
    virtual void destroy_timer(const mcf32 id) = 0;
    virtual void destroy_all_timer() = 0;

    virtual void generate_mouse_event(MCFMouseEvent type, mcf16 x, mcf16 y) = 0;
};

class CMCFEvents
{
public :
    CMCFEvents();
    ~CMCFEvents();

    static CMCFEvents* sInstance; /* For singleton */
    static CMCFEvents* get_instance();

    void connect_window_events(mcfwindow wnd, const mcfint evt);

    void create_timer(const mcf16 id, const mcf32 interval, mcf16 value, mcfboolean addToMap = TRUE) {
        get_mcf_events_impl()->create_timer(id, interval, value, addToMap);
    }

    void destroy_timer(const mcf32 id) {
        get_mcf_events_impl()->destroy_timer(id);
    }

    void destroy_all_timer() {
        get_mcf_events_impl()->destroy_all_timer();
    }

    void set_touch_event_offset(const McfPoint pos);
    McfPoint* get_touch_event_offset();

    virtual void generate_mouse_event(MCFMouseEvent type, mcf16 x, mcf16 y) {
        get_mcf_events_impl()->generate_mouse_event(type, x, y);
    }

protected :
    CMCFEventsImpl* get_mcf_events_impl();

private :
    CMCFEventsImpl* pImp_;
    McfPoint mTouchEventOffset;
};


} /* End of mcf namespace */

#ifdef __cplusplus
//MCF_END_DECLS
#endif

#endif //__MCF_EVENTS_H__
