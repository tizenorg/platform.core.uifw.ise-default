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



#ifndef __MCF_EVENTCALLBACK_H__
#define __MCF_EVENTCALLBACK_H__

//MCF_BEGIN_DECLS
#include "mcftypes.h"
#include "mcfconfig.h"
#include "mcfstructs.h"

namespace mcf
{

typedef struct {
    mcfchar *keyValue;
    mcfulong keyEvent;
    MCFKeyType keyType;
    MCFKeyModifier keyModifier;

    /* Extra information for advanced event processing */
    mcftouchdevice touchid;
    mcfint touch_event_order;

    McfPoint mousePressedPt;
    McfPoint mouseCurrentPt;
    McfPoint mouseFarthestPt;
} McfKeyEventDesc;

/**
* @brief The callback interface to handle MCF events
*
* ISE developers can inherit this interface and register it to the MCFCore class
* to handle MCF events as they want.
*/
struct IMCFEventCallback {
    virtual mcfboolean on_event_key_clicked(McfKeyEventDesc keyEventDesc) = 0;
    virtual mcfboolean on_event_drag_state_changed(McfKeyEventDesc keyEventDesc) = 0;
    virtual mcfboolean on_event_notification(MCFNotiType notiType, mcfint etcInfo) = 0;
};

}

//MCF_END_DECLS

#endif //__MCF_EVENTCALLBACK_H__
