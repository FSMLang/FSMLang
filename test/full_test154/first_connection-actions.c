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

/* my original */
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

/* what gets generated
COMMUNICATOR_EVENT_ENUM UFMN(report_done)(pFIRST_CONNECTION pfsm)
{
	DBG_PRINTF("%s", __func__);
	return share_parent_event_peer_data_synced(pfsm->instance);
}
*/

COMMUNICATOR_EVENT_ENUM UFMN(share_parent_start_auth1)(pFIRST_CONNECTION pfsm)
{
	DBG_PRINTF("%s", __func__);
	return share_parent_event_start_auth1(pfsm->instance);
}

COMMUNICATOR_EVENT_ENUM UFMN(share_parent_auth1_gatt)(pFIRST_CONNECTION pfsm)
{
	DBG_PRINTF("%s", __func__);
	return share_parent_event_auth1_gatt(pfsm->instance);
}

COMMUNICATOR_EVENT_ENUM UFMN(share_parent_start_auth2)(pFIRST_CONNECTION pfsm)
{
	DBG_PRINTF("%s", __func__);
	return share_parent_event_start_auth2(pfsm->instance);
}

COMMUNICATOR_EVENT_ENUM UFMN(share_parent_auth2_gatt)(pFIRST_CONNECTION pfsm)
{
	DBG_PRINTF("%s", __func__);
	return share_parent_event_auth2_gatt(pfsm->instance);
}

COMMUNICATOR_EVENT_ENUM UFMN(share_parent_start_sync)(pFIRST_CONNECTION pfsm)
{
	DBG_PRINTF("%s", __func__);
	return share_parent_event_start_sync(pfsm->instance);
}

COMMUNICATOR_EVENT_ENUM UFMN(share_parent_sync_gatt)(pFIRST_CONNECTION pfsm)
{
	DBG_PRINTF("%s", __func__);
	return share_parent_event_sync_gatt(pfsm->instance);
}

COMMUNICATOR_EVENT_ENUM UFMN(share_parent_comm_window_timer_expired)(pFIRST_CONNECTION pfsm)
{
	DBG_PRINTF("%s", __func__);
	return share_parent_event_comm_window_timer_expired(pfsm->instance);
}

