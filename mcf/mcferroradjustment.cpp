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


