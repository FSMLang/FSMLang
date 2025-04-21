/*
  session_actions.c

	Action functions for the establishSession FSM.

	FSMLang (fsm) - A Finite State Machine description language.
	Copyright (C) 4/20/2025  Steven Stanton

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	Steven Stanton
	sstanton@pesticidesoftware.com

	For the latest on FSMLang: http://fsmlang.github.io

	And, finally, your possession of this source code implies nothing.

*/

#include <stdbool.h>
#include "establishSession_priv.h"

HSM_COMMUNICATOR_EVENT UFMN(sendStep0Message)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

HSM_COMMUNICATOR_EVENT UFMN(sendStep1Message)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

HSM_COMMUNICATOR_EVENT UFMN(notifyParent)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return PARENT(SESSION_ESTABLISHED);
}

HSM_COMMUNICATOR_EVENT UFMN(parseMessage)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	static bool first = true;

	return first ? (first = false, THIS(STEP0_RESPONSE)) : THIS(STEP1_RESPONSE);
}

HSM_COMMUNICATOR_EVENT UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

