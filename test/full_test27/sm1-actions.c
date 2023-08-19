#include "sub_machine1.h"

TOP_LEVEL_EVENT sub_machine1_a3(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("sub_machine1_a3\n");

   return PARENT(e3);
}

TOP_LEVEL_EVENT sub_machine1_a2(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("sub_machine1_a2\n");

   return THIS(e3);
}

TOP_LEVEL_EVENT sub_machine1_a1(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("sub_machine1_a1\n");

   return THIS(e2);
}

TOP_LEVEL_EVENT sub_machine1_noAction(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("sub_machine1_noAction\n");

   return THIS(noEvent);
}

SUB_MACHINE1_STATE sub_machine1_checkTransition(pSUB_MACHINE1 pfsm, TOP_LEVEL_EVENT e)
{
   (void) pfsm;
   (void) e;

   return sub_machine1_s3;
}

