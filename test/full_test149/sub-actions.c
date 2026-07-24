
#include "sub_priv.h"

void UFMN(grab_parent_e1_data)(FSM_DATA_PTR pfsm_data, PARENT_DATA_TYPE_PTR pparent_data)
{
	DBG_PRINTF("%s", __func__);

	pfsm_data->sub_e1_count += pparent_data->e1_count;

	DBG_PRINTF("sub e1 accumulator: %u", pfsm_data->sub_e1_count);
}

