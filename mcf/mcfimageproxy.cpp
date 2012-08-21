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
