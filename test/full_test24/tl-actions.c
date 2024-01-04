#include "top_level_priv.h"

TOP_LEVEL_EVENT THIS(activate_sub_machine1)(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s\n", __func__);
	return THIS(sub_machine1_e1);
}

TOP_LEVEL_EVENT THIS(activate_sub_machine2)(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s\n", __func__);
	return THIS(sub_machine2_e1);
}

TOP_LEVEL_EVENT THIS(activate_sub_machine3)(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s\n", __func__);
	return THIS(sub_machine3_e1);
}

