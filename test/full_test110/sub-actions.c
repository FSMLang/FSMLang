
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

void test_fsm_sub_grab_parent_e1_data(pSUB_DATA pfsm_data, pTEST_FSM_DATA pparent_data)
{
	DBG_PRINTF("%s", __func__);

	pfsm_data->sub_e1_count += pparent_data->e1_count;

	DBG_PRINTF("sub e1 accumulator: %u", pfsm_data->sub_e1_count);
}

