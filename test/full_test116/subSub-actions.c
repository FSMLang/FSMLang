
#include "subSub_priv.h"

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

ACTION_RETURN_TYPE UFMN(grab_parent_e1_data)(pSUB_DATA pdata, const void *pparent_fsm)
{
	DBG_PRINTF("%s", __func__);

	const pSUB_SUB psubSub = (const pSUB_SUB) pparent_fsm;

	psubSub->data.subSub_e1_count += pdata->sub_e1_count;

	DBG_PRINTF("subSub e1 accumulator: %u", psubSub->data.subSub_e1_count);
}

