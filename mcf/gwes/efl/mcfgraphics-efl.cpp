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


#include "mcfgraphics-efl.h"
#include "mcfimageproxy.h"
#include "mcffontproxy.h"
#include "mcfdebug.h"
#include "mcfwindows.h"
#include "mcfresourcecache.h"
#include "mcfwindows-efl.h"
#include "mcfutils.h"
#include "mcfcontext.h"
#include <Ecore_Evas.h>
#include <Ecore.h>
#include <Elementary.h>
#include <vector>

#ifdef TEST_NEWBACKEND
std::vector<ImageCache> g_ImageCache;
std::vector<TextCache> g_TextCache;
mcfint hash_string(const mcfchar* str) {
    mcfint ret = 0;
    mcfint len = strlen(str);
    for(mcfint loop = 0;loop < len && str[loop];loop++) {
        ret = ((loop + 1) * str[loop]);
    }

    return ret;
}
#else
#endif
using namespace mcf;

int iNumCreate = 0;

extern void mouse_press (void *data, Evas *e, Evas_Object *obj, void *event_info);
extern void mouse_release (void *data, Evas *e, Evas_Object *obj, void *event_info);
extern void mouse_move (void *data, Evas *e, Evas_Object *obj, void *event_info);

/**
 * Constructor
 */
CMCFGraphicsImplEfl::CMCFGraphicsImplEfl()
{
    MCF_DEBUG();
    /* Initializes all window resources */
}

/**
 * De-constructor
 */
CMCFGraphicsImplEfl::~CMCFGraphicsImplEfl()
{
    MCF_DEBUG();
}

Evas_Object* extract_partimage_from_fullimage(Evas_Object* fullimage,
                                              int img_x,int img_y,
                                              int cell_x,int cell_y,
                                              int cell_cx, int cell_cy)
{
    unsigned int *data;
    unsigned int *t_data;
    int i,j;
    int w,h;
    Evas_Object *image_ob;

    evas_object_image_size_get(fullimage, &w, &h);

    data = (unsigned int*)evas_object_image_data_get(fullimage,0);
    if (data == NULL) {
        return NULL;
    }

    t_data=(unsigned int*)malloc(sizeof(unsigned int)*cell_cx*cell_cy);
    if (t_data == NULL) {
        free(data);
        return NULL;
    }

    for (i=img_y; i<img_y+cell_cy; i++) {
        for (j=img_x; j<img_x+cell_cx; j++) {
            t_data[(i-img_y)*cell_cx+(j-img_x)] = data[i*w+j];
        }
    }

    image_ob = evas_object_image_add(evas_object_evas_get(fullimage));
    if (image_ob == NULL) {
        free(data);
        free(t_data);
        return NULL;
    }
    evas_object_image_size_set(image_ob,cell_cx,cell_cy);
    evas_object_image_data_set(image_ob,t_data);
    evas_object_image_fill_set(image_ob,0,0,cell_cx,cell_cy);
    evas_object_resize(image_ob,cell_cx,cell_cy);

    evas_object_show(image_ob);
    //Changed for Memory Leak
    free(t_data);
    return image_ob;
}

