/*
	BLE protocol definitions.
*/

#pragma once

typedef enum
{
	ble_disconnect
	, ble_adv_packet
	, ble_connect
	, ble_gatt_message
	, ble_num_entries
} ble_event_e;

#define GATT_MSG_SIZE   252
#define ADV_PACKET_SIZE 252

#define AUTH1_GATT 0x0001
#define AUTH2_GATT 0x0002
#define SYNC_GATT  0x0003

typedef struct _gatt_message_str_ gatt_message_str, *pgatt_message_str;
typedef union  _ble_event_data_u_ ble_event_data, *pble_event_data;
typedef struct _ble_event_str_ ble_event_str, *pble_event_str;

struct _gatt_message_str_
{
	uint16_t characteristic;
	uint8_t  data[GATT_MSG_SIZE];
};

union _ble_event_data_u_
{
	gatt_message_str gatt_msg;
	uint8_t          adv_packet[ADV_PACKET_SIZE];
};

struct _ble_event_str_
{
	ble_event_e    type;
	ble_event_data data;
};

