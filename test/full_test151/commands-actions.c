#include "commands_priv.h"

#define SCAN_BITS ( (1 << command_peer_id) | (1 << command_peer_sn) )
#define CONNECT_BITS ( (1 << command_peer_id) | (1 << command_peer_ble_addr_type) | (1 << command_peer_ble_addr) )

#ifdef COMMUNICATOR_COMMANDS_DEBUG
static void print_pairing_info(pCOMMUNICATOR_COMMANDS_DATA);
#endif

void UFMN(grab_command_ptr)(pCOMMUNICATOR_COMMANDS_DATA pfsm_data, pCOMMUNICATOR_DATA pparent_data)
{
	pfsm_data->ppcommand = &pparent_data->pcurr_command;
}

COMMUNICATOR_EVENT_ENUM UFMN(parse_command)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	pcommand_str pcommand = *(pfsm->data.ppcommand);

	switch ((*pfsm->data.ppcommand)->tag)
	{
	case command_peer_id:
		memcpy(&pfsm->data.peer_id, &pcommand->data.peer_id, sizeof(pfsm->data.peer_id));
		pfsm->data.cmd_bits |= (1 << command_peer_id);
		break;
	case command_peer_sn:
		memcpy(&pfsm->data.peer_sn, &pcommand->data.peer_sn, sizeof(pfsm->data.peer_sn));
		pfsm->data.cmd_bits |= (1 << command_peer_sn);
		break;
	case command_peer_ble_addr:
		memcpy(&pfsm->data.peer_ble_addr, &pcommand->data.peer_ble_addr, sizeof(pfsm->data.peer_ble_addr));
		pfsm->data.cmd_bits |= (1 << command_peer_ble_addr);
		break;
	case command_peer_ble_addr_type:
		pfsm->data.peer_ble_addr_type = pcommand->data.peer_ble_addr_type;
		pfsm->data.cmd_bits |= (1 << command_peer_ble_addr_type);
		break;
	default:
		break;
	}

#ifdef COMMUNICATOR_COMMANDS_DEBUG
	print_pairing_info(&pfsm->data);
#endif

	if ((pfsm->data.cmd_bits & SCAN_BITS) == SCAN_BITS)
	{
		return PARENT(scan);
	}
	else if ((pfsm->data.cmd_bits & CONNECT_BITS) == CONNECT_BITS)
	{
		return PARENT(connect);
	}

	return THIS(noEvent);

}

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;

	return THIS(noEvent);
}

TR_FN_RETURN_TYPE UFMN(noTransitionFn)(FSM_TYPE_PTR pfsm, ACTION_RETURN_TYPE e)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	(void) e;

	return pfsm->state;
}

TR_FN_RETURN_TYPE UFMN(check_pairing_info)(FSM_TYPE_PTR pfsm, ACTION_RETURN_TYPE e)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	(void) e;

	if (
		 ((pfsm->data.cmd_bits & SCAN_BITS) == SCAN_BITS)
		 || ((pfsm->data.cmd_bits & CONNECT_BITS) == CONNECT_BITS)
		)
	{
		return STATE(pairing_info_found);
	}

	return STATE(waiting_pairing_info);
}

TR_FN_RETURN_TYPE UFMN(transitionTowaiting_pairing_info)(FSM_TYPE_PTR pfsm,COMMUNICATOR_EVENT_ENUM e)
{
	(void) e;
	(void) pfsm;

	DECLARE_TR_FN_RET_VAR(trfnret, waiting_pairing_info);

	return trfnret;
}

TR_FN_RETURN_TYPE UFMN(transitionToinitial)(FSM_TYPE_PTR pfsm,COMMUNICATOR_EVENT_ENUM e)
{
	(void) e;
	(void) pfsm;

	DECLARE_TR_FN_RET_VAR(trfnret, initial);

	return trfnret;
}

void UFMN(set_defaults)(pCOMMUNICATOR_COMMANDS_DATA pfsm_data)
{
	DBG_PRINTF("%s", __func__);

	pfsm_data->ppcommand          = NULL;
	pfsm_data->cmd_bits           = 0;
	pfsm_data->peer_ble_addr_type = 0;

	memset(&pfsm_data->peer_ble_addr,0,sizeof(pfsm_data->peer_ble_addr));
	memset(&pfsm_data->peer_id,0,sizeof(pfsm_data->peer_id));
	memset(&pfsm_data->peer_sn,0,sizeof(pfsm_data->peer_sn));
}

#ifdef COMMUNICATOR_COMMANDS_DEBUG
char * command_strings[command_num_commands] = {
	"peer_id"
	, "peer_sn"
	, "peer_ble_addr_type"
	, "peer_ble_addr"
};
static void print_pairing_info(pCOMMUNICATOR_COMMANDS_DATA pfsm_data)
{
	printf("peer_id: [%.*s]\n", (int) sizeof(pfsm_data->peer_id), (uint8_t*)&pfsm_data->peer_id);
	printf("peer_sn: [%.*s]\n",  (int) sizeof(pfsm_data->peer_sn),(uint8_t*)&pfsm_data->peer_sn);
	printf("peer_ble_addr: [%.*s]\n",  (int) sizeof(pfsm_data->peer_ble_addr),(uint8_t*)&pfsm_data->peer_ble_addr);
	printf("peer_ble_addr_type: [%u]\n", pfsm_data->peer_ble_addr_type);
	printf("cmd_bits: [ ");
	for (command_e cmd = command_first; cmd < command_num_commands; cmd++)
	{
		if (pfsm_data->cmd_bits & (1 << cmd))
		{
			printf("%s ", command_strings[cmd]);
		}
	}
	printf("]\n");
}
#endif


