#include "e1_translator_priv.h"


void UFMN(grab_data)(FSM_DATA_PTR pfsm_data, PARENT_DATA_TYPE_PTR pparent_data)
{
	DBG_PRINTF("%s", __func__);

	pfsm_data->pparent_data = pparent_data;
}

void UFMN(a2)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	pfsm->data.pparent_data->j += pfsm->data.pparent_data->pe1_data->i;
}

void UFMN(a3)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	pfsm->data.pparent_data->k += pfsm->data.pparent_data->pe1_data->i;
}

void UFMN(a4)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	pfsm->data.pparent_data->l += pfsm->data.pparent_data->pe1_data->i;
}

