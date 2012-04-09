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
