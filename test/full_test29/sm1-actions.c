#include <string.h>

#include "sub_machine1_priv.h"

TOP_LEVEL_EVENT UFMN(a3)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return PARENT(e3);
}

TOP_LEVEL_EVENT UFMN(a2)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return THIS(e3);
}

TOP_LEVEL_EVENT UFMN(a1)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return THIS(e2);
}

TOP_LEVEL_EVENT UFMN(noAction)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s", __func__);

   return THIS(noEvent);
}

SUB_MACHINE1_STATE UFMN(checkTransition)(pSUB_MACHINE1 pfsm, TOP_LEVEL_EVENT e)
{
   (void) pfsm;
   (void) e;

   return sub_machine1_s3;
}

void UFMN(translate_e7_data)(pTOP_LEVEL_DATA pfsm_data)
{
	DBG_PRINTF("%s\n", __func__);

	psub_machine1->data.field1 = pfsm_data->field1;
	memcpy(psub_machine1->data.field2,pfsm_data->field2, sizeof(psub_machine1->data.field2));

	DBG_PRINTF("The int: %d\n", psub_machine1->data.field1);
	DBG_PRINTF("The string: %s\n", psub_machine1->data.field2);
}

TOP_LEVEL_EVENT __attribute__((weak)) UFMN(handle_e7)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("weak: %s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

