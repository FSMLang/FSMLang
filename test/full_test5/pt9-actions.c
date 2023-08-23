#include "parser-test9.h"

void newMachine_a2(pNEW_MACHINE pfsm)
{
   (void) pfsm;

   printf("newMachine_a2\n");
}

void newMachine_a1(pNEW_MACHINE pfsm)
{
   (void) pfsm;

   printf("newMachine_a1\n");
}

void newMachine_noAction(pNEW_MACHINE pfsm)
{
   (void) pfsm;

   printf("newMachine_noAction\n");
}

NEW_MACHINE_STATE newMachine_transitionFn(pNEW_MACHINE pfsm, NEW_MACHINE_EVENT e)
{
   (void) pfsm;
   (void) e;

   return newMachine_s1;
}

NEW_MACHINE_STATE newMachine_transitionFn1(pNEW_MACHINE pfsm, NEW_MACHINE_EVENT e)
{
   (void) pfsm;
   (void) e;

   return newMachine_s1;
}

int main()
{
   printf("hello, world\n");

   return 0;
}
