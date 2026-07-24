#ifndef COMMAND_DEFS_H
#define COMMAND_DEFS_H

#include <stdint.h>

/*
	Define command message types.
*/

typedef enum
{
	command_first
	, command_peer_id = command_first
	, command_peer_sn
	, command_peer_ble_addr_type
	, command_peer_ble_addr
	, command_num_commands
} command_e;

typedef struct _id_str_ id_str, *pid_str;
typedef struct _sn_str_ sn_str, *psn_str;
typedef struct _ble_addr_str_ ble_addr_str, *pble_addr_str;
typedef struct _command_str_ command_str, *pcommand_str;
typedef union _command_u_ command_data, *pcommand_data;

struct _id_str_ {uint8_t data[4];};
struct _sn_str_ {uint8_t data[12];};
struct _ble_addr_str_ {uint8_t data[6];};
union _command_u_
{
	id_str       peer_id;
	sn_str       peer_sn;
	uint8_t      peer_ble_addr_type;
	ble_addr_str peer_ble_addr;
};

struct _command_str_
{
	command_e    tag;
	command_data data;
};

#endif
