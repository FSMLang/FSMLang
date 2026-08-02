#include "ble_priv.h"

TRANSLATOR_RETURN_TYPE UFMN(grab_parent_data_ptrs)(FSM_DATA_PTR pfsm_data, PARENT_DATA_TYPE_PTR pparent_data)
{
	DBG_PRINTF("%s", __func__);
	
	pfsm_data->pconfiguration = &pparent_data->configuration;

	return THIS(activate);
}

TRANSLATOR_RETURN_TYPE UFMN(translate_ble_comm)(FSM_DATA_PTR pfsm_data, PARENT_DATA_TYPE_PTR pparent_data)
{
	DBG_PRINTF("%s", __func__);
	
	(void) pfsm_data;

	ACTION_RETURN_TYPE ret;

	switch (pparent_data->pcurr_ble_event->type)
	{
	case ble_disconnect:
		ret = THIS(disconnection);
		break;
	case ble_connect:
		ret = THIS(connection);
		break;
	case ble_adv_packet:
		ret = THIS(adv_packet);
		break;
	case ble_gatt_message:
		ret = THIS(gatt_msg);
		break;
	default:
		ret = THIS(noEvent);
		break;
	}

	return ret;
}

TRANSLATOR_RETURN_TYPE UFMN(check_configuration)(pCOMMUNICATOR_BLE_DATA pfsm_data, pCOMMUNICATOR_DATA pparent_data)
{
	DBG_PRINTF("%s", __func__);
	
	(void) pfsm_data;
	pconfiguration_str pconfiguration = &pparent_data->configuration;

	if ((pconfiguration->config_bits & SCAN_BITS) == SCAN_BITS)
		return THIS(do_scan);
	else if ((pconfiguration->config_bits & CONNECT_BITS) == CONNECT_BITS)
		return THIS(do_connect);
	else
		return THIS(noEvent); //This line should not be reached; we should only be executing this function when the configuration has reached
                            // the scan or connect state.
}


ACTION_RETURN_TYPE UFMN(parse_adv_packet)(pCOMMUNICATOR_BLE pfsm)
{
	DBG_PRINTF("%s",__func__);
	(void) pfsm;
	return THIS(connection);
}

ACTION_RETURN_TYPE UFMN(report_connection)(pCOMMUNICATOR_BLE pfsm)
{
	DBG_PRINTF("%s",__func__);
	(void) pfsm;
	return PARENT(peer_connected);
}

ACTION_RETURN_TYPE UFMN(noAction)(pCOMMUNICATOR_BLE pfsm)
{
	DBG_PRINTF("%s",__func__);
	(void) pfsm;
	return THIS(noEvent);
}


void UFMN(set_defaults)(pCOMMUNICATOR_BLE_DATA pfsm_data)
{
	DBG_PRINTF("%s",__func__);
	(void) pfsm_data;
}

void UFMN(start_scanning)(pCOMMUNICATOR_BLE_DATA pfsm_data)
{
	DBG_PRINTF("%s",__func__);
	(void) pfsm_data;
}

void UFMN(stop_scanning)(pCOMMUNICATOR_BLE_DATA pfsm_data)
{
	DBG_PRINTF("%s",__func__);
	(void) pfsm_data;
}

void UFMN(start_connection_timer)(pCOMMUNICATOR_BLE_DATA pfsm_data)
{
	DBG_PRINTF("%s",__func__);
	(void) pfsm_data;
}

void UFMN(stop_connection_timer)(pCOMMUNICATOR_BLE_DATA pfsm_data)
{
	DBG_PRINTF("%s",__func__);
	(void) pfsm_data;
}


