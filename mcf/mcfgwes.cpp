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

#include "mcfgwes.h"
#include "mcfdebug.h"

using namespace mcf;

CMCFGwes* CMCFGwes::sInstance = NULL; /* For singleton */

CMCFGwes::CMCFGwes()
{
    MCF_DEBUG();
    mWnd = NULL;
    mGrps = NULL;
    mEvt = NULL;
}

CMCFGwes::~CMCFGwes()
{
    MCF_DEBUG();
    if (mWnd) delete(mWnd);
    if (mGrps) delete(mGrps);
    if (mEvt) delete(mEvt);
}

void CMCFGwes::init(mcfwindow parentWnd, mcf16 width, mcf16 height)
{
    MCF_DEBUG();
    if (mWnd == NULL) mWnd = CMCFWindows::get_instance();
    if (mGrps == NULL) mGrps = CMCFGraphics::get_instance();
    if (mEvt == NULL) mEvt = CMCFEvents::get_instance();

    mcfwindow wnd = mWnd->create_base_window(parentWnd, width, height);
    mEvt->connect_window_events(wnd, MCF_EVENT_MOUSE | MCF_EVENT_EXPOSE);
}

CMCFGwes* CMCFGwes::get_instance()
{
    if (!sInstance) {
        sInstance = new CMCFGwes();
    }
    return (CMCFGwes*)sInstance;
}

