#include "first_connection_priv.h"

ACTION_RETURN_TYPE UFMN(act_start_auth1)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

ACTION_RETURN_TYPE UFMN(act_start_auth2)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

ACTION_RETURN_TYPE UFMN(start_data_sync)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

ACTION_RETURN_TYPE UFMN(report_done)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return PARENT(peer_data_synced);
}

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

void UFMN(grab_parent_data_ptrs)(FSM_DATA_PTR pfsm_data, PARENT_DATA_TYPE_PTR pparent_data)
{
	pfsm_data->pconfiguration = &pparent_data->configuration;
}

