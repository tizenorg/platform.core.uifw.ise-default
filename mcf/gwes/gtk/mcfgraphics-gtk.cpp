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

#include "mcfgraphics-gtk.h"
#include "mcfimageproxy.h"
#include "mcffontproxy.h"
#include "mcfdebug.h"
#include "mcfwindows.h"

using namespace mcf;

/**
 * Constructor
 */
CMCFGraphicsImplGtk::CMCFGraphicsImplGtk()
{
    MCF_DEBUG();
    /* Initializes all window resources */
}

/**
 * De-constructor
 */
CMCFGraphicsImplGtk::~CMCFGraphicsImplGtk()
{
    MCF_DEBUG();
}


void
CMCFGraphicsImplGtk::draw_image(mcfwindow window, const mcfdrawctx drawCtx, mcfchar* imgPath, mcfint destX, mcfint destY,
                                mcfint destWidth, mcfint destHeight, mcfint srcX, mcfint srcY, mcfint srcWidth, mcfint srcHeight)
{
    MCF_DEBUG();
    /* pre-condition */
    mcf_assert_return(imgPath);

    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFImageProxy *proxy = CMCFImageProxy::get_instance();

    GdkPixbuf *pixbuf;
    GdkPixbuf *temp;
    GdkColormap *rgba;
    GdkBitmap* mask = NULL;

    if (windows && proxy) {
        GdkPixmap* imgdata = static_cast<GdkPixmap*>(proxy->get_image(imgPath));
        GtkWidget* widget = static_cast<GtkWidget*>(window);

        if (imgdata && widget) {
            gdk_draw_drawable(widget->window,
                              widget->style->fg_gc[GTK_STATE_NORMAL],
                              imgdata,
                              srcX, srcY,
                              destX, destY,
                              destWidth, destHeight
                              );
        }
    }
}

static GdkPixmap*
load_pixmap(const gchar *filename)
{
    MCF_DEBUG();

    GdkPixbuf *pixbuf = NULL;
    GdkPixbuf *temp = NULL;
    guchar *data = NULL;
    int width = 0;
    int height = 0;
    int stride = 0;
    int i =0;

    GdkPixmap *pixmap = NULL;
    GdkColormap *rgba;
    GdkBitmap* mask = NULL;
    if (g_file_test(filename,G_FILE_TEST_EXISTS)) {
        pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
    } else {
        return NULL;
    }

    if (!gdk_pixbuf_get_has_alpha(pixbuf)) {
        temp = pixbuf;
        pixbuf = gdk_pixbuf_add_alpha(temp, FALSE, 0, 0, 0);
        if (temp) {
            g_object_unref((temp));
        }
    }

    width = gdk_pixbuf_get_width(pixbuf);
    height = gdk_pixbuf_get_height(pixbuf);
    pixmap = gdk_pixmap_new(NULL, width, height, 32);
    gdk_draw_pixbuf (pixmap,
                     NULL,
                     pixbuf,
                     0,
                     0,
                     0,
                     0,
                     width,
                     height,
                     GDK_RGB_DITHER_NONE,
                     0,
                     0);
    g_object_unref(pixbuf);
    return pixmap;
}

mcfimage
CMCFGraphicsImplGtk::load_image(const mcfchar *imgPath)
{
    MCF_DEBUG();
    GdkPixmap* imgData = NULL;

    imgData = load_pixmap(imgPath);

    return imgData;
}

void
CMCFGraphicsImplGtk::unload_image(mcfimage imgData)
{
    MCF_DEBUG();
    /* pre-condition */
    mcf_assert_return(imgData);

    g_object_unref(static_cast<GdkPixmap*>(imgData));
}

/**
 * Initializes the drawing context for double-buffering.
 * This func should be called before using a drawing primitive at first.
 */
