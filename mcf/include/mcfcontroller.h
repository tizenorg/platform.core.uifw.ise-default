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



#include "mcfgwes.h"
#include "mcfutils.h"
#include "mcfcontext.h"
#include "mcfeventcallback.h"

#ifndef __MCF_CONTROLLER_H__
#define __MCF_CONTROLLER_H__

//MCF_BEGIN_DECLS

namespace mcf
{
enum McfInternalSignal {
    MCF_SIG_START,
    MCF_SIG_SHOW,
    MCF_SIG_HIDE,
    MCF_SIG_INPMODE_CHANGE,
    MCF_SIG_DISP_CHANGE,
    MCF_SIG_POPUP_SHOW,
    MCF_SIG_POPUP_HIDE,
    MCF_SIG_MOUSE_PRESS,
    MCF_SIG_MOUSE_LONG_PRESS,
    MCF_SIG_MOUSE_RELEASE,
    MCF_SIG_KEYEVENT,
    MCF_SIG_FOCUS_CHANGE,

    MCF_SIG_MAXNUM
};

/**
 * @brief The base class to work as a soft-based keyboard
 *
 * This class implements all functions for working as a soft-based keyboard
 * In side of ISE developer, they can modify it by their requirements.
 */
class CMCFController
{
private:
    static CMCFController* sInstance; /* For singleton */
    CMCFController();
public:
    ~CMCFController();

    static CMCFController* get_instance();
    void init();

    void set_event_callback(IMCFEventCallback *callback);

private:
    mcfboolean process_button_pressed_event(mcfwindow window, mcfint x, mcfint y, mcfbyte keyindex, mcftouchdevice touchid);
    mcfboolean process_button_long_pressed_event(mcfwindow window, mcfbyte keyindex, mcftouchdevice touchid);
    mcfboolean process_button_repeat_pressed_event(mcfwindow window, mcfbyte keyindex, mcftouchdevice touchid);
    mcfboolean process_button_move_event(mcfwindow window, mcfint x, mcfint y, mcfbyte keyindex, mcftouchdevice touchid);
    mcfboolean process_button_release_event(mcfwindow window, mcfint x, mcfint y, mcfbyte keyindex, mcftouchdevice touchid);

    mcfboolean configure_autopopup_window(mcfwindow window, mcfbyte keyindex, McfRectangle* rect);

    mcfboolean check_event_transition_enabled(const McfLayoutKeyConfigure *btnFrom, const McfLayoutKeyConfigure *btnTo);
    MCFKeyModifier get_drag_key_modifier(mcfint deltax, mcfint deltay, mcffloat dist,
                                         mcfboolean check_farthest,mcftouchdevice touchid, mcfbyte extraOption);

public:
    void handle_engine_signal(McfInternalSignal signal, mcfwindow skipwindow = MCFWINDOW_INVALID);
    mcfboolean process_input_mode_change(const mcfbyte mode, mcfboolean saveMultikeys = TRUE);
    mcfboolean process_display_change();

    mcfboolean mouse_press(mcfwindow window, mcfint x, mcfint y, mcftouchdevice touchid = 0);
    mcfboolean mouse_release(mcfwindow window, mcfint x, mcfint y, mcftouchdevice touchid = 0);
    mcfboolean mouse_move(mcfwindow window, mcfint x, mcfint y, mcftouchdevice touchid = 0);
    mcfboolean timer_event(const mcf32 id);

    mcfboolean set_longkey_duration(mcflong msc);
    mcfboolean set_longkey_cancel_dist(mcfshort dist);
    mcfboolean set_repeatkey_duration(mcflong msc);
    mcfboolean set_short_longkey_duration(mcflong msc);

    mcfboolean set_button_delay_duration(mcflong msc);

    void clear_multikey_buffer();

    MCFDebugMode get_debug_mode();
    void set_debug_mode(MCFDebugMode mode);

    void disable_input_events(mcfboolean disabled);
protected:

    /* Interval time(msc) for checking long key event */
    mcflong mLongkeyDuration;
    mcfshort mLongkeyCancelDist;
    mcflong mRepeatKeyDuration;
    mcflong mShortLongkeyDuration;

    mcflong mButtonDelayDuration;

    IMCFEventCallback *mEventCallback;

    MCFDebugMode mDebugMode;
    mcfint mDebugVariable;

    mcfint mRepeatKeySeq;

    mcfboolean mInputEventsDisabled;

    /* FIXME : A workaround for restoring multikeyidx, should be implemented after designing whole usecases */
    #define MAX_MULTIKEY_BUFFER 50
    mcfshort mMultiKeyBuffer[MAX_MULTIKEY_BUFFER][3]; /* 0 for layout, 1 for keyindex, 2 for value */
};

}

//MCF_END_DECLS

#endif //__MCF_CONTROLLER_H__
