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

#include "mcfutils.h"
#include "mcfdebug.h"

#include <stdarg.h>

#include <Ecore.h>
#include <Ecore_X.h>
#include <Ecore_Evas.h>
#include <dlog.h>

#ifndef LOG_TAG
#define LOG_TAG "ISE"
#endif

using namespace mcf;

/* FIXME : Currently the screen resolution is locally cached, should be updated when it gets changed */
mcfboolean
CMCFUtilsImplLinux::get_screen_resolution(mcfint *x, mcfint *y)
{
	MCF_DEBUG();
	static Evas_Coord win_w, win_h;
	if(win_w == 0 || win_h == 0) {
		ecore_x_window_size_get(ecore_x_window_root_first_get(), &win_w, &win_h);
	}

	if(x && y) {
		*x = win_w;
		*y = win_h;
	}
	return TRUE;
}

mcfint CMCFUtilsImplLinux::log(const mcfchar *fmt, ...)
{
	MCF_DEBUG();
	va_list argptr;
	mcfint ret = 0;

	va_start(argptr, fmt);
	LOGD(fmt, argptr);
	va_end(argptr);

	return ret;
}

