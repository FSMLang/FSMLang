#include <stdio.h>

#include "another.h"

#define MAX_LOOPS (5)

ANOTHER_NEW_MACHINE_EVENT anmEvents[] =
{
	 anotherNewMachine_e1
	, anotherNewMachine_e2
	, anotherNewMachine_e3
	, anotherNewMachine_e4
	, anotherNewMachine_anotherSubMachine1_ee1
	, anotherNewMachine_anotherSubMachine1_ee2
	, anotherNewMachine_anotherSubMachine1_ee3
	, anotherNewMachine_anotherSubMachine1_e1
	, anotherNewMachine_anotherSubMachine1_noEvent
	, anotherNewMachine_anotherSubMachine1_anotherSubSubMachine1_y1
	, anotherNewMachine_anotherSubMachine1_anotherSubSubMachine1_y2
	, anotherNewMachine_anotherSubMachine1_anotherSubSubMachine1_e1
	, anotherNewMachine_anotherSubMachine1_anotherSubSubMachine1_ee1
	, anotherNewMachine_anotherSubMachine1_anotherSubSubMachine1_noEvent
	, anotherNewMachine_anotherSubMachine2_eee1
	, anotherNewMachine_anotherSubMachine2_eee2
	, anotherNewMachine_anotherSubMachine2_eee3
	, anotherNewMachine_anotherSubMachine2_noEvent
};

int main(void)
{
	for (unsigned loop_count = 0; loop_count < MAX_LOOPS; loop_count++)
	{
		for (unsigned evt_iterator = 0; evt_iterator < sizeof(anmEvents) / sizeof (anmEvents[0]); evt_iterator++)
		{
			run_anotherNewMachine(anmEvents[evt_iterator]);
		}
	}

	return 0;
}	
