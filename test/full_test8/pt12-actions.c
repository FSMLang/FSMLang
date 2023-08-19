#include "parser-test12.h"

void newMachine_a2(pNEW_MACHINE pfsm)
{
   (void) pfsm;
   DBG_PRINTF("newMachine_a2\n");
}

void newMachine_a1(pNEW_MACHINE pfsm)
{
   (void) pfsm;
   DBG_PRINTF("newMachine_a1\n");
}

void newMachine_noAction(pNEW_MACHINE pfsm)
{
   (void) pfsm;
   DBG_PRINTF("newMachine_noAction\n");
}

void newMachine_machineTransitionFn(pNEW_MACHINE pfsm, NEW_MACHINE_STATE new_state)
{
   DBG_PRINTF("newMachine_machineTransitionFn\n\told state: %s\n\tnew state: %s\n"
              , NEW_MACHINE_STATE_NAMES[pfsm->state]
              , NEW_MACHINE_STATE_NAMES[new_state]
              );
}

NEW_MACHINE_STATE newMachine_transitionFn(pNEW_MACHINE pfsm,NEW_MACHINE_EVENT event)
{
   (void) pfsm;
   (void) event;

   DBG_PRINTF("newMachine_transitionFn\n");

   return newMachine_s3;
}

NEW_MACHINE_STATE newMachine_transitionFn1(pNEW_MACHINE pfsm,NEW_MACHINE_EVENT event)
{
   (void) pfsm;
   (void) event;

   DBG_PRINTF("newMachine_transitionFn1\n");

   return newMachine_s2;
}

int main()
{
   fprintf(stdout,"hello, world\n");

   RUN_STATE_MACHINE(pnewMachine, newMachine_e1);
   RUN_STATE_MACHINE(pnewMachine, newMachine_e2);
   RUN_STATE_MACHINE(pnewMachine, newMachine_e3);
   RUN_STATE_MACHINE(pnewMachine, newMachine_e4);

   return 0;
}

