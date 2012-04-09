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

#define __GTK__ 1

#include "mcfgraphics.h"
#ifdef  __WIN32__
#include "mcfgraphics-win32.h"
#elif defined(__EFL__)
#include "mcfgraphics-efl.h"
#elif __GTK__
#include "mcfgraphics-gtk.h"
#else
#include "mcfgraphics-cairo.h"
#endif
#include "mcfdebug.h"

using namespace mcf;

CMCFGraphics* CMCFGraphics::sInstance = NULL; /* For singleton */

CMCFGraphics::CMCFGraphics()
{
    MCF_DEBUG();
    pImp_ = 0;
}

CMCFGraphics::~CMCFGraphics()
{
    MCF_DEBUG();
}

CMCFGraphicsImpl* CMCFGraphics::get_mcf_graphics_impl()
{
    MCF_DEBUG();
    if (pImp_ == 0) {
#ifdef  __WIN32__
        pImp_ = new CMCFGraphicsImplWin32;
#elif defined(__EFL__)
        pImp_ = new CMCFGraphicsImplEfl;
#elif __GTK__
        pImp_ = new CMCFGraphicsImplGtk;
#else
        pImp_ = new CMCFGraphicsImplCairo;
#endif
    }
    return pImp_;
}

CMCFGraphics* CMCFGraphics::get_instance()
{
    if (!sInstance) {
        sInstance = new CMCFGraphics();
    }
    return (CMCFGraphics*)sInstance;
}

