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

#include "mcferroradjustment.h"
#include "mcfdebug.h"
#include "mcfresourcecache.h"
#include "mcfutils.h"

#define OFFSET_MAX 			10
#define TWO 				2

using namespace mcf;
CMCFErrorAdjustment* CMCFErrorAdjustment::sInstance; /* For singleton */

CMCFErrorAdjustment* CMCFErrorAdjustment::get_instance()
{
    if (!sInstance) {
        sInstance = new CMCFErrorAdjustment();
    }
    return (CMCFErrorAdjustment*)sInstance;
}

CMCFErrorAdjustment::CMCFErrorAdjustment()
{
    MCF_DEBUG();
}

CMCFErrorAdjustment::~CMCFErrorAdjustment()
{
    MCF_DEBUG();
}

mcfboolean
CMCFErrorAdjustment::apply_touch_offset(MCFTouchOffsetLevel level, mcfint *x, mcfint *y)
{
    MCF_DEBUG();
    CMCFEvents* events = CMCFEvents::get_instance();
    CMCFUtils *utils = CMCFUtils::get_instance();
    McfPoint *pos;
    pos = events->get_touch_event_offset();
    if(level == TOUCH_OFFSET_LEVEL_1 ){
        *x = *x + utils->get_scale_x(pos->x);
    } else if(level == TOUCH_OFFSET_LEVEL_2) {
        *x = *x + utils->get_scale_x(pos->x);
    }	
    *y = *y + utils->get_scale_y(pos->y);
    return TRUE;
}


