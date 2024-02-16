#include <unistd.h>

#include "top_level_priv.h"

TOP_LEVEL_EVENT UFMN(activate_sub_machine1)(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s\n", __func__);
	return SUB_MACHINE1(e1);
}

TOP_LEVEL_EVENT UFMN(activate_sub_machine2)(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s\n", __func__);
	return SUB_MACHINE2(e1);
}

TOP_LEVEL_EVENT UFMN(activate_sub_machine3)(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s\n", __func__);
	return SUB_MACHINE3(e1);
}

TOP_LEVEL_EVENT UFMN(a1)(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s\n", __func__);
	return THIS(noEvent);
}

void UFMN(machine_transition)(pTOP_LEVEL pfsm, TOP_LEVEL_STATE new_state)
{
	DBG_PRINTF("%s:\n\told state: %s\n\tnew state: %s\n"
				  , __func__
              , TOP_LEVEL_STATE_NAMES[pfsm->state]
              , TOP_LEVEL_STATE_NAMES[new_state]
              );
}

