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
#include "mcfwindows.h"
#include "mcfgraphics.h"
#include "mcfevents.h"

#ifndef __MCF_GWES_H__
#define __MCF_GWES_H__

#ifdef __cplusplus
//MCF_BEGIN_DECLS
#endif

namespace mcf
{
/**
 * @brief The base class to work as a soft-based keyboard
 *
 * This class implements all functions for working as a soft-based keyboard
 * In side of ISE developer, they can modify it by their requirements.
 */
class CMCFGwes
{
private:
    static CMCFGwes* sInstance; /* For singleton */
    CMCFGwes();
public :
    ~CMCFGwes();

    static CMCFGwes* get_instance();

private:

public:
    void init(mcfwindow parentWnd, mcf16 width, mcf16 height);

    CMCFGraphics *mGrps;
    CMCFWindows *mWnd;
    CMCFEvents *mEvt;
};

} /* End of mcf namespace */

#ifdef __cplusplus
//MCF_END_DECLS
#endif

#endif //__MCF_GWES_H__
