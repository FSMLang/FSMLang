#include "test_fsm_priv.h"

TRANSLATOR_RETURN_TYPE UFMN(grab_command)(pCOMMUNICATOR pfsm, pCOMMUNICATOR_COMMAND_DATA pcommand_data, COMMUNICATOR_EVENT_ENUM event)
{
	DBG_PRINTF("%s", __func__);
	
	pfsm->data.pcurr_command = &pcommand_data->command;
	return run_communicator_commands(pfsm, event);
}

TRANSLATOR_RETURN_TYPE UFMN(grab_ble_comm)(pCOMMUNICATOR pfsm, pCOMMUNICATOR_BLE_COMM_DATA pble_comm_data, COMMUNICATOR_EVENT_ENUM event)
{
	DBG_PRINTF("%s", __func__);
	
	pfsm->data.pcurr_ble_event = &pble_comm_data->ble_e;

	return run_communicator_ble(pfsm, event);
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

void UFMN(initialize_ftc)(FSM_TYPE_PTR pfsm)
{
	(void) run_communicator_first_connection(pfsm, FIRST_CONNECTION(activate));
}

void UFMN(decommission_ftc)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
}

