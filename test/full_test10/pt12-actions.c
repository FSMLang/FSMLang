#include "parser-test12_priv.h"

NEW_MACHINE_STATE newMachine_a2(pNEW_MACHINE pfsm)
{
   static unsigned count = 0;

   (void) pfsm;
   DBG_PRINTF("newMachine_a2\n");

   return (++count % 5)
            ? newMachine_s2
            : newMachine_s1
            ;
}

NEW_MACHINE_STATE newMachine_a1(pNEW_MACHINE pfsm)
{
   DBG_PRINTF("newMachine_a1\n");

   return (pfsm->state == newMachine_s2)
            ? newMachine_s3
            : newMachine_s2
            ;
}

NEW_MACHINE_STATE newMachine_noAction(pNEW_MACHINE pfsm)
{
   (void) pfsm;
   DBG_PRINTF("newMachine_noAction\n");

   return pfsm->state;
}

void newMachine_machineTransitionFn(pNEW_MACHINE pfsm, NEW_MACHINE_STATE new_state)
{
   DBG_PRINTF("newMachine_machineTransitionFn\n\told state: %s\n\tnew state: %s\n"
              , NEW_MACHINE_STATE_NAMES[pfsm->state]
              , NEW_MACHINE_STATE_NAMES[new_state]
              );
}

NEW_MACHINE_STATE newMachine_transitionFn1(pNEW_MACHINE pfsm)
{
   (void) pfsm;

   DBG_PRINTF("newMachine_transitionFn1\n");

   return newMachine_s2;
}

NEW_MACHINE_STATE newMachine_transitionTos1(pNEW_MACHINE pfsm)
{
   (void) pfsm;

   return newMachine_s1;
}

NEW_MACHINE_STATE newMachine_transitionTos2(pNEW_MACHINE pfsm)
{
   (void) pfsm;

   return newMachine_s2;
}

NEW_MACHINE_STATE newMachine_transitionTos3(pNEW_MACHINE pfsm)
{
   (void) pfsm;

   return newMachine_s3;
}

NEW_MACHINE_STATE __attribute__((weak)) UFMN(noTransitionFn)(pNEW_MACHINE pfsm)
{
	return pfsm->state;
}

int main()
{
   fprintf(stdout,"hello, world\n");

   run_newMachine(newMachine_e1);
   run_newMachine(newMachine_e2);
   run_newMachine(newMachine_e3);
   run_newMachine(newMachine_e4);

   return 0;
}

