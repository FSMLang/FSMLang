#include "parser-test11_priv.h"

NEW_MACHINE_EVENT newMachine_a2(pNEW_MACHINE pfsm)
{
   (void) pfsm;
   DBG_PRINTF("%s", __func__);

   return newMachine_noEvent;
}

NEW_MACHINE_EVENT newMachine_a1(pNEW_MACHINE pfsm)
{
   (void) pfsm;
   DBG_PRINTF("%s", __func__);

   return newMachine_noEvent;
}

NEW_MACHINE_EVENT newMachine_noAction(pNEW_MACHINE pfsm)
{
   (void) pfsm;
   DBG_PRINTF("%s", __func__);

   return newMachine_noEvent;
}

void newMachine_machineTransitionFn(pNEW_MACHINE pfsm, NEW_MACHINE_STATE new_state)
{
   DBG_PRINTF("%s\n\told state: %s\n\tnew state: %s\n"
				 , __func__
              , NEW_MACHINE_STATE_NAMES[pfsm->state]
              , NEW_MACHINE_STATE_NAMES[new_state]
              );
}

NEW_MACHINE_STATE newMachine_transitionFn(pNEW_MACHINE pfsm,NEW_MACHINE_EVENT event)
{
   (void) pfsm;
   (void) event;

   DBG_PRINTF("%s", __func__);

   return newMachine_s3;
}

NEW_MACHINE_STATE newMachine_transitionFn1(pNEW_MACHINE pfsm,NEW_MACHINE_EVENT event)
{
   (void) pfsm;
   (void) event;

   DBG_PRINTF("%s", __func__);

   return newMachine_s2;
}

NEW_MACHINE_STATE __attribute__((weak)) newMachine_transitionTos1(FSM_TYPE_PTR pfsm,NEW_MACHINE_EVENT e)
{
	(void) e;
	(void) pfsm;

	return newMachine_s1;
}

NEW_MACHINE_STATE __attribute__((weak)) UFMN(noTransitionFn)(pNEW_MACHINE pfsm, NEW_MACHINE_EVENT e)
{
	(void) e;
	return pfsm->state;
}

