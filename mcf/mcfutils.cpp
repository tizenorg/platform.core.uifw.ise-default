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


#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

#include "mcfutils.h"
#include "mcfconfig.h"
#include "mcfdebug.h"
#include "mcfcontext.h"
#include "mcfresource.h"
#include "mcfresourcecache.h"

using namespace mcf;

CMCFUtils* CMCFUtils::sInstance; /* For singleton */
#define MCF_MAX_UNIQUE_ID 1000

CMCFUtils::CMCFUtils()
{
    MCF_DEBUG();
    mXScnRate = 1.0;
    mYScnRate = 1.0;
    mScnResolutionX = 0;
    mScnResolutionY = 0;
    init();
}

CMCFUtils::~CMCFUtils()
{
    MCF_DEBUG();
}

CMCFUtils* CMCFUtils::get_instance()
{
    if (!sInstance) {
        sInstance = new CMCFUtils();
    }
    return (CMCFUtils*)sInstance;
}

void
CMCFUtils::init()
{
    MCF_DEBUG();
    GetCMCFUtilsImpl()->get_screen_resolution(&mScnResolutionX, &mScnResolutionY);
    mXScnRate = mScnResolutionX / (float)MCF_BASE_SCREEN_WIDTH;
    mYScnRate = mScnResolutionY / (float)MCF_BASE_SCREEN_HEIGHT;
    if(MCF_AUTO_DETECT_PORTRAIT_LANDSCAPE) {
        /* If the width of screen is bigger than the height, switch portrait mode and landscape mode */
        if(mScnResolutionX > mScnResolutionY) {
            mXScnRate = mScnResolutionX / (float)MCF_BASE_SCREEN_HEIGHT;
            mYScnRate = mScnResolutionY / (float)MCF_BASE_SCREEN_WIDTH;
        }
    }

    mNinePatchInfoMap.clear();
}

/**
 * Returns a scale rate (see default screen resolution in mcfconfig.h file)
 */
mcffloat
CMCFUtils::get_smallest_scale_rate()
{
    /* Try to return smaller scale rate, to avoid UI crash */
    return (mXScnRate < mYScnRate) ? mXScnRate : mYScnRate;
}

/**
* Returns a scale rate X (see default screen resolution in mcfconfig.h file)
*/
mcffloat
CMCFUtils::get_scale_rate_x()
{
    /* Try to return smaller scale rate, to avoid UI crash */
    return mXScnRate;
}

/**
* Returns a scale rate Y (see default screen resolution in mcfconfig.h file)
*/
mcffloat
CMCFUtils::get_scale_rate_y()
{
    /* Try to return smaller scale rate, to avoid UI crash */
    return mYScnRate;
}

/**
 * Recalculates x value according to the current screen resolution
 */
void
CMCFUtils::scale_x(mcf16 *x)
{
    if (x) {
    	((*x)*=mXScnRate);
    }
}

/**
* Recalculates y value according to the current screen resolution
 */
void
CMCFUtils::scale_y(mcf16 *y)
{
    if (y) {
    	((*y)*=mYScnRate);
    }
}

/**
 * Returns a calculated x value according to the current screen resolution
 */
mcf16
CMCFUtils::get_scale_x(mcf16 x)
{
    return static_cast<mcf16>(x*mXScnRate);
}

/**
 * Returns a calculated y value according to the current screen resolution
 */
mcf16
CMCFUtils::get_scale_y(mcf16 y)
{
    return static_cast<mcf16>(y*mYScnRate);
}

/**
 * portable sleep function
 */
mcfboolean
CMCFUtils::sleep(mcfulong msc)
{
    MCF_DEBUG();
    fd_set dummy;
    struct timeval toWait;
    FD_ZERO(&dummy);
    toWait.tv_sec = msc / 1000;
    toWait.tv_usec = (msc % 1000) * 1000;
    select(0, &dummy, NULL, NULL, &toWait);
    return TRUE;
}

/**
 * returns the composed path
 */
