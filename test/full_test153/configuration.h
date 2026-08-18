/*
	configuration.h

	What is needed in order to find and connect to a peer device.
*/

#pragma once

typedef enum
{
	config_item_first
	, config_item_peer_id = config_item_first
	, config_item_peer_sn
	, config_item_peer_ble_addr_type
	, config_item_peer_ble_addr
	, config_item_peer_comm_window
	, config_item_peer_comm_period
	, config_item_num_config_items
} config_item_e;

typedef struct _configuration_str_ configuration_str, *pconfiguration_str;
struct _configuration_str_
{
	uint8_t       config_bits;
	uint8_t       peer_ble_addr_type;
	ble_addr_str  peer_ble_addr;
	sn_str        peer_sn;
	id_str        peer_id;
	uint32_t      next_peer_comm_window;   /*!< The beginning of the next open communications window. */
	uint32_t      peer_comm_period;        /*!< The number of seconds between comm windows. */
};

#define SCAN_BITS ( \
(1 << config_item_peer_id) \
| (1 << config_item_peer_sn) \
)

#define CONNECT_BITS ( \
(1 << config_item_peer_id) \
| (1 << config_item_peer_ble_addr_type) \
| (1 << config_item_peer_ble_addr) \
| (1 << config_item_peer_comm_window)\
| (1 << config_item_peer_comm_period)\
)

