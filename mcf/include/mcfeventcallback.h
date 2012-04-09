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