mcfchar*
CMCFUtils::get_composed_path(mcfchar* buf, int bufLength, const mcfchar* path)
{
    MCF_DEBUG();

    if(buf && path) {
        if(mScnResolutionX == 0 || mScnResolutionY == 0) {
            CMCFUtilsImpl *impl = GetCMCFUtilsImpl();
            if(impl) {
                impl->get_screen_resolution(&mScnResolutionX, &mScnResolutionY);
            }
        }

        const mcfchar *themename = NULL;
        CMCFResourceCache *cache = CMCFResourceCache::get_instance();
        if(cache) {
            themename = cache->get_cur_themename();
            if (themename == NULL) {
                themename = DEFAULT_THEME;
            } else if (strlen(themename) == 0) {
                themename = DEFAULT_THEME;
            }
        }

        mcfboolean file_exists = FALSE;
        snprintf(buf, bufLength, "%s/%dx%d/%s/%s", IMG_PATH_PREFIX, mScnResolutionX, mScnResolutionY, themename, path);
        if(access(buf, R_OK) == 0) {
            file_exists = TRUE;
        }

        /* Check if the file exists in the current theme directory */
        if (!file_exists && themename != DEFAULT_THEME) {
            /* Use default theme name if not found */
            snprintf(buf, bufLength, "%s/%dx%d/%s/%s", IMG_PATH_PREFIX, mScnResolutionX, mScnResolutionY, DEFAULT_THEME, path);
            if(access(buf, R_OK) == 0) {
                file_exists = TRUE;
            }
        }

        /* FIXME : Should look for what other resolutions are available */
        /* If the file still doesn't exist, look for the file in different resolution */
        if (!file_exists) {
            /* Use default theme name if not found */
            snprintf(buf, bufLength, "%s/%dx%d/%s/%s", IMG_PATH_PREFIX, MCF_BASE_SCREEN_WIDTH,
            		 MCF_BASE_SCREEN_HEIGHT, DEFAULT_THEME, path);
            if(access(buf, R_OK) == 0) {
                file_exists = TRUE;
            }
        }

        /* Or maybe the path points the absolute path */
        if (!file_exists) {
            snprintf(buf, bufLength, "%s", path);
        }
    }

  return buf;
}

mcfboolean
CMCFUtils::get_autopopup_window_variables(mcfchar * const autopopupKeys[MAX_SIZE_OF_AUTOPOPUP_STRING], mcfbyte *numKeys,
		                                  mcfbyte *numColumns, mcfbyte *numRows, mcfint *width, mcfint *height)
{
    MCF_DEBUG();
    mcf_assert_return_false(autopopupKeys);
    mcf_assert_return_false(numKeys && numColumns && numRows && width && height);

    int loop;
    mcfboolean ret = FALSE;
    mcfint scrx, scry;

    if (autopopupKeys && numKeys && numColumns && numRows && width && height) {
        *numKeys = 0;
        for (loop = 0;loop < MAX_SIZE_OF_AUTOPOPUP_STRING;loop++) {
            if (autopopupKeys[loop]) {
                if (strlen(autopopupKeys[loop]) > 0) {
                    (*numKeys)++;
                }
            }
        }
        if(*numKeys > 0) {
            ret = TRUE;
        }

        this->get_screen_resolution(&scrx, &scry);

        CMCFContext *context = CMCFContext::get_instance();
        if (context->get_display() == MCFDISPLAY_LANDSCAPE) {
            int  swap = scrx;
            scrx = scry;
            scry = swap;
        }

        *numColumns = (scrx - (2 * mcf_autopopup_configure.bgPadding)) /
                      (mcf_autopopup_configure.btnWidth + mcf_autopopup_configure.btnSpacing);
        if (*numColumns > mcf_autopopup_configure.maxColumn && mcf_autopopup_configure.maxColumn > 0)
            *numColumns = mcf_autopopup_configure.maxColumn;
        *numRows = ((*numKeys - 1) / *numColumns) + 1;
        if (*numColumns > *numKeys) *numColumns = *numKeys;

        /* Try to spread evenly on each lines */
        *numColumns = (*numKeys) / (*numRows);
        if ((*numKeys) % *numRows != 0) (*numColumns)++;

        *width =
            (*numColumns * mcf_autopopup_configure.btnWidth) +
            (2 * mcf_autopopup_configure.bgPadding) +
            ((*numColumns - 1) * mcf_autopopup_configure.btnSpacing);
        *height =
            (*numRows * mcf_autopopup_configure.btnHeight) +
            (2 * mcf_autopopup_configure.bgPadding) +
            ((*numRows - 1) * mcf_autopopup_configure.btnSpacing);

        *width += (2 * mcf_autopopup_configure.wndDecoSize);
        *height += (2 * mcf_autopopup_configure.wndDecoSize);
    }

    return ret;
}

