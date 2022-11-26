/*

	pt5-actions.c

	the actions to go with parser-test5.fsm


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

#include "parser-test5.h"


int main(int argc, char **argv)
{

   printf("hello, world\n");

/*
	printf("sizes:\n\tFSM: %d\n\tarray: %d\n\n"
					,sizeof(newMachine)
					, sizeof(*newMachine.actionArray)
					);
*/

   RUN_STATE_MACHINE(pnewMachine,newMachine_e1);
   RUN_STATE_MACHINE(pnewMachine,newMachine_e2);
   RUN_STATE_MACHINE(pnewMachine,newMachine_e3);
   RUN_STATE_MACHINE(pnewMachine,newMachine_e4);

	exit(0);

}

NEW_MACHINE_EVENT newMachine_a2(pNEW_MACHINE pfsm)
{
   (void) pfsm;

   DBG_PRINTF("newMachine_a2\n");

   return newMachine_noEvent;
}

NEW_MACHINE_EVENT newMachine_a1(pNEW_MACHINE pfsm)
{
   (void) pfsm;

   DBG_PRINTF("newMachine_a1\n");

   return newMachine_noEvent;
}

NEW_MACHINE_EVENT newMachine_noAction(pNEW_MACHINE pfsm)
{
   (void) pfsm;

   DBG_PRINTF("newMachine_noAction\n");

   return newMachine_noEvent;
}

NEW_MACHINE_STATE newMachine_transitionFn(pNEW_MACHINE pfsm, NEW_MACHINE_EVENT e)
{
   (void) pfsm;

   DBG_PRINTF("newMachine_TransitionFn\n");

   return (e == newMachine_e2)
      ? newMachine_s1
      : newMachine_s3
      ;

}

NEW_MACHINE_STATE newMachine_transitionFn1(pNEW_MACHINE pfsm, NEW_MACHINE_EVENT e)
{
   (void) e;
   NEW_MACHINE_STATE s = pfsm->state;

   DBG_PRINTF("newMachine_TransitionFn1\n");

   return (s < newMachine_s3)
            ? s++
            : newMachine_s1
            ;
   
}


