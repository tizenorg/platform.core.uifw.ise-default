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
