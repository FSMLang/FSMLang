#include "top_level_priv_priv_priv.h"

TOP_LEVEL_EVENT THIS(activate_sub_machine1)(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s", __func__, __func__, __func__);
	return SUB_MACHINE1(e1);
}

TOP_LEVEL_EVENT THIS(activate_sub_machine2)(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s", __func__, __func__, __func__);
	return SUB_MACHINE2(e1);
}

TOP_LEVEL_EVENT THIS(activate_sub_machine3)(pTOP_LEVEL pfsm)
{
	(void) pfsm;
	DBG_PRINTF("%s", __func__, __func__, __func__);
	return SUB_MACHINE3(e1);
}

