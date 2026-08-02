
#include "sub2_priv.h"

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
    DBG_PRINTF("%s", __func__);
    (void) pfsm;


    return THIS(noEvent);
}

void UFMN(grab_parent_e1_data)(pSUB2_DATA pfsm_data, pTEST_FSM_DATA pparent_data)
{
	DBG_PRINTF("%s", __func__);

	pfsm_data->sub2_e1_count += pparent_data->e1_count;

	DBG_PRINTF("sub2 e1 accumulator: %u", pfsm_data->sub2_e1_count);
}

