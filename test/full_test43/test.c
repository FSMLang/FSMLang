#include "parser-test12.h"

int main()
{
   run_newMachine( (NEW_MACHINE_EVENT) ext1);
   run_newMachine( (NEW_MACHINE_EVENT) ext2);
   run_newMachine( (NEW_MACHINE_EVENT) ext3);
   run_newMachine( (NEW_MACHINE_EVENT) ext4);
   run_newMachine( (NEW_MACHINE_EVENT) ext4);

   return 0;
}

