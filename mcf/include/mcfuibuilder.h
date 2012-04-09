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

#include "mcfgwes.h"
#include "mcfutils.h"

#ifndef __MCF_UIBUILDER_H__
#define __MCF_UIBUILDER_H__

//MCF_BEGIN_DECLS

namespace mcf
{

/**
 * @brief The base class to work as a soft-based keyboard
 *
 * This class implements all functions for working as a soft-based keyboard
 * In side of ISE developer, they can modify it by their requirements.
 */
class CMCFUIBuilder
{
    friend class CMCFFontProxy;

private:
    static CMCFUIBuilder* sInstance; /* For singleton */
    CMCFUIBuilder();
public:
    ~CMCFUIBuilder();

    static CMCFUIBuilder* get_instance();

    void init(mcfwindow parentWnd);
    mcfboolean show_layout(const mcfwindow window, const mcf16 x = 0, const mcf16 y = 0,
    		               const mcf16 width = 0, const mcf16 height = 0);
    mcfboolean draw_button(const mcfwindow window, mcfdrawctx drawCtx, const mcf16 keyIdx,
    		               const MCFButtonState state, const mcfboolean fForceDrawBG = FALSE);
    mcfboolean show_autopopup(const mcfwindow parent, mcfdrawctx drawCtx, const mcf16 keyIdx);
private:
    mcfboolean draw_button_all(const mcfwindow window, const mcfdrawctx drawCtx, const mcf16 x,
    		                   const mcf16 y, const mcf16 width, const mcf16 height);
    mcfboolean draw_button_bg_by_sw(const mcfwindow window, const mcfdrawctx drawCtx,
    		                        const mcf16 keyIdx, const MCFButtonState state);
    mcfboolean draw_window_bg_by_sw(const mcfwindow window, const mcfdrawctx drawCtx, const McfSize size,
                                    const mcfdouble lineWidth, const McfColor lineColor, const McfColor fillColor);
    mcfboolean draw_button_bg_by_img(const mcfwindow window, const mcfdrawctx drawCtx,const mcf16 keyIdx,
		                             const MCFButtonState state, const mcfboolean fShift);
    mcfboolean draw_button_bg_by_layoutimg(const mcfwindow window, const mcfdrawctx drawCtx, const mcf16 keyIdx,
		                                   const MCFButtonState state, const mcfboolean fShift);
    mcfboolean draw_button_label(const mcfwindow window, const mcfdrawctx drawCtx, const mcf16 keyIdx,
                                 const MCFButtonState state, const mcfboolean fShift);

protected:

    /* Backend Primitive */
    CMCFGwes *mGwes;
    CMCFUtils *mUtils;
};

}

//MCF_END_DECLS

#endif //__MCF_UIBUILDER_H__
