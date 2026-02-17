#include "sub_subSub_priv.h"

ACTION_RETURN_TYPE UFMN(do_nothing)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
}

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;

}

void UFMN(grab_parent_e1_data)(PARENT_DATA_TYPE_PTR pdata, const void *pthis_fsm)
{
	DBG_PRINTF("%s", __func__);

	FSM_TYPE_PTR psubSub = (FSM_TYPE_PTR) pthis_fsm;

	psubSub->data.subSub_e1_count += pdata->sub_e1_count;

	DBG_PRINTF("Instance: %u: subSub e1 accumulator: %u"
					, psubSub->instance
					, psubSub->data.subSub_e1_count
					);
}

