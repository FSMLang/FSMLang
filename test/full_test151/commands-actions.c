#include "commands_priv.h"
#include <stdio.h>

#define CMD_BIT_PEER_ID        (1u)
#define CMD_BIT_PEER_SN        (2u)
#define CMD_BIT_PEER_ADDR_TYPE (4u)
#define CMD_BIT_PEER_ADDR      (8u)

COMMUNICATOR_EVENT_ENUM UFMN(noAction)(pCOMMUNICATOR_COMMANDS pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s", __func__);
	return THIS(noEvent);
}

COMMUNICATOR_EVENT_ENUM UFMN(parse_command)(pCOMMUNICATOR_COMMANDS pfsm)
{
	DBG_PRINTF("%s", __func__);

	pcommand_str pcmd = *pfsm->data.ppcommand;

	switch ((command_type) pcmd->type)
	{
	case cb_peer_id:
		pfsm->data.cmd_bits |= CMD_BIT_PEER_ID;
		pfsm->data.peer_id = pcmd->payload.id;
		break;
	case cb_peer_sn:
		pfsm->data.cmd_bits |= CMD_BIT_PEER_SN;
		pfsm->data.peer_sn = pcmd->payload.sn;
		break;
	case cb_peer_ble_addr:
		pfsm->data.cmd_bits |= CMD_BIT_PEER_ADDR;
		pfsm->data.peer_ble_addr = pcmd->payload.ble_addr;
		break;
	case cb_peer_ble_addr_type:
		pfsm->data.cmd_bits |= CMD_BIT_PEER_ADDR_TYPE;
		pfsm->data.peer_ble_addr_type = pcmd->payload.ble_addr_type;
		break;
	default:
		break;
	}

	DBG_PRINTF("peer_id: [%s]", pfsm->data.peer_id.data);
	DBG_PRINTF("peer_sn: [%s]", pfsm->data.peer_sn.data);
	DBG_PRINTF("peer_ble_addr: [%s]", pfsm->data.peer_ble_addr.addr);
	DBG_PRINTF("peer_ble_addr_type: [%u]", pfsm->data.peer_ble_addr_type);

	printf("cmd_bits: [");
	if (pfsm->data.cmd_bits & CMD_BIT_PEER_ID)        printf(" peer_id");
	if (pfsm->data.cmd_bits & CMD_BIT_PEER_SN)        printf(" peer_sn");
	if (pfsm->data.cmd_bits & CMD_BIT_PEER_ADDR_TYPE) printf(" peer_ble_addr_type");
	if (pfsm->data.cmd_bits & CMD_BIT_PEER_ADDR)      printf(" peer_ble_addr");
	printf(" ]\n");

	uint8_t scan_bits    = CMD_BIT_PEER_ID | CMD_BIT_PEER_SN;
	uint8_t connect_bits = CMD_BIT_PEER_ID | CMD_BIT_PEER_ADDR | CMD_BIT_PEER_ADDR_TYPE;

	if ((pfsm->data.cmd_bits & connect_bits) == connect_bits)
		return PARENT(connect);

	if ((pfsm->data.cmd_bits & scan_bits) == scan_bits)
		return PARENT(scan);

	return THIS(noEvent);
}

TR_FN_RETURN_TYPE UFMN(check_pairing_info)(FSM_TYPE_PTR pfsm, COMMUNICATOR_EVENT_ENUM e)
{
	(void) e;
	DBG_PRINTF("%s", __func__);

	uint8_t scan_bits    = CMD_BIT_PEER_ID | CMD_BIT_PEER_SN;
	uint8_t connect_bits = CMD_BIT_PEER_ID | CMD_BIT_PEER_ADDR | CMD_BIT_PEER_ADDR_TYPE;

	if (   ((pfsm->data.cmd_bits & connect_bits) == connect_bits)
	    || ((pfsm->data.cmd_bits & scan_bits)    == scan_bits))
		return STATE(pairing_info_found);

	return STATE(waiting_pairing_info);
}

TR_FN_RETURN_TYPE UFMN(transitionTowaiting_pairing_info)(FSM_TYPE_PTR pfsm, COMMUNICATOR_EVENT_ENUM e)
{
	(void) pfsm;
	(void) e;
	return STATE(waiting_pairing_info);
}

TR_FN_RETURN_TYPE UFMN(transitionToinitial)(FSM_TYPE_PTR pfsm, COMMUNICATOR_EVENT_ENUM e)
{
	(void) pfsm;
	(void) e;
	return STATE(initial);
}

TR_FN_RETURN_TYPE UFMN(noTransitionFn)(FSM_TYPE_PTR pfsm, COMMUNICATOR_EVENT_ENUM e)
{
	(void) e;
	return pfsm->state;
}

void UFMN(set_defaults)(pCOMMUNICATOR_COMMANDS_DATA pfsm_data)
{
	(void) pfsm_data;
}

void UFMN(grab_command_ptr)(pCOMMUNICATOR_COMMANDS_DATA pfsm_data, pCOMMUNICATOR_DATA pparent_data)
{
	pfsm_data->ppcommand = &pparent_data->pcurr_command;
}
