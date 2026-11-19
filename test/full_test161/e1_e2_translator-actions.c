#include "e1_e2_translator_priv.h"


void UFMN(grab_e1_data)(FSM_DATA_PTR pfsm_data, PARENT_DATA_TYPE_PTR pparent_data, pTEST_FSM_EVENT_DATA pevent_data)
{
	DBG_PRINTF("%s", __func__);

	pfsm_data->pparent_data = pparent_data;
	pfsm_data->event_value  = pevent_data->e1_data.i;
}

void UFMN(grab_e2_data)(FSM_DATA_PTR pfsm_data, PARENT_DATA_TYPE_PTR pparent_data, pTEST_FSM_EVENT_DATA pevent_data)
{
	DBG_PRINTF("%s", __func__);

	pfsm_data->pparent_data = pparent_data;
	pfsm_data->event_value  = pevent_data->e2_data.i / 2;
}

ACTION_RETURN_TYPE UFMN(a0)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;

	return THIS(noEvent);
}	

ACTION_RETURN_TYPE UFMN(a1)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	pfsm->data.pparent_data->i += pfsm->data.event_value;

	return PARENT(data_updated);
}	

ACTION_RETURN_TYPE UFMN(a2)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	pfsm->data.pparent_data->j += pfsm->data.event_value;

	return PARENT(data_updated);
}

ACTION_RETURN_TYPE UFMN(a3)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	pfsm->data.pparent_data->k += pfsm->data.event_value;

	return PARENT(data_updated);
}

ACTION_RETURN_TYPE UFMN(a4)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	pfsm->data.pparent_data->l += pfsm->data.event_value;

	return PARENT(data_updated);
}

