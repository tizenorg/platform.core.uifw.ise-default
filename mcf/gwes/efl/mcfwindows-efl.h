/*
 * Copyright 2012  Samsung Electronics Co., Ltd
 *
 * Licensed under the Flora License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.tizenopensource.org/license
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "mcfwindows.h"
#include <Evas.h>
#include <Eina.h>

#ifndef __MCF_WINDOWS_EFL_H__
#define __MCF_WINDOWS_EFL_H__

typedef enum {
    EFLOBJECT_NONE,
    EFLOBJECT_IMAGE,
    EFLOBJECT_CLIPOBJECT,
    EFLOBJECT_TEXTBLOCK,
    EFLOBJECT_RECTANGLE,
} EFLOBJECT_TYPE;

namespace mcf
{
typedef struct {
    EFLOBJECT_TYPE type;
    McfRectangle position;
    Evas_Object *obj;
    char *etcInfo;
    mcfboolean bExtracted;
    void *data;
} EFLObject;
class CMCFWindowsImplEfl : public CMCFWindowsImpl
{
public :
    CMCFWindowsImplEfl();
    ~CMCFWindowsImplEfl();

    mcfwindow create_base_window(const mcfwindow parentWnd, McfWindowContext *winCtx, const mcf16 width, const mcf16 height);
    mcfwindow create_window(const mcfwindow parentWnd, McfWindowContext *winCtx, const mcf16 width, const mcf16 height);
    mcfwindow create_context_popup_window(const mcfwindow parentWnd);
    mcfwindow create_dim_window(const mcfwindow parentWnd, McfWindowContext *winCtx, const mcf16 width, const mcf16 height);
    bool destroy_window(mcfwindow window);
    void show_window(const mcfwindow window, mcfboolean queue);
    void hide_window(const mcfwindow window,  const mcfboolean fForce = FALSE);
    void move_window(const mcfwindow window, const mcf16 x, const mcf16 y);
    void resize_window(const mcfwindow window, const mcf16 width, const mcf16 height);
    void move_resize_window(const mcfwindow window, const mcf16 x, const mcf16 y, const mcf16 width, const mcf16 height);
    void update_window(const mcfwindow window, const mcf16 x = 0, const mcf16 y = 0, const mcf16 width = 0, const mcf16 height = 0);
    void set_window_rotation(const mcfwindow window, const mcfint degree);
    void show_message_box(const mcfwindow parentWnd, const mcf8 msgType, mcfchar* title, mcfchar* msg);
    mcfboolean get_window_rect(const mcfwindow window, McfRectangle *rect);
    void set_parent(const mcfwindow parentWindow, const mcfwindow window);
    void set_keep_above(const mcfwindow window, const mcfboolean keepabove);
    Evas_Object* get_context_popup_window();
    /* EFL specific utility functions */
    void set_window_accepts_focus(const mcfwindow window, const mcfboolean accepts);
    void destroy_context_popup_window();
protected:
    Evas_Object *mcf_context_window;
};
} /* End of mcf namespace */
#endif
