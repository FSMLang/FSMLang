#include "parser-test9_priv.h"

void newMachine_a2(pNEW_MACHINE pfsm)
{
   (void) pfsm;

   DBG_PRINTF("%s", __func__);
}

void newMachine_a1(pNEW_MACHINE pfsm)
{
   (void) pfsm;

   DBG_PRINTF("%s", __func__);
}

void newMachine_noAction(pNEW_MACHINE pfsm)
{
   (void) pfsm;

   DBG_PRINTF("%s", __func__);
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

