#include "sub2_priv.h"

void UFMN(grab_parent_e1_data)(FSM_DATA_PTR pfsm_data, PARENT_DATA_TYPE_PTR pparent_data)
{
	DBG_PRINTF("%s", __func__);
	pfsm_data->sub2_e1_count += pparent_data->e1_count;
	DBG_PRINTF("sub2 e1 accumulator: %u", pfsm_data->sub2_e1_count);
}

TR_FN_RETURN_TYPE UFMN(toggle)(FSM_TYPE_PTR pfsm, TEST_FSM_EVENT_ENUM e)
{
	(void) e;
	if (pfsm->state == STATE(s1))
	{
		DECLARE_TR_FN_RET_VAR(retVal, s2);
		return retVal;
	}
	else
	{
		DECLARE_TR_FN_RET_VAR(retVal, s1);
		return retVal;
	}
}
