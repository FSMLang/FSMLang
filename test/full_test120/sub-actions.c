
#include "sub_priv.h"

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;

}

void UFMN(grab_parent_e1_data)(PARENT_DATA_TYPE_PTR pdata, const void *pthis_fsm)
{
	DBG_PRINTF("%s", __func__);

	FSM_TYPE_PTR psub = (FSM_TYPE_PTR) pthis_fsm;

	psub->data.sub_e1_count += pdata->e1_count;

	DBG_PRINTF("sub e1 accumulator: %u", psub->data.sub_e1_count);
}

