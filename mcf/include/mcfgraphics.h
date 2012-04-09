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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcftypes.h"
#include "mcfconfig.h"
#include "mcfstructs.h"

#ifndef __MCF_GRAPHICS_H__
#define __MCF_GRAPHICS_H__

#ifdef __cplusplus
//MCF_BEGIN_DECLS
#endif

namespace mcf
{
#define MAX_FONT_NAME_LEN 32
/**@brief  font information structure */
typedef struct _McfFontInfo {
    mcfchar fontname[MAX_FONT_NAME_LEN];
    mcfshort fontsize;
    mcfboolean isItalic;
    mcfboolean isBold;
} McfFontInfo;

const McfColor MCFCOLOR_WHITE = {255, 255, 255};
const McfColor MCFCOLOR_GREY = {128, 128, 128};
const McfColor MCFCOLOR_BLACK = {0, 0, 0};
const McfColor MCFCOLOR_RED = {255, 0, 0};
const McfColor MCFCOLOR_BLUE = {0, 0, 255};

/**
 * @brief The base class to work as a soft-based keyboard
 *
 * This class implements all functions for working as a soft-based keyboard
 * In side of ISE developer, they can modify it by their requirements.
 */
class CMCFGraphicsImpl
{
    friend class CMCFGraphics;
private:
    virtual void draw_image(mcfwindow window, const mcfdrawctx drawCtx, mcfchar* imgPath, 
                            mcfint destX, mcfint destY, mcfint destWidth, mcfint destHeight, 
                            mcfint srcX, mcfint srcY, mcfint srcWidth, mcfint srcHeight,
                            mcfboolean extractImg) = 0;
    virtual void draw_text(mcfwindow window, const mcfdrawctx drawCtx, const McfFontInfo& fontinfo, const McfColor& color,
                           const mcfchar *str, mcfint posx, mcfint posy,
                           mcfint width, mcfint height, MCFLabelAlignment align, mcfbyte paddingx, mcfbyte paddingy) = 0;
    virtual mcfdrawctx begin_paint(const mcfwindow window, const mcfboolean forcedraw = FALSE) = 0;
    virtual void end_paint(const mcfwindow window, mcfdrawctx drawCtx) = 0;
    virtual mcfimage load_image(const mcfchar *imgPath) = 0;
    virtual void unload_image(mcfimage imgData) = 0;
    virtual mcffont create_font(const McfFontInfo& info) = 0;
    virtual void destroy_font(mcffont font) = 0;
    virtual void draw_rectangle(mcfwindow window, const mcfdrawctx drawCtx, mcfdouble posx, mcfdouble posy,
                                mcfdouble width, mcfdouble height, const mcfdouble lineWidth, const McfColor& lineColor, mcfboolean fill, const McfColor& fillColor, mcfdouble radius, mcffloat alpha) = 0;
    virtual McfSize get_image_size(mcfchar* imgPath) = 0;
};

class CMCFGraphics
{
    friend class CMCFImageProxy;
    friend class CMCFFontProxy;

private:
    static CMCFGraphics* sInstance; /* For singleton */
    CMCFGraphics();
public :
    ~CMCFGraphics();

    static CMCFGraphics* get_instance();

    void draw_image(mcfwindow window, const mcfdrawctx drawCtx, mcfchar* imgPath, mcfint destX, mcfint destY,
                    mcfint destWidth = -1, mcfint destHeight = -1, mcfint srcX = 0, mcfint srcY = 0,
                    mcfint srcWidth = -1, mcfint srcHeight = -1, mcfboolean extractImg = FALSE) {
        get_mcf_graphics_impl()->draw_image(window, drawCtx, imgPath, destX, destY, destWidth, destHeight,
        		                            srcX, srcY, srcWidth, srcHeight, extractImg);
    }

    void draw_text(mcfwindow window, const mcfdrawctx drawCtx, const McfFontInfo& fontinfo, const McfColor& color,
                   const mcfchar *str, mcfint posx, mcfint posy, mcfint width = 0, mcfint height = 0, 
                   MCFLabelAlignment align = LABEL_ALIGN_LEFT_TOP, mcfbyte paddingx = 0, mcfbyte paddingy = 0) {
        get_mcf_graphics_impl()->draw_text(window, drawCtx, fontinfo, color, str, 
                                           posx, posy, width, height, align, paddingx, paddingy);
    }

    mcfdrawctx begin_paint(const mcfwindow window, const mcfboolean forcedraw = FALSE) {
        return get_mcf_graphics_impl()->begin_paint(window, forcedraw);
    }

    void end_paint(const mcfwindow window, mcfdrawctx drawCtx) {
        return get_mcf_graphics_impl()->end_paint(window, drawCtx);
    }

    void draw_rectangle(mcfwindow window, const mcfdrawctx drawCtx, mcfdouble posx, mcfdouble posy,
                        mcfdouble width, mcfdouble height, const mcfdouble lineWidth, const McfColor& lineColor,
                        mcfboolean fill, const McfColor& fillColor, mcfdouble radius = 0, mcffloat alpha = 1.0) {
        get_mcf_graphics_impl()->draw_rectangle(window, drawCtx, posx, posy, width, height, lineWidth,
        		                                lineColor, fill, fillColor, radius, alpha);
    }

    McfSize get_image_size(mcfchar* imgPath) {
        return get_mcf_graphics_impl()->get_image_size(imgPath);
    }

private:
    mcfimage load_image(const mcfchar *imgPath) {
        return get_mcf_graphics_impl()->load_image(imgPath);
    }

    void unload_image(mcfimage imgData) {
        return get_mcf_graphics_impl()->unload_image(imgData);
    }

    mcffont create_font(const McfFontInfo& info) {
        return get_mcf_graphics_impl()->create_font(info);
    }
    void destroy_font(mcffont font) {
        get_mcf_graphics_impl()->destroy_font(font);
    }

protected :
    CMCFGraphicsImpl* get_mcf_graphics_impl();

private :
    CMCFGraphicsImpl* pImp_;
};


} /* End of mcf namespace */

#ifdef __cplusplus
//MCF_END_DECLS
#endif

#endif //__MCF_GRAPHICS_H__
