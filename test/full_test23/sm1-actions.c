#include "sub_machine1_priv.h"

TOP_LEVEL_EVENT UFMN(a3)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return PARENT(e3);
}

TOP_LEVEL_EVENT UFMN(a2)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return THIS(e3);
}

TOP_LEVEL_EVENT UFMN(a1)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return THIS(e2);
}

TOP_LEVEL_EVENT UFMN(noAction)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return THIS(noEvent);
}

SUB_MACHINE1_STATE UFMN(checkTransition)(pSUB_MACHINE1 pfsm, TOP_LEVEL_EVENT e)
{
   (void) pfsm;
   (void) e;

   return sub_machine1_s3;
}

/* The next two are needed to prevent the weak functions, which will be
generated in the -tc variant, from messing up the canonical output.
*/
SUB_MACHINE1_STATE UFMN(transitionTos1)(pSUB_MACHINE1 pfsm, TOP_LEVEL_EVENT e)
{
   (void) pfsm;
   (void) e;

	return sub_machine1_s1;
}

SUB_MACHINE1_STATE UFMN(transitionTos2)(pSUB_MACHINE1 pfsm, TOP_LEVEL_EVENT e)
{
   (void) pfsm;
   (void) e;

	return sub_machine1_s2;
}

