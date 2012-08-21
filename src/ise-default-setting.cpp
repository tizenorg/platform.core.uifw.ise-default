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


#include "mcfdebug.h"
#include "ise-default-setting.h"

/* For ISE Interface. The below defines should be included in the file
for processing a controlling data between active appl and this ISE  */
#define Uses_SCIM_UTILITY
#define Uses_SCIM_OBJECT
#define Uses_SCIM_POINTER
#define Uses_SCIM_EVENT
#define Uses_SCIM_HELPER
#define Uses_SCIM_CONFIG_BASE
#include <scim.h>

using namespace scim;

extern HelperAgent helper_agent;
static const char *UUID = "d75857a5-4148-4745-89e2-1da7ddaf7999";

static Context Ise_default_context;

static void _send_command(int cmd, uint32 value);

static void _send_command(int cmd, uint32 value)
{
	MCF_DEBUG();
	Transaction trans;
	trans.put_command(cmd);
	trans.put_data(value);
	helper_agent.send_imengine_event(-1, UUID, trans);
}

/**
 * @brief Send flush command to keyboard engine.
 *
 * @return void
 */

void send_flush (void)
{
	Transaction trans;
	trans.put_command (CMD_FLUSH);
	helper_agent.send_imengine_event (-1, UUID, trans);
}


void hide()
{
	MCF_DEBUG();
	KeyEvent key(SCIM_KEY_Cancel, 0);
	KeyEvent key_release(SCIM_KEY_Cancel, SCIM_KEY_ReleaseMask);
	helper_agent.send_key_event(-1, "", key);
	helper_agent.send_key_event(-1, "", key_release);
}

void reset()
{
	MCF_DEBUG();
	memset(&Ise_default_context, 0x00, sizeof(Ise_default_context));
}

Context get_Ise_default_context()
{
	return Ise_default_context;
}

void change_completion_option(uint32 completionMode)
{
	MCF_DEBUG();
	_send_command(CMD_COMPLETION, completionMode);
	Ise_default_context.CompletionMode = completionMode;
}

void change_ldb_option(uint32 ldbLanguage)
{
	MCF_DEBUG();
	_send_command(CMD_LANGUAGE, ldbLanguage);
	Ise_default_context.Language = ldbLanguage;
}

void change_prediction_onoff(bool fEnable)
{
	DBG(" on/off fEnable=%d \n", fEnable);
	MCF_DEBUG();
	if (fEnable) {
		_send_command(CMD_STATE, ON);
	} else {
		_send_command(CMD_STATE, OFF);
	}
	Ise_default_context.OnOff = fEnable;
}

void change_inputmode(uint32 inputMode)
{
	MCF_DEBUG();
	_send_command(CMD_INPUTMODE, inputMode);
	Ise_default_context.InputMode = inputMode;
}

void change_shiftmode(uint32 shiftMode)
{
	MCF_DEBUG();
	_send_command(CMD_SHIFTMODE, shiftMode);
	Ise_default_context.ShiftMode = shiftMode;
}

void set_shiftmode(uint32 shiftMode)
{
	MCF_DEBUG();
	Ise_default_context.ShiftMode = shiftMode;
}

void change_keypad(uint32 keypad)
{
	DBG("\n\n  change_keypad keypad=%d\n\n", keypad);
	MCF_DEBUG();
	_send_command(CMD_KEYPAD, keypad);
	Ise_default_context.KeypadMode = keypad;
}

void mode_syncronize()
{
	MCF_DEBUG();
	/*this is a temp solution, beacause slot_focus_in is
	 * evoked twice at the first mcf-now shown */
	if (0 == Ise_default_context.Language &&
	    0 == Ise_default_context.KeypadMode &&
	    0 == Ise_default_context.InputMode &&
	    0 == Ise_default_context.OnOff &&
	    0 == Ise_default_context.CompletionMode &&
	    0 == Ise_default_context.SingleCommit)

		return;
	_send_command(CMD_LANGUAGE, Ise_default_context.Language);
	_send_command(CMD_KEYPAD, Ise_default_context.KeypadMode);
	_send_command(CMD_INPUTMODE, Ise_default_context.InputMode);
	_send_command(CMD_STATE, Ise_default_context.OnOff);
	_send_command(CMD_COMPLETION, Ise_default_context.CompletionMode);
	_send_command(CMD_SHIFTMODE, Ise_default_context.ShiftMode);

	if (Ise_default_context.KeypadMode == KEYPAD_3X4) {
		_send_command(CMD_SINGLECOMMIT,
			      Ise_default_context.SingleCommit);
	} else {
		_send_command(CMD_SINGLECOMMIT, SINGLE_COMMIT_OFF);
	}
}

void set_single_commit(uint32 single)
{
	MCF_DEBUG();
	_send_command(CMD_SINGLECOMMIT, single);
	Ise_default_context.SingleCommit = single;
	if (single == SINGLE_COMMIT_ON) {
		change_prediction_onoff(OFF);
		switch (Ise_default_context.CompletionMode) {
		case COMPLETION_AMBIGUOUS_LOWERCASE_STRING:
			change_completion_option
			    (COMPLETION_MULTITAB_LOWERCASE_STRING);
			break;
		case COMPLETION_AMBIGUOUS_CAMELCASE_STRING:
			change_completion_option
			    (COMPLETION_MULTITAB_CAMELCASE_STRING);
			break;
		case COMPLETION_AMBIGUOUS_UPPERCASE_STRING:
			change_completion_option
			    (COMPLETION_MULTITAB_UPPERCASE_STRING);
			break;
		}
	}
}