mcfdrawctx
CMCFGraphicsImplGtk::begin_paint(const mcfwindow window, const mcfboolean forcedraw /* = FALSE */)
{
    MCF_DEBUG();
    /* pre-condition */
    cairo_t *cr = NULL;
    /* Cairo contexts created in a GTK+ expose event handler cannot
    be cached and reused between different expose events */
    GtkWidget* widget;
    widget = static_cast<GtkWidget*>(window);
    if (!GTK_WIDGET_REALIZED(widget)) {
        gtk_widget_realize(widget);
    }
    cr = gdk_cairo_create (widget->window);
    return static_cast<mcfdrawctx>(cr);
}

/**
 * Notices that drawing tasks have done.
 */
void
CMCFGraphicsImplGtk::end_paint(const mcfwindow window, mcfdrawctx drawCtx)
{
    cairo_destroy(static_cast<cairo_t*>(drawCtx));
}

mcffont
CMCFGraphicsImplGtk::create_font(const McfFontInfo& info)
{
    return NULL;
}

void
CMCFGraphicsImplGtk::destroy_font(mcffont font)
{
}

/**
 * Draws the given text on cairo-surface
 */
void
CMCFGraphicsImplGtk::draw_text(mcfwindow window, const mcfdrawctx drawCtx, const McfFontInfo& fontinfo, const McfColor& color,
                               const mcfchar *str, mcfint posx, mcfint posy, mcfint width, mcfint height,
                               MCFLabelAlignment align, mcfbyte padding)
{
    MCF_DEBUG();
    /* pre-condition */
    mcf_assert_return(str);
    mcf_assert_return(drawCtx);

    cairo_t* cr = static_cast<cairo_t*>(drawCtx);
    cairo_identity_matrix(cr);
    cairo_set_source_rgba(cr, color.r / 256.0, color.g / 256.0, color.b / 256.0, 1.0);
    cairo_select_font_face(cr, fontinfo.fontname,
                           fontinfo.isItalic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL,
                           fontinfo.isBold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);

    cairo_set_font_size(cr, fontinfo.fontsize);

    cairo_text_extents_t textExtents;
    cairo_text_extents(cr, str, &textExtents);
    cairo_text_extents_t barExtents;
    cairo_text_extents(cr, "|", &barExtents);

    if(textExtents.width > width) {
        int newSize = fontinfo.fontsize - (fontinfo.fontsize * (float)(textExtents.width - width)) / width - 1;
        cairo_set_font_size(cr, newSize);

        cairo_text_extents(cr, str, &textExtents);
        cairo_text_extents(cr, "|", &barExtents);
    }

    if (align == LABEL_ALIGN_CENTER_TOP || align == LABEL_ALIGN_CENTER_MIDDLE || align == LABEL_ALIGN_CENTER_BOTTOM) {
        posx = posx + ((width - textExtents.width) / 2);
    } else if (align == LABEL_ALIGN_RIGHT_TOP || align == LABEL_ALIGN_RIGHT_MIDDLE || align == LABEL_ALIGN_RIGHT_BOTTOM) {
        posx = posx + (width - textExtents.width) - padding;
    } else {
        posx += padding;
    }
    if (align == LABEL_ALIGN_LEFT_MIDDLE || align == LABEL_ALIGN_CENTER_MIDDLE || align == LABEL_ALIGN_RIGHT_MIDDLE) {
        posy = posy + ((height - barExtents.height) / 2);
    } else if (align == LABEL_ALIGN_LEFT_BOTTOM || align == LABEL_ALIGN_CENTER_BOTTOM || align == LABEL_ALIGN_RIGHT_BOTTOM) {
        posy = posy + (height - barExtents.height) - padding;
    } else {
        posy += padding;
    }

    /* FIXME : A workaround for displaying '_' correctly on its position */
    /* MUST change the line below, with a full understanding on font drawing system */
    cairo_move_to(cr, posx - textExtents.x_bearing, posy - barExtents.y_bearing);
    cairo_show_text(cr, str);
}

/**
 * Draws a rectangle on cairo-surface
 */
