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



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcfuibuilder.h"
#include "mcfdebug.h"
#include "mcfresourcecache.h"
#include "mcfcontext.h"
#include "mcfimageproxy.h"
#include "mcfactionstate.h"


using namespace mcf;


CMCFUIBuilder* CMCFUIBuilder::sInstance = NULL; /* For singleton */

CMCFUIBuilder::CMCFUIBuilder()
{
    MCF_DEBUG();

    mGwes = NULL;
    mUtils = NULL;
}

CMCFUIBuilder::~CMCFUIBuilder()
{
    MCF_DEBUG();
    if (mGwes) delete mGwes;
    mGwes = NULL;
    if (mUtils) delete mUtils;
    mUtils = NULL;
}

CMCFUIBuilder*
CMCFUIBuilder::get_instance()
{
    if (!sInstance) {
        sInstance = new CMCFUIBuilder();
    }
    return (CMCFUIBuilder*)sInstance;
}

void
CMCFUIBuilder::init(mcfwindow parentWnd)
{
    MCF_DEBUG();

    mGwes = CMCFGwes::get_instance();
    mUtils = CMCFUtils::get_instance();
    /* It will create the base window */
    mcfbyte inputMode = mcf_default_configure.inputMode;
    MCFDisplay display = mcf_default_configure.display;
    mcfbyte defaultLayoutIdx = 0;

    if(mcf_check_arrindex(inputMode, MAX_INPUT_MODE) && mcf_check_arrindex(display, MCFDISPLAY_MAX)) {
        defaultLayoutIdx = mcf_input_mode_configure[inputMode].layoutId[display];
        if(mcf_check_arrindex(defaultLayoutIdx, MAX_LAYOUT)) {
            mGwes->init(parentWnd,
                        mUtils->get_scale_x(mcf_layout[defaultLayoutIdx].width),
                        mUtils->get_scale_y(mcf_layout[defaultLayoutIdx].height)
                       );
        }
    }

    CMCFContext *context = CMCFContext::get_instance();
    context->set_display(mcf_default_configure.display);
    context->set_input_mode(mcf_default_configure.inputMode);
    context->set_base_layout(mcf_input_mode_configure[mcf_default_configure.inputMode].layoutId[mcf_default_configure.display]);

    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    mcfwindow window = mGwes->mWnd->get_base_window();
    cache->recompute_layout(window);

    /* Creates the dim window */
    if(mcf_check_arrindex(defaultLayoutIdx, MAX_LAYOUT)) {
        mGwes->mWnd->create_dim_window(window, NULL, mcf_layout[defaultLayoutIdx].width, mcf_layout[defaultLayoutIdx].height);
    }

    /* For testing pre-image load */
    CMCFImageProxy *proxy = CMCFImageProxy::get_instance();
    mcfchar retPath[_POSIX_PATH_MAX] = {0,};
    for (mcfint loop = 0; loop < MAX_PRELOAD_IMG_CNT; loop++) {
        memset(retPath, 0x00, sizeof(mcfchar)*_POSIX_PATH_MAX);
        mUtils->get_composed_path(retPath, sizeof(retPath), mcf_preload_image[loop]);
        proxy->get_image(retPath);
    }
}

/**
 * Shows the current layout and contexts
 * This function will be called by expose event and invalidate rect
 */
