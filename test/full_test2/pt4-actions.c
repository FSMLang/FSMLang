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

#if defined (LINUX)
#include <stdlib.h>
#endif

#include "parser-test4.h"


int main()
{

	RUN_STATE_MACHINE(ptest4FSM,test4FSM_goodMessage);
	RUN_STATE_MACHINE(ptest4FSM,test4FSM_badMessage);
	RUN_STATE_MACHINE(ptest4FSM,test4FSM_goodMessage);
	RUN_STATE_MACHINE(ptest4FSM,test4FSM_goodMessage);
	RUN_STATE_MACHINE(ptest4FSM,test4FSM_badMessage);

	exit(0);

}

TEST4_FSM_STATE test4FSM_processMessage(pTEST4_FSM ptfsm)
{
	(void) ptfsm;

	printf("processing good message\n");

	return test4FSM_working;

}

TEST4_FSM_STATE test4FSM_sendErrorIndication(pTEST4_FSM ptfsm)
{
	(void) ptfsm;

	printf("sending error indication\n");

	return test4FSM_initial;

}

TEST4_FSM_STATE test4FSM_noAction(pTEST4_FSM ptfsm)
{
	(void) ptfsm;

	printf("taking no action\n");

	return test4FSM_noTransition;

}

