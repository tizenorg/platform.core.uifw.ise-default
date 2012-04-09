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

#ifndef __DEF_perf_test_H_
#define __DEF_perf_test_H_

#include <Ecore.h>

/* performance evaluation code */
#define PERF_TEST_DEBUG
#ifdef PERF_TEST_DEBUG
#define PERF_TEST_START(szFunc) \
	static double time1, time2, diff, acc_time;\
	printf("%s start\n", szFunc); \
	time1=ecore_time_get();\
	acc_time = 0;

#define PERF_TEST_MID(szFunc) \
	time2 = ecore_time_get();\
	diff=time2-time1;\
	acc_time+=diff;\
	printf("%s: Time: %15.3lf\n", szFunc, diff); \
	time1=ecore_time_get();

#define PERF_TEST_END(szFunc) \
	time2 = ecore_time_get();\
	diff = time2-time1;\
	acc_time += diff;\
	printf("%s end: Time: %15.3lf\n", szFunc, acc_time);

#define MEASURE_LINK_LOAD_TIME() \
{ \
	struct timeval timev;\
	gettimeofday(&timev, NULL);\
	printf("Main Entry   : %d.%d\n", timev.tv_sec, timev.tv_usec);\
	fflush(stdout);\
}

#else
#define PERF_TEST_START(szFunc)
#define PERF_TEST_MID(szFunc)
#define PERF_TEST_END(szFunc)
#define MEASURE_LINK_LOAD_TIME()
#endif /* End of PERF_TEST_DEBUG */

#endif /* __DEF_perf_test_H__ */

