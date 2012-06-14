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



#ifndef __MCF_VERSION_H__
#define __MCF_VERSION_H__

#define MCF_VERSION "111209-1"

#endif //__MCF_VERSION_H

/*
Version History
    100810-1
        - Started recording version information
    101208-1
        - Added Shift Lock state to MCFShiftState and implemented drag decoration feature
    101210-1
        - Fixed Memory Leak regarding evas_textblock_style_free()
    101215-1
        - Added external interfaces for en/disabling magnifier, sound, vibration
        - Added external interfaces for setting longkey timeout / longkey cancel distance
        - Added EFL Backend implementations for dim windows
    101216-1
        - Fixed the bug that fast magnifier window does not work properly in landscape mode
        - Removed cache flush codes except in hide case
        - Disable longkey timer if dragging is recognized on direction button
    110112-1
        - Added experimental preedit zoom window feature (Not official)
    110122-1
        - Applied MCF_AUTO_DETECT_PORTRAIT_LANDSCAPE feature to mcf_input_mode_popup_configure and mcf_modifier_decoration
    110201-1
        - Fixed the bug that magnifier window does not get closed when opening autopopup window
    110208-1
        - Changed not to call ecore_x_window_geometry_get and ecore_x_window_size_get too often
    110217-1
        - Implemented APIs for setting magnifier duration
    110518-1
        - Implemented SubLayout feature
	111209-1
        - MCF Cleanup and SLP coding guidelines applied
*/
