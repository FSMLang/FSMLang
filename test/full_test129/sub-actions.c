
#include "sub_priv.h"

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
    DBG_PRINTF("%s", __func__);
    (void) pfsm;

    return THIS(noEvent);
}

void UFMN(grab_parent_e1_data)(FSM_DATA_PTR pfsm_data, PARENT_DATA_TYPE_PTR pparent_data)
{
	DBG_PRINTF("%s", __func__);

	pfsm_data->sub_e1_count += pparent_data->e1_count;

	DBG_PRINTF("sub e1 accumulator: %u", pfsm_data->sub_e1_count);
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

#if defined(FSM_VARIANT_CC)
TEST_FSM_EVENT_ENUM UFMN(do_nothing)(pSUB pfsm)
{
	DBG_PRINTF("%s", __func__);
	return sub_pass_shared_event(pfsm, sharing_sub_e1);
}
#endif

