/*
  fsm_statistics.c

   print machine statistics
 
    FSMLang (fsm) - A Finite State Machine description language.
    Copyright (C) 2002  Steven Stanton
    Copyright (C) 3/25/2023  Steven Stanton

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Steven Stanton
    sstantin@pesticidesoftware.com

    For the latest on FSMLang: http://fsmlang.pesticidesoftware.com

    And, finally, your possession of this source code implies nothing.

*/

#include "fsm_statistics.h"
#include "fsm_unused.h"

#if defined (CYGWIN) || defined (LINUX)
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#endif
#include <string.h>
#include <stdlib.h>

#define name_separator(A) ((A)->first ? "" : ", ")

bool print_action_array = false;

static int  initMachineStatisticsWriter(pFSMOutputGenerator,char*);
static void writeMachineStatistics(pFSMOutputGenerator,pMACHINE_INFO);
static void closeMachineStatisticsWriter(pFSMOutputGenerator,int);
static bool write_machine_statistics(pLIST_ELEMENT,void*);
static bool write_events(pLIST_ELEMENT,void*);
static bool write_states(pLIST_ELEMENT,void*);
static bool print_pid_name(pLIST_ELEMENT,void*);
static bool compute_pid_name_len(pLIST_ELEMENT,void*);
static bool write_action_array_pointers(pLIST_ELEMENT,void*);

FSMOutputGenerator MachineStatisticsWriter = {
   initMachineStatisticsWriter,
   writeMachineStatistics,
   closeMachineStatisticsWriter
};

pFSMOutputGenerator pMachineStatisticsWriter = &MachineStatisticsWriter;

/**
 * @brief Since we're writing to stdout, there is nothing to initialize
 * 
 * @author Steven Stanton (3/25/2023)
 * 
 * @param pfsmog 'this' (not used)
 * @param ofBase not used
 * 
 *
 * @ref_global none
 *
 * @mod_global none
 *
 * @return int always 0 (success)
 *
 ***********************************************************************************************************************/
static int initMachineStatisticsWriter(pFSMOutputGenerator pfsmog, char *ofBase)
{
   (void) pfsmog;
   (void) ofBase;

   return 0;
}

/**
 * @brief Write statistics for a machine
 * 
 * @author Steven Stanton (3/25/2023)
 * 
 * @param pfsmog 'this' (not used)
 * @param pmi pointer to the machine info block
 * 
 *
 * @ref_global none
 *
 * @mod_global none
 *
 * @thread_safe yes
 *
 ***********************************************************************************************************************/
static void writeMachineStatistics(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
   FSMLANG_MAYBE_UNUSED(pfsmog);
   LIST_ELEMENT elem = { .mbr = pmi , .next = NULL };

   (void) write_machine_statistics(&elem, NULL);

}

/**
 * @brief Since we're writing to stdout, there is nothing to do here
 * 
 * @author Steven Stanton (3/25/2023)
 * 
 * @param pfsmog 'this' (not used)
 * @param status exit status
 * 
 *
 * @ref_global none
 *
 * @mod_global none
 *
 ***********************************************************************************************************************/
static void closeMachineStatisticsWriter(pFSMOutputGenerator pfsmog, int status)
{
   (void) pfsmog;
   (void) status;
}


/**
 * @brief write the statistics of a machine found in a list
 *        element
 * 
 * @author Steven Stanton (3/25/2023)
 * 
 * @param pelem points to the list element containing the machine info pointer
 * @param data is not used
 * 
 *
 * @ref_global none
 *
 * @mod_global none
 *
 * @thread_safe yes
 * 
 * @return bool always returns false, indicating that list processing should not stop.
 *
 ***********************************************************************************************************************/
