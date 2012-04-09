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

#include "mcftypes.h"
#include "mcfstructs.h"
#include "mcfuibuilder.h"
#include "mcfcontroller.h"

#ifndef __MCF_CORE_H__
#define __MCF_CORE_H__

//MCF_BEGIN_DECLS

namespace mcf
{
/**
 * @brief The base class to work as a soft-based keyboard
 *
 * This class implements all functions for working as a soft-based keyboard
 * In side of ISE developer, they can modify it by their requirements.
 */
class CMCFCore
{
public:
    CMCFCore();
    CMCFCore(mcfwindow parentWnd);

    ~CMCFCore();

private:

public:
    mcfboolean run();
    void show();
    void hide();

    void set_event_callback(IMCFEventCallback *callback);
    mcfboolean set_input_mode(mcf8 mode, mcfboolean clearEverything = FALSE);
    void set_update_pending(mcfboolean pend, mcfboolean autoUpdate = TRUE);

    mcf8 get_display_mode();
    mcf8 get_input_mode();

    mcfint set_private_key(McfPrivateKeyProperties* properties, mcfboolean fRedraw);
    mcfint set_private_key(mcfshort inputModeIdx, mcfbyte layoutIdx, mcfbyte keyIdx,
    		               mcfchar* label, mcfchar* imagelabel[MCF_BUTTON_STATE_MAX],
    		               mcfchar* imagebg[MCF_BUTTON_STATE_MAX], mcfulong keyEvent,
    		               mcfchar *keyValue, mcfboolean fRedraw);
    void unset_private_key(mcfint customID);
    mcfbyte find_keyidx_by_customid(mcfshort inputModeIdx, mcfbyte layoutIdx, mcfshort customID);
    void clone_keyproperties(McfPrivateKeyProperties* priv, mcfbyte inputModeIdx, mcfbyte layoutIdx, mcfbyte keyIdx);

    void enable_button(mcfbyte keyIdx);
    void disable_button(mcfbyte keyIdx);

    mcfboolean set_cur_themename(const mcfchar *themename);
    mcfboolean set_cur_sublayout_id(mcfint id);
    mcfint get_cur_sublayout_id();

    MCFShiftState get_shift_state();
    void set_shift_state(MCFShiftState state);

    void notify_app_focus_changed();
    void reset_popup_timeout();
    void close_all_popups();

    void get_window_size(mcfint *width, mcfint *height);
    void get_screen_resolution(mcfint *width, mcfint *height);

    mcf16 get_scale_x(mcf16 x);
    mcf16 get_scale_y(mcf16 y);
    mcffloat get_scale_rate();

    mcfchar* get_composed_path(mcfchar* buf, int bufLength, const mcfchar* path);

    mcfint get_multi_touch_context_num();
    mcfboolean get_multi_touch_event(mcfint seqorder, McfKeyEventDesc *desc);

    void set_debug_mode(MCFDebugMode mode);
    MCFDebugMode get_debug_mode();

    mcfboolean set_longkey_duration(mcflong msc);
    mcfboolean set_longkey_cancel_dist(mcfshort dist);
    mcfboolean set_repeatkey_duration(mcflong msc);
    mcfboolean set_short_longkey_duration(mcflong msc);

    mcfboolean set_button_delay_duration(mcflong msc);

    void disable_input_events(mcfboolean disabled);
};

}

//MCF_END_DECLS

#endif //__MCF_CORE_H__
