
#include "sub_priv.h"

ACTION_RETURN_TYPE UFMN(do_nothing)(FSM_TYPE_PTR pfsm)
{
    DBG_PRINTF("%s", __func__);
    (void) pfsm;
    return STATE(noTransition);
}

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
    DBG_PRINTF("%s", __func__);
    (void) pfsm;

    return STATE(noTransition);
}

void test_fsm_sub_grab_parent_e1_data(pTEST_FSM_DATA pdata)
{
    DBG_PRINTF("%s", __func__);

    psub->data.sub_e1_count += pdata->e1_count;

    DBG_PRINTF("sub e1 accumulator: %u", psub->data.sub_e1_count);
}

