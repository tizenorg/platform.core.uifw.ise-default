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


#include <stdio.h>
#include <stdlib.h>

#include "mcfcontext.h"
#include "mcfdebug.h"
#include "mcfwindows.h"
#include "mcfutils.h"
#include "mcfresourcecache.h"
#include <algorithm>

using namespace mcf;

CMCFContext* CMCFContext::sInstance = NULL; /* For singleton */

CMCFContext::CMCFContext()
{
    MCF_DEBUG();

    reset();
}

CMCFContext::~CMCFContext()
{
    MCF_DEBUG();
}

CMCFContext*
CMCFContext::get_instance()
{
    if (!sInstance) {
        sInstance = new CMCFContext();
    }
    return (CMCFContext*)sInstance;
}

void
CMCFContext::reset()
{
    MCF_DEBUG();

    mDisplay = static_cast<MCFDisplay>(0);
    mInputMode = 0;

    mShiftState = MCF_SHIFT_STATE_OFF;
    mRotationDegree = 0;
    mPrevDisplay = MCFDISPLAY_PORTRAIT;
    mPrevInputMode = NOT_USED;

    mMultiTouchContext.clear();
    mLastTouchDeviceId = MCFTOUCHDEVICE_INVALID;

    mCurSubLayoutID = SUBLAYOUTID_DEFAULT;
}

mcfshort
CMCFContext::get_popup_layout( mcfwindow window ) const
{
    MCF_DEBUG();

    mcfshort ret = NOT_USED;
    CMCFWindows *windows = CMCFWindows::get_instance();
    McfWindowContext *ctx = windows->get_window_context(window, FALSE);

    if (ctx) {
        ret = ctx->layout;
    }

    return ret;
}

void
CMCFContext::set_popup_layout( mcfwindow window, mcfshort val )
{
    MCF_DEBUG();

    CMCFWindows *windows = CMCFWindows::get_instance();
    McfWindowContext *ctx = windows->get_window_context(window, FALSE);

    if (ctx) {
        ctx->layout = val;
    }
}

void
CMCFContext::set_base_layout( mcfshort val )
{
    MCF_DEBUG();
    CMCFWindows *windows = CMCFWindows::get_instance();
    McfWindowContext *ctx = windows->get_window_context(windows->get_base_window(), FALSE);
    if (ctx) {
        ctx->layout = val;
    }
}

mcfshort
CMCFContext::get_base_layout() const
{
    MCF_DEBUG();

    mcfshort ret = NOT_USED;
    CMCFWindows *windows = CMCFWindows::get_instance();
    McfWindowContext *ctx = windows->get_window_context(windows->get_base_window(), FALSE);
    if (ctx) {
        ret = ctx->layout;
    }

    return ret;
}

mcf16
CMCFContext::get_cur_pressed_event_id(mcftouchdevice touchid)
{
    mcf16 ret = NOT_USED;
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ret = ctx->mEventUniqueId;
    }
    return ret;
}

void
CMCFContext::set_cur_pressed_event_id(mcftouchdevice touchid, mcf16 id)
{
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ctx->mEventUniqueId = id;
    }
}

mcf8
CMCFContext::get_cur_pressed_key(mcftouchdevice touchid)
{
    mcf8 ret = NOT_USED;
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ret = ctx->mCurPressedKey;
    }
    return ret;
}

void
CMCFContext::set_cur_pressed_key(mcftouchdevice touchid, mcf8 val)
{
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ctx->mCurPressedKey = val;
    }
}

mcfwindow
CMCFContext::get_cur_pressed_window(mcftouchdevice touchid)
{
    mcfwindow ret = MCFWINDOW_INVALID;
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ret = ctx->mCurPressedWindow;
    }
    return ret;
}

void
CMCFContext::set_cur_pressed_window(mcftouchdevice touchid, mcfwindow val)
{
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ctx->mCurPressedWindow = val;
    }
}

McfPoint
CMCFContext::get_cur_pressed_point(mcftouchdevice touchid)
{
    McfPoint ret = {0, 0};
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ret = ctx->mCurPressedPoint;
    }
    return ret;
}

void
CMCFContext::set_cur_pressed_point(mcftouchdevice touchid, mcfint x, mcfint y)
{
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ctx->mCurPressedPoint.x = x;
        ctx->mCurPressedPoint.y = y;
        set_farthest_move_point(touchid, x, y); // reset farthest move point
    }
}

McfPoint
CMCFContext::get_cur_move_point(mcftouchdevice touchid)
{
    McfPoint ret = {0, 0};
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ret = ctx->mCurMovePoint;
    }
    return ret;
}

void
CMCFContext::set_cur_move_point(mcftouchdevice touchid, mcfint x, mcfint y)
{
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ctx->mCurMovePoint.x = x;
        ctx->mCurMovePoint.y = y;
    }
}

mcfwindow
CMCFContext::get_cur_move_window(mcftouchdevice touchid)
{
    mcfwindow ret = MCFWINDOW_INVALID;
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ret = ctx->mCurMoveWindow;
    }
    return ret;
}