void
CMCFGraphicsImplEfl::draw_image(mcfwindow window, const mcfdrawctx drawCtx, mcfchar* imgPath,
                                mcfint destX, mcfint destY, mcfint destWidth, mcfint destHeight,
                                mcfint srcX, mcfint srcY, mcfint srcWidth, mcfint srcHeight, mcfboolean extractImg)
{
    MCF_DEBUG();

    mcf_assert_return(imgPath);

    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFUtils *utils = CMCFUtils::get_instance();
    McfWindowContext *winctx = NULL;
    McfWindowContext *targetctx = NULL;
    if(windows) {
        winctx = windows->get_window_context(window, FALSE);
        targetctx = windows->get_window_context(drawCtx, FALSE);
    }

    if (winctx && targetctx && imgPath && utils && cache && windows) {
        if (strlen(imgPath) > 0) {
#ifdef TEST_NEWBACKEND
            mcfboolean bFound = FALSE;
            mcfboolean bOrgSizeMinusOne = (srcWidth == -1 && srcHeight == -1);
            mcfint hashval = hash_string(imgPath);
            for(mcfint loop = 0;loop < g_ImageCache.size() && !bFound;loop++) {
                if(g_ImageCache[loop].used &&
                   window == g_ImageCache[loop].window &&
                   hashval == g_ImageCache[loop].imgPathHash &&
                   destX == g_ImageCache[loop].destX &&
                   destY == g_ImageCache[loop].destY &&
                   destWidth == g_ImageCache[loop].destWidth &&
                   destHeight == g_ImageCache[loop].destHeight &&
                   srcX == g_ImageCache[loop].srcX &&
                   srcY == g_ImageCache[loop].srcY &&
                   srcWidth == g_ImageCache[loop].srcWidth &&
                   srcHeight == g_ImageCache[loop].srcHeight &&
                   extractImg == g_ImageCache[loop].extractImg) {
                        if(strcmp(imgPath, g_ImageCache[loop].imgPath) == 0) {
                            bFound = TRUE;
                            evas_object_show(g_ImageCache[loop].image);
                            evas_object_raise(g_ImageCache[loop].image);
                            if(g_ImageCache[loop].clipper) {
                                evas_object_show(g_ImageCache[loop].clipper);
                                evas_object_raise(g_ImageCache[loop].clipper);
                            }
                        }
                    }
            }
            if(!bFound) {
#endif
            EFLObject *obj = new EFLObject;
            EFLObject *clipobj = NULL;
            if (obj) {
                Evas_Object *winobj = (Evas_Object*)window;

                Evas *evas = evas_object_evas_get(winobj);
                Evas_Object *imgobj = evas_object_image_add(evas);

                obj->bExtracted = FALSE;

                if (imgobj) {
                    int imgWidth = 0;
                    int imgHeight = 0;
                    evas_object_image_file_set(imgobj, imgPath, NULL);
                    evas_object_image_size_get(imgobj, &imgWidth, &imgHeight);
                    const McfNinePatchInfo *nine_patch_info = utils->get_nine_patch_info(imgPath);
                    if(nine_patch_info) {
                        evas_object_image_border_set(imgobj,
                                                     nine_patch_info->left, nine_patch_info->right,
                                                     nine_patch_info->top, nine_patch_info->bottom);
                    }
                    const McfLayout *layout = cache->get_cur_layout(window);
                    if(layout) {
                        if(layout->displayType == MCFDISPLAY_PORTRAIT) {
                            imgWidth = utils->get_scale_x(imgWidth);
                            imgHeight = utils->get_scale_y(imgHeight);
                        } else {
                            imgWidth = utils->get_scale_y(imgWidth);
                            imgHeight = utils->get_scale_x(imgHeight);
                        }
                    }
                    if(srcWidth == -1 && srcHeight == -1) {
                        srcWidth = imgWidth;
                        srcHeight = imgHeight;
                    }
                    if((srcWidth > 0 && srcHeight > 0) && (imgWidth != destWidth || imgHeight != destHeight) && extractImg) {
#ifdef EXTRACT_PARTIMAGE
                        Evas_Object *newobj = extract_partimage_from_fullimage(imgobj, srcX, srcY, 0, 0, srcWidth, srcHeight);
                        obj->bExtracted = TRUE;
                        evas_object_del(imgobj);
                        imgobj = newobj;
                        evas_object_move(imgobj, destX, destY);
                        if (destWidth > 0 && destHeight > 0) {
                            evas_object_image_fill_set(imgobj, 0, 0, destWidth,destHeight);
                            evas_object_resize(imgobj, destWidth, destHeight);
                        }
#else
                        evas_object_move(imgobj, destX - srcX, destY - srcY);
                        evas_object_image_fill_set(imgobj, 0, 0, imgWidth, imgHeight);
                        evas_object_resize(imgobj, imgWidth, imgHeight);

                        Evas_Object *clipper = evas_object_rectangle_add(evas);
                        evas_object_color_set(clipper, 255, 255, 255, 255);
                        evas_object_move(clipper, destX, destY);
                        evas_object_resize(clipper, destWidth, destHeight);
                        evas_object_clip_set(imgobj, clipper);
                        evas_object_show(clipper);

                        clipobj = new EFLObject;
                        clipobj->obj = clipper;
                        clipobj->type = EFLOBJECT_CLIPOBJECT;
                        clipobj->position.x = destX;
                        clipobj->position.y = destY;
                        clipobj->position.width = destWidth;
                        clipobj->position.height = destHeight;
                        clipobj->etcInfo = imgPath;
                        clipobj->bExtracted = FALSE;
                        clipobj->data = NULL;

                        if(winctx->isVirtual) {
                            McfWindowContext *basectx = windows->get_window_context(windows->get_base_window());
                            if(basectx) {
                                clipobj->position.x -= (winctx->x - basectx->x);
                                clipobj->position.y -= (winctx->y - basectx->y);
                            }
                        }
#endif
                    } else {
                        evas_object_move(imgobj, destX, destY);
                        if (destWidth > 0 && destHeight > 0) {
                            evas_object_image_fill_set(imgobj, 0, 0, destWidth,destHeight);
                            evas_object_resize(imgobj, destWidth, destHeight);
                        }
                    }
                    evas_object_raise(imgobj);
                    evas_object_show(imgobj);

                    obj->obj = imgobj;
                    obj->type = EFLOBJECT_IMAGE;
                    obj->position.x = destX;
                    obj->position.y = destY;
                    obj->position.width = destWidth;
                    obj->position.height = destHeight;
                    obj->etcInfo = imgPath;
                    obj->data = NULL;

                    if(winctx->isVirtual) {
                        McfWindowContext *basectx = windows->get_window_context(windows->get_base_window());
                        if(basectx) {
                            obj->position.x -= (winctx->x - basectx->x);
                            obj->position.y -= (winctx->y - basectx->y);
                        }
                    }

                    targetctx->etcInfoPtr = eina_list_append((Eina_List*)(targetctx->etcInfoPtr), obj);
                    if(clipobj) {
                        targetctx->etcInfoPtr = eina_list_append((Eina_List*)(targetctx->etcInfoPtr), clipobj);
                    }

                    /* FIXME : this is for placing the background image at the lowest depth */
                    mcfint window_layer = 29000;
                    if(!windows->is_base_window(reinterpret_cast<mcfwindow>(drawCtx))) {
                        window_layer = 29010;
                    }

                    if(winctx->width == destWidth && winctx->height == destHeight) {
                        evas_object_layer_set(imgobj, window_layer + 0);
                    } else {
                        evas_object_layer_set(imgobj, window_layer + 1);
                    }
                }
            }
#ifdef TEST_NEWBACKEND
                ImageCache cache;
                cache.used = true;
                cache.window = window;
                strncpy(cache.imgPath, imgPath, sizeof(cache.imgPath));
                cache.imgPathHash = hashval;
                cache.destX = destX;
                cache.destY = destY;
                cache.destWidth = destWidth;
                cache.destHeight = destHeight;
                cache.srcX = srcX;
                cache.srcY = srcY;
                if(bOrgSizeMinusOne) {
                    cache.srcWidth = -1;
                    cache.srcHeight = -1;
                } else {
                    cache.srcWidth = srcWidth;
                    cache.srcHeight = srcHeight;
                }
                cache.extractImg= extractImg;
                cache.image = obj->obj;
                if(clipobj) {
                    cache.clipper = clipobj->obj;
                } else {
                    cache.clipper = NULL;
                }

                mcfboolean bInserted = FALSE;
                for(mcfint loop = 0;loop < g_ImageCache.size() && !bInserted;loop++) {
                    if(!g_ImageCache[loop].used) {
                        g_ImageCache[loop] = cache;
                    }
                }
                if(!bInserted) {
                    g_ImageCache.push_back(cache);
                }
            }
#endif
        }
    }
}

