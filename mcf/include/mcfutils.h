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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <map>

#include "mcftypes.h"
#include "mcfconfig.h"
#include "mcfstructs.h"

#ifndef __MCF_UTILS_H__
#define __MCF_UTILS_H__

#ifdef __cplusplus
//MCF_BEGIN_DECLS
#endif

#ifndef _POSIX_PATH_MAX
#define _POSIX_PATH_MAX 255
#endif

namespace mcf
{

/**
 * @brief The base class to work as a soft-based keyboard
 *
 * This class implements all functions for working as a soft-based keyboard
 * In side of ISE developer, they can modify it by their requirements.
 */
class CMCFUtilsImpl
{
public :
    virtual mcfboolean get_screen_resolution(mcfint *x, mcfint *y) = 0;

    virtual mcfint log(const mcfchar *fmt, ...) = 0;
};

class CMCFUtilsImplLinux : public CMCFUtilsImpl
{
public :
    mcfboolean get_screen_resolution(mcfint *x, mcfint *y);

    mcfint log(const mcfchar *fmt, ...);
};

class CMCFUtilsImplWin32 : public CMCFUtilsImpl
{
public :
    mcfboolean get_screen_resolution(mcfint *x, mcfint *y);

    mcfint log(const mcfchar *fmt, ...);
};

class _CMCFUtils
{
public :
    _CMCFUtils() {
        pImp_ = 0;
    }
    virtual mcfboolean get_screen_resolution(mcfint *x, mcfint *y) = 0;

protected :
    CMCFUtilsImpl* GetCMCFUtilsImpl() {
        if (pImp_ == 0) {
#ifdef  __WIN32__
            pImp_ = new CMCFUtilsImplWin32;
#else
            pImp_ = new CMCFUtilsImplLinux;
#endif
        }
        return pImp_;
    }

private :
    CMCFUtilsImpl* pImp_;
};

class CMCFUtils : public _CMCFUtils
{
private:
    static CMCFUtils* sInstance; /* For singleton */
    CMCFUtils();
public :
    ~CMCFUtils();

    static CMCFUtils* get_instance();

    void init();

    void scale_x(mcf16 *x);
    void scale_y(mcf16 *y);
    mcf16 get_scale_x(mcf16 x);
    mcf16 get_scale_y(mcf16 y);
    mcffloat get_smallest_scale_rate();
    mcffloat get_scale_rate_x();
    mcffloat get_scale_rate_y();

    mcfchar* get_str_dup(const mcfchar* str);

    mcfboolean get_screen_resolution(mcfint *x, mcfint *y) {
        return GetCMCFUtilsImpl()->get_screen_resolution(x, y);
    }

    mcfboolean sleep(mcfulong msc);

    mcfchar* get_composed_path(mcfchar* buf, int bufLength, const mcfchar* path);
    const McfNinePatchInfo* get_nine_patch_info(const mcfchar *imgPath);

    mcfboolean get_autopopup_window_variables(mcfchar * const autopopupKeys[MAX_SIZE_OF_AUTOPOPUP_STRING], mcfbyte *numKeys,
		                                      mcfbyte *numColumns, mcfbyte *numRows, mcfint *width, mcfint *height);

    mcf16 get_unique_id();

    mcfboolean is_rect_overlap(McfRectangle rect1, McfRectangle rect2);
    mcffloat get_distance(mcfint x1, mcfint y1, mcfint x2, mcfint y2);
    mcffloat get_distance(McfPoint pt1, McfPoint pt2);

    mcffloat get_approximate_distance(mcfint x1, mcfint y1, mcfint x2, mcfint y2);
    mcffloat get_approximate_distance(McfPoint pt1, McfPoint pt2);

    mcfint log(const mcfchar *fmt, ...);
private :
    mcffloat mXScnRate;
    mcffloat mYScnRate;

    mcfint mScnResolutionX;
    mcfint mScnResolutionY;

    std::map<mcfchar*, McfNinePatchInfo> mNinePatchInfoMap;
};


} /* End of mcf namespace */

#ifdef __cplusplus
//MCF_END_DECLS
#endif

#endif //__MCF_UTILS_H__
