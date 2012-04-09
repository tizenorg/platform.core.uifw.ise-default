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

#include "mcfgraphics.h"
#include <gtk/gtk.h>

#ifndef __MCF_GRAPHICS_GTK_H__
#define __MCF_GRAPHICS_GTK_H__

namespace mcf
{
class CMCFGraphicsImplGtk : public CMCFGraphicsImpl
{
public :
    CMCFGraphicsImplGtk();
    ~CMCFGraphicsImplGtk();

    void draw_image(mcfwindow window, const mcfdrawctx drawCtx, mcfchar* imgPath, mcfint destX, mcfint destY,
                    mcfint destWidth, mcfint destHeight, mcfint srcX, mcfint srcY, mcfint srcWidth, mcfint srcHeight);
    mcfimage load_image(const mcfchar* imgPath);
    void unload_image(mcfimage imgData);

    mcffont create_font(const McfFontInfo& info);
    void destroy_font(mcffont font);
    void draw_text(mcfwindow window, const mcfdrawctx drawCtx, const McfFontInfo& fontinfo, const McfColor& color,
                   const mcfchar *str, mcfint posx, mcfint posy, mcfint width, mcfint height,
                   MCFLabelAlignment align, mcfbyte padding);

    void draw_rectangle(mcfwindow window, const mcfdrawctx drawCtx, mcfdouble posx, mcfdouble posy,
                        mcfdouble width, mcfdouble height, const mcfdouble lineWidth, const McfColor& lineColor,
                        mcfboolean fill, const McfColor& fillColor, mcfdouble radius, mcffloat alpha);
    mcfdrawctx begin_paint(const mcfwindow window, const mcfboolean forcedraw = FALSE);
    void end_paint(const mcfwindow window, mcfdrawctx drawCtx);
    McfSize get_image_size(mcfchar* imgPath);

private:
};
} /* End of mcf namespace */
#endif __MCF_GRAPHICS_GTK_H__
