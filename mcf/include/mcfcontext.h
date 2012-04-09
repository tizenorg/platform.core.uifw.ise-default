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

#ifndef __MCF_CONTEXT_H__
#define __MCF_CONTEXT_H__

//MCF_BEGIN_DECLS
#include <sys/time.h>
#include "mcftypes.h"
#include "mcfconfig.h"
#include "mcfstructs.h"
#include "mcfeventcallback.h"
#include <map>
#include <list>
#include <string>

namespace mcf
{

typedef struct { // A context information dependant on each multitouch events
    mcfboolean mUsed;

    mcf8 mCurPressedKey;
    mcfwindow mCurPressedWindow;
    McfPoint mCurPressedPoint;
    struct timeval mCurPressedTime;

    mcfwindow mCurMoveWindow;
    McfPoint mCurMovePoint;

    McfPoint mFarthestMovePoint;
    mcfint mFarthestMoveDist;

    mcf8 mPrevPressedKey;
    mcfwindow mPrevPressedWindow;

    MCFDragState mCurDragState;
    MCFDragState mPrevDragState;

    MCFKeyModifier mCurKeyModifier;

    mcf16 mEventUniqueId;

    mcfboolean mIsSubEvent;
} MultiTouchContext;

/**
 * @brief The base class to work as a soft-based keyboard
 *
 * This class implements all functions for working as a soft-based keyboard
 * In side of ISE developer, they can modify it by their requirements.
 */
class CMCFContext
{
private:
    static CMCFContext* sInstance; /* For singleton */
    CMCFContext();
public:
    ~CMCFContext();

    /* reset to the default context status */
    void reset();

    /* for singleton */
    static CMCFContext* get_instance();

    MCFDisplay get_display() const {
        return mDisplay;
    }
    void set_display(MCFDisplay val) {
        mPrevDisplay = mDisplay;
        mDisplay = val;
    }

    mcf8 get_input_mode() const {
        return mInputMode;
    }
    void set_input_mode(mcf8 val) {
        mPrevInputMode = mInputMode;
        mInputMode = val;
    }

    mcfshort get_base_layout() const;
    void set_base_layout(mcfshort val);

    mcfshort get_popup_layout(mcfwindow window) const;
    void set_popup_layout(mcfwindow window, mcfshort val);

    MCFDisplay get_prev_display() const {
        return mPrevDisplay;
    }
    mcf8 get_prev_inputmode() const {
        return mPrevInputMode;
    }

    mcfboolean get_hidden_state() const {
        return mHidden;
    }
    void set_hidden_state(mcfboolean val) {
        mHidden = val;
    }

    MCFShiftState get_shift_state() const {
        MCFShiftState ret = MCF_SHIFT_STATE_OFF;
        if(mShiftState >= 0 && mShiftState < MCF_SHIFT_STATE_MAX) {
            ret = mShiftState;
        }
        return ret;
    }
    void set_shift_state(MCFShiftState val) {
        if(val >= 0 && val < MCF_SHIFT_STATE_MAX) {
            mShiftState = val;
        }
    }

    mcf8 get_last_pressed_key() {
        return mLastPressedKey;
    }
	void set_last_pressed_key(mcf8 val) {
        mLastPressedKey = val;
    }
	mcfwindow get_last_pressed_window() {
        return mLastPressedWindow;
    }
	void set_last_pressed_window(mcfwindow window) {
        mLastPressedWindow = window;
    }

    mcftouchdevice get_last_touch_device_id() const {
        return mLastTouchDeviceId;
    }
    void set_last_touch_device_id(mcftouchdevice touchid) {
        mLastTouchDeviceId = touchid;
    }

    void create_multi_touch_context(mcftouchdevice touchid, mcfboolean isSubEvent = FALSE);
    void destroy_multi_touch_context(mcftouchdevice touchid);
    MultiTouchContext* find_multi_touch_context(mcftouchdevice touchid);

    mcf16 get_cur_pressed_event_id(mcftouchdevice touchid);
    void set_cur_pressed_event_id(mcftouchdevice touchid, mcf16 id);
    mcf8 get_cur_pressed_key(mcftouchdevice touchid);
    void set_cur_pressed_key(mcftouchdevice touchid, mcf8 val);
    mcfwindow get_cur_pressed_window(mcftouchdevice touchid);
    void set_cur_pressed_window(mcftouchdevice touchid, mcfwindow window);
    McfPoint get_cur_pressed_point(mcftouchdevice touchid);
    void set_cur_pressed_point(mcftouchdevice touchid, mcfint x, mcfint y);
    struct timeval get_cur_pressed_time(mcftouchdevice touchid);
    void set_cur_pressed_time(mcftouchdevice touchid);

    McfPoint get_cur_move_point(mcftouchdevice touchid);
    void set_cur_move_point(mcftouchdevice touchid, mcfint x, mcfint y);
    mcfwindow get_cur_move_window(mcftouchdevice touchid);
    void set_cur_move_window(mcftouchdevice touchid, mcfwindow window);
    McfPoint get_farthest_move_point(mcftouchdevice touchid);
    void set_farthest_move_point(mcftouchdevice touchid, mcfint x, mcfint y);
    mcfint get_farthest_move_dist(mcftouchdevice touchid);

    mcf8 get_prev_pressed_key(mcftouchdevice touchid);
    void set_prev_pressed_key(mcftouchdevice touchid, mcf8 val);
    mcfwindow get_prev_pressed_window(mcftouchdevice touchid);
    void set_prev_pressed_window(mcftouchdevice touchid, mcfwindow window);

    MCFDragState get_cur_drag_state(mcftouchdevice touchid);
    void set_cur_drag_state(mcftouchdevice touchid, MCFDragState state);
    MCFKeyModifier get_cur_key_modifier(mcftouchdevice touchid);
    void set_cur_key_modifier(mcftouchdevice touchid, MCFKeyModifier modifier);

    MCFDragState get_prev_drag_state(mcftouchdevice touchid);
    void set_prev_drag_state(mcftouchdevice touchid, MCFDragState state);

    mcfint get_multi_touch_context_num();
    mcfboolean get_multi_touch_event(mcfint seqorder, McfKeyEventDesc *desc);
    mcfint get_multi_touch_event_order(mcftouchdevice touchid);

    mcfint get_cur_sublayout_id();
    mcfboolean set_cur_sublayout_id(mcfint id);

protected:
	/* stores the current context */
    MCFDisplay mDisplay;
    mcf8 mInputMode;

    /* stores the last context */
    MCFDisplay mPrevDisplay;
    mcf8 mPrevInputMode;

    mcfboolean mHidden;
    MCFShiftState mShiftState;

    mcfint mCurSubLayoutID;

    mcfwindow mLastPressedWindow;
    mcf8 mLastPressedKey;

    mcftouchdevice mLastTouchDeviceId;
    std::map<mcftouchdevice, MultiTouchContext> mMultiTouchContext;
    std::list<mcftouchdevice> mMultiTouchSeq;
};

}

//MCF_END_DECLS

#endif //__MCF_CONTEXT_H__
