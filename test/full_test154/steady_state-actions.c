#include "steady_state_priv.h"

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

COMMUNICATOR_EVENT_ENUM UFMN(share_parent_start_auth2)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	return share_parent_event_start_auth2(pfsm->instance);
}

COMMUNICATOR_EVENT_ENUM UFMN(share_parent_auth2_gatt)(pSTEADY_STATE pfsm)
{
	DBG_PRINTF("%s", __func__);
	return share_parent_event_auth2_gatt(pfsm->instance);
}

COMMUNICATOR_EVENT_ENUM UFMN(share_parent_start_sync)(pSTEADY_STATE pfsm)
{
	DBG_PRINTF("%s", __func__);
	return share_parent_event_start_sync(pfsm->instance);
}

COMMUNICATOR_EVENT_ENUM UFMN(share_parent_sync_gatt)(pSTEADY_STATE pfsm)
{
	DBG_PRINTF("%s", __func__);
	return share_parent_event_sync_gatt(pfsm->instance);
}

COMMUNICATOR_EVENT_ENUM UFMN(share_parent_comm_window_timer_expired)(pSTEADY_STATE pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) share_parent_event_comm_window_starting(pfsm->instance);
	return share_parent_event_comm_window_timer_expired(pfsm->instance);
}

