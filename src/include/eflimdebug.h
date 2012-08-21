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
