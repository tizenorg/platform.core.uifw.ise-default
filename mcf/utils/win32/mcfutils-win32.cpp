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

#include <stdarg.h>

#include "mcfutils.h"
#include "mcfdebug.h"

using namespace mcf;

mcfboolean
CMCFUtilsImplWin32::get_screen_resolution(mcfint *x, mcfint *y)
{
	MCF_DEBUG();
	return TRUE;
}

mcfint CMCFUtilsImplWin32::log(const mcfchar *fmt, ...)
{
	MCF_DEBUG();
	va_list argptr;
	mcfint ret = 0;

	va_start(argptr, fmt);
	printf(fmt, argptr);
	va_end(argptr);

	return ret;
}
