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



#include "mcfwindows.h"
#include <gtk/gtk.h>

#ifndef __MCF_WINDOWS_GTK_H__
#define __MCF_WINDOWS_GTK_H__

namespace mcf
{
class CMCFWindowsImplGtk : public CMCFWindowsImpl
{
public :
    CMCFWindowsImplGtk();
    ~CMCFWindowsImplGtk();

    mcfwindow create_base_window(const mcfwindow parentWnd, McfWindowContext *winCtx, const mcf16 width, const mcf16 height);
    mcfwindow create_window(const mcfwindow parentWnd, McfWindowContext *winCtx, const mcf16 width, const mcf16 height);
    mcfwindow create_dim_window(const mcfwindow parentWnd, McfWindowContext *winCtx, const mcf16 width, const mcf16 height);
    bool destroy_window(mcfwindow window);
    void show_window(const mcfwindow window, mcfboolean queue);
    void hide_window(const mcfwindow window,  const mcfboolean fForce = FALSE);
    void move_window(const mcfwindow window, const mcf16 x, const mcf16 y);
    void resize_window(const mcfwindow window, const mcf16 width, const mcf16 height);
    void move_resize_window(const mcfwindow window, const mcf16 x, const mcf16 y, const mcf16 width, const mcf16 height);
    void update_window(const mcfwindow window, const mcf16 x = 0, const mcf16 y = 0, const mcf16 width = 0, const mcf16 height = 0);
    void set_window_rotation(const mcfwindow window);
    void show_message_box(const mcfwindow parentWnd, const mcf8 msgType, mcfchar* title, mcfchar* msg);
    mcfboolean get_window_rect(const mcfwindow window, McfRectangle *rect);
    void set_parent(const mcfwindow parentWindow, const mcfwindow window);
    void set_keep_above(const mcfwindow window, const mcfboolean keepabove);
};
} /* End of mcf namespace */
#endif __MCF_WINDOWS_GTK_H__