void
CMCFGraphicsImplGtk::draw_rectangle(mcfwindow window, const mcfdrawctx drawCtx, mcfdouble posx, mcfdouble posy,
                                    mcfdouble width, mcfdouble height, const mcfdouble lineWidth, const McfColor& lineColor,
                                    mcfboolean fill, const McfColor& fillColor, mcfdouble radius, mcffloat alpha)
{
    MCF_DEBUG();
    /* pre-condition */
    mcf_assert_return(drawCtx);

    cairo_t* cr = static_cast<cairo_t*>(drawCtx);
    cairo_identity_matrix(cr);

    double x1,y1;

    x1=posx+width;
    y1=posy+height;
    if (!width || !height)
        return;
    if (width/2<radius) {
        if (height/2<radius) {
            cairo_move_to  (cr, posx, (posy + y1)/2);
            cairo_curve_to (cr, posx ,posy, posx, posy, (posx + x1)/2, posy);
            cairo_curve_to (cr, x1, posy, x1, posy, x1, (posy + y1)/2);
            cairo_curve_to (cr, x1, y1, x1, y1, (x1 + posx)/2, y1);
            cairo_curve_to (cr, posx, y1, posx, y1, posx, (posy + y1)/2);
        } else {
            cairo_move_to  (cr, posx, posy + radius);
            cairo_curve_to (cr, posx ,posy, posx, posy, (posx + x1)/2, posy);
            cairo_curve_to (cr, x1, posy, x1, posy, x1, posy + radius);
            cairo_line_to (cr, x1 , y1 - radius);
            cairo_curve_to (cr, x1, y1, x1, y1, (x1 + posx)/2, y1);
            cairo_curve_to (cr, posx, y1, posx, y1, posx, y1- radius);
        }
    } else {
        if (height/2<radius) {
            cairo_move_to  (cr, posx, (posy + y1)/2);
            cairo_curve_to (cr, posx , posy, posx , posy, posx + radius, posy);
            cairo_line_to (cr, x1 - radius, posy);
            cairo_curve_to (cr, x1, posy, x1, posy, x1, (posy + y1)/2);
            cairo_curve_to (cr, x1, y1, x1, y1, x1 - radius, y1);
            cairo_line_to (cr, posx + radius, y1);
            cairo_curve_to (cr, posx, y1, posx, y1, posx, (posy + y1)/2);
        } else {
            cairo_move_to  (cr, posx, posy + radius);
            cairo_curve_to (cr, posx , posy, posx , posy, posx + radius, posy);
            cairo_line_to (cr, x1 - radius, posy);
            cairo_curve_to (cr, x1, posy, x1, posy, x1, posy + radius);
            cairo_line_to (cr, x1 , y1 - radius);
            cairo_curve_to (cr, x1, y1, x1, y1, x1 - radius, y1);
            cairo_line_to (cr, posx + radius, y1);
            cairo_curve_to (cr, posx, y1, posx, y1, posx, y1- radius);
        }
    }
    cairo_close_path (cr);

    if (fill) {
        cairo_set_source_rgba (cr, fillColor.r / 256.0, fillColor.g / 256.0, fillColor.b / 256.0, alpha); //0.9
        cairo_fill_preserve (cr);
    }

    cairo_set_source_rgb (cr, lineColor.r / 256.0, lineColor.g / 256.0, lineColor.b / 256.0); // 1  . 0
    cairo_set_line_width (cr, lineWidth);//5.0
    cairo_stroke (cr);
}

McfSize
CMCFGraphicsImplGtk::get_image_size(mcfchar* imgPath)
{
    MCF_DEBUG();
    McfSize ret = { 0, 0 };
    GdkPixbuf *pixbuf = NULL;
    if (g_file_test(imgPath,G_FILE_TEST_EXISTS)) {
        pixbuf = gdk_pixbuf_new_from_file(imgPath, NULL);
    } else {
        return ret;
    }

    ret.width = gdk_pixbuf_get_width(pixbuf);
    ret.height = gdk_pixbuf_get_height(pixbuf);
    g_object_unref(pixbuf);
    return ret;
}


