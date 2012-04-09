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
