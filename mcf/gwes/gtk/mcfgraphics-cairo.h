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



#include "mcfgraphics.h"
#include <gtk/gtk.h>

#ifndef __MCF_GRAPHICS_CAIRO_H__
#define __MCF_GRAPHICS_CAIRO_H__

namespace mcf
{
class CMCFGraphicsImplCairo : public CMCFGraphicsImpl
{
public :
    CMCFGraphicsImplCairo();
    ~CMCFGraphicsImplCairo();

    void draw_image(mcfwindow window, const mcfdrawctx drawCtx, mcfchar* imgPath, mcfint destX, mcfint destY,
                    mcfint destWidth, mcfint destHeight, mcfint srcX, mcfint srcY, mcfint srcWidth, mcfint srcHeight);
    void draw_text(mcfwindow window, const mcfdrawctx drawCtx, const McfFontInfo& fontinfo, const McfColor& color,
                   const mcfchar *str, mcfint posx, mcfint posy, mcfint width, mcfint height,
                   MCFLabelAlignment align, mcfbyte padding);
    mcfimage load_image(const mcfchar* imgPath);
    void unload_image(mcfimage imgData);
    mcfdrawctx begin_paint(const mcfwindow window, const mcfboolean forcedraw = FALSE);
    void end_paint(const mcfwindow window, mcfdrawctx drawCtx);
    mcffont create_font(const McfFontInfo& info);
    void destroy_font(mcffont font);
    void draw_rectangle(mcfwindow window, const mcfdrawctx drawCtx, mcfdouble posx, mcfdouble posy,
                        mcfdouble width, mcfdouble height, const mcfdouble lineWidth, const McfColor& lineColor,
                        mcfboolean fill, const McfColor& fillColor, mcfdouble radius, mcffloat alpha);
    McfSize get_image_size(mcfchar* imgPath);
private:
};
} /* End of mcf namespace */
#endif __MCF_GRAPHICS_CAIRO_H__
