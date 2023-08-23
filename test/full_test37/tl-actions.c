#include <unistd.h>

#include "top_level.h"

ACTION_RETURN_TYPE THIS(activate_sub_machine1)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return SUB_MACHINE1(e1);
}

ACTION_RETURN_TYPE THIS(activate_sub_machine2)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return SUB_MACHINE2(e1);
}

ACTION_RETURN_TYPE THIS(activate_sub_machine3)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return SUB_MACHINE3(e1);
}

int main ()
{
   RUN_STATE_MACHINE(ptop_level, TOP_LEVEL(e1));
   RUN_STATE_MACHINE(ptop_level, TOP_LEVEL(e2));
   RUN_STATE_MACHINE(ptop_level, TOP_LEVEL(e5));

   for (int i = 0; i < 5; i++)
   {
      DBG_PRINTF("\n%d\n"
            , i
            );
      RUN_STATE_MACHINE(ptop_level, TOP_LEVEL(e5));
      RUN_STATE_MACHINE(ptop_level, TOP_LEVEL(e6));
      ptop_level->data.field1 = i;
      snprintf(ptop_level->data.field2,sizeof(ptop_level->data.field2)-1, "loop: %.1d", i);
      RUN_STATE_MACHINE(ptop_level, TOP_LEVEL(e7));
   }

	DBG_PRINTF("\n");
   RUN_STATE_MACHINE(ptop_level, TOP_LEVEL(e8));
   RUN_STATE_MACHINE(ptop_level, TOP_LEVEL(e5));

   return 0;
}

ACTION_RETURN_TYPE THIS(a1)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	DBG_PRINTF(__func__);
	return THIS(noEvent);
}

