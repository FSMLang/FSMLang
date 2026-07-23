#include "test_fsm_priv.h"
#include <stdio.h>
#include <string.h>

#define INIT_COMMUNICATOR_DATA {.pcurr_command = NULL}
#define INIT_COMMANDS_DATA {.ppcommand = NULL}
COMMUNICATOR_INSTANCE(scan_fsm,    INIT_COMMUNICATOR_DATA, INIT_COMMANDS_DATA);
COMMUNICATOR_INSTANCE(connect_fsm, INIT_COMMUNICATOR_DATA, INIT_COMMANDS_DATA);

int main(void)
{
	/* scan_happy_path */
	printf("\ntest: scan_happy_path\n");

	COMMUNICATOR_EVENT e_activate = {.event = communicator_activate};
	communicatorFSM(pscan_fsm, &e_activate);

	command_str cmd_peer_id;
	memset(&cmd_peer_id, 0, sizeof(cmd_peer_id));
	cmd_peer_id.type = cb_peer_id;
	memcpy(cmd_peer_id.payload.id.data, "5678", 4);

	COMMUNICATOR_EVENT e_scan_cmd1 = {
		.event = communicator_command,
		.event_data.command_data.command = cmd_peer_id
	};
	communicatorFSM(pscan_fsm, &e_scan_cmd1);

	command_str cmd_peer_sn;
	memset(&cmd_peer_sn, 0, sizeof(cmd_peer_sn));
	cmd_peer_sn.type = cb_peer_sn;
	memcpy(cmd_peer_sn.payload.sn.data, "012345678901", 12);

	COMMUNICATOR_EVENT e_scan_cmd2 = {
		.event = communicator_command,
		.event_data.command_data.command = cmd_peer_sn
	};
	communicatorFSM(pscan_fsm, &e_scan_cmd2);

	/* connect_happy_path */
	printf("\ntest: connect_happy_path\n");

	COMMUNICATOR_EVENT e_activate2 = {.event = communicator_activate};
	communicatorFSM(pconnect_fsm, &e_activate2);

	command_str cmd_peer_id2;
	memset(&cmd_peer_id2, 0, sizeof(cmd_peer_id2));
	cmd_peer_id2.type = cb_peer_id;
	memcpy(cmd_peer_id2.payload.id.data, "5678", 4);

	COMMUNICATOR_EVENT e_connect_cmd1 = {
		.event = communicator_command,
		.event_data.command_data.command = cmd_peer_id2
	};
	communicatorFSM(pconnect_fsm, &e_connect_cmd1);

	command_str cmd_peer_ble_addr;
	memset(&cmd_peer_ble_addr, 0, sizeof(cmd_peer_ble_addr));
	cmd_peer_ble_addr.type = cb_peer_ble_addr;
	memcpy(cmd_peer_ble_addr.payload.ble_addr.addr, "012345", 6);

	COMMUNICATOR_EVENT e_connect_cmd2 = {
		.event = communicator_command,
		.event_data.command_data.command = cmd_peer_ble_addr
	};
	communicatorFSM(pconnect_fsm, &e_connect_cmd2);

	command_str cmd_peer_ble_addr_type;
	memset(&cmd_peer_ble_addr_type, 0, sizeof(cmd_peer_ble_addr_type));
	cmd_peer_ble_addr_type.type = cb_peer_ble_addr_type;
	cmd_peer_ble_addr_type.payload.ble_addr_type = 1;

	COMMUNICATOR_EVENT e_connect_cmd3 = {
		.event = communicator_command,
		.event_data.command_data.command = cmd_peer_ble_addr_type
	};
	communicatorFSM(pconnect_fsm, &e_connect_cmd3);

	return 0;
}
