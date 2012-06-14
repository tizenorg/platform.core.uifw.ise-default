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



#include "mcfconfig.h"

#ifndef __MCF_ACTIONSTATE_H__
#define __MCF_ACTIONSTATE_H__

//MCF_BEGIN_DECLS

namespace mcf
{

#define MAX_ACTION_BUF 50

/**
 * @brief The base class to work as a soft-based keyboard
 *
 * This class implements all functions for working as a soft-based keyboard
 * In side of ISE developer, they can modify it by their requirements.
 */
class CMCFActionState
{
private:
    static CMCFActionState* sInstance; /* For singleton */
    CMCFActionState();
public:
    ~CMCFActionState();

    /* for singleton */
    static CMCFActionState* get_instance();

    /* reset the action state to init status */
    void reset();
    void clear_action_buf();
    MCFActionState get_cur_action_state() {
        return mCurActionState;
    }
    void set_cur_action_state(MCFActionState state) {
        mCurActionState = state;
    }

public:
    /* stores the state information */
    MCFActionState mActionBuf[MAX_ACTION_BUF];
    MCFActionState mCurActionState;
};

}

//MCF_END_DECLS

#endif //__MCF_ACTIONSTATE_H__
