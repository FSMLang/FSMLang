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

ACTION_RETURN_TYPE UFMN(sendStep0Message)(FSM_TYPE_PTR pfsm)
{
    DBG_PRINTF("%s", __func__);
    (void) pfsm;
    return THIS(noEvent);
}

ACTION_RETURN_TYPE UFMN(sendStep1Message)(FSM_TYPE_PTR pfsm)
{
    DBG_PRINTF("%s", __func__);
    (void) pfsm;
    return THIS(noEvent);
}

ACTION_RETURN_TYPE UFMN(notifyParent)(FSM_TYPE_PTR pfsm)
{
    DBG_PRINTF("%s", __func__);
    (void) pfsm;
    return PARENT(SESSION_ESTABLISHED);
}

ACTION_RETURN_TYPE UFMN(parseMessage)(PARENT_DATA_TYPE_PTR pparent_data)
{
    DBG_PRINTF("%s", __func__);

    return pparent_data->current_msg == msg_step0_response ? THIS(STEP0_RESPONSE) : THIS(STEP1_RESPONSE);
}

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
    DBG_PRINTF("%s", __func__);
    (void) pfsm;
    return THIS(noEvent);
}