mcfimage
CMCFGraphicsImplEfl::load_image(const mcfchar *imgPath)
{
    MCF_DEBUG();
    return NULL;
}

void
CMCFGraphicsImplEfl::unload_image(mcfimage imgData)
{
    MCF_DEBUG();
}

/**
 * Initializes the drawing context for double-buffering.
 * This func should be called before using a drawing primitive at first.
 */
mcfdrawctx
CMCFGraphicsImplEfl::begin_paint(const mcfwindow window, const mcfboolean forcedraw /* = FALSE */)
{
    MCF_DEBUG();
    mcfdrawctx drawctx = reinterpret_cast<mcfdrawctx>(window);

    return drawctx;
}

/**
 * Notices that drawing tasks have done.
 */
void
CMCFGraphicsImplEfl::end_paint(const mcfwindow window, mcfdrawctx drawCtx)
{

}

mcffont
CMCFGraphicsImplEfl::create_font(const McfFontInfo& info)
{
    return NULL;
}

void
CMCFGraphicsImplEfl::destroy_font(mcffont font)
{
}


typedef struct {
    const char *szString;
    const float actual_width;
} HARDCODED_WIDTH;
HARDCODED_WIDTH hardcoded_width[] = {
    {"\xe3\x85\x82" , 0.59 }, // q
    {"\xe3\x85\x83" , 0.59 }, // Q
    {"\xe3\x85\x88" , 0.59 }, // w
    {"\xe3\x85\x89" , 0.59 }, // W
    {"\xe3\x84\xb7" , 0.59 }, // e
    {"\xe3\x84\xb8" , 0.59 }, // E
    {"\xe3\x84\xb1" , 0.59 }, // r
    {"\xe3\x84\xb2" , 0.59 }, // R
    {"\xe3\x85\x85" , 0.59 }, // t
    {"\xe3\x85\x86" , 0.59 }, // T
    {"\xe3\x85\x9b" , 0.72 }, // y
    {"\xe3\x85\x95" , 0.40 }, // u
    {"\xe3\x85\x91" , 0.40 }, // i
    {"\xe3\x85\x90" , 0.50 }, // o
    {"\xe3\x85\x92" , 0.55 }, // O
    {"\xe3\x85\x94" , 0.59 }, // p
    {"\xe3\x85\x96" , 0.59 }, // P
    {"\xe3\x85\x81" , 0.59 }, // a
    {"\xe3\x84\xb4" , 0.59 }, // s
    {"\xe3\x85\x87" , 0.59 }, // d
    {"\xe3\x84\xb9" , 0.59 }, // f
    {"\xe3\x85\x8e" , 0.59 }, // g
    {"\xe3\x85\x97" , 0.72 }, // h
    {"\xe3\x85\x93" , 0.40 }, // j
    {"\xe3\x85\x8f" , 0.40 }, // k
    {"\xe3\x85\xa3" , 0.30 }, // l
    {"\xe3\x85\x8b" , 0.59 }, // z
    {"\xe3\x85\x8c" , 0.59 }, // x
    {"\xe3\x85\x8a" , 0.59 }, // c
    {"\xe3\x85\x8d" , 0.59 }, // v
    {"\xe3\x85\xa0" , 0.72 }, // b
    {"\xe3\x85\x9c" , 0.72 }, // n
    {"\xe3\x85\xa1" , 0.75 }, // m

    {"\xe3\x85\xa3" , 0.30 }, // ��
    {"\xe3\x86\x8d" , 0.30 }, // .
    {"\xe3\x85\xa1" , 0.75 }, // ��
    {"\xe3\x84\xb1\xe3\x85\x8b" , 1.18 }, // ����
    {"\xe3\x84\xb4\xe3\x84\xb9" , 1.18 }, // ����
    {"\xe3\x84\xb7\xe3\x85\x8c" , 1.18 }, // ����
    {"\xe3\x85\x82\xe3\x85\x8d" , 1.18 }, // ����
    {"\xe3\x85\x85\xe3\x85\x8e" , 1.18 }, // ����
    {"\xe3\x85\x88\xe3\x85\x8a" , 1.18 }, // ����
    {"\xe3\x85\x87\xe3\x85\x81" , 1.18 }, // ����
    {"\xe3\x85\x87\xe3\x85\x81" , 1.18 }, // ����
    {"\xe3\x84\xb1\xe3\x84\xb4" , 1.38 }, // ����
    {"\xe1\x84\x88" , 0.59 }, // ��
    {"\xe1\x84\x8d" , 0.59 }, // ��
    {"\xe1\x84\x84" , 0.59 }, // ��
    {"\xe1\x84\x81" , 0.59 }, // ��
    {"\xe1\x84\x8a" , 0.59 }, // ��

    {"\xe2\x86\x90" , 0.50 }, // Arrows
    {"\xe2\x86\x91" , 0.40 }, //
    {"\xe2\x86\x92" , 0.55 }, //
    {"\xe2\x86\x93" , 0.40 }, //

	{"\xe3\x85\xa0" , 0.1 }, // ��
	{"\341\205\262" , 0.9 }, // ��
};

