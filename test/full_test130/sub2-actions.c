
#include "sub2_priv.h"

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
    DBG_PRINTF("%s", __func__);
    (void) pfsm;
}

void UFMN(grab_parent_e1_data)(pTEST_FSM_DATA pdata, const void *pthis_fsm)
{
    DBG_PRINTF("%s", __func__);

    FSM_TYPE_PTR psub2 = (FSM_TYPE_PTR) pthis_fsm;

    psub2->data.sub2_e1_count += pdata->e1_count;

    DBG_PRINTF("Instance: %u: sub2 e1 accumulator: %u"
               , psub2->instance
               , psub2->data.sub2_e1_count
              );
}

TR_FN_RETURN_TYPE UFMN(toggle)(pSUB2 pfsm,TEST_FSM_EVENT_ENUM event)
{
	(void) event;

	DECLARE_TR_FN_RET_VAR(ret, s1);

	if (pfsm->state == STATE(s1))
	{
		RETURN_STATE(ret, s2);
	}

	return ret;
}
