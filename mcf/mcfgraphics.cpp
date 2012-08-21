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

