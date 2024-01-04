#include <unistd.h>

#include "top_level.h"

int main ()
{
   run_top_level(THIS(e1));
   run_top_level(THIS(e2));
   run_top_level(THIS(e5));

	 for (int i = 0; i < 5; i++)
   {
      DBG_PRINTF("\n%d\n"
            , i
            );
      run_top_level(THIS(e5));
   }

   return 0;
}

