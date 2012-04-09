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

/* Definition for debug message trace */
#define mzsc(m_code)						"\x1b[" # m_code
#define mzs_sync()							(void)fflush(stdout)
#define mzs_puts(m_code)					(void)fputs(m_code,stdout)
#define mzs_clear_screen()					mzs_puts(mzsc(H) mzsc(J))
#define mzs_move(m_x,m_y)					mzs_puts(mzsc(m_y;m_x ## H))
#define mzs_move_x(m_y)					mzs_puts(mzsc(;m_y ## H))
#define mzs_move_y(m_y)					mzs_puts(mzsc(m_y ## d))
#define mzs_puts_xy(m_x,m_y,m_message)	mzs_puts(mzsc(m_y;m_x ## H) m_message)
#define mzc_normal		mzsc(0m)
#define mzc_black		mzsc(1;30m)
#define mzc_red			mzsc(1;31m)
#define mzc_green		mzsc(1;32m)
#define mzc_yellow		mzsc(1;33m)
#define mzc_blue			mzsc(1;34m)
#define mzc_magenta		mzsc(1;35m)
#define mzc_cyan		mzsc(1;36m)
#define mzc_white		mzsc(1;37m)

#define DEBUG() printf("%04d" mzc_blue " %s" mzc_normal ".\n", __LINE__, __FUNCTION__ );
//#define DEBUG()
