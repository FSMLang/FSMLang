#include <stdio.h>
#include <string.h>

#include "test_fsm.h"

#define INIT_COMMUNICATOR_DATA {{0}, NULL}
#define INIT_COMMANDS_DATA     {0}

/* Since instances require constant pointers to submachines, we need to declare them here. */
COMMUNICATOR_INSTANCE(scan, INIT_COMMUNICATOR_DATA, INIT_COMMANDS_DATA);
COMMUNICATOR_INSTANCE(connect, INIT_COMMUNICATOR_DATA, INIT_COMMANDS_DATA);

static void run_communicator(pCOMMUNICATOR, pCOMMUNICATOR_EVENT);

typedef void (*test_fn)(void);
static void scan_happy_path(void);
static void connect_happy_path(void);
test_fn tests[] = {
	scan_happy_path
	, connect_happy_path
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
