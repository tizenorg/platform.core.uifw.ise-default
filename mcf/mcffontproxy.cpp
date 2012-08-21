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


#include "mcfgraphics.h"
#include "mcffontproxy.h"
#include "mcfdebug.h"

using namespace mcf;

CMCFFontProxy* CMCFFontProxy::sInstance = NULL; /* For singleton */

/**
* Constructor
*/
CMCFFontProxy::CMCFFontProxy()
{
    MCF_DEBUG();

    for (int loop = 0;loop < FONT_PROXY_SIZE;loop++) {
        mFontCacheItems[loop].font = NULL;
        memset(mFontCacheItems[loop].fontname, 0x00, sizeof(mFontCacheItems[loop].fontname));
        mFontCacheItems[loop].fontsize = 0;
        mFontCacheItems[loop].isItalic = FALSE;
        mFontCacheItems[loop].isBold = FALSE;
    }
}

/**
* De-constructor
*/
CMCFFontProxy::~CMCFFontProxy()
{
    MCF_DEBUG();
    free_fonts();
}

CMCFFontProxy* CMCFFontProxy::get_instance()
{
    if (!sInstance) {
        sInstance = new CMCFFontProxy();
    }
    return (CMCFFontProxy*)sInstance;
}

mcffont
CMCFFontProxy::get_font(const McfFontInfo& info)
{
    MCF_DEBUG();

    mcffont ret = NULL;

    for (int loop = 0;loop < FONT_PROXY_SIZE && ret == NULL;loop++) {
        if (strcmp(info.fontname, mFontCacheItems[loop].fontname) == 0 &&
            info.fontsize == mFontCacheItems[loop].fontsize &&
            info.isItalic == mFontCacheItems[loop].isItalic &&
            info.isBold == mFontCacheItems[loop].isBold) {
            ret = mFontCacheItems[loop].font;
            break;
        }
    }

    if (ret == NULL) {
        mcfboolean isEmpty = FALSE;
        CMCFGraphics *graphics = CMCFGraphics::get_instance();
        ret = graphics->create_font(info);

        if (ret) {
            for (int loop = 0;loop < FONT_PROXY_SIZE && !isEmpty;loop++) {
                if (mFontCacheItems[loop].font == NULL) {
                    isEmpty = TRUE;
                    strncpy(mFontCacheItems[loop].fontname, info.fontname, MAX_FONT_NAME_LEN);
                    mFontCacheItems[loop].fontname[MAX_FONT_NAME_LEN] = '\0';
                    mFontCacheItems[loop].fontsize = info.fontsize;
                    mFontCacheItems[loop].isItalic = info.isItalic;
                    mFontCacheItems[loop].isBold = info.isBold;
                    mFontCacheItems[loop].font = ret;
                }
            }
        }
    }

    return ret;
}

void
CMCFFontProxy::free_fonts()
{
    MCF_DEBUG();

    for (int loop = 0;loop < FONT_PROXY_SIZE;loop++) {
        if (mFontCacheItems[loop].font) {
            CMCFGraphics *graphics = CMCFGraphics::get_instance();
            graphics->destroy_font(mFontCacheItems[loop].font);
            mFontCacheItems[loop].font = NULL;
        }
    }
}
