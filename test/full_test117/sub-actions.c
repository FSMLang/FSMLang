
#include "sub_priv.h"

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;

}

ACTION_RETURN_TYPE test_fsm_sub_grab_parent_e1_data(PARENT_DATA_TYPE_PTR pdata, const void *pfsm)
{
	DBG_PRINTF("%s", __func__);

	((FSM_TYPE_PTR)pfsm)->data.sub_e1_count += pdata->e1_count;

	DBG_PRINTF("sub e1 accumulator: %u", ((FSM_TYPE_PTR)pfsm)->data.sub_e1_count);
}

