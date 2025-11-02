
#include "sub2_priv.h"

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;

	return STATE(noTransition);
}

void UFMN(grab_parent_e1_data)(pTEST_FSM_DATA pdata, const void *pparent_fsm)
{
	DBG_PRINTF("%s", __func__);

	const FSM_TYPE_PTR psub2 = (const FSM_TYPE_PTR) pparent_fsm;

	psub2->data.sub2_e1_count += pdata->e1_count;

	DBG_PRINTF("sub2 e1 accumulator: %u", psub2->data.sub2_e1_count);
}

