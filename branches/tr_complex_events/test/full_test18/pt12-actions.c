/** 
 * pt12-actions.c 
 *  
 * action and other functionss for parser test 12 
 */

#include "parser-test12.h"
#include "test.h"

void systemManager_restart_standby_timer(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_restart_standby_timer\n");
}

void systemManager_set_t_fault_and_abort_sp(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_set_t_fault_and_abort_sp\n");
}

void systemManager_set_t_and_i_fault_and_abort_sp(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_set_t_and_i_fault_and_abort_sp\n");
}

void systemManager_call_sys_mgr_sp_fsm(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_call_sys_mgr_sp_fsm\n");
}

void systemManager_set_system_safety_fault(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_set_system_safety_fault\n");
}

void systemManager_set_inibiting_fault_and_stop_pump(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_set_inibiting_fault_and_stop_pump\n");
}

void systemManager_set_inibiting_fault(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_set_inibiting_fault\n");
}

void systemManager_update_system_date(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_update_system_date\n");
}

void systemManager_update_system_time(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_update_system_time\n");
}

void systemManager_set_standby_time_and_enable_timer(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_set_standby_time_and_enable_timer\n");
}

void systemManager_set_standby_time_and_disable_timer(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_set_standby_time_and_disable_timer\n");
}

void systemManager_reset_total_compressor_run_time(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_reset_total_compressor_run_time\n");
}

void systemManager_restore_tank_mode_and_start_pump(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_restore_tank_mode_and_start_pump\n");
}

void systemManager_save_tank_mode_and_stop_pump(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_save_tank_mode_and_stop_pump\n");
}

void systemManager_clear_system_safety_faults(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_clear_system_safety_faults\n");
}

void systemManager_clear_terminating_faults(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_clear_terminating_faults\n");
}

void systemManager_clear_inhibiting_faults(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_clear_inhibiting_faults\n");
}

void systemManager_init_time_control_and_notify(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_init_time_control_and_notify\n");
}

void systemManager_increment_instrument_run_time(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_increment_instrument_run_time\n");
}

void systemManager_increment_compressor_run_time(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_increment_compressor_run_time\n");
}

void systemManager_perhaps_pet_the_dog(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_perhaps_pet_the_dog\n");
}

void systemManager_continue_post_post_init(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_continue_post_post_init\n");
}

void systemManager_set_system_health_event_crc_failed(pSYSTEM_MANAGER pfsm)
{
   DBG_PRINTF("systemManager_set_system_health_event_crc_failed\n");
}


void systemManager_changeStateAndNotify(pSYSTEM_MANAGER pfsm,SYSTEM_MANAGER_STATE s)
{
   DBG_PRINTF("systemManager_changeStateAndNotify\n");
}


SYSTEM_MANAGER_STATE systemManager_fault_transition_on_sp_state(pSYSTEM_MANAGER pfsm,SYSTEM_MANAGER_EVENT e)
{
   DBG_PRINTF("systemManager_fault_transition_on_sp_state\n");
}

