
#include "sub_basement_priv.h"

ACTION_RETURN_TYPE UFMN(do_nothing)(FSM_TYPE_PTR pfsm)
{
    DBG_PRINTF("%s", __func__);
    (void) pfsm;
    return STATE(noTransition);
}

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
    DBG_PRINTF("%s", __func__);
    (void) pfsm;

    return STATE(noTransition);
}

void UFMN(grab_parent_e1_data)(pSUB_DATA pdata, const void *pfsm)
{
    DBG_PRINTF("%s", __func__);

	 DECLARE_INSTANCE(pbasement);

    pbasement->data.basement_e1_count += pdata->sub_e1_count;

    DBG_PRINTF("basement e1 accumulator: %u", pbasement->data.basement_e1_count);
}

