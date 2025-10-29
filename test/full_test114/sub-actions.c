
#include "sub_priv.h"

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

ACTION_RETURN_TYPE test_fsm_sub_grab_parent_e1_data(pTEST_FSM_DATA pdata, const void *pparent_fsm)
{
	DBG_PRINTF("%s", __func__);

	const pTEST_FSM_SUB psub = (const pTEST_FSM_SUB) pparent_fsm;

	psub->data.sub_e1_count += pdata->e1_count;

	DBG_PRINTF("sub e1 accumulator: %u", psub->data.sub_e1_count);
}

