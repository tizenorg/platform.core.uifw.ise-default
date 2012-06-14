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

