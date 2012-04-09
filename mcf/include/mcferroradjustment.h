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

#include "mcftypes.h"
#include "mcfstructs.h"

#ifndef __MCF_ERRORADJUSTMENT_H__
#define __MCF_ERRORADJUSTMENT_H__

//MCF_BEGIN_DECLS

#define DEFAULT_VIBE_DURATION 100

namespace mcf
{
/**
 * @brief The base class to work as a soft-based keyboard
 *
 * This class implements all functions for working as a soft-based keyboard
 * In side of ISE developer, they can modify it by their requirements.
 */
class CMCFErrorAdjustment
{
private:
    static CMCFErrorAdjustment* sInstance; /* For singleton */

public:
    CMCFErrorAdjustment();
    CMCFErrorAdjustment(mcfwindow parentWnd);

    ~CMCFErrorAdjustment();
    static CMCFErrorAdjustment* get_instance();

private:

public:
    mcfboolean apply_touch_offset(MCFTouchOffsetLevel level, mcfint *x, mcfint *y);
 
protected:

};

}

//MCF_END_DECLS

#endif //__MCF_ERRORADJUSTMENT_H__
