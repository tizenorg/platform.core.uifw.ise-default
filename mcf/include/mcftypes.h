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

#ifndef __MCF_TYPES_H__
#define __MCF_TYPES_H__

#define MCF_BEGIN_DECLS extern "C" {
#define MCF_END_DECLS }

#ifndef	FALSE
#define	FALSE (0)
#endif

#ifndef	TRUE
#define	TRUE (!FALSE)
#endif

#ifndef	NOT_USED
#define	NOT_USED -1
#endif

#ifndef	USED
#define	USED 1
#endif

typedef signed char mcf8;
typedef signed short mcf16;
typedef signed int mcf32;

typedef unsigned char mcfu8;
typedef unsigned short mcfu16;
typedef unsigned int mcfu32;

typedef char mcfchar;
typedef short mcfshort;
typedef long mcflong;
typedef int mcfint;
typedef char mcfboolean;

typedef unsigned char mcfbyte;
typedef unsigned char mcfuchar;
typedef unsigned short mcfushort;
typedef unsigned long mcfulong;
typedef unsigned int mcfuint;

typedef float mcffloat;
typedef double mcfdouble;

/* mcfwindow should be a struct - containing a flag whether it is a virtual window or not*/
#define MCFWINDOW_INVALID 0
#define MCFWINDOW_VIRTUAL_DIM -1

typedef void* mcfwindow;
typedef void* mcfimage;
typedef void* mcffont;
typedef void* mcfdrawctx;
typedef mcfint mcftouchdevice;
#define MCFTOUCHDEVICE_INVALID -1

typedef struct _McfPoint {
    mcfint x, y;
}McfPoint;

typedef struct _McfSize {
    mcfint width, height;
}McfSize;

typedef struct _McfRect {
    mcfint left, top, right, bottom;
}McfRect;

typedef struct _McfRectangle {
    mcfint x, y, width, height;
}McfRectangle;

typedef struct _McfColor {
    mcfbyte r, g, b, a;
}McfColor;

#define MCF_MAKEWORD(low, high) ((mcfu16)(((mcfu8)(low)) | ((mcfu16)((mcfu8)(high))) << 8))
#define MCF_MAKELONG(low, high) ((mcfu32)(((mcfu16)(low)) | ((mcfu32)((mcfu16)(high))) << 16))
#define MCF_LOWORD(i32) ((mcfu16)((i32) & 0xFFFF))
#define MCF_HIWORD(i32) ((mcfu16)(((mcfu32)(i32) >> 16) & 0xFFFF))
#define MCF_LOBYTE(i16) ((mcfu8)((i16) & 0xFF))
#define MCF_HIBYTE(i16) ((mcfu8)(((mcfu16)(i16) >> 8) & 0xFF))


#endif //__MCF_TYPES_H__
