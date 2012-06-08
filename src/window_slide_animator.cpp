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



#define Uses_SCIM_HELPER

#include <scim.h>
#include <mcf.h>
#include "ise.h"
#include "include/window_slide_animator.h"
#include <ui-gadget.h>

using namespace mcf;
extern struct ui_gadget *ug;
extern Evas_Object *box;
extern Evas_Object *main_window;

CWindowSlideAnimator::CWindowSlideAnimator()
{
	mTargetWindow = NULL;
	mTimer = NULL;
}

CWindowSlideAnimator::~CWindowSlideAnimator()
{
	mTargetWindow = NULL;
	if (mTimer) {
		ecore_timer_del(mTimer);
		mTimer = NULL;
	}
}

void CWindowSlideAnimator::set_target_window(Evas_Object *window)
{
	mTargetWindow = window;
}

void CWindowSlideAnimator::set_start_position(int startx, int starty)
{
	mStartPt.x = startx;
	mStartPt.y = starty;
}

void CWindowSlideAnimator::set_end_position(int endx, int endy)
{
	mEndPt.x = endx;
	mEndPt.y = endy;
}

void CWindowSlideAnimator::start_animation()
{
	if (mTimer) {
		ecore_timer_del(mTimer);
		mTimer = NULL;
	}
	gettimeofday(&mStartTime, NULL);
	mTimer = ecore_timer_add(ANIM_INTERVAL,
			 CWindowSlideAnimator::timer_event, this);
	evas_object_move(mTargetWindow, mStartPt.x, mStartPt.y);
}

void CWindowSlideAnimator::stop_animation()
{
	if (mTimer) {
		ecore_timer_del(mTimer);
		mTimer = NULL;
	}
	evas_object_move(mTargetWindow, mEndPt.x, mEndPt.y);
}

Eina_Bool CWindowSlideAnimator::timer_event(void *data)
{
	MCF_DEBUG();
	int retVal = false;

	CWindowSlideAnimator *pAnimator =
	    static_cast<CWindowSlideAnimator *>(data);
	if (pAnimator) {
		mcffloat fElapsedTime = pAnimator->get_elapsed_time();

		if (fElapsedTime < ANIM_TIME && fElapsedTime > 0) {
			int posx = pAnimator->get_start_position().x
					   + ((float)(pAnimator->get_end_position().x -
								  pAnimator->get_start_position().x)) *
					  	 (fElapsedTime / ANIM_TIME);
			int posy = pAnimator->get_start_position().y +
					   ((float)(pAnimator->get_end_position().y -
						   		pAnimator->get_start_position().y)) *
					   (fElapsedTime / ANIM_TIME);
			Evas_Object *window = pAnimator->get_target_window();
			evas_object_move(pAnimator->get_target_window(), posx,
					 posy);
			retVal = true;

		} else {
			evas_object_move(pAnimator->get_target_window(),
					 pAnimator->get_end_position().x,
					 pAnimator->get_end_position().y);
			ecore_timer_del(pAnimator->get_timer_handle());
			pAnimator->set_timer_handle(NULL);
			if (pAnimator->get_decorator()) {
				pAnimator->
				    get_decorator()->animation_timer_cb();
			}
			if (ug) {
				ug_destroy(ug);
				ug = NULL;
			}
			if (box) {
				elm_win_resize_object_del(main_window, box);
				evas_object_del(box);
				box = NULL;
			}

		}
	}

	return TRUE;
}

void CWindowSlideAnimator::get_window_position(int width, int height,
						      int *showx,
						      int *showy, int *hidex,
						      int *hidey)
{
	int lshowx, lshowy;
	int lhidex, lhidey;

	Evas_Coord win_w, win_h;
	ecore_x_window_size_get(ecore_x_window_root_first_get(), &win_w,
				&win_h);
	lshowx = (win_w - width) / 2;
	lshowy = win_h - height;
	lhidex = lshowx;
	lhidey = win_h;

	if (showx)
		*showx = lshowx;
	if (showy)
		*showy = lshowy;
	if (hidex)
		*hidex = lhidex;
	if (hidey)
		*hidey = lhidey;
}

mcffloat CWindowSlideAnimator::get_elapsed_time()
{
	struct timeval tCurrentTime;
	struct timeval tDelta;

	gettimeofday(&tCurrentTime, NULL);
	timersub(&tCurrentTime, &mStartTime, &tDelta);

	return (tDelta.tv_sec) + (tDelta.tv_usec / 1000000.0f);
}

extern scim::HelperAgent helper_agent;

CKesslerISEDecorator::CKesslerISEDecorator()
{
	mTimer = NULL;
}

CKesslerISEDecorator::~CKesslerISEDecorator()
{
	destroy();
}

void CKesslerISEDecorator::create()
{
	destroy();

	mAnimator.set_decorator(this);
}