mcfboolean
CMCFUIBuilder::show_layout(const mcfwindow window, const mcf16 x, const mcf16 y, const mcf16 width, const mcf16 height)
{
    MCF_DEBUG();

    mcfboolean ret = FALSE;

    CMCFEvents *events = CMCFEvents::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFGraphics *graphics = CMCFGraphics::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    CMCFContext *context = CMCFContext::get_instance();

    if(events && windows && graphics && cache && context) {
    /* FIXME : The drawCtx should be acquired from the base window also, if the target window is virtual
       However, for ease of developement, leave the drawctx to be acquired from the target window for now
       Should modify the EFLObject list management routine after fixing the issue described above
    */
        mcfdrawctx drawCtx = graphics->begin_paint(window);

        MCFDisplay currDisplay =  context->get_display();
        events->set_touch_event_offset(mcf_default_configure.touchOffset[currDisplay]);

        if (window == windows->get_dim_window()) {
            McfWindowContext *dimctx = windows->get_window_context(window);
            if(dimctx) {
                McfSize size;
                McfColor color;
                size.width = dimctx->width;
                size.height = dimctx->height;
                color = mcf_dim_window_configure.dimColor;
                draw_window_bg_by_sw(window, drawCtx, size, 0.0, color, color);
            }
        } else {
            /* For the base and popup window */
            const McfLayout* layout = cache->get_cur_layout(window);
            if (layout) {
                CMCFWindows *windows = CMCFWindows::get_instance();
                McfWindowContext *winctx = windows->get_window_context(window, FALSE);
                mcf_assert_return_false(layout);

                if (layout->fUseSWbg) {
                    if (winctx) {
                        McfSize size;
                        size.width = winctx->width;
                        size.height = winctx->height;
                        draw_window_bg_by_sw(window, drawCtx, size, layout->bgLineWidth, layout->bgLineColor, layout->bgColor);
                }
            } else if (layout->imgPath[BUTTON_STATE_NORMAL]) {
                    /* If the target window is virtual window, let's draw it on the base window */
                    mcfint targetx = layout->x;
                    mcfint targety = layout->y;
                    mcfwindow targetwin = window;
                    McfWindowContext *winctx = windows->get_window_context(window, FALSE);
                    if(winctx->isVirtual) {
                        McfWindowContext *basectx = windows->get_window_context(windows->get_base_window(), FALSE);
                        if(basectx) {
                            targetwin = windows->get_base_window();
                            targetx += winctx->x - basectx->x;
                            targety += winctx->y - basectx->y;
                        }
                    }
                    if (strlen(layout->imgPath[BUTTON_STATE_NORMAL]) > 0) {
                        mcfchar retPath[_POSIX_PATH_MAX] = {0,};
                        mUtils->get_composed_path(retPath, sizeof(retPath), layout->imgPath[BUTTON_STATE_NORMAL]);
                        // Temporary testing for EFL backend.. Otherwise the background image covers other buttons
                        if (winctx && (x + y + width + height == 0)) {
                            graphics->draw_image(targetwin, drawCtx, retPath, targetx, targety, layout->width, layout->height,
                                                 winctx->imgOffsetx, winctx->imgOffsety, -1, -1, layout->fExtractBG);
                        }
                    }
                }
                draw_button_all(window, drawCtx, x, y, width, height);
            }
        }
        graphics->end_paint(window, drawCtx);
    }

    return ret;
}

/**
 * Draws all of buttons
 */
mcfboolean
CMCFUIBuilder::draw_button_all(const mcfwindow window, const mcfdrawctx drawCtx, const mcf16 x /* = 0 */,
		                       const mcf16 y /* = 0 */, const mcf16 width /* = 0 */, const mcf16 height /* = 0 */)
{
    MCF_DEBUG();
    mcf_assert_return_false(window);

    mcfboolean drawall = FALSE;
    McfRectangle updatearea = {x, y, width, height};
    if (x + y + width + height == 0) {
        drawall = TRUE;
    }
    CMCFContext *context = CMCFContext::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    CMCFUtils *utils = CMCFUtils::get_instance();
    McfLayoutKeyCoordination* coordination = NULL;
    McfButtonContext* btncontext = NULL;

    if(context && cache && utils) {
    for (mcfint idx = 0; idx < MAX_KEY; idx++) {
        coordination = cache->get_cur_layout_key_coordination(window, idx);
        btncontext = cache->get_cur_button_context(window, idx);
            if(coordination && btncontext) {
                if(coordination->fValid &&
                    (coordination->subLayoutID == SUBLAYOUTID_NONE || coordination->subLayoutID == context->get_cur_sublayout_id())) {
                    McfRectangle itemrect = {coordination->x, coordination->y, coordination->width, coordination->height};
                    if (drawall || utils->is_rect_overlap(itemrect, updatearea)) {
                        if (!draw_button(window, drawCtx, idx, btncontext->state)) {
                            break;
                        }
                    }
                }
            }
        }
    }
    return TRUE;
}