void
CMCFContext::set_cur_move_window(mcftouchdevice touchid, mcfwindow window)
{
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ctx->mCurMoveWindow = window;
    }
}

struct timeval
CMCFContext::get_cur_pressed_time(mcftouchdevice touchid)
{
    struct timeval ret = {0};
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ret = ctx->mCurPressedTime;
    }
    return ret;
}

void
CMCFContext::set_cur_pressed_time(mcftouchdevice touchid)
{
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        gettimeofday(&(ctx->mCurPressedTime), NULL);
    }
}

McfPoint
CMCFContext::get_farthest_move_point(mcftouchdevice touchid)
{
    McfPoint ret = {0, 0};
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ret = ctx->mFarthestMovePoint;
    }
    return ret;
}

void
CMCFContext::set_farthest_move_point(mcftouchdevice touchid, mcfint x, mcfint y)
{
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        CMCFUtils *utils = CMCFUtils::get_instance();
        mcfint deltax = ctx->mCurPressedPoint.x - x;
        mcfint deltay = ctx->mCurPressedPoint.y - y;
        ctx->mFarthestMoveDist = utils->get_approximate_distance(x, y, ctx->mCurPressedPoint.x, ctx->mCurPressedPoint.y);

        ctx->mFarthestMovePoint.x = x;
        ctx->mFarthestMovePoint.y = y;
    }
}

mcfint
CMCFContext::get_farthest_move_dist(mcftouchdevice touchid)
{
    mcfint ret = 0;
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ret = ctx->mFarthestMoveDist;
    }
    return ret;
}

mcf8
CMCFContext::get_prev_pressed_key(mcftouchdevice touchid)
{
    mcf8 ret = NOT_USED;
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ret = ctx->mPrevPressedKey;
    }
    return ret;
}

void
CMCFContext::set_prev_pressed_key(mcftouchdevice touchid, mcf8 val)
{
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ctx->mPrevPressedKey = val;
    }
}

mcfwindow
CMCFContext::get_prev_pressed_window(mcftouchdevice touchid)
{
    mcfwindow ret = MCFWINDOW_INVALID;
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ret = ctx->mPrevPressedWindow;
    }
    return ret;
}

void
CMCFContext::set_prev_pressed_window(mcftouchdevice touchid, mcfwindow val)
{
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ctx->mPrevPressedWindow = val;
    }
}

MCFDragState
CMCFContext::get_cur_drag_state(mcftouchdevice touchid)
{
    MCFDragState ret = MCF_DRAG_STATE_NONE;
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ret = ctx->mCurDragState;
    }
    return ret;
}

void
CMCFContext::set_cur_drag_state(mcftouchdevice touchid, MCFDragState state)
{
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ctx->mCurDragState = state;
    }
}

MCFDragState
CMCFContext::get_prev_drag_state(mcftouchdevice touchid)
{
    MCFDragState ret = MCF_DRAG_STATE_NONE;
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ret = ctx->mPrevDragState;
    }
    return ret;
}

void
CMCFContext::set_prev_drag_state(mcftouchdevice touchid, MCFDragState state)
{
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ctx->mPrevDragState = state;
    }
}

MCFKeyModifier
CMCFContext::get_cur_key_modifier(mcftouchdevice touchid)
{
    MCFKeyModifier ret = KEY_MODIFIER_NONE;
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ret = ctx->mCurKeyModifier;
    }
    return ret;
}

void
CMCFContext::set_cur_key_modifier(mcftouchdevice touchid, MCFKeyModifier modifier)
{
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        ctx->mCurKeyModifier = modifier;
    }
}

void
CMCFContext::create_multi_touch_context(mcftouchdevice touchid, mcfboolean isSubEvent)
{
    MultiTouchContext newctx;
    memset(&newctx, 0x00, sizeof(MultiTouchContext));
    newctx.mUsed = TRUE;
    newctx.mCurPressedKey = NOT_USED;
    newctx.mCurPressedWindow = MCFWINDOW_INVALID;
    newctx.mEventUniqueId = 0;
    newctx.mPrevPressedKey = NOT_USED;
    newctx.mPrevPressedWindow = MCFWINDOW_INVALID;
    newctx.mCurPressedPoint.x = newctx.mCurPressedPoint.y = 0;
    newctx.mCurPressedTime.tv_sec = newctx.mCurPressedTime.tv_usec = 0;
    newctx.mCurDragState = MCF_DRAG_STATE_NONE;
    newctx.mIsSubEvent = isSubEvent;
    mMultiTouchContext[touchid] = newctx;

    mMultiTouchSeq.insert(mMultiTouchSeq.end(), touchid);

    CMCFUtils *utils = CMCFUtils::get_instance();
    if(utils) {
        for(std::list<mcftouchdevice>::iterator iter = mMultiTouchSeq.begin();iter != mMultiTouchSeq.end();std::advance(iter, 1)) {
            utils->log("LIST : %d\n", *iter);
        }
        utils->log("\n");
    }
}

