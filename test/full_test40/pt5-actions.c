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

#include "parser-test5_priv.h"

void newMachine_a2(pNEW_MACHINE pfsm)
{
   (void) pfsm;

   DBG_PRINTF("%s\n", __func__);
}

void newMachine_a1(pNEW_MACHINE pfsm)
{
   (void) pfsm;

   DBG_PRINTF("%s\n", __func__);
}

void newMachine_noAction(pNEW_MACHINE pfsm)
{
   (void) pfsm;

   DBG_PRINTF("%s", __func__);
}

NEW_MACHINE_STATE newMachine_transitionFn(pNEW_MACHINE pfsm, NEW_MACHINE_EVENT e)
{
   (void) pfsm;

   DBG_PRINTF("%s\n", __func__);

   return (e == newMachine_e2)
      ? newMachine_s1
      : newMachine_s3
      ;

}

NEW_MACHINE_STATE newMachine_transitionFn1(pNEW_MACHINE pfsm, NEW_MACHINE_EVENT e)
{
   (void) e;
   NEW_MACHINE_STATE s = pfsm->state;

   DBG_PRINTF("%s\n", __func__);

   return (s < newMachine_s3)
            ? s++
            : newMachine_s1
            ;
   
}


void newMachine_machineTransition(pNEW_MACHINE pfsm, NEW_MACHINE_STATE new_state)
{
   DBG_PRINTF("%s:\n\told state: %s\n\tnew state: %s\n"
				  , __func__
              , NEW_MACHINE_STATE_NAMES[pfsm->state]
              , NEW_MACHINE_STATE_NAMES[new_state]
              );
}

NEW_MACHINE_STATE __attribute__((weak)) newMachine_transitionTos2(FSM_TYPE_PTR pfsm,NEW_MACHINE_EVENT e)
{
	(void) e;
	(void) pfsm;

	return newMachine_s2;
}

NEW_MACHINE_STATE __attribute__((weak)) UFMN(noTransitionFn)(pNEW_MACHINE pfsm, NEW_MACHINE_EVENT e)
{
	(void) e;
	return pfsm->state;
}

