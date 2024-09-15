#include "parser-test12_priv.h"

NEW_MACHINE_STATE newMachine_a2(pNEW_MACHINE pfsm)
{
   static unsigned count = 0;

   (void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return (++count % 5)
            ? newMachine_s2
            : newMachine_s1
            ;
}

NEW_MACHINE_STATE newMachine_a1(pNEW_MACHINE pfsm)
{
   DBG_PRINTF("%s\n", __func__);

   return (pfsm->state == newMachine_s2)
            ? newMachine_s3
            : newMachine_s2
            ;
}

NEW_MACHINE_STATE newMachine_noAction(pNEW_MACHINE pfsm)
{
   return pfsm->state;
}

void newMachine_machineTransitionFn(pNEW_MACHINE pfsm, NEW_MACHINE_STATE new_state)
{
   DBG_PRINTF("%s\n\told state: %s\n\tnew state: %s\n"
				, __func__
              , NEW_MACHINE_STATE_NAMES[pfsm->state]
              , NEW_MACHINE_STATE_NAMES[new_state]
              );
}

NEW_MACHINE_STATE newMachine_transitionFn1(pNEW_MACHINE pfsm)
{
   (void) pfsm;

   DBG_PRINTF("%s\n", __func__);

   return newMachine_s2;
}

NEW_MACHINE_STATE __attribute__((weak)) newMachine_transitionTos2(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;

	return newMachine_s2;
}

NEW_MACHINE_STATE __attribute__((weak)) newMachine_transitionTos3(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;

	return newMachine_s3;
}

NEW_MACHINE_STATE __attribute__((weak)) newMachine_transitionTos1(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;

	return newMachine_s1;
}

NEW_MACHINE_STATE __attribute__((weak)) UFMN(noTransitionFn)(pNEW_MACHINE pfsm)
{
	DBG_PRINTF("%s\n", __func__);
	(void) pfsm;
	return THIS(noTransition);
}

void __attribute__((weak)) newMachine_onEntryTo_s1(void)
{
	DBG_PRINTF("weak: %s", __func__);
}

void __attribute__((weak)) newMachine_onExitFrom_s3(void)
{
	DBG_PRINTF("weak: %s", __func__);
}