void CKesslerISEDecorator::destroy()
{
	if (mTimer) {
		ecore_timer_del(mTimer);
		mTimer = NULL;
	}
}

void CKesslerISEDecorator::set_target_window(Evas_Object *window)
{
	mAnimator.set_target_window(window);
}

void CKesslerISEDecorator::start_show_animation(int width, int height)
{
	int startx, starty;
	int endx, endy;

	CWindowSlideAnimator::get_window_position(width, height,
							 &endx, &endy, &startx,
							 &starty);

	if (startx != mAnimator.get_start_position().x ||
	    starty != mAnimator.get_start_position().y ||
	    endx != mAnimator.get_end_position().x ||
	    endy != mAnimator.get_end_position().y ||
	    ecore_timer_pending_get(mAnimator.get_timer_handle()) == 0 ||
	    mAnimator.get_timer_handle() == NULL) {
		mAnimator.set_start_position(startx, starty);
		mAnimator.set_end_position(endx, endy);
		mAnimator.start_animation();
		mAnimationType = ANIMATION_TYPE_SHOW;
		helper_agent.update_input_context(ECORE_IMF_INPUT_PANEL_STATE_EVENT,
								ECORE_IMF_INPUT_PANEL_STATE_WILL_SHOW);
	}
}

#include <X11/Xlib.h>
void CKesslerISEDecorator::start_hide_animation(int width, int height)
{
	int startx, starty;
	int endx, endy;

	CWindowSlideAnimator::get_window_position(width, height,
							 &startx, &starty,
							 &endx, &endy);

	McfRectangle rect;
	Window junkwin;
	Ecore_X_Window_Attributes attrs;
	CMCFWindows *windows = CMCFWindows::get_instance();
	ecore_x_window_geometry_get(elm_win_xwindow_get
				    (static_cast <
				     Evas_Object *
				     >(windows->get_base_window())), &(rect.x),
				    &(rect.y), &(rect.width), &(rect.height));
	ecore_x_window_attributes_get(elm_win_xwindow_get
				      (static_cast <
				       Evas_Object *
				       >(windows->get_base_window())), &attrs);
	XTranslateCoordinates((Display *) ecore_x_display_get(),
			      (Drawable) elm_win_xwindow_get(static_cast <
							     Evas_Object *
							     >
							     (windows->get_base_window
							      ())), attrs.root,
			      -attrs.border, -attrs.border, &(rect.x),
			      &(rect.y), &junkwin);

	/* Do not start animation again if the same animation is currently playing */
	if (mAnimator.get_end_position().x != endx
	    || mAnimator.get_end_position().y != endy
	    || mAnimator.get_timer_handle() == NULL) {
		mAnimator.set_start_position(rect.x, rect.y);
		mAnimator.set_end_position(endx, endy);
		mAnimator.start_animation();
		mAnimationType = ANIMATION_TYPE_HIDE;
	}
}

void CKesslerISEDecorator::finish_show_animation(int width, int height)
{
	int startx, starty;
	int endx, endy;

	CWindowSlideAnimator::get_window_position(width, height,
							 &endx, &endy, &startx,
							 &starty);

	if ((startx != mAnimator.get_start_position().x ||
	     starty != mAnimator.get_start_position().y ||
	     endx != mAnimator.get_end_position().x ||
	     endy != mAnimator.get_end_position().y)
	    || mAnimator.get_timer_handle() == NULL) {
		mAnimator.set_start_position(startx, starty);
		mAnimator.set_end_position(endx, endy);
		mAnimator.stop_animation();
#ifdef HAVE_CONFORMANT_AUTOSCROLL
		_send_keypad_geom_atom_info(main_window, KEYPAD_STATE_ON);
#endif
	}
}

Eina_Bool CKesslerISEDecorator::timer_event(void *data)
{
	CKesslerISEDecorator *decorator =
		static_cast<CKesslerISEDecorator *>(data);
	printf("CKesslerISEDecorator::timer_event : %p\n", decorator);

	if (decorator) {
		decorator->set_timer_handle(NULL);
	}

	return false;
}

void CKesslerISEDecorator::animation_timer_cb()
{
	if (mAnimationType == ANIMATION_TYPE_SHOW) {
		helper_agent.update_input_context
		    (ECORE_IMF_INPUT_PANEL_STATE_EVENT,
		     ECORE_IMF_INPUT_PANEL_STATE_SHOW);
#ifdef HAVE_CONFORMANT_AUTOSCROLL
		_send_keypad_geom_atom_info(main_window, KEYPAD_STATE_ON);
#endif

	} else {
		helper_agent.update_input_context
		    (ECORE_IMF_INPUT_PANEL_STATE_EVENT,
		     ECORE_IMF_INPUT_PANEL_STATE_HIDE);
		// Memory optimization */
		elm_cache_all_flush();
	}
}