int find_hardcoded_width(const char *szString, int fontsize) {
    for(unsigned int loop = 0;loop < sizeof(hardcoded_width) / sizeof(HARDCODED_WIDTH);loop++) {
        if(strcmp(szString, hardcoded_width[loop].szString) == 0) {
            return hardcoded_width[loop].actual_width * fontsize;
        }
    }
    return 0;
}

/**
 * Draws the given text on cairo-surface
 */
void
CMCFGraphicsImplEfl::draw_text(mcfwindow window, const mcfdrawctx drawCtx, const McfFontInfo& fontinfo, const McfColor& color,
                               const mcfchar *str, mcfint posx, mcfint posy, mcfint width, mcfint height,
                               MCFLabelAlignment align, mcfbyte paddingx, mcfbyte paddingy)
{
    MCF_DEBUG();

    CMCFWindows *windows = CMCFWindows::get_instance();
    McfWindowContext *winctx = NULL;
    McfWindowContext *targetctx = NULL;

    if (windows) {
        winctx = windows->get_window_context(window, FALSE);
        targetctx = windows->get_window_context(drawCtx, FALSE);
    }

    if (winctx && targetctx && str && windows) {
        if (strlen(str) > 0) {
#ifdef TEST_NEWBACKEND
            mcfboolean bFound = FALSE;
            mcfint hashval = hash_string(str);
            mcfint org_posx = posx;
            mcfint org_posy = posy;
            for(mcfint loop = 0;loop < g_TextCache.size() && !bFound;loop++) {
                    if(g_TextCache[loop].used &&
                       window == g_TextCache[loop].window &&
                       strncmp(fontinfo.fontname, g_TextCache[loop].fontinfo.fontname, MAX_FONT_NAME_LEN) == 0 &&
                       fontinfo.fontsize == g_TextCache[loop].fontinfo.fontsize &&
                       fontinfo.isBold == g_TextCache[loop].fontinfo.isBold &&
                       fontinfo.isItalic == g_TextCache[loop].fontinfo.isItalic &&
                       memcmp(&color, &(g_TextCache[loop].color), sizeof(McfColor)) == 0 &&
                       hashval == g_TextCache[loop].strHash &&
                       posx == g_TextCache[loop].posx&&
                       posy == g_TextCache[loop].posy &&
                       width == g_TextCache[loop].width &&
                       height == g_TextCache[loop].height &&
                       align == g_TextCache[loop].align &&
                       paddingx == g_TextCache[loop].paddingx &&
                       paddingy == g_TextCache[loop].paddingy) {
                        if(strcmp(str, g_TextCache[loop].str) == 0) {
                            bFound = TRUE;
                            evas_object_show(g_TextCache[loop].text);
                            evas_object_raise(g_TextCache[loop].text);
                        }
                    }
            }
            if(!bFound) {
#endif
            EFLObject *obj = new EFLObject;
            if (obj) {
                Evas_Object *winobj = (Evas_Object*)window;
                Evas *evas = evas_object_evas_get(winobj);
                Evas_Object *txtobj = evas_object_textblock_add(evas);

                if (txtobj) {
                    mcfchar strStyle[128];
                    sprintf(strStyle, "DEFAULT='font=%s font_size=%d align=%s color=#%02X%02X%02X%02X wrap=word left_margin=%d right_margin=%d'",
                            fontinfo.fontname, fontinfo.fontsize,
                            (((int)align % 3) == 1 ) ? "center" : ((((int)align % 3) == 2 ) ? "right" : "left"),
                            color.r, color.g, color.b, color.a, paddingx, paddingx);

                    Evas_Textblock_Style *st;
                    st = evas_textblock_style_new();
                    evas_textblock_style_set(st, strStyle);
                    evas_object_textblock_style_set(txtobj, st);
                    evas_object_textblock_text_markup_set(txtobj, str);
                    evas_object_resize(txtobj, width, height);

                    obj->bExtracted = FALSE;
                    obj->type = EFLOBJECT_TEXTBLOCK;
                    obj->obj = txtobj;
                    obj->position.x = posx;
                    obj->position.y = posy;
                    obj->position.width = width;
                    obj->position.height = height;
                    obj->etcInfo = (char*)str;
                    obj->data = st;

                    if(winctx->isVirtual) {
                        McfWindowContext *basectx = windows->get_window_context(windows->get_base_window());
                        if(basectx) {
                            obj->position.x -= (winctx->x - basectx->x);
                            obj->position.y -= (winctx->y - basectx->y);
                        }
                    }

                    mcfint calwidth, calheight;
                    evas_object_textblock_size_native_get(txtobj, &calwidth, &calheight);
                    /* FIXME : The following 2 lines are workaround for problem that EFL does not return correct font size */
                    mcfint hardcoded_width = find_hardcoded_width(str, fontinfo.fontsize);
                    if(hardcoded_width != 0) calwidth = hardcoded_width;

                    if(calwidth > width) {
                        sprintf(strStyle, "DEFAULT='font=%s font_size=%d align=%s color=#%02X%02X%02X%02X wrap=word left_margin=%d right_margin=%d'",
                                fontinfo.fontname, (int)(0.9 * fontinfo.fontsize * ((float)width / (float)calwidth)),
                                (((int)align % 3) == 1 ) ? "center" : ((((int)align % 3) == 2 ) ? "right" : "left"),
                                color.r, color.g, color.b, color.a, paddingx, paddingx);
                        evas_textblock_style_set(st, strStyle);
                        evas_object_textblock_style_set(txtobj, st);
                        evas_object_textblock_text_markup_set(txtobj, str);
                        evas_object_resize(txtobj, width, height);
                        evas_object_textblock_size_native_get(txtobj, &calwidth, &calheight);
                    }

                    if (align == LABEL_ALIGN_LEFT_MIDDLE ||
                        align == LABEL_ALIGN_CENTER_MIDDLE ||
                        align == LABEL_ALIGN_RIGHT_MIDDLE) {
                        posy = posy + ((height - calheight) / 2) + paddingy;
                    } else if (align == LABEL_ALIGN_LEFT_BOTTOM ||
                    		   align == LABEL_ALIGN_CENTER_BOTTOM ||
                    		   align == LABEL_ALIGN_RIGHT_BOTTOM) {
                        posy = posy + (height - calheight) - paddingy;
                    } else {
                        posy += paddingy;
                    }

                    evas_object_move(txtobj, posx, posy);
                    evas_object_raise(txtobj);
                    evas_object_show(txtobj);

                    targetctx->etcInfoPtr = eina_list_append((Eina_List*)(targetctx->etcInfoPtr), obj);

                    mcfint window_layer = 29000;
                    if(!windows->is_base_window(reinterpret_cast<mcfwindow>(drawCtx))) {
                        window_layer = 29010;
                    }
                    evas_object_layer_set(txtobj, window_layer + 1);
                }
            }
#ifdef TEST_NEWBACKEND
                TextCache cache;
                cache.used = true;
                cache.window = window;
                cache.fontinfo = fontinfo;
                cache.color = color;
                strncpy(cache.fontinfo.fontname, fontinfo.fontname, MAX_FONT_NAME_LEN);
                cache.fontinfo.fontsize = fontinfo.fontsize;
                cache.fontinfo.isBold = fontinfo.isBold;
                cache.fontinfo.isItalic = fontinfo.isItalic;
                memcpy(&(cache.color), &(color), sizeof(McfColor));
                strncpy(cache.str, str, sizeof(cache.str));
                cache.strHash = hashval;
                cache.posx = org_posx;
                cache.posy = org_posy;
                cache.width = width;
                cache.height = height;
                cache.align = align;
                cache.paddingx = paddingx;
                cache.paddingy = paddingy;

                cache.text = obj->obj;

                mcfboolean bInserted = FALSE;
                for(mcfint loop = 0;loop < g_TextCache.size() && !bInserted;loop++) {
                    if(!g_TextCache[loop].used) {
                        g_TextCache[loop] = cache;
                    }
                }
                if(!bInserted) {
                    g_TextCache.push_back(cache);
                }
            }
#endif
        }
    }
}

