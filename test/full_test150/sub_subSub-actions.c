#include "sub_subSub_priv.h"

void UFMN(grab_parent_e1_data)(FSM_DATA_PTR pfsm_data, PARENT_DATA_TYPE_PTR pparent_data)
{
	DBG_PRINTF("%s", __func__);
	pfsm_data->subSub_e1_count += pparent_data->sub_e1_count;
	DBG_PRINTF("subSub e1 accumulator: %u", pfsm_data->subSub_e1_count);
}
