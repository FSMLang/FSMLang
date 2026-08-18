#include "commands_priv.h"

#ifdef COMMUNICATOR_COMMANDS_DEBUG
static void print_configuration(FSM_DATA_PTR);
#endif

void UFMN(grab_parent_data_ptrs)(FSM_DATA_PTR pfsm_data, pCOMMUNICATOR_DATA pparent_data)
{
	pfsm_data->ppcommand      = &pparent_data->pcurr_command;
	pfsm_data->pconfiguration = &pparent_data->configuration;
}

COMMUNICATOR_EVENT_ENUM UFMN(parse_command)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	FSM_DATA_PTR pfsm_data = &pfsm->data;
	pcommand_str pcommand  = *(pfsm_data->ppcommand);

	switch ((*pfsm_data->ppcommand)->tag)
	{
	case command_peer_id:
		memcpy(&pfsm_data->pconfiguration->peer_id, &pcommand->data.peer_id, sizeof(pfsm_data->pconfiguration->peer_id));
		pfsm_data->pconfiguration->config_bits |= (1 << config_item_peer_id);
		break;
	case command_peer_sn:
		memcpy(&pfsm_data->pconfiguration->peer_sn, &pcommand->data.peer_sn, sizeof(pfsm_data->pconfiguration->peer_sn));
		pfsm_data->pconfiguration->config_bits |= (1 << config_item_peer_sn);
		break;
	case command_peer_ble_addr:
		memcpy(&pfsm_data->pconfiguration->peer_ble_addr, &pcommand->data.peer_ble_addr, sizeof(pfsm_data->pconfiguration->peer_ble_addr));
		pfsm_data->pconfiguration->config_bits |= (1 << config_item_peer_ble_addr);
		break;
	case command_peer_ble_addr_type:
		pfsm_data->pconfiguration->peer_ble_addr_type = pcommand->data.peer_ble_addr_type;
		pfsm_data->pconfiguration->config_bits |= (1 << config_item_peer_ble_addr_type);
		break;
	case command_peer_comm_window:
		pfsm_data->pconfiguration->next_peer_comm_window = pcommand->data.peer_comm_window;
		pfsm_data->pconfiguration->config_bits |= (1 << config_item_peer_comm_window);
		break;
	case command_peer_comm_period:
		pfsm_data->pconfiguration->peer_comm_period = pcommand->data.peer_comm_period;
		pfsm_data->pconfiguration->config_bits |= (1 << config_item_peer_comm_period);
		break;
	default:
		break;
	}

#ifdef COMMUNICATOR_COMMANDS_DEBUG
	print_configuration(&pfsm->data);
#endif

	if (
		((pfsm_data->pconfiguration->config_bits & SCAN_BITS) == SCAN_BITS)
		|| ((pfsm_data->pconfiguration->config_bits & CONNECT_BITS) == CONNECT_BITS)
		)
	{
		return PARENT(configuration_complete);
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
	FSM_DATA_PTR pfsm_data = &pfsm->data;

	if (
		 ((pfsm_data->pconfiguration->config_bits & SCAN_BITS) == SCAN_BITS)
		 || ((pfsm_data->pconfiguration->config_bits & CONNECT_BITS) == CONNECT_BITS)
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

void UFMN(set_defaults)(FSM_DATA_PTR pfsm_data)
{
	DBG_PRINTF("%s", __func__);

	pfsm_data->ppcommand                          = NULL;
	pfsm_data->pconfiguration->config_bits           = 0;
	pfsm_data->pconfiguration->peer_ble_addr_type = 0;

	memset(&pfsm_data->pconfiguration->peer_ble_addr,0,sizeof(pfsm_data->pconfiguration->peer_ble_addr));
	memset(&pfsm_data->pconfiguration->peer_id,0,sizeof(pfsm_data->pconfiguration->peer_id));
	memset(&pfsm_data->pconfiguration->peer_sn,0,sizeof(pfsm_data->pconfiguration->peer_sn));
}

#ifdef COMMUNICATOR_COMMANDS_DEBUG
char * command_strings[command_num_commands] = {
	"peer_id"
	, "peer_sn"
	, "peer_ble_addr_type"
	, "peer_ble_addr"
	, "peer_comm_window"
	, "peer_comm_period"
};
static char *dun_developer = "Please add missing command string.";

static void print_configuration(FSM_DATA_PTR pfsm_data)
{
	printf("peer_id: [%.*s]\n", (int) sizeof(pfsm_data->pconfiguration->peer_id), (uint8_t*)&pfsm_data->pconfiguration->peer_id);
	printf("peer_sn: [%.*s]\n",  (int) sizeof(pfsm_data->pconfiguration->peer_sn),(uint8_t*)&pfsm_data->pconfiguration->peer_sn);
	printf("peer_ble_addr: [%.*s]\n",  (int) sizeof(pfsm_data->pconfiguration->peer_ble_addr),(uint8_t*)&pfsm_data->pconfiguration->peer_ble_addr);
	printf("peer_ble_addr_type: [%u]\n", pfsm_data->pconfiguration->peer_ble_addr_type);
	printf("next_peer_comm_window: [%u]\n", pfsm_data->pconfiguration->next_peer_comm_window);
	printf("peer_comm_period: [%u]\n", pfsm_data->pconfiguration->peer_comm_period);

	printf("config_bits: [ ");
	for (config_item_e config_item = config_item_first; config_item < config_item_num_config_items; config_item++)
	{
		if (pfsm_data->pconfiguration->config_bits & (1 << config_item))
		{
			printf("%s ", command_strings[config_item] ? command_strings[config_item] : dun_developer);
		}
	}
	printf("]\n");
}

#endif


