
#include "sub_priv.h"

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
    DBG_PRINTF("%s", __func__);
    (void) pfsm;

    return THIS(noEvent);
}

void UFMN(grab_parent_e1_data)(PARENT_DATA_TYPE_PTR pdata, const void *pthis_fsm)
{
    DBG_PRINTF("%s", __func__);

    FSM_TYPE_PTR psub = (FSM_TYPE_PTR) pthis_fsm;

    psub->data.sub_e1_count += pdata->e1_count;

    DBG_PRINTF("Instance: %u: sub e1 accumulator: %u"
               , psub->instance
               , psub->data.sub_e1_count
              );
}

void UFMN(onEntryTo_s1)(pSUB_DATA pdata)
{
    DBG_PRINTF("%s", __func__);
	 (void) pdata;
}

void UFMN(onExitFrom_s1)(pSUB_DATA pdata)
{
    DBG_PRINTF("%s", __func__);
	 (void) pdata;
}

TEST_FSM_EVENT_ENUM UFMN(do_nothing)(pSUB pfsm)
{
	DBG_PRINTF("%s", __func__);
	return sub_pass_shared_event(pfsm, sharing_sub_e1);
}

