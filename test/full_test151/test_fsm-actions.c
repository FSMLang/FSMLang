#include "test_fsm_priv.h"

TRANSLATOR_RETURN_TYPE UFMN(grab_command)(pCOMMUNICATOR_DATA pfsm_data, pCOMMUNICATOR_COMMAND_DATA pcommand_data)
{
	DBG_PRINTF("%s", __func__);
	
	pfsm_data->pcurr_command = &pcommand_data->command;
}

TRANSLATOR_RETURN_TYPE UFMN(grab_ble_comm)(pCOMMUNICATOR_DATA pfsm_data, pCOMMUNICATOR_BLE_COMM_DATA pble_comm_data)
{
	DBG_PRINTF("%s", __func__);
	
	pfsm_data->pcurr_ble_event = &pble_comm_data->ble_e;
}

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;

	return THIS(noEvent);
}

void UFMN(set_defaults)(pCOMMUNICATOR_DATA pfsm_data)
{
	DBG_PRINTF("%s", __func__);

	memset(pfsm_data,0,sizeof(*pfsm_data));
}


