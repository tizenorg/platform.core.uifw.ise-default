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

#ifndef __MCF_GRAPHICS_EFL_H__
#define __MCF_GRAPHICS_EFL_H__

#ifdef TEST_NEWBACKEND
#include <Ecore_Evas.h>
#include <Ecore.h>
typedef struct {
    mcfboolean used;

    Evas_Object *image;
    Evas_Object *clipper;

    mcfwindow window;
    mcfchar imgPath[_POSIX_PATH_MAX];
    mcfint imgPathHash;
    mcfint destX;
    mcfint destY;
    mcfint destWidth;
    mcfint destHeight;
    mcfint srcX;
    mcfint srcY;
    mcfint srcWidth;
    mcfint srcHeight;
    mcfboolean extractImg;
} ImageCache;

typedef struct {
    mcfboolean used;

    Evas_Object *text;

    mcfwindow window;
    mcf::McfFontInfo fontinfo;
    McfColor color;
    mcfchar str[_POSIX_PATH_MAX];;
    mcfint strHash;
    mcfint posx;
    mcfint posy;
    mcfint width;
    mcfint height;
    MCFLabelAlignment align;
    mcfbyte paddingx;
    mcfbyte paddingy;
} TextCache;
#else
#endif

namespace mcf
{
class CMCFGraphicsImplEfl : public CMCFGraphicsImpl
{
public :
    CMCFGraphicsImplEfl();
    ~CMCFGraphicsImplEfl();

    void draw_image(mcfwindow window, const mcfdrawctx drawCtx, mcfchar* imgPath,
                    mcfint destX, mcfint destY, mcfint destWidth, mcfint destHeight,
                    mcfint srcX, mcfint srcY, mcfint srcWidth, mcfint srcHeight, mcfboolean extractImg);
    mcfimage load_image(const mcfchar* imgPath);
    void unload_image(mcfimage imgData);

    mcffont create_font(const McfFontInfo& info);
    void destroy_font(mcffont font);
    void draw_text(mcfwindow window, const mcfdrawctx drawCtx, const McfFontInfo& fontinfo, const McfColor& color,
                   const mcfchar *str, mcfint posx, mcfint posy, mcfint width, mcfint height,
                   MCFLabelAlignment align, mcfbyte paddingx, mcfbyte paddingy);

    void draw_rectangle(mcfwindow window, const mcfdrawctx drawCtx, mcfdouble posx, mcfdouble posy,
                        mcfdouble width, mcfdouble height, const mcfdouble lineWidth, const McfColor& lineColor,
                        mcfboolean fill, const McfColor& fillColor, mcfdouble radius, mcffloat alpha);
    mcfdrawctx begin_paint(const mcfwindow window, const mcfboolean forcedraw = FALSE);
    void end_paint(const mcfwindow window, mcfdrawctx drawCtx);
    McfSize get_image_size(mcfchar* imgPath);

private:
};
} /* End of mcf namespace */
#endif
