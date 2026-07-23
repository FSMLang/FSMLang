#ifndef _COMMAND_DEFS_H_
#define _COMMAND_DEFS_H_

#include <stdint.h>

typedef struct { char data[5];  } id_str,      *pid_str;
typedef struct { char data[13]; } sn_str,      *psn_str;
typedef struct { char addr[7];  } ble_addr_str, *pble_addr_str;

typedef struct _command_str_
{
	int type;
	union
	{
		id_str       id;
		sn_str       sn;
		ble_addr_str ble_addr;
		uint8_t      ble_addr_type;
	} payload;
} command_str, *pcommand_str;

#endif
