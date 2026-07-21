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

TOP_LEVEL_EVENT UFMN(handle_e7)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s", __func__);

   return THIS(noEvent);
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

void UFMN(translate_e7_data)(FSM_DATA_PTR pfsm_data, PARENT_DATA_TYPE_PTR pparent_data)
{
	DBG_PRINTF("%s\n", __func__);

	pfsm_data->field1 = pparent_data->field1;
	memcpy(pfsm_data->field2,pparent_data->field2, sizeof(pfsm_data->field2));

	DBG_PRINTF("The int: %d\n",  pfsm_data->field1);
	DBG_PRINTF("The string: %s\n",  pfsm_data->field2);
}

