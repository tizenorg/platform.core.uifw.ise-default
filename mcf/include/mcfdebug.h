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


#ifndef __MCF_DEBUG_H__
#define __MCF_DEBUG_H__

#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


#define MCF_DEBUG_ELAPASED_TIME_START()
#define MCF_DEBUG_ELAPASED_TIME_END()
#define MCF_DEBUG()
#define MCF_DEBUG_START()
#define MCF_DEBUG_END()


#define mcf_assert(_e)
#define mcf_assert_return(_e)
#define mcf_assert_return_false(_e)
#define mcf_assert_return_null mcf_assert_return_false


#define mcf_check_arrindex(idx, arrsize) ((idx >= 0) && (idx < arrsize))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __MCF_DEBUG_H__ */
