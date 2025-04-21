/*
  hsmc-actions.c

	Action functions for the hsmCommunicator top-level FSM.

    FSMLang (fsm) - A Finite State Machine description language.
    Copyright (C) 2024  Steven Stanton

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

    For the latest on FSMLang: https://fsmlang.github.io/ 

    And, finally, your possession of this source code implies nothing.

*/

#include "hsmCommunicator_priv.h"

HSM_COMMUNICATOR_EVENT  UFMN(startSessionEstablishment)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;

	queue_count++;

	return ESTABLISH_SESSION(ESTABLISH_SESSION_REQUEST);
}

HSM_COMMUNICATOR_EVENT  UFMN(completeSessionStart)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return hsmCommunicator_pass_shared_event(sharing_hsmCommunicator_SEND_MESSAGE);
}

HSM_COMMUNICATOR_EVENT  UFMN(passMessageReceived)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return hsmCommunicator_pass_shared_event(sharing_hsmCommunicator_MESSAGE_RECEIVED);
}

HSM_COMMUNICATOR_EVENT  UFMN(requestMessageTransmission)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;

	queue_count++;

	return hsmCommunicator_pass_shared_event(sharing_hsmCommunicator_SEND_MESSAGE);
}

HSM_COMMUNICATOR_EVENT  UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

int main(void)
{
	run_hsmCommunicator(THIS(SEND_MESSAGE));
	run_hsmCommunicator(THIS(MESSAGE_RECEIVED));
	run_hsmCommunicator(THIS(MESSAGE_RECEIVED));
	run_hsmCommunicator(THIS(MESSAGE_RECEIVED));

	return 0;
}

