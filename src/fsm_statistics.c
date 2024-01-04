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

static int  initMachineStatisticsWriter(pFSMOutputGenerator,char*);
static void writeMachineStatistics(pFSMOutputGenerator,pMACHINE_INFO);
static void closeMachineStatisticsWriter(pFSMOutputGenerator,int);
static bool write_machine_statistics(pLIST_ELEMENT,void*);

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

   printf("number of actions: %u\n"
          , pmi->action_list->count
          );

   if (pmi->data)
   {
	   printf("machine has data\n");
   }

   printf("number of sub-machines: %u\n"
          , pmi->machine_list ? pmi->machine_list->count : 0
          );

   printf("Action Array:\n");
   for (unsigned e = 0; e < pmi->event_list->count; e++)
   {

      printf("event %s: "
             , eventNameByIndex(pmi, e)
             );

      for (unsigned s = 0; s < pmi->state_list->count; s++)
      {
         printf("%s%16p"
                , s ? ", " : "  "
                , (void*) pmi->actionArray[e][s]
                );
      }
      printf("   %s\n"
             , eventPidByIndex(pmi, e)->type_data.event_data.single_pai_for_all_states
                    ? "single pai"
                    : "multiple pai's"
             );

   }

   printf("\n");

   if (pmi->machine_list)
   {
      iterate_list(pmi->machine_list, write_machine_statistics, NULL);
   }

   return false;
}


