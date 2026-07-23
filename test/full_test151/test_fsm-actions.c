#include "test_fsm_priv.h"

COMMUNICATOR_EVENT_ENUM UFMN(noAction)(pCOMMUNICATOR pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s", __func__);
	return THIS(noEvent);
}

COMMUNICATOR_EVENT_ENUM UFMN(grab_command)(pCOMMUNICATOR_DATA pfsm_data, pCOMMUNICATOR_COMMAND_DATA pdata)
{
	DBG_PRINTF("%s", __func__);
	pfsm_data->pcurr_command = &pdata->command;
	return communicator_command;
}

COMMUNICATOR_EVENT_ENUM UFMN(translate_ble_comm)(pCOMMUNICATOR_DATA pfsm_data, pCOMMUNICATOR_BLE_COMM_DATA pdata)
{
	(void) pfsm_data;
	(void) pdata;
	return communicator_ble_proto;
}

void UFMN(set_defaults)(pCOMMUNICATOR_DATA pfsm_data)
{
	(void) pfsm_data;
	DBG_PRINTF("%s", __func__);
}

void UFMN(start_scanning)(pCOMMUNICATOR_DATA pfsm_data)
{
	(void) pfsm_data;
	DBG_PRINTF("%s", __func__);
}

void UFMN(stop_scanning)(pCOMMUNICATOR_DATA pfsm_data)
{
	(void) pfsm_data;
	DBG_PRINTF("%s", __func__);
}
