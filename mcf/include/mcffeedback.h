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

#ifndef __MCF_FEEDBACK_H__
#define __MCF_FEEDBACK_H__

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
class CMCFFeedback
{
private:
    static CMCFFeedback* sInstance; /* For singleton */

public:
    CMCFFeedback();
    CMCFFeedback(mcfwindow parentWnd);

    ~CMCFFeedback();
    static CMCFFeedback* get_instance();

public:
    mcfboolean button_pressed(mcfwindow window, mcfbyte keyindex);
    mcfboolean button_moved(mcfwindow window, mcfbyte keyindex);
    mcfboolean button_released(mcfwindow window, mcfbyte keyindex);

protected:

};

}

//MCF_END_DECLS

#endif //__MCF_FEEDBACK_H__
