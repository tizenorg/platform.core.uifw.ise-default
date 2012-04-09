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

#include "mcffeedback.h"
#include "mcfdebug.h"
#include "mcfresourcecache.h"
#include "mcfutils.h"
#include "mcfcontext.h"

using namespace mcf;
CMCFFeedback* CMCFFeedback::sInstance; /* For singleton */

CMCFFeedback* CMCFFeedback::get_instance()
{
    if (!sInstance) {
        sInstance = new CMCFFeedback();
    }
    return (CMCFFeedback*)sInstance;
}

CMCFFeedback::CMCFFeedback()
{
    MCF_DEBUG();
}

CMCFFeedback::~CMCFFeedback()
{
    MCF_DEBUG();
}

mcfboolean
CMCFFeedback::button_pressed(mcfwindow window, mcfbyte keyindex)
{
    MCF_DEBUG();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    if(cache) {
        const McfLayoutKeyConfigure *configure = cache->get_cur_layout_key_configure(window, keyindex);
        CMCFUtils *utils = CMCFUtils::get_instance();
        CMCFContext *context = CMCFContext::get_instance();
    }
    return TRUE;
}

mcfboolean
CMCFFeedback::button_moved(mcfwindow window, mcfbyte keyindex)
{
    MCF_DEBUG();
    return TRUE;
}

mcfboolean
CMCFFeedback::button_released(mcfwindow window, mcfbyte keyindex)
{
    MCF_DEBUG();
    return TRUE;
}

