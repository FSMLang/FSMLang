#include "sub_machine1.h"

TOP__LEVEL_EVENT sub_machine1_a3(pSUB__MACHINE1 pfsm)
{
   printf("sub_machine1_a3\n");

   return top_level_e3;
}

TOP__LEVEL_EVENT sub_machine1_a2(pSUB__MACHINE1 pfsm)
{
   printf("sub_machine1_a2\n");

   return sub_machine1_e3;
}

TOP__LEVEL_EVENT sub_machine1_a1(pSUB__MACHINE1 pfsm)
{
   printf("sub_machine1_a1\n");

   return sub_machine1_e2;
}

TOP__LEVEL_EVENT sub_machine1_noAction(pSUB__MACHINE1 pfsm)
{
   printf("sub_machine1_noAction\n");

   return sub_machine1_noEvent;
}

