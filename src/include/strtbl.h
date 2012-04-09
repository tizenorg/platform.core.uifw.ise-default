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

#ifndef __STRING_TABLE_H__
#define __STRING_TABLE_H__

#include <libintl.h>

#define _(str) gettext(str)
#define gettext_noop(str) (str)
#define N_(str) gettext_noop(str)

struct _tc { /* text class */
	char *name; /* text class name */
	int size; /* font size */
};

struct _ts { /* text set */
	char *name; /* part name */
	char *msgid; /* message ID */
};

#endif /* __STRING_TABLE_H__ */
