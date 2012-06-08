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



#ifndef __MCF_FONT_PROXY_H__
#define __MCF_FONT_PROXY_H__

#include "mcfgraphics.h"

namespace mcf
{
class CMCFFontProxy
{
private:
    static CMCFFontProxy* sInstance; /* For singleton */
    CMCFFontProxy();
public :
    ~CMCFFontProxy();

    static CMCFFontProxy* get_instance();

    mcffont get_font(const McfFontInfo& info);
    void free_fonts();

private:
    typedef struct _McfFontCacheItem {
        mcfchar fontname[MAX_FONT_NAME_LEN + 1];
        mcfshort fontsize;
        mcfboolean isItalic;
        mcfboolean isBold;
        mcffont font;
    } McfFontCacheItem;

    static const mcfint FONT_PROXY_SIZE = 128;
    McfFontCacheItem mFontCacheItems[FONT_PROXY_SIZE];
};
} /* End of mcf namespace */
#endif
