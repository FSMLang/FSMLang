#include "test_fsm_priv.h"

COMMUNICATOR_EVENT_ENUM UFMN(grab_command)(pCOMMUNICATOR_DATA pfsm_data, pCOMMUNICATOR_COMMAND_DATA pcommand_data)
{
	DBG_PRINTF("%s", __func__);
	
	pfsm_data->pcurr_command = &pcommand_data->command;

	return THIS(command);
}

COMMUNICATOR_EVENT_ENUM UFMN(translate_ble_comm)(pCOMMUNICATOR_DATA pfsm_data, pCOMMUNICATOR_BLE_COMM_DATA pevent_data)
{
	DBG_PRINTF("%s", __func__);
	
	ACTION_RETURN_TYPE ret;

	switch (pevent_data->ble_e.type)
	{
	case ble_disconnect:
	case ble_connect:
	case ble_adv_packet:
		ret = THIS(ble_proto);
		break;
	case ble_gatt_message:
		ret = THIS(peer_msg);
		break;
	default:
		ret = THIS(noEvent);
		break;
	}

	memcpy(&pfsm_data->curr_ble_event, pevent_data, sizeof(ble_event_str));
	return ret;
}

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;

	return THIS(noEvent);
}

void UFMN(start_scanning)(pCOMMUNICATOR_DATA pfsm_data)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm_data;
}

void UFMN(stop_scanning)(pCOMMUNICATOR_DATA pfsm_data)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm_data;
}

void UFMN(set_defaults)(pCOMMUNICATOR_DATA pfsm_data)
{
	DBG_PRINTF("%s", __func__);

	pfsm_data->pcurr_command = NULL;
	memset(&pfsm_data->curr_ble_event,0,sizeof(pfsm_data->curr_ble_event));
}


