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


#include "mcfevents.h"
#ifdef  __WIN32__
#include "mcfevents-win32.h"
#elif defined(__EFL__)
#include "mcfevents-efl.h"
#else
#include "mcfevents-gtk.h"
#endif
#include "mcfdebug.h"
#include "mcfwindows.h"

using namespace mcf;

CMCFEvents* CMCFEvents::sInstance = NULL; /* For singleton */

CMCFEvents::CMCFEvents()
{
    MCF_DEBUG();
    pImp_ = 0;
    mTouchEventOffset.x = mTouchEventOffset.y = 0;
}

CMCFEvents::~CMCFEvents()
{
    MCF_DEBUG();
}

CMCFEventsImpl* CMCFEvents::get_mcf_events_impl()
{
    MCF_DEBUG();
    if (pImp_ == 0) {
#ifdef  __WIN32__
        pImp_ = new CMCFEventsImplWin32;
#elif defined(__EFL__)
        pImp_ = new CMCFEventsImplEfl;
#else
        pImp_ = new CMCFEventsImplGtk;
#endif
    }
    return pImp_;
}

CMCFEvents* CMCFEvents::get_instance()
{
    if (!sInstance) {
        sInstance = new CMCFEvents();
    }
    return (CMCFEvents*)sInstance;
}

void
CMCFEvents::connect_window_events( mcfwindow wnd, const mcfint evt )
{
    CMCFWindows *windows = CMCFWindows::get_instance();
    McfWindowContext *context = windows->get_window_context(wnd, FALSE);
    if (context) {
        if (!(context->isVirtual)) {
            get_mcf_events_impl()->connect_window_events(wnd, evt);
        }
    }
}

void
CMCFEvents::set_touch_event_offset(const McfPoint pos)
{
    mTouchEventOffset = pos;
}

McfPoint*
CMCFEvents::get_touch_event_offset()
{
    return &mTouchEventOffset;
}


