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


#include "mcfutils.h"
#include "mcfdebug.h"

#include <stdarg.h>

#include <Ecore.h>
#include <Ecore_X.h>
#include <Ecore_Evas.h>

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
	va_end(argptr);

	return ret;
}