/**
 * Draws the button of the given key index by type of the button
 * @remark draw_button_all
 */
mcfboolean
CMCFUIBuilder::draw_button(const mcfwindow window, mcfdrawctx drawCtx, const mcf16 keyIdx,
		                   const MCFButtonState state, const mcfboolean fForceDrawBG /* = FALSE */)
{
    MCF_DEBUG();
    mcf_assert_return_false(window);
    mcf_assert_return_false(keyIdx > NOT_USED && keyIdx < MAX_KEY);
    mcf_assert_return_false(state >= BUTTON_STATE_NORMAL && state < MCF_BUTTON_STATE_MAX);

    CMCFContext *context = CMCFContext::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    CMCFGraphics *graphics = CMCFGraphics::get_instance();

    if(cache && context && graphics) {
        McfButtonContext* btncontext = cache->get_cur_button_context(window, keyIdx);

        if(btncontext) {
            if (!btncontext->used) {
                return FALSE;
            }

            /* creates a cairo surface if the value of the given drawCtx is NULL */
            mcfboolean fNeedEndPaint = FALSE;
            if (drawCtx == NULL) {
                drawCtx = graphics->begin_paint(window);
                fNeedEndPaint = TRUE;
            }

            /* FIXME : There is a case that begin_pain fails. Inspection on the root cause is needed */
            if (drawCtx) {
                MCFShiftState shiftidx = context->get_shift_state();
                if(shiftidx < 0 || shiftidx >= MCF_SHIFT_STATE_MAX) {
                    shiftidx = MCF_SHIFT_STATE_OFF;
                }

                const McfLayout* layout = cache->get_cur_layout(window);
                const McfLayoutKeyProperties* properties = cache->get_cur_layout_key_properties(window, keyIdx);

                /* 1. drawing the background of the button */
                /* check it according to the following check-list */
                /* check it whether uses SW style button */
                if(layout && properties) {
                    if (layout->fUseSWBtn) {
                        /* case 1 (uses Software button) */
                        draw_button_bg_by_sw(window, drawCtx, keyIdx, state);
                    } else {
                        /* check it whether uses an individual images */
                        if (properties->bgImgPath[shiftidx][state]) {
                            if (strcmp(properties->bgImgPath[shiftidx][state], MCF_BACKGROUND_IMAGE_STRING) != 0) {
                                /* case 2 (uses an indivisual image) */
                                draw_button_bg_by_img(window, drawCtx, keyIdx, state, shiftidx);
                            } else {
                                /* case 3 (uses the layout background image) */
                                draw_button_bg_by_layoutimg(window, drawCtx, keyIdx, state, shiftidx);
                            }
                        } else if (fForceDrawBG) {
                            draw_button_bg_by_layoutimg(window, drawCtx, keyIdx, state, shiftidx);
                        }
                        /* case 4 (don't draw anything for button's background if imgPath is NULL) */
                    }

                    /* 2. displaying the label of the button */
                    draw_button_label(window, drawCtx, keyIdx, state, shiftidx);
                }
            }

            /* destrorys the cairo surface if the value of the given(parameter) drawCtx is NULL */
            if (fNeedEndPaint) {
                graphics->end_paint(window, drawCtx);
            }
        } else {
            return FALSE;
        }
    }
    return TRUE;
}

/**
 * Draws labels for the target button
 * @remark draw_button
 */
