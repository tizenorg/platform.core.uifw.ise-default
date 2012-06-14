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
