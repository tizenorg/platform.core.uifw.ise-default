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