static bool write_machine_statistics(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO pmi = (pMACHINE_INFO) pelem->mbr;

   (void) data;

   printf("machine name: ");
   printAncestry(pmi, stdout, "::", alc_lower, ai_include_self);
   printf("\n");

   printf("reentrance protection: %s\n"
          , pmi->modFlags & mfReentrant ? "yes" : " no"
          );

   printf("machine transition: %s\n"
          , pmi->machineTransition ? pmi->machineTransition->name : "none"
          );

   printf("actions return: ");
   if (pmi->modFlags & mfActionsReturnStates)
   {
      printf("states");
   }
   else if (pmi->modFlags & mfActionsReturnVoid)
   {
      printf("void");
   }
   else
   {
      printf("events");
   }
   printf("\n");

   printf("number of events: %u\n"
          , pmi->event_list->count
          );
   printf("number of events handled in zero states: %u\n"
		  , pmi->events_with_zero_handlers
		  );

   printf("number of events handled in one state: %u\n"
		  , pmi->events_with_one_handler
		  );

   printf("machine has events with single pai: %s\n"
          , pmi->has_single_pai_events ? "yes" : "no"
         );

   printf("number of states: %u\n"
          , pmi->state_list->count
          );

   printf("number of states with entry fns: %u\n"
          , pmi->states_with_entry_fns_count
          );

   printf("number of states with exit fns: %u\n"
          , pmi->states_with_exit_fns_count
          );

   printf("number of states handling zero events: %u\n"
		  , pmi->states_with_zero_events
		  );

   printf("number of states handling one event: %u\n"
		  , pmi->states_with_one_event
		  );

   printf("number of states with no way in: %u\n"
		  , pmi->states_with_no_way_in
		  );

   printf("number of states with no way out: %u\n"
		  , pmi->states_with_no_way_out
		  );

   printf("number of actions: %u\n"
          , pmi->action_list->count
          );

   if (pmi->data)
   {
	   printf("machine has data\n");
   }

   if (pmi->machine_list)
   {
	   printf("number of sub-machines: %u\n"
			  , pmi->machine_list->count
			  );

	   printf("depth of sub-machines: %u\n"
			  , pmi->sub_machine_depth 
			  );
   }

   unsigned        str_len = 0;
   ITERATOR_HELPER ih      = {
	   .pmi        = pmi
	   , .counter0 = &str_len
	   , .first    = true
   };

   iterate_list(pmi->event_list, compute_pid_name_len, &ih);

   char *actions_triggered = "Actions Triggered";
   int  min_str_len = strlen(actions_triggered) + 1;

   printf("Action Array:\n");
   printf("%-20.20s%-*.*s%-14.14s %-11.11s %-11.11s %-*.*s\n"
		  , "Event Name"
		  , (int)(print_action_array ? pmi->state_list->count * 16 : 0)
		  , (int)(print_action_array ? pmi->state_list->count * 16 : 0)
		  , print_action_array ? "PAI locations" : ""
		  , "PAI Count"
		  , "State Count"
		  , "Avg Use (%)"
		  , str_len > min_str_len ? str_len : min_str_len
		  , str_len > min_str_len ? str_len : min_str_len
		  , actions_triggered
		  );

   //ih.first does not need to be reset here.
   iterate_list(pmi->event_list, write_events, &ih);

   printf("\n");

   ih.first = true;
   str_len  = 0;
   iterate_list(pmi->state_list, compute_pid_name_len, &ih);

   printf("%-20.20s %-14.14s %-11.11s %-20.20s %-*.*s\n"
		  , "State Name"
		  , "Events Handled"
		  , "Avg Use (%)"
		  , "Transitions (In/Out)"
		  , str_len > min_str_len ? str_len : min_str_len
		  , str_len > min_str_len ? str_len : min_str_len
		  , actions_triggered
		  );

   //ih.first does not need to be reset here.
   iterate_list(pmi->state_list, write_states, &ih);

   printf("\n");

   if (pmi->machine_list)
   {
      iterate_list(pmi->machine_list, write_machine_statistics, NULL);
   }

   return false;
}


static bool write_events(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pevent = (pID_INFO)         pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;
	pEVENT_DATA      ped    = &pevent->type_data.event_data;

	printf("%-20.20s"
		   , pevent->name
		   );

	if (print_action_array)
	{
		pih->event = pelem->ordinal;
		iterate_list(pih->pmi->state_list, write_action_array_pointers, pih);
		printf(" ");
	}

	printf("%-14.14s"
		   , ped->single_pai_for_all_states
				  ? "single"
				  : "multiple"
		   );

	printf(" %-11u"
		   , ped->phandling_states->count
		  );

	printf(" %-11u"
		   , ped->state_density_pct
		  );

	pih->first = true;
	iterate_list(ped->pactions_list, print_pid_name, pih);

	printf("\n");

	return false;
}

static bool write_action_array_pointers(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate = (pID_INFO)         pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;

	printf("%s%-14p"
		   , pelem->ordinal ? ", " : ""
		   , (void *)pih->pmi->actionArray[pih->event][pelem->ordinal]
		   );

	return false;
}

static bool write_states(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate = (pID_INFO)         pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;
	pSTATE_DATA      psd    = &pstate->type_data.state_data;

	printf("%-20.20s %-14u %-11u %-2u/ %-2u"
		   , pstate->name
		   , psd->pevents_handled->count
		   , psd->event_density_pct
		   , psd->pinbound_transitions->count
		   , psd->poutbound_transitions->count
		   );

	if (
		psd->pinbound_transitions->count
		|| psd->poutbound_transitions->count
		)
	{
		printf(": ");
	}

	unsigned str_len = 0;
	pih->counter0    = &str_len;
	pih->first       = true;
	if (psd->pinbound_transitions->count)
	{
		iterate_list(psd->pinbound_transitions, print_pid_name, pih);
	}
	else
	{
		printf("none");
	}


	pih->first = true;
	if (psd->poutbound_transitions->count)
	{
		printf(" / ");
		iterate_list(psd->poutbound_transitions, print_pid_name, pih);
	}


	pih->first = true;
	if (psd->pactions_list->count)
	{
		printf(" ");
		iterate_list(psd->pactions_list, print_pid_name, pih);
	}

	printf("\n");

	return false;
}

static bool print_pid_name(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate = (pID_INFO)         pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;

	printf("%s%s"
		   , name_separator(pih)
		   , pstate->name
		   );

	pih->first = false;

	return false;
}

static bool compute_pid_name_len(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate = (pID_INFO)         pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;

	if (pih->counter0)
	{
		(*pih->counter0) += strlen(name_separator(pih));
		(*pih->counter0) += strlen(pstate->name);
	}

	pih->first = false;

	return false;
}

