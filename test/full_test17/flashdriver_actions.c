/**
 * this test failed at compilation due to the FnBug.
 */
#include "flashdriver.h"

#include <stdio.h>

_FLASH_DRIVER_EVENT FlashDriver_addToQueue(p_FLASH_DRIVER pfsm)
{
	(void) pfsm;

   printf("addToQueue\n");

   return FlashDriver_noEvent;
}

_FLASH_DRIVER_EVENT FlashDriver_checkQueue(p_FLASH_DRIVER pfsm)
{
	(void) pfsm;

   printf("checkQueue\n");

   return FlashDriver_noEvent;
}

_FLASH_DRIVER_EVENT FlashDriver_eraseFlash(p_FLASH_DRIVER pfsm)
{
	(void) pfsm;

   printf("eraseFlash\n");

   return FlashDriver_noEvent;
}

_FLASH_DRIVER_EVENT FlashDriver_writeFlash(p_FLASH_DRIVER pfsm)
{
	(void) pfsm;

   printf("writeFlash\n");

   return FlashDriver_noEvent;
}

_FLASH_DRIVER_EVENT FlashDriver_checkFlashState(p_FLASH_DRIVER pfsm)
{
	(void) pfsm;

   printf("checkFlashState\n");

   return FlashDriver_noEvent;
}

_FLASH_DRIVER_EVENT FlashDriver_noAction(p_FLASH_DRIVER pfsm)
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

