/**
 * this test failed at compilation due to the FnBug.
 */
#include "flashdriver.h"

#include <stdio.h>

FLASH_DRIVER_EVENT FlashDriver_addToQueue(pFLASH_DRIVER pfsm)
{
	(void) pfsm;

   printf("addToQueue\n");

   return FlashDriver_noEvent;
}

FLASH_DRIVER_EVENT FlashDriver_checkQueue(pFLASH_DRIVER pfsm)
{
	(void) pfsm;

   printf("checkQueue\n");

   return FlashDriver_noEvent;
}

FLASH_DRIVER_EVENT FlashDriver_eraseFlash(pFLASH_DRIVER pfsm)
{
	(void) pfsm;

   printf("eraseFlash\n");

   return FlashDriver_noEvent;
}

FLASH_DRIVER_EVENT FlashDriver_writeFlash(pFLASH_DRIVER pfsm)
{
	(void) pfsm;

   printf("writeFlash\n");

   return FlashDriver_noEvent;
}

FLASH_DRIVER_EVENT FlashDriver_checkFlashState(pFLASH_DRIVER pfsm)
{
	(void) pfsm;

   printf("checkFlashState\n");

   return FlashDriver_noEvent;
}

FLASH_DRIVER_EVENT FlashDriver_noAction(pFLASH_DRIVER pfsm)
{
	(void) pfsm;

   printf("noAction\n");

   return FlashDriver_noEvent;
}

int main()
{
   printf("flasdriver test\n");

   return 0;
}