mcfboolean
CMCFUIBuilder::draw_button_label(const mcfwindow window, const mcfdrawctx drawCtx, const mcf16 keyIdx,
		                         const MCFButtonState state, const mcfboolean fShift)
{
    MCF_DEBUG();

    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFGraphics *graphics = CMCFGraphics::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    const McfLayoutKeyConfigure* configure = cache->get_cur_layout_key_configure(window, keyIdx);
    const McfLayoutKeyCoordination* coords = cache->get_cur_layout_key_coordination(window, keyIdx);
    const McfLayoutKeyProperties* properties = cache->get_cur_layout_key_properties(window, keyIdx);

    mcf_assert_return_false(window);
    mcf_assert_return_false(drawCtx);
    mcf_assert_return_false(properties);
    mcf_assert_return_false(configure);

    if(windows && graphics && cache && configure && coords && properties) {
        /* If the target window is virtual window, let's draw it on the base window */
    mcfint targetaddx = 0;
    mcfint targetaddy = 0;
    mcfwindow targetwin = window;
    McfWindowContext *winctx = windows->get_window_context(window, FALSE);

        if(winctx->isVirtual) {
            McfWindowContext *basectx = windows->get_window_context(windows->get_base_window(), FALSE);
            if(basectx) {
                targetwin = windows->get_base_window();
                targetaddx = winctx->x - basectx->x;
                targetaddy = winctx->y - basectx->y;
            }
        }

        /* for image label  */
        if (properties->labelImgPath[fShift][state]) {
            if (strlen(properties->labelImgPath[fShift][state]) > 0) {
                mcfchar retPath[_POSIX_PATH_MAX] = {0,};
                mUtils->get_composed_path(retPath, sizeof(retPath), properties->labelImgPath[fShift][state]);

                McfSize imgSize = mGwes->mGrps->get_image_size(retPath);
                if (imgSize.width == 0 && imgSize.height == 0) {
                        imgSize.width = coords->width;
                        imgSize.height = coords->height;
                }
                imgSize.width *= mUtils->get_smallest_scale_rate();
                imgSize.height *= mUtils->get_smallest_scale_rate();

                /* Make sure the image label is not bigger than the given coordination */

                    if(imgSize.width > coords->width) {
                        imgSize.width = coords->width;
                    }
                    if(imgSize.height > coords->height) {
                        imgSize.height = coords->height;
                    }


                McfPoint pos = {0,};

                    pos.x = coords->x + ((coords->width - imgSize.width) / 2);
                    pos.y = coords->y + ((coords->height -imgSize.height) / 2);


                graphics->draw_image(targetwin,
                                     drawCtx,
                                     retPath,
                                     pos.x + targetaddx,
                                     pos.y + targetaddy,
                                     imgSize.width,
                                     imgSize.height
                                    );
            }
        }

    /* for text */
        for (int idx = 0; idx < properties->labelCnt; idx++) {
            McfFontInfo info;
            CMCFResourceCache *cache = CMCFResourceCache::get_instance();
            const McfLabelProperties *labelproperties = cache->get_label_properties(properties->labelPropId, idx);
            if (labelproperties) {
                const mcfchar *label = properties->label[fShift][idx];
                /* If the button type is BUTTON_TYPE_ROTATION, display current keyvalue */
                if (idx == 0) {
                    if (configure->buttonType == BUTTON_TYPE_ROTATION) {
                        McfButtonContext* btncontext = cache->get_cur_button_context(window, keyIdx);
                        if (btncontext->multikeyIdx < MAX_SIZE_OF_MULTITAP_CHAR) {
                            label = properties->keyValue[fShift][btncontext->multikeyIdx];
                        }
                    }
                }
                if (labelproperties->fontName) {
                    strncpy(info.fontname, labelproperties->fontName, MAX_FONT_NAME_LEN - 1);
                    info.fontname[MAX_FONT_NAME_LEN - 1] = '\0';
                    info.fontsize = labelproperties->fontSize;
                    info.isBold = info.isItalic = true;

                    CMCFContext *context = CMCFContext::get_instance();
                    MCFShiftState shiftstate = context->get_shift_state();
                    if (labelproperties->shadowDistance > 0 && labelproperties->shadowColor[shiftstate][state].a != 0) {
                        mcfint deltax = 0;
                        mcfint deltay = 0;

                        if (labelproperties->shadowDirection == SHADOW_DIRECTION_LEFT_TOP ||
                            labelproperties->shadowDirection == SHADOW_DIRECTION_LEFT_MIDDLE ||
                            labelproperties->shadowDirection == SHADOW_DIRECTION_LEFT_BOTTOM) {
                            deltax -= labelproperties->shadowDistance;
                        } else if (labelproperties->shadowDirection == SHADOW_DIRECTION_RIGHT_TOP ||
                                   labelproperties->shadowDirection == SHADOW_DIRECTION_RIGHT_MIDDLE ||
                                   labelproperties->shadowDirection == SHADOW_DIRECTION_RIGHT_BOTTOM) {
                            deltax += labelproperties->shadowDistance;
                        }

                        if (labelproperties->shadowDirection == SHADOW_DIRECTION_LEFT_TOP ||
                            labelproperties->shadowDirection == SHADOW_DIRECTION_CENTER_TOP ||
                            labelproperties->shadowDirection == SHADOW_DIRECTION_RIGHT_TOP) {
                            deltay -= labelproperties->shadowDistance;
                        } else if (labelproperties->shadowDirection == SHADOW_DIRECTION_LEFT_BOTTOM ||
                                   labelproperties->shadowDirection == SHADOW_DIRECTION_CENTER_BOTTOM ||
                                   labelproperties->shadowDirection == SHADOW_DIRECTION_RIGHT_BOTTOM) {
                            deltay += labelproperties->shadowDistance;
                        }

                        graphics->draw_text(targetwin,
                                            drawCtx,
                                            info,
                                            labelproperties->shadowColor[shiftstate][state],
                                            label,
                                            (mcfint)coords->x + deltax + targetaddx,
                                            (mcfint)coords->y + deltax + targetaddy,
                                            (mcfint)coords->width,
                                            (mcfint)coords->height,
                                            labelproperties->alignment,
                                            labelproperties->paddingX,
                                            labelproperties->paddingY
                                           );
                    }

                    graphics->draw_text(targetwin,
                                        drawCtx,
                                        info,
                                        labelproperties->fontColor[shiftstate][state],
                                        label,
                                        (mcfint)coords->x + targetaddx,
                                        (mcfint)coords->y + targetaddy,
                                        (mcfint)coords->width,
                                        (mcfint)coords->height,
                                        labelproperties->alignment,
                                        labelproperties->paddingX,
                                        labelproperties->paddingY
                                       );
                }
            }
        }
    }

    return TRUE;
}

