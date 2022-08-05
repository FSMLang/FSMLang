#include "sub_machine2.h"

TOP_LEVEL_EVENT sub_machine2_a3(pSUB_MACHINE2 pfsm)
{
   printf("sub_machine2_a3\n");

   return top_level_e4;
}

TOP_LEVEL_EVENT sub_machine2_a2(pSUB_MACHINE2 pfsm)
{
   printf("sub_machine2_a2\n");

   return sub_machine2_e3;
}

TOP_LEVEL_EVENT sub_machine2_a1(pSUB_MACHINE2 pfsm)
{
   printf("sub_machine2_a1\n");

   return sub_machine2_e2;
}

TOP_LEVEL_EVENT sub_machine2_noAction(pSUB_MACHINE2 pfsm)
{
   printf("sub_machine2_noAction\n");

   return sub_machine2_noEvent;
}

