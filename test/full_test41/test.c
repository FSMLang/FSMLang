#include <unistd.h>

#include "top_level_priv.h"

int main ()
{
   run_top_level(TOP_LEVEL(e1));
   run_top_level(TOP_LEVEL(e2));
   run_top_level(TOP_LEVEL(e5));

   for (int i = 0; i < 5; i++)
   {
      DBG_PRINTF("\n%d\n"
            , i
            );
      run_top_level(TOP_LEVEL(e5));
      run_top_level(TOP_LEVEL(e6));
      ptop_level->data.field1 = i;
      snprintf(ptop_level->data.field2,sizeof(ptop_level->data.field2)-1, "loop: %.1d", i);
      run_top_level(TOP_LEVEL(e7));
   }

	DBG_PRINTF("\n");
   run_top_level(TOP_LEVEL(e8));
   run_top_level(TOP_LEVEL(e5));

   return 0;
}

