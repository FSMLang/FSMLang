#include <string.h>

#include "sub_machine1.h"

TOP_LEVEL_EVENT sub_machine1_a3(pSUB_MACHINE1 pfsm)
{
   DBG_PRINTF("sub_machine1_a3\n");

   return PARENT(e3);
}

TOP_LEVEL_EVENT sub_machine1_a2(pSUB_MACHINE1 pfsm)
{
   DBG_PRINTF("sub_machine1_a2\n");

   return THIS(e3);
}

TOP_LEVEL_EVENT sub_machine1_a1(pSUB_MACHINE1 pfsm)
{
   DBG_PRINTF("sub_machine1_a1\n");

   return THIS(e2);
}

TOP_LEVEL_EVENT sub_machine1_noAction(pSUB_MACHINE1 pfsm)
{
   DBG_PRINTF("sub_machine1_noAction\n");

   return THIS(noEvent);
}

SUB_MACHINE1_STATE sub_machine1_checkTransition(pSUB_MACHINE1 pfsm, TOP_LEVEL_EVENT e)
{
   (void) pfsm;
   (void) e;

   return sub_machine1_s3;
}

void sub_machine1_translate_e7_data(pTOP_LEVEL pfsm)
{
	DBG_PRINTF("sub_machine1_translate_e7_data\n");

	psub_machine1->data.field1 = pfsm->data.field1;
	memcpy(psub_machine1->data.field2,pfsm->data.field2, sizeof(psub_machine1->data.field2));

	DBG_PRINTF("The int: %d\n", psub_machine1->data.field1);
	DBG_PRINTF("The string: %s\n", psub_machine1->data.field2);
}

TOP_LEVEL_EVENT __attribute__((weak)) sub_machine1_handle_e7(pSUB_MACHINE1 pfsm)
{
	DBG_PRINTF("weak: sub_machine1_handle_e7");
	return THIS(noEvent);
}

