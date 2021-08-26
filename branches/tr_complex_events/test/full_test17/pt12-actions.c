/*

	pt12-actions.c

	the actions to go with parser-test12.fsm


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

#include "parser-test12.h"

void newMachine_a2(pNEW_MACHINE pfsm)
{
   static unsigned count = 0;

   (void) pfsm;
   DBG_PRINTF("newMachine_a2\n");

}

void newMachine_a1(pNEW_MACHINE pfsm)
{
   DBG_PRINTF("newMachine_a1\n");

}

void newMachine_a3(pNEW_MACHINE pfsm)
{
   DBG_PRINTF("newMachine_a3\n");

}

void newMachine_noEvent(pNEW_MACHINE pfsm)
{
   DBG_PRINTF("newMachine_noAction\n");
}

void newMachine_baz(pNEW_MACHINE pfsm, NEW_MACHINE_STATE new_state)
{
   DBG_PRINTF("newMachine_machineTransitionFn\n\told state: %s\n\tnew state: %s\n"
              , NEW_MACHINE_STATE_NAMES[pfsm->state]
              , NEW_MACHINE_STATE_NAMES[new_state]
              );
}

NEW_MACHINE_STATE newMachine_transitionFn(pNEW_MACHINE pfsm, NEW_MACHINE_EVENT e)
{
   (void) pfsm;
   (void) e;

   DBG_PRINTF("newMachine_transitionFn\n");

   return newMachine_s3;
}

NEW_MACHINE_STATE newMachine_transitionFn1(pNEW_MACHINE pfsm, NEW_MACHINE_EVENT e)
{
   (void) pfsm;
   (void) e;

   DBG_PRINTF("newMachine_transitionFn1\n");

   return newMachine_s2;
}

NEW_MACHINE_EVENT newMachine_discriminate_ext5(pNEW_MACHINE pfsm)
{
   return pfsm->data.ce.head.head;
}

NEW_MACHINE_EVENT newMachine_discriminate_exts54(pNEW_MACHINE pfsm)
{
   return pfsm->data.ce.body.event_54.e;
}

NEW_MACHINE_EVENT newMachine_discriminate_exts542(pNEW_MACHINE pfsm)
{
   return pfsm->data.ce.body.event_542.e;
}

NEW_MACHINE_EVENT newMachine_discriminate_ext52(pNEW_MACHINE pfsm)
{
   return pfsm->data.ce.body.event_52.e;
}

NEW_MACHINE_EVENT newMachine_discriminate_ext4(pNEW_MACHINE pfsm)
{
   return pfsm->data.e4.e;
}

int main(int argc, char **argv)
{
   fprintf(stdout,"%s: hello, world\n", argv[0]);

   RUN_STATE_MACHINE(pnewMachine, ext5);
   RUN_STATE_MACHINE(pnewMachine, ext1);
   RUN_STATE_MACHINE(pnewMachine, ext2);
   RUN_STATE_MACHINE(pnewMachine, ext3);
   RUN_STATE_MACHINE(pnewMachine, ext4);

   return 0;
}

