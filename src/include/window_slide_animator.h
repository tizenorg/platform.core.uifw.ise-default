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

#include <Ecore_X.h>
#include <Ecore_IMF.h>
#include <Elementary.h>
#include <mcf.h>

#define CLOSE_WINDOW_HEIGHT 40

class CKesslerISEDecorator;

class CWindowSlideAnimator
{
	friend class CKesslerISEDecorator;
public:
	CWindowSlideAnimator();
	~CWindowSlideAnimator();

	void			set_decorator(CKesslerISEDecorator *decorator) { mDecorator = decorator; }
	CKesslerISEDecorator*	get_decorator() { return mDecorator; }

	void			set_target_window(Evas_Object *window);
	void			set_start_position(int startx, int starty);
	void			set_end_position(int endx, int endy);

	Evas_Object*	get_target_window()			{ return mTargetWindow; }
	McfPoint		get_start_position()		{ return mStartPt; }
	McfPoint		get_end_position()			{ return mEndPt; }

	void			start_animation();
	void			stop_animation();

	static void		get_window_position(int width, int height, int *showx, int *showy, int *hidex, int *hidey);

protected:
	static Eina_Bool timer_event(void *data);

	void			set_timer_handle(Ecore_Timer *handle)	{ mTimer = handle; }
	Ecore_Timer*	get_timer_handle()			{ return mTimer; }

	struct timeval*	get_start_time()			{ return &mStartTime; }
	float			get_elapsed_time();

protected:
	static const mcffloat ANIM_TIME = 0.30f;
	static const mcffloat ANIM_INTERVAL = (1.0f / 60); // Aiming for 30 fps

	Evas_Object* mTargetWindow;
	McfPoint mStartPt, mEndPt;

	struct timeval mStartTime;
	Ecore_Timer *mTimer;

	CKesslerISEDecorator *mDecorator;
};

class CKesslerISEDecorator
{
public:
	CKesslerISEDecorator();
	~CKesslerISEDecorator();

	void			create();
	void			destroy();

	void			set_target_window(Evas_Object *window);

	void			start_show_animation(int width, int height);
	void			start_hide_animation(int width, int height);
	void			finish_show_animation(int width, int height);

	void			animation_timer_cb();

protected:
	static Eina_Bool timer_event(void *data);

	void			set_timer_handle(Ecore_Timer *handle)	{ mTimer = handle; }
	Ecore_Timer*	get_timer_handle()			{ return mTimer; }

protected:
	static const mcffloat CLOSE_TIME = 2.0f;
	static const int CLOSE_BAR_HEIGHT = CLOSE_WINDOW_HEIGHT;

	enum ANIMATION_TYPE {
		ANIMATION_TYPE_SHOW,
		ANIMATION_TYPE_HIDE,
	};
	ANIMATION_TYPE mAnimationType;

	CWindowSlideAnimator mAnimator;

	struct timeval mStartTime;
	Ecore_Timer *mTimer;
};
