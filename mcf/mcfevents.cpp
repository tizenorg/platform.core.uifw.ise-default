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