void
CMCFContext::destroy_multi_touch_context(mcftouchdevice touchid)
{
    MultiTouchContext *ctx = find_multi_touch_context(touchid);
    if(ctx) {
        memset(ctx, 0x00, sizeof(MultiTouchContext));
        mMultiTouchContext.erase(touchid);
    }

    mcfboolean bFound = TRUE;
    std::list<mcftouchdevice>::iterator iter;
    do {
        iter = std::find(mMultiTouchSeq.begin(), mMultiTouchSeq.end(), touchid);
        if(iter != mMultiTouchSeq.end()) {
            mMultiTouchSeq.erase(iter);
        } else {
            bFound = FALSE;
        }
    } while(bFound);

	CMCFUtils *utils = CMCFUtils::get_instance();
	if(utils) {
		for(std::list<mcftouchdevice>::iterator iter = mMultiTouchSeq.begin();iter != mMultiTouchSeq.end();std::advance(iter, 1)) {
			utils->log("LIST : %d\n", *iter);
		}
		utils->log("\n");
	}
}

MultiTouchContext*
CMCFContext::find_multi_touch_context(mcftouchdevice touchid)
{
    MultiTouchContext* ret = NULL;
    std::map<mcftouchdevice, MultiTouchContext>::iterator iter = mMultiTouchContext.find(touchid);
    if(iter != mMultiTouchContext.end()) {
        ret = &(iter->second);
    }

    return ret;
}

mcfint
CMCFContext::get_multi_touch_context_num()
{
    return mMultiTouchSeq.size();
}

mcfboolean
CMCFContext::get_multi_touch_event(mcfint seqorder, McfKeyEventDesc *desc)
{
    mcfboolean ret = FALSE;
    mcfint iSeqIndex = 0;

    CMCFResourceCache *cache = CMCFResourceCache::get_instance();

    for(std::list<mcftouchdevice>::iterator list_iter = mMultiTouchSeq.begin();
        !ret && list_iter != mMultiTouchSeq.end();std::advance(list_iter, 1)) {
            if(iSeqIndex == seqorder) {
                MultiTouchContext *pContext = find_multi_touch_context(*list_iter);
                if(pContext) {
                    const McfLayoutKeyConfigure *configure =
                        cache->get_cur_layout_key_configure(pContext->mCurPressedWindow, pContext->mCurPressedKey);
                    const McfLayoutKeyProperties *properties=
                        cache->get_cur_layout_key_properties(pContext->mCurPressedWindow, pContext->mCurPressedKey);

                    if(properties && configure) {
                        MCFShiftState shiftidx = get_shift_state();
                        if(shiftidx < 0 || shiftidx >= MCF_SHIFT_STATE_MAX) shiftidx = MCF_SHIFT_STATE_OFF;
                        desc->keyEvent = properties->keyEvent[shiftidx][0];
                        desc->keyValue = properties->keyValue[shiftidx][0];
                        desc->keyType = configure->keyType;
                    } else {
                        desc->keyEvent = 0;
                        desc->keyValue = NULL;
                        desc->keyType = KEY_TYPE_NONE;
                    }
                    desc->keyModifier = KEY_MODIFIER_NONE;

                    desc->touchid = (*list_iter);
                    desc->mousePressedPt = pContext->mCurPressedPoint;
                    desc->mouseCurrentPt = pContext->mCurMovePoint;
                    desc->mouseFarthestPt = pContext->mFarthestMovePoint;

                    desc->touch_event_order = iSeqIndex;

                    ret = TRUE;
                }
            }
            iSeqIndex++;
    }

    return ret;
}

mcfint
CMCFContext::get_multi_touch_event_order(mcftouchdevice touchid)
{
    mcfint ret = -1;
    mcfint iSeqIndex = 0;

    CMCFResourceCache *cache = CMCFResourceCache::get_instance();

    for(std::list<mcftouchdevice>::iterator list_iter = mMultiTouchSeq.begin();
        (ret == -1) && list_iter != mMultiTouchSeq.end();std::advance(list_iter, 1)) {
            if(touchid == (*list_iter)) {
                ret = iSeqIndex;
            } else {
                iSeqIndex++;
            }
    }

    return ret;
}

mcfint CMCFContext::get_cur_sublayout_id()
{
    mcfint retid = SUBLAYOUTID_DEFAULT;
    if(mcf_check_arrindex(mInputMode, MAX_INPUT_MODE) && mcf_check_arrindex(mCurSubLayoutID, SUBLAYOUTID_MAX)) {
        if(mcf_sublayout_support_table[mInputMode][mCurSubLayoutID]) {
            /* Return mCurSubLayoutID only if it is supported by current input mode */
            retid = mCurSubLayoutID;
        }
    }
    return retid;
}

mcfboolean CMCFContext::set_cur_sublayout_id(mcfint id)
{
    mcfboolean ret = FALSE;
    if(mcf_check_arrindex(id, SUBLAYOUTID_MAX)) {
        mCurSubLayoutID = id;
    }
    return ret;
}