/**
 * Draws a rectangle on cairo-surface
 */
void
CMCFGraphicsImplEfl::draw_rectangle(mcfwindow window, const mcfdrawctx drawCtx, mcfdouble posx, mcfdouble posy,
                                    mcfdouble width, mcfdouble height, const mcfdouble lineWidth,
                                    const McfColor& lineColor, mcfboolean fill, const McfColor& fillColor,
                                    mcfdouble radius, mcffloat alpha)
{
    MCF_DEBUG();

    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFUtils *utils = CMCFUtils::get_instance();
    McfWindowContext *winctx = NULL;
    McfWindowContext *targetctx = NULL;

    if(windows) {
        winctx = windows->get_window_context(window, FALSE);
        targetctx = windows->get_window_context(drawCtx, FALSE);
    }

    if (winctx && utils && cache && windows) {
        EFLObject *obj = new EFLObject;
        if (obj) {
            Evas_Object *winobj = (Evas_Object*)window;
            Evas *evas = evas_object_evas_get(winobj);
            Evas_Object *rectobj = evas_object_rectangle_add(evas);

            evas_object_size_hint_weight_set(rectobj, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
            elm_win_resize_object_add(winobj, rectobj);
            evas_object_color_set(rectobj, fillColor.r, fillColor.g, fillColor.b, fillColor.a);

            evas_object_move(rectobj, posx, posy);
            evas_object_resize(rectobj, width, height);

            evas_object_show(rectobj);

            obj->obj = rectobj;
            obj->type = EFLOBJECT_RECTANGLE;
            obj->position.x = posx;
            obj->position.y = posy;
            obj->position.width = width;
            obj->position.height = height;
            obj->etcInfo = NULL;
            obj->data = NULL;

            if(winctx->isVirtual) {
                McfWindowContext *basectx = windows->get_window_context(windows->get_base_window());
                if(basectx) {
                    obj->position.x -= (winctx->x - basectx->x);
                    obj->position.y -= (winctx->y - basectx->y);
                }
            }

            targetctx->etcInfoPtr = eina_list_append((Eina_List*)(targetctx->etcInfoPtr), obj);

            mcfint window_layer = 29000;
            if(!windows->is_base_window(reinterpret_cast<mcfwindow>(drawCtx))) {
                window_layer = 29010;
            }
            evas_object_layer_set(rectobj, window_layer + 1);
        }
    }
}

McfSize
CMCFGraphicsImplEfl::get_image_size(mcfchar* imgPath)
{
    MCF_DEBUG();
    McfSize ret = { 0, 0 };

    CMCFWindows *windows = CMCFWindows::get_instance();

    Evas_Object *winobj = (Evas_Object*)(windows->get_base_window());
    Evas *evas = evas_object_evas_get(winobj);
    Evas_Object *imgobj = evas_object_image_add(evas);

    if (imgobj) {
        int w, h;
        evas_object_image_file_set(imgobj, imgPath, NULL);
        evas_object_image_size_get(imgobj, &w, &h);
        evas_object_del(imgobj);
        ret.width = w;ret.height = h;
    }

    return ret;
}