/**
* Draws window's background using software
* @remark
*/
mcfboolean
CMCFUIBuilder::draw_window_bg_by_sw(const mcfwindow window, const mcfdrawctx drawCtx, const McfSize size,
                                    const mcfdouble lineWidth, const McfColor lineColor, const McfColor fillColor)
{
    MCF_DEBUG();
    mcf_assert_return_false(window);
    mcf_assert_return_false(drawCtx);
    mcf_assert_return_false(layout);

    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFGraphics *graphics = CMCFGraphics::get_instance();

    if(graphics && windows) {
        /* If the target window is virtual window, let's draw it on the base window */
        mcfwindow targetwin = window;
        McfWindowContext *winctx = windows->get_window_context(window, FALSE);
        if(winctx->isVirtual) {
            targetwin = windows->get_base_window();
        }

        graphics->draw_rectangle(targetwin, drawCtx, 0, 0, size.width, size.height, lineWidth, lineColor, TRUE, fillColor);
    }

    return TRUE;
}

/**
 * Draws a button using software
 * @remark draw_button
 */
mcfboolean
CMCFUIBuilder::draw_button_bg_by_sw(const mcfwindow window, const mcfdrawctx drawCtx,
		                            const mcf16 keyIdx, const MCFButtonState state)
{
    MCF_DEBUG();

    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    const McfLayoutKeyCoordination* coords = cache->get_cur_layout_key_coordination(window, keyIdx);

    mcf_assert_return_false(window);
    mcf_assert_return_false(drawCtx);
    mcf_assert_return_false(coords);

    mcf_assert_return_false(state >= BUTTON_STATE_NORMAL && state < MCF_BUTTON_STATE_MAX);

    mcfdouble lineWidth = mcf_swbutton_style[mcf_default_configure.swBtnStyleIdx].lineWidth[state];
    McfColor &lineColor = mcf_swbutton_style[mcf_default_configure.swBtnStyleIdx].lineColor[state];
    McfColor &bgColor = mcf_swbutton_style[mcf_default_configure.swBtnStyleIdx].bgColor[state];
    mcfdouble lineCurve = mcf_swbutton_style[mcf_default_configure.swBtnStyleIdx].lineCurve;
    mcffloat bgAlpha = mcf_swbutton_style[mcf_default_configure.swBtnStyleIdx].bgAlpha;

    mGwes->mGrps->draw_rectangle(window,
                                 drawCtx,
                                 coords->x,
                                 coords->y,
                                 coords->width,
                                 coords->height,
                                 lineWidth,
                                 lineColor,
                                 TRUE,
                                 bgColor,
                                 lineCurve,
                                 bgAlpha);
    return TRUE;
}

