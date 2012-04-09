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
