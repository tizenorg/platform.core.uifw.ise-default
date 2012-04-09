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
#include "mcfimageproxy.h"
#include "mcfdebug.h"

using namespace mcf;

CMCFImageProxy* CMCFImageProxy::sInstance = NULL; /* For singleton */

/**
* Constructor
*/
CMCFImageProxy::CMCFImageProxy()
{
    MCF_DEBUG();

    for (int loop = 0;loop < IMAGE_PROXY_SIZE;loop++) {
        mImageCacheItems[loop].imgData = NULL;
        memset(mImageCacheItems[loop].imgPath, 0x00, sizeof(mImageCacheItems[loop].imgPath));
    }
}

/**
* De-constructor
*/
CMCFImageProxy::~CMCFImageProxy()
{
    MCF_DEBUG();
    free_images();
}

CMCFImageProxy* CMCFImageProxy::get_instance()
{
    if (!sInstance) {
        sInstance = new CMCFImageProxy();
    }
    return (CMCFImageProxy*)sInstance;
}

mcfimage
CMCFImageProxy::get_image(const mcfchar* imgPath)
{
    MCF_DEBUG();

    mcfimage ret = NULL;

    /* FIXME : Better to use std::map for searching image cache item */
    for (int loop = 0;loop < IMAGE_PROXY_SIZE && ret == NULL;loop++) {
        if (strcmp(imgPath, mImageCacheItems[loop].imgPath) == 0) {
            ret = mImageCacheItems[loop].imgData;
            break;
        }
    }

    if (ret == NULL) {
        mcfboolean isEmpty = FALSE;
        CMCFGraphics *graphics = CMCFGraphics::get_instance();
        ret = graphics->load_image(imgPath);

        if (ret) {
            for (int loop = 0;loop < IMAGE_PROXY_SIZE && !isEmpty;loop++) {
                if (mImageCacheItems[loop].imgData == NULL) {
                    isEmpty = TRUE;
                    strncpy(mImageCacheItems[loop].imgPath, imgPath, MAX_IMAGE_PATH_LEN);
                    mImageCacheItems[loop].imgPath[MAX_IMAGE_PATH_LEN] = '\0';
                    mImageCacheItems[loop].imgData = ret;
                }
            }
        }
    }

    return ret;
}

void
CMCFImageProxy::free_images()
{
    MCF_DEBUG();

    for (int loop = 0;loop < IMAGE_PROXY_SIZE;loop++) {
        if (mImageCacheItems[loop].imgData) {
            CMCFGraphics *graphics = CMCFGraphics::get_instance();
            graphics->unload_image(mImageCacheItems[loop].imgData);
            mImageCacheItems[loop].imgData = NULL;
        }
    }
}
