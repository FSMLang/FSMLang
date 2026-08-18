#include <stdio.h>
#include <string.h>

#include "test_fsm.h"

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

	run_communicator_instance0(&e);

	e.event = THIS(command);
	e.event_data.command_data.command.tag = command_peer_id;
	memcpy(&e.event_data.command_data.command.data.peer_id
          , "5678"
          , sizeof(e.event_data.command_data.command.data.peer_id)
         );
	run_communicator_instance0(&e);

	e.event_data.command_data.command.tag = command_peer_sn;
	memcpy(&e.event_data.command_data.command.data.peer_sn
          , "012345678901"
          , sizeof(e.event_data.command_data.command.data.peer_sn)
         );
	run_communicator_instance0(&e);

	e.event = THIS(ble_comm);
	e.event_data.ble_comm_data.ble_e.type = ble_adv_packet;
	run_communicator_instance0(&e);

	e.event = THIS(auth1_complete);
	run_communicator_instance0(&e);

	e.event = THIS(auth2_complete);
	run_communicator_instance0(&e);

	e.event = THIS(peer_data_synced);
	run_communicator_instance0(&e);

}

static void connect_happy_path()
{
	printf("\ntest: %s\n", __func__);

	COMMUNICATOR_EVENT e;

	e.event = THIS(activate);

	run_communicator_instance1(&e);

	e.event = THIS(command);
	e.event_data.command_data.command.tag = command_peer_id;
	memcpy(&e.event_data.command_data.command.data.peer_id
          , "5678"
          , sizeof(e.event_data.command_data.command.data.peer_id)
         );
	run_communicator_instance1(&e);

	e.event_data.command_data.command.tag = command_peer_ble_addr;
	memcpy(&e.event_data.command_data.command.data.peer_ble_addr
          , "012345"
          , sizeof(e.event_data.command_data.command.data.peer_ble_addr)
         );
	run_communicator_instance1(&e);

	e.event_data.command_data.command.tag = command_peer_ble_addr_type;
	e.event_data.command_data.command.data.peer_ble_addr_type = 1;
	run_communicator_instance1(&e);

	e.event_data.command_data.command.tag = command_peer_comm_window;
	e.event_data.command_data.command.data.peer_comm_window = 0;
	run_communicator_instance1(&e);

	e.event_data.command_data.command.tag = command_peer_comm_period;
	e.event_data.command_data.command.data.peer_comm_period = 0;
	run_communicator_instance1(&e);

}

static void force_find_and_run(void)
{
	printf("\ntest: %s\n", __func__);

	COMMUNICATOR_EVENT e;
	e.event = FIRST_CONNECTION(activate);

	run_communicator_instance2(&e);
}

int main(void)
{
	test_fn *tf;

	for (tf = &tests[0]; *tf; tf++)
		(*tf)();
	
	return 0;

}
