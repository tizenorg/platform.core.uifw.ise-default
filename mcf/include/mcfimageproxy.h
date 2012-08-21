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


#ifndef __MCF_IMAGE_PROXY_H__
#define __MCF_IMAGE_PROXY_H__

namespace mcf
{
class CMCFImageProxy
{
private:
	static CMCFImageProxy* sInstance; /* For singleton */
	CMCFImageProxy();
public :
    ~CMCFImageProxy();
    
    static CMCFImageProxy* get_instance();

    mcfimage get_image(const mcfchar* imgPath);
    void free_images();

private:
    static const mcfint MAX_IMAGE_PATH_LEN = 255;
    typedef struct _McfImageCacheItem {
        mcfchar imgPath[MAX_IMAGE_PATH_LEN + 1];
        mcfimage imgData;
    } McfImageCacheItem;

    static const mcfint IMAGE_PROXY_SIZE = 1;
    McfImageCacheItem mImageCacheItems[IMAGE_PROXY_SIZE];
};
} /* End of mcf namespace */
#endif