/**
 * Draws a button using the set image
 * @remark draw_button
 */
mcfboolean
CMCFUIBuilder::draw_button_bg_by_img(const mcfwindow window, const mcfdrawctx drawCtx,const mcf16 keyIdx,
		                             const MCFButtonState state, const mcfboolean fShift)
{
    MCF_DEBUG();

    CMCFContext *context = CMCFContext::get_instance();
    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFGraphics *graphics = CMCFGraphics::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    const McfLayoutKeyCoordination* coordination = cache->get_cur_layout_key_coordination(window, keyIdx);
    const McfLayoutKeyProperties* properties = cache->get_cur_layout_key_properties(window, keyIdx);

    mcf_assert_return_false(window);
    mcf_assert_return_false(drawCtx);
    mcf_assert_return_false(coords);

    mcf_assert_return_false(state >= BUTTON_STATE_NORMAL && state < MCF_BUTTON_STATE_MAX);

    mcfchar retPath[_POSIX_PATH_MAX] = {0,};

    if(context && graphics && cache && coordination && properties) {
        mcfboolean pathComposed = FALSE;
        /* Check if we need to decorate the button's drag state */
        if(context->get_cur_key_modifier(context->get_last_touch_device_id()) != KEY_MODIFIER_NONE &&
           context->get_cur_pressed_window(context->get_last_touch_device_id()) == window &&
           context->get_cur_pressed_key(context->get_last_touch_device_id()) == keyIdx &&
           properties->modifierDecorationId) {
                mcfchar *szDecorationBgImgPath = NULL;
                const McfModifierDecoration *decoration = NULL;
                if(mcf_check_arrindex(properties->modifierDecorationId,
                    sizeof(mcf_modifier_decoration) / sizeof(McfModifierDecoration))) {
                    decoration = &(mcf_modifier_decoration[properties->modifierDecorationId]);
                }
                if(decoration) {
                    mcfshort display = context->get_display();
                    if(!mcf_check_arrindex(display, MCFDISPLAY_MAX)) display = 0;
                    MCFKeyModifier modifier = context->get_cur_key_modifier(context->get_last_touch_device_id());
                    if(!mcf_check_arrindex(modifier, KEY_MODIFIER_MAX)) modifier = KEY_MODIFIER_NONE;
                    szDecorationBgImgPath = decoration->bgImgPath[display][modifier];
                }
                if(szDecorationBgImgPath) {
                    if(strlen(szDecorationBgImgPath) > 0) {
                        mUtils->get_composed_path(retPath, sizeof(retPath), szDecorationBgImgPath);
                        pathComposed = TRUE;
                    }
                }
        }
        if(!pathComposed) {
            mUtils->get_composed_path(retPath, sizeof(retPath), properties->bgImgPath[fShift][state]);
        }

        /* If the target window is virtual window, let's draw it on the base window */
        mcfint targetx = coordination->x;
        mcfint targety = coordination->y;
        mcfwindow targetwin = window;
        McfWindowContext *winctx = windows->get_window_context(window, FALSE);
        if(winctx->isVirtual) {
            McfWindowContext *basectx = windows->get_window_context(windows->get_base_window(), FALSE);
            if(basectx) {
                targetwin = windows->get_base_window();
                targetx += winctx->x - basectx->x;
                targety += winctx->y - basectx->y;
            }
        }

        graphics->draw_image(targetwin,
                             drawCtx,
                             retPath,
                            (mcfint)targetx,
                            (mcfint)targety,
                            (mcfint)coordination->width,
                            (mcfint)coordination->height
                            );
    }

    return TRUE;
}

