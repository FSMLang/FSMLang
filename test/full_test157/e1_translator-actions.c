#include "e1_translator_priv.h"


void UFMN(grab_data)(FSM_DATA_PTR pfsm_data, PARENT_DATA_TYPE_PTR pparent_data)
{
	DBG_PRINTF("%s", __func__);

	pfsm_data->pparent_data = pparent_data;
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

	pfsm->data.pparent_data->i += pfsm->data.pparent_data->pe1_data->i;

	return PARENT(e1);
}	

ACTION_RETURN_TYPE UFMN(a2)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	pfsm->data.pparent_data->j += pfsm->data.pparent_data->pe1_data->i;

	return PARENT(e1);
}

ACTION_RETURN_TYPE UFMN(a3)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	pfsm->data.pparent_data->k += pfsm->data.pparent_data->pe1_data->i;

	return PARENT(e1);
}

ACTION_RETURN_TYPE UFMN(a4)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	pfsm->data.pparent_data->l += pfsm->data.pparent_data->pe1_data->i;

	return PARENT(e1);
}

ACTION_RETURN_TYPE UFMN(a5)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;

	return PARENT(e2);
}	

ACTION_RETURN_TYPE UFMN(a6)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	pfsm->data.pparent_data->i += pfsm->data.pparent_data->pe1_data->i;

	return PARENT(e1);
}	

ACTION_RETURN_TYPE UFMN(a7)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;

	return PARENT(e2);
}	

ACTION_RETURN_TYPE UFMN(a8)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;

	return PARENT(e3);
}	

ACTION_RETURN_TYPE UFMN(a9)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;

	return PARENT(e4);
}	

ACTION_RETURN_TYPE UFMN(a10)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;

	return PARENT(e3);
}	

