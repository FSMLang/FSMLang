#include "parser-test12.h"

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
	(void) pfsm;
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

int main()
{
   fprintf(stdout,"hello, world\n");

   RUN_STATE_MACHINE(pnewMachine, ext1);
   RUN_STATE_MACHINE(pnewMachine, ext2);
   RUN_STATE_MACHINE(pnewMachine, ext3);
   RUN_STATE_MACHINE(pnewMachine, ext4);
   RUN_STATE_MACHINE(pnewMachine, ext4);

   return 0;
}

