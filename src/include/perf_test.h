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

