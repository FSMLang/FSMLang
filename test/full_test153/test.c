#include <stdio.h>
#include <string.h>

#include "test_fsm.h"

/*{0,0,{0},{0},{0}},*/
#define INIT_COMMUNICATOR_DATA {{0}, 0, 0}
#define INIT_COMMANDS_DATA     {0}
#define INIT_BLE_DATA          {0}
#define INIT_FIRST_CONNECTION_DATA {0}

/* 
* Since instances require constant pointers to submachines,
* we need to declare them here, rather than in the respective tests.
*/
COMMUNICATOR_INSTANCE(scan, INIT_COMMUNICATOR_DATA, INIT_COMMANDS_DATA, INIT_BLE_DATA, INIT_FIRST_CONNECTION_DATA);
COMMUNICATOR_INSTANCE(connect, INIT_COMMUNICATOR_DATA, INIT_COMMANDS_DATA, INIT_BLE_DATA, INIT_FIRST_CONNECTION_DATA);
COMMUNICATOR_INSTANCE(find_and_run, INIT_COMMUNICATOR_DATA, INIT_COMMANDS_DATA, INIT_BLE_DATA, INIT_FIRST_CONNECTION_DATA);

static void run_communicator(pCOMMUNICATOR, pCOMMUNICATOR_EVENT);

typedef void (*test_fn)(void);
static void scan_happy_path(void);
static void connect_happy_path(void);
static void force_find_and_run(void);
test_fn tests[] = {
	scan_happy_path
	, connect_happy_path
	, force_find_and_run
	, NULL
};


static void scan_happy_path()
{
	printf("\ntest: %s\n", __func__);

	COMMUNICATOR_EVENT e;

	e.event = THIS(activate);

	run_communicator(pscan, &e);

	e.event = THIS(command);
	e.event_data.command_data.command.tag = command_peer_id;
	memcpy(&e.event_data.command_data.command.data.peer_id
          , "5678"
          , sizeof(e.event_data.command_data.command.data.peer_id)
         );
	run_communicator(pscan, &e);

	e.event_data.command_data.command.tag = command_peer_sn;
	memcpy(&e.event_data.command_data.command.data.peer_sn
          , "012345678901"
          , sizeof(e.event_data.command_data.command.data.peer_sn)
         );
	run_communicator(pscan, &e);

	e.event = THIS(ble_comm);
	e.event_data.ble_comm_data.ble_e.type = ble_adv_packet;
	run_communicator(pscan, &e);

	e.event = THIS(auth1_complete);
	run_communicator(pscan, &e);

	e.event = THIS(auth2_complete);
	run_communicator(pscan, &e);

	e.event = THIS(peer_data_synced);
	run_communicator(pscan, &e);

}

static void connect_happy_path()
{
	printf("\ntest: %s\n", __func__);

	COMMUNICATOR_EVENT e;

	e.event = THIS(activate);

	run_communicator(pconnect, &e);

	e.event = THIS(command);
	e.event_data.command_data.command.tag = command_peer_id;
	memcpy(&e.event_data.command_data.command.data.peer_id
          , "5678"
          , sizeof(e.event_data.command_data.command.data.peer_id)
         );
	run_communicator(pconnect, &e);

	e.event_data.command_data.command.tag = command_peer_ble_addr;
	memcpy(&e.event_data.command_data.command.data.peer_ble_addr
          , "012345"
          , sizeof(e.event_data.command_data.command.data.peer_ble_addr)
         );
	run_communicator(pconnect, &e);

	e.event_data.command_data.command.tag = command_peer_ble_addr_type;
	e.event_data.command_data.command.data.peer_ble_addr_type = 1;
	run_communicator(pconnect, &e);

	e.event_data.command_data.command.tag = command_peer_comm_window;
	e.event_data.command_data.command.data.peer_comm_window = 0;
	run_communicator(pconnect, &e);

	e.event_data.command_data.command.tag = command_peer_comm_period;
	e.event_data.command_data.command.data.peer_comm_period = 0;
	run_communicator(pconnect, &e);

}

static void force_find_and_run(void)
{
	printf("\ntest: %s\n", __func__);

	COMMUNICATOR_EVENT e;
	e.event = FIRST_CONNECTION(activate);

	run_communicator(pfind_and_run, &e);
}

static void run_communicator(pCOMMUNICATOR pfsm, pCOMMUNICATOR_EVENT pevent)
{
	pfsm->fsm(pfsm,pevent);
}

int main(void)
{
	test_fn *tf;

	for (tf = &tests[0]; *tf; tf++)
		(*tf)();
	
	return 0;

}
