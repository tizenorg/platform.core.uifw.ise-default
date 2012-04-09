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

#include "mcfevents.h"
#include <gtk/gtk.h>
#include <map>


#ifndef __MCF_EVENTS_GTK_H__
#define __MCF_EVENTS_GTK_H__

namespace mcf
{
class CMCFEventsImplGtk : public CMCFEventsImpl
{
public :
    CMCFEventsImplGtk();
    ~CMCFEventsImplGtk();

    /* Implementation about interface functions */
    void connect_window_events(const mcfwindow wnd, const mcfint evt);
    void create_timer(const mcf16 id, const mcf32 interval, mcf16 value, mcfboolean addToMap);
    void destroy_timer(const mcf32 id);
    void destroy_all_timer();

private:
    std::map<int, int> idMap;
};
} /* End of mcf namespace */
#endif __MCF_EVENTS_GTK_H__
