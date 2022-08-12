#ifndef TEST_H
#define TEST_H

#include <stdio.h>

#define DBG_PRINTF(...) printf(__VA_ARGS__); printf("\n")

#define FSM_END_CRITICAL printf("end state: %s\n\n",TEST_FSM_STATE_NAMES[pfsm->state]);


#endif /* --------------------------  Nothing below  ----------------------- */
