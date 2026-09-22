#include "e1_translator_s1_implementor_priv.h"


void UFMN(grab_data)(FSM_DATA_PTR pfsm_data, PARENT_DATA_TYPE_PTR pparent_data)
{
	DBG_PRINTF("%s", __func__);

	pfsm_data->pparent_data = pparent_data->pparent_data;
}

ACTION_RETURN_TYPE UFMN(a0)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return STATE(noTransition);
}	

ACTION_RETURN_TYPE UFMN(a1)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	pfsm->data.pparent_data->i += pfsm->data.pparent_data->pe1_data->i;
	return STATE(noTransition);
}	