/**
 * Draws a button using the set layout image
 * @remark draw_button
 */
mcfboolean
CMCFUIBuilder::draw_button_bg_by_layoutimg(const mcfwindow window, const mcfdrawctx drawCtx, const mcf16 keyIdx,
		                                   const MCFButtonState state, const mcfboolean fShift)
{
    MCF_DEBUG();

    CMCFContext *context = CMCFContext::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    const McfLayout* layout = cache->get_cur_layout(window);
    const McfLayoutKeyCoordination* coordination = cache->get_cur_layout_key_coordination(window, keyIdx);
    const McfLayoutKeyProperties* properties = cache->get_cur_layout_key_properties(window, keyIdx);

    mcf_assert_return_false(window);
    mcf_assert_return_false(drawCtx);
    mcf_assert_return_false(coords);

    mcf_assert_return_false(state >= BUTTON_STATE_NORMAL && state < MCF_BUTTON_STATE_MAX);

    CMCFWindows *windows = CMCFWindows::get_instance();
    McfWindowContext *winctx = windows->get_window_context(window, FALSE);

    mcfchar retPath[_POSIX_PATH_MAX] = {0,};
    if(context && cache && coordination && properties && winctx) {
        mcfboolean pathComposed = FALSE;
        /* Check if we need to decorate the button's drag state */
        if(context->get_cur_key_modifier(context->get_last_touch_device_id()) != KEY_MODIFIER_NONE &&
           context->get_cur_pressed_window(context->get_last_touch_device_id()) == window &&
           context->get_cur_pressed_key(context->get_last_touch_device_id()) == keyIdx &&
           properties->modifierDecorationId) {
                mcfchar *szDecorationBgImgPath = NULL;
                const McfModifierDecoration *decoration = NULL;
                if(mcf_check_arrindex(properties->modifierDecorationId,
                    sizeof(mcf_modifier_decoration) / sizeof(McfModifierDecoration ))) {
                        decoration = &(mcf_modifier_decoration[properties->modifierDecorationId]);
                }
                if(decoration) {
                    mcfshort display = context->get_display();
                    if(!mcf_check_arrindex(display, MCFDISPLAY_MAX)) display = 0;
                    MCFKeyModifier modifier = context->get_cur_key_modifier(context->get_last_touch_device_id());
                    if(!mcf_check_arrindex(modifier, KEY_MODIFIER_MAX)) modifier = KEY_MODIFIER_NONE;
                    szDecorationBgImgPath = decoration->bgImgPath[display][modifier];
                }
                if(szDecorationBgImgPath) {
                    if(strlen(szDecorationBgImgPath) > 0) {
                        mUtils->get_composed_path(retPath, sizeof(retPath), szDecorationBgImgPath);
                        pathComposed = TRUE;
                    }
                }
        }
        if(!pathComposed) {
            mUtils->get_composed_path(retPath, sizeof(retPath), layout->imgPath[state]);
        }

        mGwes->mGrps->draw_image(window,
                                 drawCtx,
                                 retPath,
                                 (mcfint)coordination->x,
                                 (mcfint)coordination->y,
                                 (mcfint)coordination->width,
                                 (mcfint)coordination->height,
                                 winctx->imgOffsetx + (mcfint)coordination->x,
                                 winctx->imgOffsety + (mcfint)coordination->y,
                                 (mcfint)coordination->width,
                                 (mcfint)coordination->height,
                                 TRUE
                                );
    }
    return TRUE;
}

