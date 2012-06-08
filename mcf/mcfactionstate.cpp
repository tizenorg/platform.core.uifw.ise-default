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



#include <memory.h>
#include "mcfactionstate.h"
#include "mcfdebug.h"

using namespace mcf;

CMCFActionState* CMCFActionState::sInstance = NULL; /* For singleton */

CMCFActionState::CMCFActionState()
{
    MCF_DEBUG();

    reset();
}

CMCFActionState::~CMCFActionState()
{
    MCF_DEBUG();
}

CMCFActionState*
CMCFActionState::get_instance()
{
    if (!sInstance) {
        sInstance = new CMCFActionState();
    }
    return (CMCFActionState*)sInstance;
}


void
CMCFActionState::reset()
{
    mCurActionState = ACTION_STATE_BASE_INIT;
    memset(mActionBuf, 0x00, sizeof(mActionBuf));
}

void
CMCFActionState::clear_action_buf()
{

}
