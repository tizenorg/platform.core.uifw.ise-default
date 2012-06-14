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
