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
