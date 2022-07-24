#include <unistd.h>

#include "top_level.h"

TOP__LEVEL_EVENT top_level_a1(pTOP__LEVEL pfsm)
{
   printf("top_level_a1\n");

   return top_level_noEvent;
}

TOP__LEVEL_EVENT top_level_activate_sub_machine1(pTOP__LEVEL pfsm)
{
   printf("top_level_activate_sub_machine1\n");

   pfsm->active_sub_machine = 0;
   return top_level_sub_machine1_e1;
}

TOP__LEVEL_EVENT top_level_activate_sub_machine2(pTOP__LEVEL pfsm)
{
   printf("top_level_activate_sub_machine2\n");

   pfsm->active_sub_machine = 1;
   return top_level_sub_machine2_e1;
}

TOP__LEVEL_EVENT top_level_activate_sub_machine3(pTOP__LEVEL pfsm)
{
   printf("top_level_activate_sub_machine3\n");

   pfsm->active_sub_machine = 2;
   return top_level_sub_machine3_e1;
}

TOP__LEVEL_EVENT top_level_noAction(pTOP__LEVEL pfsm)
{
   printf("top_level_noAction\n");

   return top_level_noEvent;
}

int main (int argc, char **argv)
{
   printf("%s: Hello, world.\n"
          , argv[0]
          );

   RUN_STATE_MACHINE(ptop_level, top_level_e1);
   RUN_STATE_MACHINE(ptop_level, top_level_e2);
   RUN_STATE_MACHINE(ptop_level, top_level_e5);

	 for (int i = 0; i < 5; i++)
   {
      printf("\n%d\n"
            , i
            );
      RUN_STATE_MACHINE(ptop_level, top_level_e5);
   }

   return 0;
}

