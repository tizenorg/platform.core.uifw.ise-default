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

