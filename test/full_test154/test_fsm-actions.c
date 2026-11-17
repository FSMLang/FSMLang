#include "test_fsm_priv.h"

TRANSLATOR_RETURN_TYPE UFMN(grab_command)(pCOMMUNICATOR pfsm, pCOMMUNICATOR_EVENT pevent)
{
	DBG_PRINTF("%s", __func__);
	
	pfsm->data.pcurr_command = &pevent->event_data.command_data.command;
	return run_communicator_commands(pfsm, pevent);
}

TRANSLATOR_RETURN_TYPE UFMN(grab_ble_comm)(pCOMMUNICATOR pfsm, pCOMMUNICATOR_EVENT pevent)
{
	DBG_PRINTF("%s", __func__);
	
	return run_communicator_ble(pfsm, pevent);
}

COMMUNICATOR_EVENT_ENUM UFMN(activate_all)(pCOMMUNICATOR pfsm)
{
	DBG_PRINTF("%s", __func__);
	return communicator_pass_shared_event(pfsm, sharing_communicator_activate);
}

COMMUNICATOR_EVENT_ENUM UFMN(start_looking)(pCOMMUNICATOR pfsm)
{
	DBG_PRINTF("%s", __func__);
	return communicator_pass_shared_event(pfsm, sharing_communicator_configuration_complete);
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

void UFMN(initialize_ss)(FSM_TYPE_PTR pfsm)
{
	(void) run_communicator_steady_state(pfsm, STEADY_STATE(activate));
}

