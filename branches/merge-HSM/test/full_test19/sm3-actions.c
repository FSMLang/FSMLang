#include "sub_machine3.h"

TOP__LEVEL_EVENT sub_machine3_a3(pSUB__MACHINE3 pfsm)
{
   printf("sub_machine3_a3\n");

   return top_level_noEvent;
}

TOP__LEVEL_EVENT sub_machine3_a2(pSUB__MACHINE3 pfsm)
{
   printf("sub_machine3_a2\n");

   return sub_machine3_e3;
}

TOP__LEVEL_EVENT sub_machine3_a1(pSUB__MACHINE3 pfsm)
{
   printf("sub_machine3_a1\n");

   return sub_machine3_e2;
}

TOP__LEVEL_EVENT sub_machine3_noAction(pSUB__MACHINE3 pfsm)
{
   printf("sub_machine3_noAction\n");

   return sub_machine3_noEvent;
}

