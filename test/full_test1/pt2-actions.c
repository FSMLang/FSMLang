/*

	pt2-actions.c

	the actions to go with parser-test2.fsm


		FSMLang (fsm) - A Finite State Machine description language.
    Copyright (C) 2002  Steven Stanton

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
		ringwinner@users.sourceforge.net

		For the latest on FSMLang: http://fsmlang.sourceforge.net

		And, finally, your possession of this source code implies nothing.

*/

#include <stdio.h>

#if defined (LINUX) || defined (CYGWIN)
#include <stdlib.h>
#endif

#include "parser-test2.h"

extern TEST_FSM_ACTION_FN testFSM_action_fns[3];

int main()
{

   printf("hello, world\n");

/*
	printf("sizes:\n\tFSM: %d\n\tactionArray: %d\naction_fns: %d\n\n"
					,sizeof(testFSM)
					, sizeof(*testFSM.actionArray)
					, sizeof(testFSM_action_fns)
					);
*/


	RUN_STATE_MACHINE(ptestFSM,testFSM_goodMessage);
	RUN_STATE_MACHINE(ptestFSM,testFSM_badMessage);
	RUN_STATE_MACHINE(ptestFSM,testFSM_goodMessage);
	RUN_STATE_MACHINE(ptestFSM,testFSM_goodMessage);
	RUN_STATE_MACHINE(ptestFSM,testFSM_badMessage);

	exit(0);

}

TEST_FSM_EVENT testFSM_processMessage(pTEST_FSM ptfsm)
{
	(void) ptfsm;

	printf("processing good message\n");

	return testFSM_noEvent;

}

TEST_FSM_EVENT testFSM_sendErrorIndication(pTEST_FSM ptfsm)
{
	(void) ptfsm;

	printf("sending error indication\n");

	return testFSM_noEvent;

}


TEST_FSM_EVENT testFSM_noAction(pTEST_FSM ptfsm)
{
	(void) ptfsm;

	printf("taking no action\n");

	return testFSM_noEvent;

}