/**
 * Returns a duplicated string pointer by given str
 */
mcfchar*
CMCFUtils::get_str_dup(const mcfchar* str)
{
    MCF_DEBUG();
    mcfchar *new_str;
    mcfint length;

    if (str) {
        length = strlen(str) + 1;
        new_str = (mcfchar*)malloc(length);
        memcpy(new_str, str, length);
        new_str[length - 1] = '\0';
    } else {
        new_str = NULL;
    }

    return new_str;
}

/**
 * Returns an unique ID
 */
mcf16
CMCFUtils::get_unique_id()
{
    MCF_DEBUG();
    static mcf16 uniId;
    if (uniId < MCF_MAX_UNIQUE_ID) {
        uniId++;
    } else {
        uniId = 0;
    }
    return uniId;
}

mcfboolean
CMCFUtils::is_rect_overlap(McfRectangle rect1, McfRectangle rect2)
{
    if (rect1.x < rect2.x + rect2.width && rect1.x + rect1.width > rect2.x &&
            rect1.y < rect2.y + rect2.height && rect1.y + rect1.height > rect2.y) {
        return true;
    }
    return false;
}

mcffloat
CMCFUtils::get_distance( mcfint x1, mcfint y1, mcfint x2, mcfint y2 )
{
    mcfint dx = x1 - x2;
    mcfint dy = y1 - y2;
    return sqrt((dx * dx) + (dy * dy));
}

mcffloat
CMCFUtils::get_distance( McfPoint pt1, McfPoint pt2 )
{
    return get_distance(pt1.x, pt1.y, pt2.x, pt2.y);
}

mcffloat
CMCFUtils::get_approximate_distance( mcfint x1, mcfint y1, mcfint x2, mcfint y2 )
{
    mcfint dx = abs(x1 - x2);
    mcfint dy = abs(y1 - y2);
    return dx + dy;
}

mcffloat
CMCFUtils::get_approximate_distance( McfPoint pt1, McfPoint pt2 )
{
    return get_approximate_distance(pt1.x, pt1.y, pt2.x, pt2.y);
}

const McfNinePatchInfo*
CMCFUtils::get_nine_patch_info(const mcfchar *imgPath)
{
    MCF_DEBUG();

    if(!imgPath) return NULL;

    /* FIXME : This is implemented in very inefficient way. Should optimize later. */
    for (int loop = 0;loop < (sizeof(mcf_nine_patch_info) / sizeof(McfNinePatchInfo));loop++) {
        mcfchar buf[_POSIX_PATH_MAX];
        get_composed_path(buf, sizeof(buf), mcf_nine_patch_info[loop].imgPath);
        if (strcmp(buf, imgPath) == 0) {
            return &mcf_nine_patch_info[loop];
        }
    }

    return NULL;
}

mcfint CMCFUtils::log(const mcfchar *fmt, ...)
{
    MCF_DEBUG();
    va_list argptr;
    mcfint ret = 0;

    va_start(argptr, fmt);
    CMCFUtilsImpl *impl = GetCMCFUtilsImpl();
    if(impl) {
        ret = impl->log(fmt, argptr);
    }
    va_end(argptr);

    return ret;
}
