#define FSMLANG_DEVELOP
/**
*  fsm_c_common.c
*
*    Functions common to C machine creation.
*
*    FSMLang (fsm) - A Finite State Machine description language.
*    Copyright (C) 2022  Steven Stanton
*
*    This program is free software; you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation; either version 2 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software
*    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*    Steven Stanton
*    ringwinner@users.sourceforge.net
*
*    For the latest on FSMLang: http://fsmlang.sourceforge.net
*
*    And, finally, your possession of this source code implies nothing.
*
*  Long Description:
*
*   Date        Author      Description of change
*   8/24/2022      sstanton  Initial Version
*/

#include "fsm_c_common.h"
#include "fsm_unused.h"

#if defined (CYGWIN) || defined (LINUX)
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#endif
#if defined (LINUX) || defined (VS) || defined (CYGWIN)
#include <time.h>
#endif
#include <string.h>
#include <stdlib.h>

#if defined (CYGWIN) || defined (LINUX)
#include "y.tab.h"
#elif defined (VS)
#include "parser.h"
#endif

#include "revision.h"

typedef enum ENTRY_OR_EXIT
{
    eoe_entry
    , eoe_exit
} ENTRY_OR_EXIT;

bool generate_weak_fns = true;
bool core_logging_only = false;
bool include_svg_img   = false;


static char  *eventXRefFormat0Str = "\t%5u  ";
static char  *eventXRefFormat1Str = "_%s\n";

static bool declare_event_user_data_structs                                          (pLIST_ELEMENT,void*);
static bool declare_event_user_data_union_mbrs                                       (pLIST_ELEMENT,void*);
static bool print_sub_machine_as_enum_member                                         (pLIST_ELEMENT,void*);
static bool declare_sub_machine_if                                                   (pLIST_ELEMENT,void*);
static bool define_weak_action_function                                              (pLIST_ELEMENT,void*);
static bool define_event_passing_actions                                             (pLIST_ELEMENT,void*);
static bool print_event_macro                                                        (pLIST_ELEMENT,void*);
static bool declare_shared_event_lists                                               (pLIST_ELEMENT,void*);
static bool declare_shared_event_data_blocks                                         (pLIST_ELEMENT,void*);
static void declare_parent_event_reference_data_structures                           (pCMachineData,pMACHINE_INFO);
static void define_parent_event_reference_elements                                   (pCMachineData,pMACHINE_INFO);
static bool define_shared_event_lists                                                (pLIST_ELEMENT,void*);
static bool reference_shared_event_data_blocks                                       (pLIST_ELEMENT,void*);
static bool print_inhibited_state_case                                               (pLIST_ELEMENT,void*);
static bool write_state_entry_fn_switch_case                                         (pLIST_ELEMENT,void*);
static bool write_state_exit_fn_switch_case                                          (pLIST_ELEMENT,void*);
static void print_state_entry_or_exit_fn_switch_case                                 (pID_INFO,pITERATOR_CALLBACK_HELPER,ENTRY_OR_EXIT);
static void defineSubMachineInhibitor                                                (pCMachineData,pMACHINE_INFO);
static void print_entry_or_exit_fn_signature                                         (pID_INFO,pITERATOR_CALLBACK_HELPER,ENTRY_OR_EXIT);
static bool print_event_cross_reference                                              (pLIST_ELEMENT,void*);
static bool print_sub_machine_event_cross_reference                                  (pLIST_ELEMENT,void*);
static void print_event_cross_reference_entry                                        (char*,pITERATOR_HELPER);
static void print_action_function_declaration                                        (pMACHINE_INFO,FILE*,char*);
static void print_transition_fn_declaration_for_when_actions_return_states           (pMACHINE_INFO,FILE*,char*);
static void print_state_only_transition_fn_declaration_for_when_actions_return_states(pMACHINE_INFO,FILE*,char*);
static void print_transition_fn_declaration_for_when_actions_return_events           (pMACHINE_INFO,FILE*,char*);
static void print_state_only_transition_fn_declaration_for_when_actions_return_events(pMACHINE_INFO,FILE*,char*);
static bool print_event_enum_member                                                  (pLIST_ELEMENT,void*);
static bool print_state_enum_member                                                  (pLIST_ELEMENT,void*);


int initCMachine(pFSMOutputGenerator pfsmog, char *fileName)
{
   pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

   if ((pfsmcog->pcmd = newCMachineData(fileName)))
   {
	   writeCFilePreambles(pfsmcog->pcmd);

      return 0;
   }

   return 1;

}

int initCMachineFN(pFSMOutputGenerator pfsmog, char *fileName)
{
   pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

   if ((pfsmcog->pcmd = newCMachineData(fileName)))
   {
      return 0;
   }

   return 1;

}

void writeCFilePreambles(pCMachineData pcmd)
{
	char	*cp, *cp1;

	if (!(pcmd->cFile = openFile(pcmd->cName, "w")))
	{

		fprintf(stderr
				, "%s: unable to open %s\n"
				, me
				, pcmd->cName
				);

	   CHECK_AND_FREE(pcmd->cName);
	   CHECK_AND_FREE(pcmd->hName);

	   FREE_AND_CLEAR(pcmd);

	}
	else if (!(pcmd->hFile = openFile(pcmd->hName, "w")))
	{

		fprintf(stderr
				, "%s: unable to open %s\n"
				, me
				, pcmd->hName
				);

	   FCLOSE_AND_CLEAR(pcmd->cFile);

	   FREE_AND_CLEAR(pcmd->cName);
	   CHECK_AND_FREE(pcmd->hName);

	   FREE_AND_CLEAR(pcmd);

	}
	else
	{

	   /* the header file */
	   cp1 = getFileNameNoDir(pcmd->hName);
	   for (cp = cp1; *cp; cp++)

		  /* capitalise things, and change the '.' or '-' to '_' */
		  *cp = ((*cp == '.') || (*cp == '-')) ? '_' : (char)toupper(*cp);

	   fprintf(pcmd->hFile
			   , "/**\n\t%s\n\n"
			   , pcmd->hName
			   );
	   fprintf(pcmd->hFile
			   , "\tThis file automatically generated by FSMLang\n*/\n\n"
			   );

	   fprintf(pcmd->hFile, "#ifndef _%s_\n", cp1);
	   fprintf(pcmd->hFile, "#define _%s_\n\n", cp1);

	   /* the source file */
	   fprintf(pcmd->cFile
			   , "/**\n\t%s\n\n"
			   , pcmd->cName
			   );
	   fprintf(pcmd->cFile
			   , "\tThis file automatically generated by FSMLang\n*/\n\n"
			   );

	   /* put the call to the header file into the source */
	   fprintf(pcmd->cFile
			   , "#include \"%s\"\n"
			   , pcmd->hName
			   );

	   fprintf(pcmd->cFile, "#include <stddef.h>\n\n"
			   );

	   /* protect ourselves from not having DBG_PRINTF defined */
	   fprintf(pcmd->cFile, "#ifndef DBG_PRINTF\n#define DBG_PRINTF(...)\n");

	   fprintf(pcmd->cFile, "#endif\n\n");

	   CHECK_AND_FREE(cp1);

	}

}

int initCSubMachine(pFSMOutputGenerator pfsmog, char *fileName)
{
   pFSMCSubMachineOutputGenerator pfsmcsmog = (pFSMCSubMachineOutputGenerator) pfsmog;

   if ((pfsmcsmog->pcmd = newCMachineData(fileName)))
   {
	   writeCFilePreambles(pfsmcsmog->pcmd);

      /* put the call to the parent header file into the header */
      fprintf(pfsmcsmog->pcmd->hFile
              , "#include \"%s\"\n\n"
              , pfsmcsmog->parent_fsmcog->pcmd->hName
              );

      return 0;
   }

   return 1;

}

int initCSubMachineFN(pFSMOutputGenerator pfsmog, char *fileName)
{
   pFSMCSubMachineOutputGenerator pfsmcsmog = (pFSMCSubMachineOutputGenerator) pfsmog;

   if ((pfsmcsmog->pcmd = newCMachineData(fileName)))
   {
      return 0;
   }

   return 1;

}

void closeCMachine(pFSMOutputGenerator pfsmog, int how)
{
   pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

   destroyCMachineData(pfsmcog->pcmd, how);
}

void closeCMachineFN(pFSMOutputGenerator pfsmog, int how)
{
   pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

   FREE_AND_CLEAR(pfsmcog->pcmd->cName);
   FREE_AND_CLEAR(pfsmcog->pcmd->hName);

}

void addEventCrossReference(pCMachineData pcmw, pMACHINE_INFO pmi, pITERATOR_HELPER pih)
{

   if ( pmi->external_event_designation_count > 0 )
   {
      if (assignExternalEventValues(pmi) == false)
      {
         printf("warning: cannot print cross reference with external designations\n");
         return;
      }
   }

   unsigned enum_val = 0;

   fprintf(pcmw->hFile
           , "/*\n\tEvent Cross Reference:\n\n"
          );

   pih->first     = true;
   pih->counter0  = &enum_val;
   pih->pmi       = pmi;

   iterate_list(pmi->event_list, print_event_cross_reference, pih);

   if (
       !(pmi->modFlags & mfActionsReturnStates)
       && !(pmi->modFlags & mfActionsReturnVoid)
      )
   {
      print_event_cross_reference_entry("noEvent", pih);
   }

   print_event_cross_reference_entry("numEvents", pih);

   if (pmi->machine_list)
   {
      iterate_list(pmi->machine_list, print_sub_machine_event_cross_reference, pih);

	  pih->pmi = pmi;
	  print_event_cross_reference_entry("numAllEvents", pih);
   }


   fprintf(pcmw->hFile
           , "\n*/\n"
          );

}

void commonHeaderStart(pCMachineData pcmw, pMACHINE_INFO pmi, char *arrayName)
{
   unsigned				i;
   ITERATOR_HELPER   helper;

   /* put the native code segment out to the header */
   if (pmi->native) fprintf(pcmw->hFile, "%s\n", pmi->native);

   helper.first   = false;
   helper.fout    = pcmw->hFile;
   helper.pmi     = pmi;

   fprintf(pcmw->hFile, "#ifdef %s_DEBUG\n", cp);
   fprintf(pcmw->hFile, "#include <stdio.h>\n");
   fprintf(pcmw->hFile, "#include <stdlib.h>\n");
   fprintf(pcmw->hFile, "#endif\n\n");

   if (pmi->has_single_pai_events)
   {
      fprintf(pcmw->hFile, "#include <stdbool.h>\n");
   }

   fprintf(pcmw->hFile
           , "#define FSM_VERSION \"%s\"\n"
           , rev_string
           );

   if (core_logging_only)
   {
      fprintf(pcmw->hFile, "#ifndef NON_CORE_DEBUG_PRINTF\n#define NON_CORE_DEBUG_PRINTF(...) \n#endif\n\n");
   }

   /* put the "call the state machine" macro into the header */
   fprintf(pcmw->hFile, "\n#define RUN_STATE_MACHINE(A,B) \\\n");
   fprintf(pcmw->hFile
           , "\t((*(A)->fsm)((A),((%s"
           , pmi->data_block_count ? "p" : ""
           );
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, "_EVENT) B)))\n\n");

   /* put the "declare a state machine" macro into the header */
   fprintf(pcmw->hFile
           , "#ifndef INIT_FSM_DATA\n#define INIT_FSM_DATA {0}\n#endif\n"
           );

   fprintf(pcmw->hFile, "#define DECLARE_");
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, "_MACHINE(A) \\\n");

   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile
           , " (A) =\\\n{\\\n%s\t"
           , pmi->data ? "\tINIT_FSM_DATA,\\\n" : ""
		  );
   fprintf(pcmw->hFile
		   , "%s_%s,\\\n\t"
		   , pmi->name->name
           , stateNameByIndex(pmi, 0)
          );
   fprintf(pcmw->hFile
		   , "%s_noEvent,\\\n\t&"
		   , pmi->name->name
		   );
   fprintf(pcmw->hFile
		   , "%s_%s_array,\\\n\t"
		   , pmi->name->name
           , arrayName
		   );
   fprintf(pcmw->hFile
		   , "%sFSM\\\n};\\\n"
		   , pmi->name->name
		   );
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, " *p##A = &A;\n\n");

   fprintf(pcmw->hFile
           , "/* Event naming convenience macros. */\n"
           );

   fprintf(pcmw->hFile, "#undef THIS\n#define THIS(A) ");
   fprintf(pcmw->hFile
		   , "%s_##A\n"
		   , pmi->name->name
		   );

   if (pmi->machine_list)
   {
      fprintf(pcmw->hFile, "#undef ");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, "\n#define ");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, "(A) ");
	  fprintf(pcmw->hFile
			  , "%s_##A\n"
			  , pmi->name->name
			  );

      iterate_list(pmi->machine_list, print_event_macro, &helper);

      fprintf(pcmw->hFile
              , "\n"
              );
   }

   if (add_event_cross_reference)
   {
      addEventCrossReference(pcmw, pmi, &helper);
   }

   /* put the event enum into the header file */
   fprintf(pcmw->hFile, "typedef enum ");
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile
		   , "_EVENT%s {\n"
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM" : ""
          );


   if (pmi->external_event_designation_count > 0 )
   {
      if (assignExternalEventValues(pmi) == false)
      {
         printf("warning: cannot use external event designations\n");
      }
   }


   helper.first = true;
   iterate_list(pmi->event_list, print_event_enum_member, &helper);

   if (
       !(pmi->modFlags & mfActionsReturnStates)
       && !(pmi->modFlags & mfActionsReturnVoid)
      )
   {
      fprintf(pcmw->hFile
			  , "\t, %s_noEvent\n"
			  , pmi->name->name
			  );
   }

   fprintf(pcmw->hFile
		   , "\t, %s_numEvents"
		   , pmi->name->name
		   );
   if (assignExternalEventValues(pmi))
   {
      fprintf(pcmw->hFile
			  , " = %u"
              , pmi->event_list->count
              );
   }
   fprintf(pcmw->hFile, "\n");

   if (pmi->machine_list)
   {
      iterate_list(pmi->machine_list,print_sub_machine_events,&helper);

      fprintf(pcmw->hFile
              , "\t, %s_numAllEvents\n"
              , pmi->name->name
              );
   }

   fprintf(pcmw->hFile
           , "}%s"
           , compact_action_array ? "__attribute__((__packed__)) " : " "
          );
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile
		   , "_EVENT%s;\n\n"
           , pmi->data_block_count ? "_ENUM" : ""
		  );

   fprintf(pcmw->hFile, "#undef ACTION_RETURN_TYPE\n#define ACTION_RETURN_TYPE ");
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile
		   , "_EVENT%s\n"
           , pmi->data_block_count ? "_ENUM" : ""
          );

   /* if we have user data on some events, declare the event structure */
   if (pmi->data_block_count)
   {
      iterate_list(pmi->event_list, declare_event_user_data_structs, &helper);

      /* create the union of pointers */
      fprintf(pcmw->hFile
              , "typedef union {\n"
              );

      iterate_list(pmi->event_list, declare_event_user_data_union_mbrs, &helper);

      fprintf(pcmw->hFile, "} ");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, "_EVENT_DATA, *p");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, "_EVENT_DATA;\n\n");

      /* create the event struct */
      fprintf(pcmw->hFile, "typedef struct {\n\t");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, "_EVENT_ENUM event;\n\t");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, "_EVENT_DATA event_data;\n} ");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, "_EVENT, *p");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, "_EVENT;\n\n");

   }

   if (generate_run_function)
   {
      fprintf(pcmw->hFile, "\nvoid run_%s(%s"
              , pmi->name->name
              , pmi->data_block_count ? "p" : ""
             );
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, "_EVENT);\n\n");
   }

   fprintf(pcmw->hFile, "#ifdef ");
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, "_DEBUG\n");
   fprintf(pcmw->hFile, "extern char *");
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, "_EVENT_NAMES[];\n");
   fprintf(pcmw->hFile, "#endif\n\n");

   /* put the state enum into the header file */
   fprintf(pcmw->hFile
           , "typedef enum {\n"
          );

   helper.first = true;
   iterate_list(pmi->state_list, print_state_enum_member, &helper);
   

   /*
     Though not a state, this needs have a value
       different from any state. Moreover, it could
       be said to bear the same relation to states
       as noEvent does to events.
   */
   if (pmi->modFlags & mfActionsReturnStates)
   {
      fprintf(pcmw->hFile
              , "\t, %s_noTransition\n"
              , pmi->name->name
             );
   }

   fprintf(pcmw->hFile
           , "\t, %s_numStates\n"
           , pmi->name->name
           );

   fprintf(pcmw->hFile
           , "}%s"
           , compact_action_array ? " __attribute__((__packed__))" : " "
          );
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, "_STATE;\n\n");

   fprintf(pcmw->hFile, "#ifdef ");
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, "_DEBUG\n");
   fprintf(pcmw->hFile, "extern char *");
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, "_STATE_NAMES[];\n");
   fprintf(pcmw->hFile, "#endif\n\n");

   /* put the data struct typedef into the header */
   if (pmi->data)
   {
      fprintf(pcmw->hFile
              , "typedef struct _%s_data_struct_ "
              , pmi->name->name
			 );
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, "_DATA, *p");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, "_DATA;\n");
   }

   /* put the machine struct typedef into the header */
   fprintf(pcmw->hFile
		   , "typedef struct _%s_struct_ "
           pmi->name->name,
		  );
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, ", *p");
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, ";\n");

   fprintf(pcmw->hFile, "#undef FSM_TYPE_PTR\n#define FSM_TYPE_PTR p");
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, "\n");

   if (generate_instance)
   {
      fprintf(pcmw->hFile, "extern ");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile
			  , " %s;\n\n"
              pmi->name->name
			 );

      fprintf(pcmw->hFile, "extern p");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile
			  , " p%s;\n\n"
              , pmi->name->name
             );
   }

   /* put the action function typedef into the header */
   if (pmi->modFlags & mfActionsReturnStates)
   {
      fprintf(pcmw->hFile, "typedef ");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, "_STATE (*");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, "_ACTION_FN)(p");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, ");\n\n");
   }
   else if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmw->hFile, "typedef void (*");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, "_ACTION_FN)(p");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, ");\n\n");
   }
   else
   {
      fprintf(pcmw->hFile, "typedef ");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile
			  , "_EVENT%s (*"
              , pmi->data_block_count ? "_ENUM"  : ""
             );
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, "_ACTION_FN)(p");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, ");\n\n");
   }

   /* typedef transition functions, if we have any */
   if (pmi->transition_fn_list->count)
   {
      fprintf(pcmw->hFile, "typedef ");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, "_STATE (*");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, "_TRANSITION_FN)(p");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile, ",");
	  streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
	  fprintf(pcmw->hFile
			  , "_EVENT%s);\n\n"
              , pmi->data_block_count ? "_ENUM"  : ""
             );
   }

   /* typedef the FSM function */
   fprintf(pcmw->hFile, "typedef void (*");
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, "_FSM)(p");
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile
		   , ",%s"
           , pmi->data_block_count ? "p"  : ""
           );
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, "_EVENT);\n\n");

   /* declare the fsm function */
   fprintf(pcmw->hFile
           , "void %sFSM(p"
           , pmi->name->name
           );
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile
		   , ",%s"
           , pmi->data_block_count ? "p"  : ""
		  );
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, "_EVENT);\n\n");

   if (pmi->machine_list)
   {
	   printSubMachinesDeclarations(pcmw,pmi);
   }

   /* put the data structure definition into the header */
   if (pmi->data)
   {
      fprintf(pcmw->hFile
              , "struct _%s_data_struct_ {\n"
              , pmi->name->name
              );

      helper.tab_level = 1; 
      iterate_list(pmi->data, print_data_field, &helper);

      fprintf(pcmw->hFile 
              , "};\n\n"
              );
   }

}

void commonHeaderEnd(pCMachineData pcmw, pMACHINE_INFO pmi, bool needNoOp)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmw      = pcmw;
   ich.pmi       = pmi;
   ich.needNoOp  = needNoOp;

   /* declare the action functions themselves */
   iterate_list(pmi->action_list, declare_action_function, &ich);

   /* declare the dummy, or no op action */
   if (needNoOp)
   {
	   print_action_function_declaration(pmi, pcmw->hFile, "noAction");
   }

   fprintf(pcmw->hFile
           , "\n"
          );

   /* declare any machine transition function */
   if (pmi->machineTransition)
   {
	   fprintf(pcmw->hFile, "void ");
	   printAncestry(pmi, pcmw->hFile, "_", alc_lower, ai_include_self);
	   fprintf(pcmw->hFile, "_%s(p"
			   , pmi->machineTransition->name
			  );
	   printAncestry(pmi, pcmw->hFile, "_", alc_upper, ai_include_self);
	   fprintf(pcmw->hFile, ",");
	   printAncestry(pmi, pcmw->hFile, "_", alc_upper, ai_include_self);
	   fprintf(pcmw->hFile, "_STATE);\n\n");
   }

   /* declare any transition functions */
   if (pmi->modFlags & mfActionsReturnStates)
   {
	   print_transition_fn_declaration_for_when_actions_return_states(pmi,pcmw->hFile, "noTransition");

      if (pmi->transition_fn_list->count)
      {
         iterate_list(pmi->transition_fn_list
                      , declare_transition_fn_for_when_actions_return_states
                      , &ich
                      );

         iterate_list(pmi->transition_list
                      , declare_state_only_transition_functions_for_when_actions_return_states 
                      , &ich
                      );
      }

      fprintf(pcmw->hFile, "\n");

   }
   else
   {

      if (pmi->transition_fn_list->count)
      {
         iterate_list(pmi->transition_fn_list
                      , declare_transition_fn_for_when_actions_return_events
                      , &ich
                      );

         iterate_list(pmi->transition_list
                      , declare_state_only_transition_functions_for_when_actions_return_events
                      , &ich
                      );
      }

      fprintf(pcmw->hFile, "\n");

      if (pmi->transition_fn_list->count)
      {
		  print_transition_fn_declaration_for_when_actions_return_events(pich->pmi
																		 , pich->pcmw->hFile
																		 , "noTransitionFn"
																		 );
		  fprintf(pcmw->cFile, "\n");
      }
   }

   /* declare any entry or exit functions */
   if (pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
   {
      iterate_list(pmi->state_list, declare_state_entry_and_exit_functions, &ich);
      fprintf(pcmw->hFile, "\n");
   }

   /* declare needed data translators */
   if (pmi->data_block_count)
   {
      iterate_list(pmi->event_list, declare_data_translator_functions, &ich);
      fprintf(pcmw->hFile, "\n");
   }

}

void generateRunFunction(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   fprintf(pcmw->cFile
           ,"void run_%s(%s"
           , pmi->name->name
           , pmi->data_block_count ? "p" : ""
           );
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->cFile, "_EVENT e)\n{\n");

   fprintf(pcmw->cFile
           , "\tif (p%s)\n\t{\n\t\tRUN_STATE_MACHINE(p%s,e);\n\t}\n}\n\n"
           , pmi->name->name
           , pmi->name->name
           );
}

void generateInstance(pCMachineData pcmw, pMACHINE_INFO pmi, char *arrayName)
{
   /* instantiate the machine and the pointer to it */
   /* the (empty) data struct and the state */
	printAncestry(pmi, pcmw->cFile, "_", alc_upper, ai_include_self);
   fprintf(pcmw->cFile, " ");
   printAncestry(pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->cFile, " = {");
   printAncestry(pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->cFile
		   , "%s\n\t"
           pmi->data ? "\n\tINIT_FSM_DATA," : "",
		   );
   printAncestry(pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->cFile
		   , "_%s,\n",
           stateNameByIndex(pmi, 0)
           );

   fprintf(pcmw->cFile, "\t");
   printAncestry(pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->cFile
		   , "_%s,\n"
           , eventNameByIndex(pmi, 0)
          );

   fprintf(pcmw->cFile, "\t&");
   printAncestry(pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->cFile
		   , "_%s_array,\n"
           , arrayName
          );

   if (pmi->machine_list)
   {
      fprintf(pcmw->cFile, "\t&");
      fprintf(pcmw->cFile, "_sub_fsm_if_array,\n");
   }

   fprintf(pcmw->cFile, "\t");
   printAncestry(pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->cFile, "FSM\n};\n\n");

   fprintf(pcmw->cFile, "p");
   printAncestry(pmi, pcmw->cFile, "_", alc_upper, ai_include_self);
   fprintf(pcmw->cFile, " p");
   printAncestry(pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->cFile, " = &");
   printAncestry(pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->cFile, ";\n\n");

}

void defineWeakActionFunctionStubs(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmw      = pcmw;
   ich.pmi       = pmi;

   iterate_list(pmi->action_list, define_weak_action_function, &ich);
}

void defineWeakNoActionFunctionStubs(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmw->cFile
              , "void __attribute__((weak)) %s_noAction(p%s pfsm)\n"
              , pmi->name->name
              , cp
             );
      fprintf(pcmw->cFile
              , "{\n\t(void) pfsm;\n\t%s(\"weak: %s_noAction\");\n}\n\n"
              , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
              , pmi->name->name
              );
   }
   else
   {
      if (pmi->modFlags & mfActionsReturnStates)
      {
         fprintf(pcmw->cFile
                 , "%s_STATE __attribute__((weak)) %s_noAction(p%s pfsm)\n"
                 , cp
                 , pmi->name->name
                 , cp
                );
         fprintf(pcmw->cFile
                 , "{\n\t%s(\"weak: %s_noAction\");\n"
                 , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                 , pmi->name->name
                 );
         fprintf(pcmw->cFile
                 , "\t(void) pfsm;\n"
                 );

         fprintf(pcmw->cFile
                 , "\treturn THIS(noTransition);\n}\n\n"
                 );
      }
      else
      {
         fprintf(pcmw->cFile
                 , "%s_EVENT%s __attribute__((weak)) %s_noAction(p%s pfsm)\n"
                 , cp
                 , pmi->data_block_count ? "_ENUM"  : ""
                 , pmi->name->name
                 , cp
                );
         fprintf(pcmw->cFile
                 , "{\n\t%s(\"weak: %s_noAction\");\n"
                 , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                 , pmi->name->name
                 );
         fprintf(pcmw->cFile
                 , "\t(void) pfsm;\n"
                 );

         fprintf(pcmw->cFile
                 , "\treturn THIS(noEvent);\n}\n\n"
                 );
      }
   }
}

void defineSubMachineWeakNoActionFunctionStubs(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   char *parent_cp = hungarianToUnderbarCaps(pmi->parent->name->name);

   if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmw->cFile
              , "void __attribute__((weak)) %s_noAction(p%s pfsm)\n"
              , pmi->name->name
              , cp
             );
      fprintf(pcmw->cFile
              , "{\n\t%s(\"weak: %s_noAction\");\n}\n\n"
              , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
              , pmi->name->name
              );
      fprintf(pcmw->cFile
              , "\t(void) pfsm;\n"
              );
   }
   else
   {
      if (pmi->modFlags & mfActionsReturnStates)
      {
         fprintf(pcmw->cFile
                 , "%s_STATE __attribute__((weak)) %s_noAction(p%s pfsm)\n"
                 , cp
                 , pmi->name->name
                 , cp
                );
         fprintf(pcmw->cFile
                 , "{\n\t%s(\"weak: %s_noAction\");\n"
                 , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                 , pmi->name->name
                 );
         fprintf(pcmw->cFile
                 , "\t(void) pfsm;\n"
                 );
         fprintf(pcmw->cFile
                 , "\treturn THIS(noTransition);\n}\n\n"
                 );
      }
      else
      {
         fprintf(pcmw->cFile
                 , "%s_EVENT%s __attribute__((weak)) %s_noAction(p%s pfsm)\n"
                 , parent_cp
                 , pmi->parent->data_block_count ? "_ENUM"  : ""
                 , pmi->name->name
                 , cp
                );
         fprintf(pcmw->cFile
                 , "{\n\t%s(\"weak: %s_noAction\");\n"
                 , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                 , pmi->name->name
                 );
         fprintf(pcmw->cFile
                 , "\t(void) pfsm;\n"
                 );
         fprintf(pcmw->cFile
                 , "\treturn PARENT(noEvent);\n}\n\n"
                 );
      }
   }

   FREE_AND_CLEAR(parent_cp);
}

void defineWeakStateEntryAndExitFunctionStubs(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   ITERATOR_CALLBACK_HELPER ich;

   ich.pmi    = pmi;
   ich.pcmw   = pcmw;

   if (pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
   {
      iterate_list(pmi->state_list, define_state_entry_and_exit_functions, &ich);
   }

}

void writeStateTransitions(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   pID_INFO pid_info;

   if (pmi->modFlags & mfActionsReturnStates)
   {
      /* grab state transitions from action array */
      for (unsigned e = 0; e < pmi->event_list->count; e++)
      {
         for (unsigned s = 0; s < pmi->state_list->count; s++)
         {
            if (pmi->actionArray[e][s])
            {
               if (
                   (!strlen(pmi->actionArray[e][s]->action->name))
                   && (pmi->actionArray[e][s]->transition->type == STATE)
                  )
               {
                  fprintf(pcmw->cFile
                          , "\n%s_STATE __attribute__((weak)) %s_transitionTo%s(p%s pfsm)\n{\n"
                          , cp
                          , pmi->name->name
                          , pmi->actionArray[e][s]->transition->name
                          , cp
                         );
                  fprintf(pcmw->cFile
                          , "\t(void) pfsm;\n\n\t%s(\"weak: %s_transitionTo%s\");\n\treturn %s_%s;\n}\n"
                          , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                          , pmi->name->name
                          , pmi->actionArray[e][s]->transition->name
                          , pmi->name->name
                          , pmi->actionArray[e][s]->transition->name
                         );
               }
            }
         }
      }
   }
   else
   {
      for (unsigned i = 0; i < pmi->transition_list->count; i++)
      {
         pid_info = transitionPidByIndex(pmi, i);
         if (pid_info->type == STATE)
         {
            fprintf(pcmw->cFile
                    , "\n%s_STATE __attribute__((weak)) %s_transitionTo%s(p%s pfsm,%s_EVENT%s e)\n{\n"
                    , cp
                    , pmi->name->name
                    , pid_info->name
                    , cp
                    , cp
                    , pmi->data_block_count ? "_ENUM"  : ""
                   );
            fprintf(pcmw->cFile
                    , "\t(void) pfsm;\n\t(void) e;\n\t%s(\"weak: %s_transitionTo%s\");\n\treturn %s_%s;\n}\n"
                    , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                    , pmi->name->name
                    , pid_info->name
                    , pmi->name->name
                    , pid_info->name
                   );
         }
      }
   }
}

void subMachineWriteStateTransitions(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   pID_INFO pid_info;
   char *parent_cp = hungarianToUnderbarCaps(pmi->parent->name->name);

   if (pmi->modFlags & mfActionsReturnStates)
   {
      /* grab state transitions from action array */
      for (unsigned e = 0; e < pmi->event_list->count; e++)
      {
         for (unsigned s = 0; s < pmi->state_list->count; s++)
         {
            if (pmi->actionArray[e][s])
            {
               if (
                   (!strlen(pmi->actionArray[e][s]->action->name))
                   && (pmi->actionArray[e][s]->transition->type == STATE)
                  )
               {
                  fprintf(pcmw->cFile
                          , "\n%s_STATE __attribute__((weak)) %s_transitionTo%s(p%s pfsm)\n{\n"
                          , cp
                          , pmi->name->name
                          , pmi->actionArray[e][s]->transition->name
                          , cp
                         );
                  fprintf(pcmw->cFile
                          , "\t(void) pfsm;\n\n\t%s(\"weak: %s_transitionTo%s\");\n\treturn %s_%s;\n}\n"
                          , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                          , pmi->name->name
                          , pmi->actionArray[e][s]->transition->name
                          , pmi->name->name
                          , pmi->actionArray[e][s]->transition->name
                         );
               }
            }
         }
      }
   }
   else
   {
      for (unsigned i = 0; i < pmi->transition_list->count; i++)
      {
         pid_info = transitionPidByIndex(pmi, i);
         if (pid_info->type == STATE)
         {
            fprintf(pcmw->cFile
                    , "\n%s_STATE __attribute__((weak)) %s_transitionTo%s(p%s pfsm,%s_EVENT%s e)\n{\n"
                    , cp
                    , pmi->name->name
                    , pid_info->name
                    , cp
                    , parent_cp
                    , pmi->data_block_count ? "_ENUM"  : ""
                   );
            fprintf(pcmw->cFile
                    , "\t(void) pfsm;\n\t(void) e;\n\t%s(\"weak: %s_transitionTo%s\");\n\treturn %s_%s;\n}\n"
                    , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                    , pmi->name->name
                    , pid_info->name
                    , pmi->name->name
                    , pid_info->name
                   );
         }
      }
   }

   FREE_AND_CLEAR(parent_cp);
}

/**
 * @brief Writes machine debug info in a short way - without name prefix
 * 
 * @param pcmw 
 * @param pmi 
 * @param cp 
 */
void writeDebugInfoShort(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   unsigned i;
   ITERATOR_HELPER helper;

   fprintf(pcmw->cFile, "\n#ifdef %s_DEBUG\n", cp);

   fprintf(pcmw->cFile, "char *%s_EVENT_NAMES[] = {\n", cp);

   fprintf(pcmw->cFile, "\t \"%s\"\n", eventNameByIndex(pmi, 0));

   for (i = 1; i < pmi->event_list->count; i++)
   {
      fprintf(pcmw->cFile, "\t, \"%s\"\n", eventNameByIndex(pmi, i));
   }

   if (!(pmi->modFlags & mfActionsReturnStates) && !(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile, "\t, \"noEvent\"\n");
   }

   fprintf(pcmw->cFile, "\t, \"numEvents\"\n");

   if (pmi->machine_list)
   {
      helper.first   = false;
      helper.pparent = pmi;
      helper.fout    = pcmw->cFile;

      iterate_list(pmi->machine_list,print_sub_machine_event_names,&helper);
   }

   fprintf(pcmw->cFile, "};\n\n");

   fprintf(pcmw->cFile, "char *%s_STATE_NAMES[] = {\n", cp);

   fprintf(pcmw->cFile, "\t \"%s\"\n", stateNameByIndex(pmi, 0));

   for (i = 1; i < pmi->state_list->count; i++)
   {
      fprintf(pcmw->cFile, "\t,\"%s\"\n", stateNameByIndex(pmi, i));
   }

   if (pmi->modFlags & mfActionsReturnStates)
   {
      fprintf(pcmw->cFile, "\t,\"noTransition\"\n");
   }

   fprintf(pcmw->cFile, "};\n\n");

   fprintf(pcmw->cFile, "#endif\n");
}

/**
 * @brief Writes machine debug information with machine name prefix
 * 
 * @param pcmw 
 * @param pmi 
 * @param cp 
 */
void writeDebugInfoLong(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   unsigned i;
   ITERATOR_HELPER helper;

   fprintf(pcmw->cFile, "\n#ifdef %s_DEBUG\n", cp);

   fprintf(pcmw->cFile, "char *%s_EVENT_NAMES[] = {\n", cp);

   fprintf(pcmw->cFile, "\t \"%s_%s\"\n"
           , pmi->name->name,
           eventNameByIndex(pmi, 0)
           );

   for (i = 1; i < pmi->event_list->count; i++)
   {
      fprintf(pcmw->cFile, "\t, \"%s_%s\"\n"
              , pmi->name->name
              , eventNameByIndex(pmi, i)
              );
   }

   if (!(pmi->modFlags & mfActionsReturnStates) && !(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile, "\t, \"%s_noEvent\"\n",
              pmi->name->name);
   }

   fprintf(pcmw->cFile, "\t, \"%s_numEvents\"\n",
           pmi->name->name);

   if (pmi->machine_list)
   {
      helper.first   = false;
      helper.pparent = pmi;
      helper.fout    = pcmw->cFile;

      iterate_list(pmi->machine_list,print_sub_machine_event_names,&helper);
   }

   fprintf(pcmw->cFile, "};\n\n");

   fprintf(pcmw->cFile, "char *%s_STATE_NAMES[] = {\n", cp);

   fprintf(pcmw->cFile, "\t \"%s_%s\"\n",
           pmi->name->name,
           stateNameByIndex(pmi, 0));

   for (i = 1; i < pmi->state_list->count; i++)
   {
      fprintf(pcmw->cFile
              , "\t,\"%s_%s\"\n"
              , pmi->name->name
              , stateNameByIndex(pmi, i)
              );
   }

   if (pmi->modFlags & mfActionsReturnStates)
   {
      fprintf(pcmw->cFile
              , "\t,\"%s_noTransition\"\n"
              , pmi->name->name
              );
   }

   fprintf(pcmw->cFile, "};\n\n");

   fprintf(pcmw->cFile, "#endif\n");
}

void writeDebugInfo(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
    short_dbg_names ? writeDebugInfoShort(pcmw, pmi, cp) : writeDebugInfoLong(pcmw, pmi, cp);
}


/**
  newpCMachineData

  Allocates a new CMachineData struct.
  Sets cName to baseFileName + ".c"
  Sets hName to baseFileName + ".h"
  Sets cFile to openFile(cName,"w")
  Sets hFile to openFile(hName,"w")

  If baseFileName is NULL, cFile = hFile = stdout; and,
    cName = hName = NULL.

  Returns :

    pCMachineData on success,
    NULL						on failure.

*/
pCMachineData	newCMachineData(char *baseFileName)
{

   pCMachineData	pcmw;

   if ((pcmw = calloc(sizeof(CMachineData), 1)) != NULL)
   {

      if (!baseFileName)
      {

         pcmw->cFile = pcmw->hFile = stdout;

      }
      else
      {

         pcmw->cName  = createFileName(baseFileName, ".c");
         pcmw->hName  = createFileName(baseFileName, ".h");

      }

   }

   return pcmw;

}

void destroyCMachineData(pCMachineData pcmw, int good)
{

   if (!pcmw) return;

   if (good)
   {

      /* close the #ifndef on the header file */
      fprintf(pcmw->hFile, "#endif\n");

   }

   fclose(pcmw->hFile);
   fclose(pcmw->cFile);

   if (!good)
   {

      unlink(pcmw->hName);
      unlink(pcmw->cName);

   }

   CHECK_AND_FREE(pcmw->hName);
   CHECK_AND_FREE(pcmw->cName);

   free(pcmw);

}

bool assignExternalEventValues(pMACHINE_INFO pmi)
{
   return (
           (pmi->modFlags & (mfActionsReturnStates | mfActionsReturnVoid))
           && (pmi->event_list->count == pmi->external_event_designation_count)
           && !compact_action_array
          );

}

static bool print_state_enum_member(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO state       = (pID_INFO)         pelem->mbr;
	pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

	fprintf(pih->fout
			, "\t%s%s_%s\n"
			, pih->first ? (pih->first = false, "") : ", "
			, pih->pmi->name->name
			, state->name
			);

	return false;
}

static bool print_event_enum_member(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO event       = (pID_INFO)         pelem->mbr;
	pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

	fprintf(pih->fout
			, first ? (pih->first = false, "\t %s") : "\t, %s"
			, pih->pmi->name->name
			);
	fprintf(pih->fout
			, assignExternalEventValues(pih->pmi)
			  ? "_%s = %s\n"
			  : "_%s%s\n"
			, event->name
			, assignExternalEventValues(pih->pmi)
			  ? event->type_data.event_data.externalDesignation->name
			  : ""
		   );
	return false;
}
static bool print_event_cross_reference(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO event       = (pID_INFO)         pelem->mbr;
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   print_event_cross_reference_entry(event->name, pih);

   return false;
}

static bool print_sub_machine_event_cross_reference(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO pmi    = (pMACHINE_INFO)    pelem->mbr;
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   pih->pmi     = pmi;

   iterate_list(pmi->event_list, print_event_cross_reference, pih);

   if (
       !(pmi->modFlags & mfActionsReturnStates)
       && !(pmi->modFlags & mfActionsReturnVoid)
      )
   {
      print_event_cross_reference_entry("noEvent", pih);
   }

   if (pmi->machine_list)
   {
	   iterate_list(pmi->machine_list, print_sub_machine_event_cross_reference, pih);
	   pih->pmi = pmi;
   }

   return false;
}

static void print_event_cross_reference_entry(char *event_name, pITERATOR_HELPER pih)
{
   fprintf(pih->fout
           , eventXRefFormat0Str
           , (*pih->counter0)++
		   );
   printAncestry(pih->pmi, pih->fout, "_", alc_lower, ai_include_self);
   fprintf(pih->fout
		   , eventXRefFormat1Str
           , event_name
           );
}

static bool declare_event_user_data_union_mbrs(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO         pevent = (pID_INFO) pelem->mbr;
   pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;
   char             *event_name_cp;

   if (pevent->type_data.event_data.puser_event_data)
   {
      if (pevent->type_data.event_data.puser_event_data->data_fields)
      {
         event_name_cp = hungarianToUnderbarCaps(pevent->name);

         print_tab_levels(pih->fout, pih->tab_level);
         fprintf(pih->fout
                 , "%s_%s_DATA %s_data;\n"
                 , pih->cp
                 , event_name_cp
                 , pevent->name
                 );

         CHECK_AND_FREE(event_name_cp);
      }
   }

   return false;
}

static bool declare_event_user_data_structs(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO         pevent = (pID_INFO) pelem->mbr;
   pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;

   if (pevent->type_data.event_data.puser_event_data)
   {
      if (pevent->type_data.event_data.puser_event_data->data_fields)
      {
         fprintf(pih->fout, "typedef struct _");
		 streamHungarianToUnderbarCaps(pih->fout, pih->pmi->name->name);
         fprintf(pih->fout, "_");
		 streamHungarianToUnderbarCaps(pih->fout, pevent->name);
         fprintf(pih->fout, "_data_ {\n");

         pih->tab_level = 1;
         iterate_list(pevent->type_data.event_data.puser_event_data->data_fields
                      , print_data_field
                      , pih
                      );

         fprintf(pih->fout, "} ");
		 streamHungarianToUnderbarCaps(pih->fout, pih->pmi->name->name);
         fprintf(pih->fout, "_");
		 streamHungarianToUnderbarCaps(pih->fout, pevent->name);
         fprintf(pih->fout, "_DATA, *p");
		 streamHungarianToUnderbarCaps(pih->fout, pih->pmi->name->name);
         fprintf(pih->fout, "_");
		 streamHungarianToUnderbarCaps(pih->fout, pevent->name);
         fprintf(pih->fout, "_DATA;\n\n");
      }
   }

   return false;
}

static bool print_sub_machine_as_enum_member(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO pmi    = (pMACHINE_INFO) pelem->mbr;
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   fprintf(pih->fout
		   , "\t%s "
           , pih->first ? "" : ", "
		   );
   printAncestry(pmi, pih->fout, "_", alc_lower, ai_include_self);
   fprintf(pih->fout, "\n");

   if (pih->first)
   {
      pih->first = false;

      fprintf(pih->fout, "\t, ");
	  printAncestry(pmi, pih->fout, "_", alc_lower, ai_omit_self);
	  fprintf(pih->fout, "_firstSubMachine = ");
	  printAncestry(pmi, pih->fout, "_", alc_lower, ai_include_self);
	  fprintf(pih->fout, "\n");

   }

   return false;
}

static bool declare_sub_machine_if(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO pmi    = (pMACHINE_INFO) pelem->mbr;
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   fprintf(pih->fout, "extern ");
   streamHungarianToUnderbarCaps(pih->fout, pmi->parent->name->name);
   fprintf(pih->fout, "_SUB_FSM_IF ");
   fprintf(pih->fout
		   , "%s_sub_fsm_if;\n"
		   , pmi->name->name
		   );

   return false;
}

static void declare_parent_event_reference_data_structures(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   ITERATOR_HELPER ih;

   fprintf(pcmw->hFile
           ,"/* Some sub-machines share parent events. */\n"
           );

   fprintf(pcmw->hFile, "typedef void (*");
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, "_DATA_TRANSLATION_FN)(p");
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, ");\n");

   fprintf(pcmw->hFile,"typedef struct _");
   streamStrCaseAware(pcmw->hFile, pmi->name->name, alc_lower);
   fprintf(pcmw->hFile, "_shared_event_str_ ");
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, "_SHARED_EVENT_STR, *p");
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, "_SHARED_EVENT_STR;\n");

   fprintf(pcmw->hFile, "struct _");
   streamStrCaseAware(pcmw->hFile, pmi->name->name, alc_lower);
   fprintf(pcmw->hFile, "_shared_event_str_\n{");

   fprintf(pcmw->hFile, "\t");
   printAncestry(ultimateAncestor(pmi), pcmw->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmw->hFile
		   , "_EVENT%s      event;\n"
           , pmi->data_block_count ? "_ENUM"  : ""
           );

   fprintf(pcmw->hFile, "\t");
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, "_DATA_TRANSLATION_FN  data_translation_fn;\n");

   fprintf(pcmw->hFile, "\tp");
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, "_SUB_FSM_IF          psub_fsm_if;\n");

   fprintf(pcmw->hFile
           , "};\n"
           );

   fprintf(pcmw->hFile, "extern ");
   printAncestry(ultimateAncestor(pmi), pcmw->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmw->hFile,
			 "_EVENT%s "
           , pmi->data_block_count ? "_ENUM"  : ""
           );
   streamStrCaseAware(pcmw->hFile, pmi->name->name, alc_lower);
   fprintf(pcmw->hFile, "_pass_shared_event(p");
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, ",p");
   streamHungarianToUnderbarCaps(pcmw->hFile, pmi->name->name);
   fprintf(pcmw->hFile, "_SHARED_EVENT_STR[]);\n\n");

   ih.fout = pcmw->hFile;
   ih.pmi  = pmi;

   iterate_list(pmi->event_list,declare_shared_event_lists,&ih);

   fprintf(pcmw->hFile
           , "\n"
           );
}

static bool define_shared_event_lists(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pevent      = (pID_INFO)pelem->mbr;
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   if (pevent->type_data.event_data.psharing_sub_machines)
   {
      pih->pid   = pevent;
      pih->first = true;

      fprintf(pih->fout, "p");
	  streamHungarianToUnderbarCaps(pih->fout, pih->pmi->name->name);
      fprintf(pih->fout
			  , "_SHARED_EVENT_STR sharing_%s_%s[] =\n{\n"
              , pih->pmi->name->name
              , pevent->name
              );

      iterate_list(pevent->type_data.event_data.psharing_sub_machines, reference_shared_event_data_blocks, pih);

      fprintf(pih->fout
              , "\t, NULL\n"
              );

      fprintf(pih->fout
              , "};\n\n"
              );

   }

   return false;
}

static bool print_inhibited_state_case(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pid                   = (pID_INFO)pelem->mbr;
   pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

   if (pid->type_data.state_data.state_flags & sfInibitSubMachines)
   {
      pich->counter++;
      fprintf(pich->pcmw->cFile, "\t\tcase %s_%s:\n"
             , pich->pmi->name->name
             , pid->name
             );
   }

   return false;
}

static bool reference_shared_event_data_blocks(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO pmi    = (pMACHINE_INFO)pelem->mbr;
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   fprintf(pih->fout
           , "\t%s&%s_share_%s_%s_str\n"
           , pih->first ? "  " : ", "
           , pmi->name->name
           , pmi->parent->name->name
           , pih->pid->name
           );

   pih->first = false;

   return false;
}

static void define_parent_event_reference_elements(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   ITERATOR_HELPER ih;

   ih.pmi   = pmi;
   ih.fout  = pcmw->cFile;
   ih.first = true;

   /* define arrays */
   iterate_list(pmi->event_list, define_shared_event_lists, &ih);

   /* passing function */
   streamHungarianToUnderbarCaps(pcmw->cFile, ultimateAncestor(pmi)->name->name);
   fprintf(pcmw->cFile
           , "_EVENT%s %s_pass_shared_event(p"
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
		   , pmi->name->name
           );
   streamHungarianToUnderbarCaps(pcmw->cFile, pmi->name->name);
   fprintf(pcmw->cFile, " pfsm, p");
   streamHungarianToUnderbarCaps(pcmw->cFile, pmi->name->name);
   fprintf(pcmw->cFile, "_SHARED_EVENT_STR sharer_list[])\n{\n");

   fprintf(pcmw->cFile, "\t");
   streamHungarianToUnderbarCaps(pcmw->cFile, ultimateAncestor(pmi)->name->name);
   fprintf(pcmw->cFile
		   , "_EVENT%s return_event = THIS(noEvent);\n"
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
           );

   fprintf(pcmw->cFile, "\tfor (p");
   streamHungarianToUnderbarCaps(pcmw->cFile, pmi->name->name);
   fprintf(pcmw->cFile
		   , "_SHARED_EVENT_STR *pcurrent_sharer = sharer_list;\n\t     *pcurrent_sharer && return_event == THIS(noEvent);\n\t     pcurrent_sharer++)\n\t{\n"
           );

   fprintf(pcmw->cFile
           , "\t\tif ((*pcurrent_sharer)->data_translation_fn)\n"
           );

   fprintf(pcmw->cFile
           , "\t\t\t(*(*pcurrent_sharer)->data_translation_fn)(pfsm);\n"
           );

   fprintf(pcmw->cFile
           , "\t\treturn_event = (*(*pcurrent_sharer)->psub_fsm_if->subFSM)((*pcurrent_sharer)->event);\n"
           );

   fprintf(pcmw->cFile
           , "\t}\n\n"
           );

   fprintf(pcmw->cFile
           , "\treturn return_event;\n}\n\n"
           );

}

bool declare_transition_fn_for_when_actions_return_states(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   print_transition_fn_declaration_for_when_actions_return_states(pich->pmi, pich->pcmw->hFile, pid_info->name);

   return false;
}

bool declare_state_only_transition_functions_for_when_actions_return_states(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   print_state_only_transition_fn_declaration_for_when_actions_return_states(pich->pmi, pich->pcmw_hFile, pid_info->name);

   return false;
}

static void print_transition_fn_declaration_for_when_actions_return_events(pMACHINE_INFO pmi, FILE *fout, char *name)
{
	printAncestry(pmi, fout, "_", alc_upper, ai_include_self);
	fprintf(fout, "_STATE ");
	printAncestry(pmi, fout, "_", alc_lower, ai_include_self);
	fprintf(fout
			, "_%s("
			, name
			);
	fprintf(fout, "p");
	printAncestry(pmi, fout, "_", alc_upper, ai_include_self);
	fprintf(fout, ",");
	streamHungarianToUnderbarCaps(fout, ultimateAncestor(pmi)->name->name);
	fprintf(fout
			, "_EVENT%s);\n"
			, ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
		   );
}

static void print_state_only_transition_fn_declaration_for_when_actions_return_events(pMACHINE_INFO pmi, FILE *fout, char *name)
{
	printAncestry(pmi, fout, "_", alc_upper, ai_include_self);
	fprintf(fout, "_STATE ");
	printAncestry(pmi, fout, "_", alc_lower, ai_include_self);
	fprintf(fout
			, "_TransitionTo%s("
			, name
			);
	fprintf(fout, "p");
	printAncestry(pmi, fout, "_", alc_upper, ai_include_self);
	fprintf(fout, ",");
	streamHungarianToUnderbarCaps(fout, ultimateAncestor(pmi)->name->name);
	fprintf(fout
			, "_EVENT%s);\n"
			, ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
		   );
}

bool declare_transition_fn_for_when_actions_return_events(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   print_transition_fn_declaration_for_when_actions_return_events(pich->pmi, pich->pcmw->hFile, pid_info->name);

   return false;
}

bool declare_state_only_transition_functions_for_when_actions_return_events(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   print_state_only_transition_fn_declaration_for_when_actions_return_events(pich->pmi, pich->pcmw->hFile, pid_info->name);

   return false;
}

static void print_entry_or_exit_fn_signature(pID_INFO pstate, pITERATOR_CALLBACK_HELPER pich)
{
   pID_INFO fn        = which == eoe_entry ? pstate->type_data.state_data.entry_fn : pstate->type_data.state_data.exit_fn;
   char    *no_fn_str = which == eoe_entry ? "onEntryTo_"                          : "onExitFrom_";
   FILE    *file      = pich->define       ? pich->pcmw->cFile                     : pich->pcmw->hFile
    
   fprintf(file
           , "void %s"
           , pich->define                      ? "__attribute__((weak)) " : ""
          );
   printAncestry(pmi, file, "_", alc_lower, ai_include_self);
   fprintf(file
           , "_%s%s(%s%s%s%s)%s\n"
           , fn                                ? fn->name                 : no_fn_str
           , fn                                ? ""                       : pstate->name
           , pich->pmi->data                   ? "p"                      : "void"
           , pich->pmi->data                   ? pich->cp                 : ""
           , pich->pmi->data                   ? "_DATA"                  : ""
           , (pich->pmi->data && pich->define) ? " pdata"                 : ""
           , pich->define                      ? ""                       : ";"
           );
}

bool declare_state_entry_and_exit_functions(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pstate                = ((pID_INFO)pelem->mbr);

   //we are declaring, not defining
   pich->define = false;

   if (pstate->type_data.state_data.state_flags & sfHasEntryFn)
   {
      print_entry_or_exit_fn_signature(pstate, pich, eoe_entry);
   }

   if (pstate->type_data.state_data.state_flags & sfHasExitFn)
   {
       print_entry_or_exit_fn_signature(pstate, pich, eoe_exit);
   }

   return false;
}

bool define_state_entry_and_exit_functions(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pstate                = ((pID_INFO)pelem->mbr);

   //we are defining
   pich->define = true;

   if (pstate->type_data.state_data.state_flags & sfHasEntryFn)
   {
      print_entry_fn_signature(pstate, pich);

      fprintf(pich->pcmw->cFile
              ,"{\n%s\tDBG_PRINTF(\"weak: %s_%s%s\");\n}\n"
              , pich->pmi->data
                ? "\t(void) pdata;\n\n"
                : ""
              , pich->pmi->name->name
              , pstate->type_data.state_data.entry_fn
                ? pstate->type_data.state_data.entry_fn->name
                : "onEntryTo_"
              , pstate->type_data.state_data.entry_fn
                ? ""
                : pstate->name
              );

      fprintf(pich->pcmw->cFile, "\n");
   }

   if (pstate->type_data.state_data.state_flags & sfHasExitFn)
   {
      print_exit_fn_signature(pstate, pich);

      fprintf(pich->pcmw->cFile
              ,"{\n%s\tDBG_PRINTF(\"weak: %s_%s%s\");\n}\n"
              , pich->pmi->data
                ? "\t(void) pdata;\n\n"
                : ""
              , pich->pmi->name->name
              , pstate->type_data.state_data.exit_fn
                ? pstate->type_data.state_data.exit_fn->name
                : "onExitFrom_"
              , pstate->type_data.state_data.exit_fn
                ? ""
                : pstate->name
              );

      fprintf(pich->pcmw->cFile, "\n");
   }

   return false;
}

bool declare_data_translator_functions(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pevent                = ((pID_INFO)pelem->mbr);

   if (pevent->type_data.event_data.puser_event_data)
   {
	  fprintf(pich->pcmw->hFile, "void ");
	  printAncestry(pich->pmi, pich->pcmw->hFile, "_", alc_lower, ai_include_self);

      if (pevent->type_data.event_data.puser_event_data->translator)
      {
         fprintf(pich->pcmw->hFile
                 , "_%s(p"
                 , pevent->type_data.event_data.puser_event_data->translator->name
				 );
      }
      else
      {
		  fprintf(pich->pcmw->hFile
				  , "_translate_%s_data(p"
				  , pevent->name
				  );
      }
	  printAncestry(pich->pmi, pich->pcmw->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pich->pcmw->hFile, "_DATA,p");
	  printAncestry(pich->pmi, pich->pcmw->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pich->pcmw->hFile, "_");
	  streamHungarianToUnderbarCaps(pich->pcmw->hFile, pevent->name);
	  fprintf(pich->pcmw->hFile, "_DATA);\n");
   }

   return false;
}

bool define_weak_data_translator_functions(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pevent                = ((pID_INFO)pelem->mbr);

   if (pevent->type_data.event_data.puser_event_data)
   {
      fprintf(pich->pcmw->cFile, "void __attribute__((weak)) ");
      if (pevent->type_data.event_data.puser_event_data->translator)
      {
          printNameWithAncestry(pevent->type_data.event_data.puser_event->translator, pich->pmi, pich->pcmw->cFile, " ", alc_lower, ai_include_self);
          fprintf(pich->pcmw->cFile, "(p");
          printNameWithAncestry("DATA", pich->pmi, pich->pcmw->cFile, " ", alc_upper, ai_include_self);
          fprintf(pich->pcmw->cFile, " pfsm_data, p");
          printAncestry(pich->pmi, pich->pcmw->cFile, " ", alc_upper, ai_include_self);
          streamHungarianToUnderbarCaps(pich->pcmw->cFile, pevent->name);
          fprintf(pich->pcmw->cFile, " pdata)\n{\n\t(void) pfsm_data;\n\t(void) pdata;\n\t%s(\"weak: "
                  , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                 );
          printNameWithAncestry(pevent->type_data.event_data.puser_event->translator, pich->pmi, pich->pcmw->cFile, " ", alc_lower, ai_include_self);
      }
      else
      {
         printNameWithAncestry("translate", pich->pmi, pich->pcmw->cFile, " ", alc_lower, ai_include_self);
         fprintf(pich->pcmw->cFile
                 , "_%s_data(p"
                 , pevent->name
                );
         printNameWithAncestry("DATA", pich->pmi, pich->pcmw->cFile, " ", alc_upper, ai_include_self);
         fprintf(pich->pcmw->cFile, " pfsm_data, p");
         printAncestry(pich->pmi, pich->pcmw->cFile, " ", alc_upper, ai_include_self);
         streamHungarianToUnderbarCaps(pich->pcmw->cFile, pevent->name);
         fprintf(pich->pcmw->cFile, " pdata)\n{\n\t(void) pfsm_data;\n\t(void) pdata;\n\t%s(\"weak: "
                 , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                );
         printNameWithAncestry("translate", pich->pmi, pich->pcmw->cFile, " ", alc_lower, ai_include_self);
         fprintf(pich->pcmw->cFile
                 , "_%s_data(p"
                 , pevent->name
                );
      }
      fprintf(pich->pcmw->cFile, "\");\n}\n\n");
   }

   return false;
}

bool sub_machine_define_weak_data_translator_functions(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pevent                = ((pID_INFO)pelem->mbr);

   if (pevent->type_data.event_data.puser_event_data)
   {
      if (pevent->type_data.event_data.puser_event_data->translator)
      {
         fprintf(pich->pcmw->cFile
                 , "void __attribute__((weak)) %s_%s(p%s pfsm)\n{\n\t(void) pfsm;\n\t%s(\"weak: %s_%s\");\n}\n\n"
                 , pich->pmi->name->name
                 , pevent->type_data.event_data.puser_event_data->translator->name
                 , pich->parent_cp
                 , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                 , pich->pmi->name->name
                 , pevent->type_data.event_data.puser_event_data->translator->name
                );
      }
   }

   return false;
}

void subMachineHeaderStart(pCMachineData pcmw, pMACHINE_INFO pmi, char *arrayName)
{
   unsigned         i;
   ITERATOR_HELPER  helper;

   helper.fout      = pcmw->hFile;

   /* put the native code segment out to the header */
   if (pmi->native) fprintf(pcmw->hFile, "%s\n", pmi->native);

   fprintf(pcmw->hFile, "#ifdef %s_DEBUG\n", cp);
   fprintf(pcmw->hFile, "#include <stdio.h>\n");
   fprintf(pcmw->hFile, "#include <stdlib.h>\n");
   fprintf(pcmw->hFile, "#endif\n\n");

   /* put the "declare a state machine" macro into the header */
   fprintf(pcmw->hFile, "#define DECLARE_");
   printAncestry(pmi, pcmw->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmw->hFile, "_MACHINE(A) \\\n");
   printAncestry(pmi, pcmw->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmw->hFile
           , " A =\\\n{\\\n%s"
           , pmi->data ? "\tINIT_FSM_DATA,\\\n" : ""
		  );
   fprintf(pcmw->hFile
		   , "\t%s_%s,\\n"
           , pmi->name->name
           , stateNameByIndex(pmi, 0)
		  );
   fprintf(pcmw->hFile
		   , "\t%s_%s_noEvent,\\\n"
           , pmi->name->name
           , pmi->parent->name->name
		  );
   fprintf(pcmw->hFile
		   , "\t&%s_%s_array,\\\n"
           , pmi->name->name
           , arrayName
		  );
   fprintf(pcmw->hFile
		   , "\t%sFSM\\\n};\\\n"
           , pmi->name->name
		   );
   printAncestry(pmi->, pcmw->hFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->hFile, " *p##A = &(A);\n\n");

   fprintf(pcmw->hFile
           , "/*\n\tsub-machine events are included in the top-level machine event enumeration.\n"
           );

   fprintf(pcmw->hFile
           , "\tThese macros set the appropriate names for events from THIS machine\n"
           );

   fprintf(pcmw->hFile
           , "\tand those from the PARENT machine.\n"
           );

   fprintf(pcmw->hFile
           , "\n\tThey may be turned off as needed.\n*/\n"
           );

   fprintf(pcmw->hFile
           , "#ifndef NO_EVENT_CONVENIENCE_MACROS\n"
           );

   fprintf(pcmw->hFile, "#undef THIS\n#define THIS(A) ");
   printAncestry(pmi->, pcmw->hFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->hFile, "_##A\n");

   fprintf(pcmw->hFile, "#define PARENT(A) ");
   printAncestry(pmi->, pcmw->hFile, "_", alc_lower, ai_omit_self);
   fprintf(pcmw->hFile, "_##A\n");

   fprintf(pcmw->hFile
           , "#endif\n"
           );

   fprintf(pcmw->hFile, "\n#ifdef ");
   printAncestry(pmi->, pcmw->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmw->hFile, "_DEBUG\n");
   fprintf(pcmw->hFile, "extern char *");
   printAncestry(pmi->, pcmw->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmw->hFile, "_EVENT_NAMES[];\n");
   fprintf(pcmw->hFile, "#endif\n\n");

   /* put the state enum into the header file */
   fprintf(pcmw->hFile
           , "typedef enum {\n"
          );


   helper.first = true;
   iterate_list(pmi->state_list, print_state_enum_member, &helper);

   /*
     Though not a state, this needs have a value
       different from any state. Moreover, it could
       be said to bear the same relation to states
       as noEvent does to events.
   */
   if (pmi->modFlags & mfActionsReturnStates)
   {
      fprintf(pcmw->hFile
              , "\t, %s_noTransition\n"
              , pmi->name->name
             );
   }

   fprintf(pcmw->hFile
           , "\t, %s_numStates\n"
           , pmi->name->name
           );

   fprintf(pcmw->hFile
           , "}%s%s_STATE;\n\n"
           , compact_action_array ? " __attribute__((__packed__))" : " "
           , cp
          );

   fprintf(pcmw->hFile, "#ifdef %s_DEBUG\n", cp);
   fprintf(pcmw->hFile, "extern char *%s_STATE_NAMES[];\n", cp);
   fprintf(pcmw->hFile, "#endif\n\n");

   /* put the data struct typedef into the header */
   if (pmi->data)
   {
      fprintf(pcmw->hFile, "typedef struct _");
	  printAncestry(pmi->, pcmw->hFile, "_", alc_lower, ai_include_self);
	  fprintf(pcmw->hFile, "_data_struct_ ");
	  printAncestry(pmi->, pcmw->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmw->hFile, "_DATA, *p");
	  printAncestry(pmi->, pcmw->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmw->hFile, "_DATA;\n");
   }

   /* put the machine struct typedef into the header */
   fprintf(pcmw->hFile, "typedef struct _");
   printAncestry(pmi->, pcmw->hFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->hFile, "_struct_ ");
   printAncestry(pmi->, pcmw->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmw->hFile, ", *p");
   printAncestry(pmi->, pcmw->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmw->hFile, ";\n");

   fprintf(pcmw->hFile, "#undef FSM_TYPE_PTR\n#define FSM_TYPE_PTR p");
   printAncestry(pmi->, pcmw->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmw->hFile, "\n");

   if (generate_instance)
   {
      fprintf(pcmw->hFile, "extern ");
	  printAncestry(pmi->, pcmw->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmw->hFile, " ");
	  printAncestry(pmi->, pcmw->hFile, "_", alc_lower, ai_include_self);
	  fprintf(pcmw->hFile, ";\n\n");

      fprintf(pcmw->hFile, "extern p");
	  printAncestry(pmi->, pcmw->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmw->hFile, " p");
	  printAncestry(pmi->, pcmw->hFile, "_", alc_lower, ai_include_self);
	  fprintf(pcmw->hFile, ";\n\n");
   }

   /* put the action function typedef into the header */
   fprintf(pcmw->hFile, "typedef ");
   if (pmi->modFlags & mfActionsReturnStates)
   {
	  printAncestry(pmi->, pcmw->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmw->hFile, "_STATE (*");
	  printAncestry(pmi->, pcmw->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmw->hFile, "_ACTION_FN)(p");
	  printAncestry(pmi->, pcmw->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmw->hFile, ");\n\n");
   }
   else if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmw->hFile, "void (*");
	  printAncestry(pmi->, pcmw->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmw->hFile, "_ACTION_FN)(p");
	  printAncestry(pmi->, pcmw->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmw->hFile, ");\n\n");
   }
   else
   {
	  streamHungarianToUnderbarCaps(pcmw->hFile, ultimateAncestor(pmi));
      fprintf(pcmw->hFile
			  , "_EVENT%s (*"
			  , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
			  );
	  printAncestry(pmi, pcmw->hFile, "_", alc_upper, ai_include_self);
      fprintf(pcmw->hFile, "_ACTION_FN)(p");
	  printAncestry(pmi, pcmw->hFile, "_", alc_upper, ai_include_self);
      fprintf(pcmw->hFile, ");\n\n");
   }

   /* typedef transition functions, if we have any */
   if (pmi->transition_fn_list->count)
   {
      fprintf(pcmw->hFile, "typedef ");
	  printAncestry(pmi, pcmw->hFile, "_", alc_upper, ai_include_self);
      fprintf(pcmw->hFile, "_STATE (*");
	  printAncestry(pmi, pcmw->hFile, "_", alc_upper, ai_include_self);
      fprintf(pcmw->hFile, "_TRANSITION_FN)(p");
	  printAncestry(pmi, pcmw->hFile, "_", alc_upper, ai_include_self);
      fprintf(pcmw->hFile, ",");
	  streamHungarianToUnderbarCaps(pcmw->hFile, ultimateAncestor(pmi));
      fprintf(pcmw->hFile
			  , "_EVENT%s);\n\n"
              , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
             );

   }

   /* typedef the FSM function */
   fprintf(pcmw->hFile, "typedef ");
   streamHungarianToUnderbarCaps(pcmw->hFile, ultimateAncestor(pmi));
   fprintf(pcmw->hFile
		   , "_EVENT%s (*"
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
		  );
   fprintf(pcmw->hFile, "_FSM)(p");
   printAncestry(pmi, pcmw->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmw->hFile, ",");
   streamHungarianToUnderbarCaps(pcmw->hFile, ultimateAncestor(pmi));
   fprintf(pcmw->hFile
		   , "_EVENT%s);\n\n"
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
           );

   /* declare the fsm function */
   streamHungarianToUnderbarCaps(pcmw->hFile, ultimateAncestor(pmi));
   fprintf(pcmw->hFile
           , "_EVENT%s "
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
		  );
   printAncestry(pmi, pcmw->hFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->hFile, "FSM(p");
   printAncestry(pmi, pcmw->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmw->hFile, ",");
   streamHungarianToUnderbarCaps(pcmw->hFile, ultimateAncestor(pmi));
   fprintf(pcmw->hFile
		   , "_EVENT%s);\n\n"
           , ultimateAncestor(pmi)->parent->data_block_count ? "_ENUM"  : ""
           );

   /* put the data structure definition into the header */
   if (pmi->data)
   {
      fprintf(pcmw->hFile, "struct _");
	  printAncestry(pmi, pcmw->hFile, "_", alc_lower, ai_include_self);
      fprintf(pcmw->hFile, "_data_struct_ {\n");

      helper.tab_level = 1;
      iterate_list(pmi->data, print_data_field, &helper);

      fprintf(pcmw->hFile 
              , "};\n\n"
              );
   }

   if (pmi->machine_list)
   {
	   printSubMachinesDeclarations(pcmw, pmi);
   }
   

}

static void print_transition_fn_declaration_for_when_actions_return_states(pMACHINE_INFO pmi, FILE *fout, char *name)
{
	printAncestry(pmi, fout, "_", alc_upper, ai_include_self);
	fprintf(fout, "_STATE ");
	printAncestry(pmi, fout, "_", alc_lower, ai_include_self);
	fprintf(fout, "_%s(p"
			, name
			);
	printAncestry(pmi, fout, "_", alc_upper, ai_include_self);
	fprintf(fout, ");\n");
}

static void print_state_only_transition_fn_declaration_for_when_actions_return_states(pMACHINE_INFO pmi, FILE *fout, char *name)
{
	printAncestry(pmi, fout, "_", alc_upper, ai_include_self);
	fprintf(fout, "_STATE ");
	printAncestry(pmi, fout, "_", alc_lower, ai_include_self);
	fprintf(fout, "_TransitionTo%s(p"
			, name
			);
	printAncestry(pmi, fout, "_", alc_upper, ai_include_self);
	fprintf(fout, ");\n");
}

static bool define_needed_shared_event_structures(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pevent      = (pID_INFO) pelem->mbr;
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   if (pevent->type_data.event_data.shared_with_parent)
   {

	  printAncestry(pih->pmi, pih->fout, "_", alc_upper, ai_include_self | ai_stop_at_parent);
      fprintf(pih->fout, "_SHARED_EVENT_STR ");
	  fprintf(pih->fout
			  , "%s_share_"
              , pih->pmi->name->name
			  );
	  printAncestry(pih->pmi, pih->fout, "_", alc_lower, ai_include_self | ai_stop_at_parent);
	  fprintf(pih->fout
			  , "_%s_str = {\n"
              , pevent->name
              );

      fprintf(pih->fout
              , "\t  .event               = THIS(%s)\n"
              , pevent->name
              );

      fprintf(pih->fout
              , "\t, .data_translation_fn = "
              );

      if (pevent->type_data.event_data.puser_event_data)
      {
         if (pevent->type_data.event_data.puser_event_data->translator)
         {
            fprintf(pih->fout
                    , "%s_%s\n"
                    , pih->pmi->name->name
                    , pevent->type_data.event_data.puser_event_data->translator->name
                    );
         }
         else
         {
            fprintf(pih->fout
                    , "NULL\n"
                    );

         }
      }
      else
      {
         fprintf(pih->fout
                 , "NULL\n"
                 );

      }

      fprintf(pih->fout, "\t, .psub_fsm_if         = &");
	  printAncestry(pih->pmi, pih->fout, "_", alc_lower, ai_include_self);
	  fprintf(pih->fout "_sub_fsm_if\n");

      fprintf(pih->fout
              , "};\n\n"
              );

   }

   return false;
}

void possiblyDefineSubMachineSharedEventStructures (pCMachineData pcmw, pMACHINE_INFO pmi)
{
   ITERATOR_HELPER ih;

   if (pmi->shared_event_count)
   {
      ih.pmi  = pmi;
      ih.fout = pcmw->cFile;

      iterate_list(pmi->event_list, define_needed_shared_event_structures, &ih);
   }

}

void defineSubMachineIF (pCMachineData pcmw, pMACHINE_INFO pmi)
{
   printAncestry(pich->pmi, pich->pcmw->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmw->cFile
           , "_EVENT%s "
           , pmi->parent->data_block_count ? "_ENUM"  : ""
 		  );
   printAncestry(pmi, pcmw->hFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->cFile "_sub_machine_fn(");
   printAncestry(pich->pmi, pich->pcmw->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmw->cFile
 		  , "_EVENT%s e)\n{\n"
           , pmi->parent->data_block_count ? "_ENUM"  : ""
           );

   fprintf(pcmw->cFile, "\treturn ");
   printAncestry(pmi, pcmw->hFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->cFile, "FSM(p");
   printAncestry(pmi, pcmw->hFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->cFile, ", e);\n}\n\n\n");

   printAncestry(pich->pmi, pich->pcmw->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmw->cFile, "_SUB_FSM_IF ");
   printAncestry(pmi, pcmw->hFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->cFile, "_sub_fsm_if =\n{\n\t");

  fprintf(pcmw->cFile, "\t.subFSM = ");
  printAncestry(pmi, pcmw->hFile, "_", alc_lower, ai_include_self);
  fprintf(pcmw->cFile, "_sub_machine_fn\n");

  fprintf(pcmw->cFile, "\t, .first_event = ");
  printAncestry(pmi, pcmw->hFile, "_", alc_lower, ai_include_self);
  fprintf(pcmw->cFile
 		 , "_%s\n"
          , eventNameByIndex(pmi, 0)
          );

  fprintf(pcmw->cFile, "\t, .last_event = ");
  printAncestry(pmi, pcmw->hFile, "_", alc_lower, ai_include_self);
  fprintf(pcmw->cFile
 		 , "_%s\n"
          , eventNameByIndex(pmi, pmi->event_list->count - 1)
          );

  fprintf(pcmw->cFile
          ,"};\n\n"
          );

}

void defineSubMachineArray(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   ITERATOR_CALLBACK_HELPER ich;

   if (pmi->machine_list)
   {
      fprintf(pcmw->cFile, "\np");
	  printAncestry(pmi, pcmw->cFile, "_", alc_upper, ai_include_self);
      fprintf(pcmw->cFile, "_SUB_FSM_IF ");
	  printAncestry(pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
      fprintf(pcmw->cFile, "_sub_fsm_if_array[");
	  printAncestry(pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
      fprintf(pcmw->cFile, "_numSubMachines] =\n{\n");

      ich.pcmw  = pcmw;
      ich.pmi   = pmi;
      ich.first = true;
      iterate_list(pmi->machine_list, print_sub_machine_if,&ich);

      fprintf(pcmw->cFile
              ,"};\n\n"
              );

      if (pmi->parent_event_reference_count)
      {
         define_parent_event_reference_elements(pcmw, pmi);
      }

   }
}

static void print_action_function_declaration(pMACHINE_INFO pmi, FILE *fout, char *name)
{
	if (pmi->modFlags & mfActionsReturnVoid)
	{
	   fprintf(fout, "void ");
	}
	else
	{
	   if (pmi->modFlags & mfActionsReturnStates)
	   {
		  printAncestry(pmi, fout, "_", alc_upper, ai_include_self);
		  fprintf(fout, "_STATE ");
	   }
	   else
	   {
		  streamHungarianToUnderbarCaps(fout, ultimateAncestor(pmi)->name->name);
		  fprintf(fout
				  , "_EVENT%s "
				  , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
				 );
	   }
	}

	printAncestry(pmi, fout, "_", alc_lower, ai_include_self);
	fprintf(fout "_%s(p"
			, name
			);
	printAncestry(pmi, fout, "_", alc_upper, ai_include_self);
	fprintf(fout, ");\n");

}

bool declare_action_function(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   if (pid_info->name && strlen(pid_info->name))
   {
	   print_action_function_declaration(pich->pmi, pich->pcmw->hFile, pid_info->name);
   }

   return false;
}

bool print_sub_machine_if(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO pmi              = (pMACHINE_INFO) pelem->mbr;
   pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

   fprintf(pich->pcmw->cFile
           ,"\t%s&"
           ,pich->first ? (pich->first = false, "") : ", "
          );
   printAncestry(pich->pmi, pich->pcmw->cFile, "_", alc_lower, ai_include_self);
   fprintf(pich->pcmw->cFile, "_sub_fsm_if\n");

   return false;
}

bool define_weak_action_function(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   if (pid_info->name && strlen(pid_info->name))
   {

      if (pich->pmi->modFlags & mfActionsReturnVoid)
      {
         fprintf(pich->pcmw->cFile, "void __attribute__((weak)) ");
		 printAncestry(pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
		 fprintf(pcmw->cFile
				 , "_%s(p"
                 , pid_info->name
                 , pich->cp
                );
		 printAncestry(pmi, pcmw->cFile, "_", alc_upper, ai_include_self);
		 fprintf(pcmw->cFile, " pfsm)\n{\n");

         fprintf(pich->pcmw->cFile
                 , "\t%s(\"weak: "
                 , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
				 );
		 printAncestry(pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
		 fprintf(pcmw->cFile
				 , "_%s\");\n"
                 , pid_info->name
                 );

         fprintf(pich->pcmw->cFile
                 , "\t(void) pfsm;\n"
                 );

      }
      else
      {
         if (pich->pmi->modFlags & mfActionsReturnStates)
         {
			printAncestry(pmi, pcmw->cFile, "_", alc_upper, ai_include_self);
            fprintf(pich->pcmw->cFile, "_STATE __attribute__((weak)) ");
			printAncestry(pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
			fprintf(pcmw->cFile
					, "_%s(p"
                    , pid_info->name
					);
			printAncestry(pmi, pcmw->cFile, "_", alc_upper, ai_include_self);
			fprintf(pcmw->cFile, " pfsm)\n{\n");

            fprintf(pich->pcmw->cFile
                    , "\t%s(\"weak: "
                    , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                    );
			printAncestry(pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
			fprintf(pcmw->cFile
					, "_%s\");\n"
                    , pid_info->name
					);

            fprintf(pich->pcmw->cFile
                    , "\t(void) pfsm;\n"
                    );

            fprintf(pich->pcmw->cFile, "\treturn ");
			printAncestry(pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
			fprintf(pcmw->cFile, "_noTransition;\n");

         }
         else
         {
			 printAncestry(pmi, pcmw->cFile, "_", alc_upper, ai_include_self);
            fprintf(pich->pcmw->cFile
                    , "_EVENT%s __attribute__((weak)) "
                    , ultimateAncestor(pich->pmi)->data_block_count ? "_ENUM"  : ""
                   );
			printAncestry(pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
			fprintf(pcmw->cFile
					, "_%s(p"
                    , pid_info->name
					);
			printAncestry(pmi, pcmw->cFile, "_", alc_upper, ai_include_self);
			fprintf(pcmw->cFile, " pfsm)\n{\n");

            fprintf(pich->pcmw->cFile
                    , "\t%s(\"weak: "
                    , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                    );
			printAncestry(pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
			fprintf(pcmw->cFile
					, "_%s\");\n"
                    , pid_info->name
					);

            fprintf(pich->pcmw->cFile
                    , "\t(void) pfsm;\n"
                    );

            fprintf(pich->pcmw->cFile
                    , "\treturn THIS(noEvent);\n"
                    );

         }
      }

      fprintf(pich->pcmw->cFile
              , "}\n\n"
              );

   }

   return false;
}

bool define_event_passing_actions(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   if (pid_info->name && strlen(pid_info->name))
   {
      /* if this action is associated with a shared event, it will have exactly one event */
      pID_INFO pevent = (pID_INFO)find_nth_list_member(pid_info->actionInfo->matrix->event_list,0);

      /* and, that event will have a list of sharing machines */
      if (pevent->type_data.event_data.psharing_sub_machines)
      {
          streamHungarianToUnderbarCaps(pich->pcmw->cFile, ultimateAncestor(pich->pmi)->name->name);
         fprintf(pich->pcmw->cFile
                 , "_EVENT%s "
                 , ultimateAncestor(pich->pmi)->data_block_count ? "_ENUM"  : ""
                );
         printNameWithAncestry(pid_info->name, pich->pmi, pich->pcmw->cFile, " ", alc_lower, ai_include_self);
         fprintf(pich->pcmw->cFile, "(p");
         printAncestry(pich->pmi, pich->pcmw->cFile, "_", alc_upper, ai_include_self);
         fprintf(pich->pcmw->cFile, " pfsm)\n{\n");

         fprintf(pich->pcmw->cFile
                 , "\t%s(\"weak: "
                 , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                );
         printNameWithAncestry(pid_info->name, pich->pmi, pich->pcmw->cFile, " ", alc_lower, ai_include_self);
         fprintf(pich->pcmw->cFile, "\");\n");

         fprintf(pich->pcmw->cFile
                 , "\treturn %s_pass_shared_event(pfsm, sharing_%s_%s);\n}\n\n"
                 , pich->pmi->name->name
                 , pich->pmi->name->name
                 , pevent->name
                 );
      }

   }

   return false;
}

void defineEventPassingActions(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmw      = pcmw;
   ich.pmi       = pmi;

   iterate_list(pmi->action_list
                , define_event_passing_actions
                , &ich
                );
}

void defineWeakDataTranslatorStubs(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmw      = pcmw;
   ich.pmi       = pmi;

   iterate_list(pmi->event_list
                , define_weak_data_translator_functions
                , &ich
                );

   fprintf(pcmw->hFile, "\n");
}

void defineSubMachineWeakDataTranslatorStubs(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmw      = pcmw;
   ich.pmi       = pmi;
   ich.cp        = cp;
   ich.parent_cp = hungarianToUnderbarCaps(pmi->parent->name->name);

   iterate_list(pmi->event_list
                , sub_machine_define_weak_data_translator_functions
                , &ich
                );

   fprintf(pcmw->cFile, "\n");

   CHECK_AND_FREE(ich.parent_cp);
}

static void defineSubMachineInhibitor(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmw      = pcmw;
   ich.pmi       = pmi;
   ich.counter   = 0;

   fprintf(pcmw->cFile, "\nstatic bool doNotInhibitSubMachines(");
   streamHungarianToUnderbarCaps(pcmw->cFile, pmi->name->name);
   fprintf(pcmw->cFile, "_STATE s)\n{\n");

   fprintf(pcmw->cFile, "\tswitch (s)\n\t{\n");

   iterate_list(pmi->state_list
                , print_inhibited_state_case
                , &ich
                );

   fprintf(pcmw->cFile
           , "\t\t\treturn false;\n%s\n\t}\n"
           , ich.counter < pmi->state_list->count ? "\n\t\tdefault:\n\t\t\treturn true;" : ""
           );

   fprintf(pcmw->cFile
           , "}\n\n"
           );
 
   if (pmi->submachine_inhibitor_count == pmi->state_list->count)
   {
      printf("warning: (%s) all states inhibit sub machines\n"
             , pmi->name->name
             );
   }
}

void defineSubMachineFinder(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   if (pmi->submachine_inhibitor_count)
   {
      defineSubMachineInhibitor(pcmw, pmi);
   }

   fprintf(pcmw->cFile, "\nstatic ");
   streamHungarianToUnderbarCaps(pcmw->cFile, pmi->name->name);
   fprintf(pcmw->cFile
           , "_EVENT%s findAndRunSubMachine(p"
           , pmi->data_block_count ? "_ENUM"  : ""
          );
   streamHungarianToUnderbarCaps(pcmw->cFile, pmi->name->name);
   fprintf(pcmw->cFile, " pfsm, ");
   streamHungarianToUnderbarCaps(pcmw->cFile, pmi->name->name);
   fprintf(pcmw->cFile
           , "_EVENT%s e)\n{\n"
           , pmi->data_block_count ? "_ENUM"  : ""
          );

   fprintf(pcmw->cFile, "\tfor (");
   streamHungarianToUnderbarCaps(pcmw->cFile, pmi->name->name);
   fprintf(pcmw->cFile
           , "_SUB_MACHINES machineIterator = %s_firstSubMachine;\n\t     machineIterator < %s_numSubMachines;\n\t     machineIterator++\n\t    )\n\t{\n"
           , pmi->name->name
           , pmi->name->name
           );

   fprintf(pcmw->cFile
           , "\t\t\tif (\n\t\t\t   ((*pfsm->subMachineArray)[machineIterator]->first_event <= e)\n"
           );

   fprintf(pcmw->cFile
           , "\t\t\t   && ((*pfsm->subMachineArray)[machineIterator]->last_event >= e)\n\t\t\t    )\n"
           );

   fprintf(pcmw->cFile
           , "\t\t\t{\n"
           );

   fprintf(pcmw->cFile
           , "\t\t\t\treturn ((*(*pfsm->subMachineArray)[machineIterator]->subFSM)(e));\n"
          );

   fprintf(pcmw->cFile
           , "\t\t\t}\n"
           );

   fprintf(pcmw->cFile, "\t}\n\n\treturn ");
   printNameWithAncestry("noEvent", pmi, pcmw->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmw->cFile, ";\n\n}\n\n");
}

bool print_event_macro(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO    pmi = ((pMACHINE_INFO)    pelem->mbr);
   pITERATOR_HELPER pih = ((pITERATOR_HELPER) data      );

   fprintf(pih->fout, "#undef ");
   printAncestry(pmi, pih->fout, "_", alc_upper, ai_include_self | ai_omit_ultimate);
   fprintf(pih->fout, "\n#define ");
   printAncestry(pmi, pih->fout, "_", alc_upper, ai_include_self | ai_omit_ultimate);
   fprintf(pih->fout, "(A) ");
   printAncestry(pmi, pih->fout, "_", alc_lower, ai_include_self);
   fprintf(pih->fout, "_##A\n");

   if (pmi->machine_list)
   {
	   iterate_list(pmi->machine_list, print_event_macro, data);
   }

   return false;

}

static bool declare_shared_event_data_blocks(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO pmi    = (pMACHINE_INFO)pelem->mbr;
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   fprintf(pih->fout, "extern ");
   streamHungarianToUnderbarCaps(pih->fout, pmi->parent->name->name);
   fprintf(pih->fout, "_SHARED_EVENT_STR ");
   fprintf(pih->fout
		   , "%s_share_%s_%s_str;\n"
           , pmi->name->name
           , pmi->parent->name->name
           , pih->pid->name
           );

   return false;
}

static bool declare_shared_event_lists(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pevent      = (pID_INFO)pelem->mbr;
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   if (pevent->type_data.event_data.psharing_sub_machines)
   {

      pih->pid = pevent;
      iterate_list(pevent->type_data.event_data.psharing_sub_machines
				   , declare_shared_event_data_blocks
				   , pih
				  );

      fprintf(pih->fout, "extern p");
	  streamHungarianToUnderbarCaps(pih->fout, pih->pmi->name->name);
	  fprintf(pih->fout
			  , "_SHARED_EVENT_STR sharing_%s_%s[];\n\n"
              , pih->pmi->name->name
              , pevent->name
              );

   }

   return false;
}

void addNativeImplementationIfThereIsAny(pMACHINE_INFO pmi, FILE *fout)
{
   if (pmi->native_impl)
   {
      fprintf(fout
              , "%s\n"
              , pmi->native_impl
              );
   }
}

void declareSubMachineManagers(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   fprintf(pcmd->cFile, "static ");
   streamHungarianToUnderbarCaps(pcmd->cFile, ultimateAncestor(pmi)->name->name);
   fprintf(pcmd->cFile
		   , "_EVENT%s findAndRunSubMachine(p"
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
		  );
   printAncestry(pmi, pcmd->cFile, alc_upper, ai_include_self);
   fprintf(pcmd->cFile ",");
   streamHungarianToUnderbarCaps(pcmd->cFile, ultimateAncestor(pmi)->name->name);
   fprintf(pcmd->cFile
		   , "_EVENT%s);\n"
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
           );

   if (pmi->submachine_inhibitor_count)
   {
      fprintf(pcmd->cFile, "static bool doNotInhibitSubMachines(");
	  printAncestry(pmi, pcmd->cFile, alc_upper, ai_include_self);
	  fprintf(pcmd->cFile "_STATE);\n");
   }

   fprintf(pcmd->cFile, "\n");
}

void declareEventDataManager(pCMachineData pcmd)
{
   fprintf(pcmd->cFile, "static void translateEventData(p");
   printAncestry(pmi, pcmd->cFile, alc_upper, ai_include_self);
   fprintf(pcmd->cFile "_DATA,p");
   printAncestry(pmi, pcmd->cFile, alc_upper, ai_include_self);
   fprintf(pcmd->cFile, "_EVENT);\n\n");
}

static bool write_event_data_manager_switch_case(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pevent                = (pID_INFO)pelem->mbr;
   pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

   if (pevent->type_data.event_data.puser_event_data)
   {
      pich->counter++;
      fprintf(pich->pcmw->cFile
              , "\tcase %s_%s:\n"
              , pich->pmi->name->name
              , pevent->name
              );

      fprintf(pich->pcmw->cFile
              , pevent->type_data.event_data.puser_event_data->translator
                 ? "\t\t%s_%s(pfsm_data, &pevent->event_data.%s_data);\n\t\tbreak;\n"
                 : "\t\t%s_translate_%s_data(pfsm_data, &pevent->event_data.%s_data);\n\t\tbreak;\n"
              , pich->pmi->name->name
              , pevent->type_data.event_data.puser_event_data->translator
                ? pevent->type_data.event_data.puser_event_data->translator->name
                : pevent->name
              , pevent->name
              );

   }

   return false;
}

void defineEventDataManager(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   ITERATOR_CALLBACK_HELPER ich = {0};

   ich.pcmw    = pcmd;
   ich.pmi     = pmi;

   fprintf(pcmd->cFile, "static void translateEventData(p");
   streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
   fprintf(pcmd->cFile, "_DATA pfsm_data,p");
   streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
   fprintf(pcmd->cFile, "_EVENT pevent)\n{\n");

   fprintf(pcmd->cFile
           , "\tswitch(pevent->event)\n\t{\n"
           );

   iterate_list(pmi->event_list, write_event_data_manager_switch_case, &ich);

   fprintf(pcmd->cFile
           , "\tdefault:\n\t\tbreak;\n\t}\n\n}\n\n"
           );
}

void declareStateEntryAndExitManagers(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   if (pmi->states_with_entry_fns_count)
   {
      fprintf(pcmd->cFile,"static void runAppropriateEntryFunction(");
   }
   if (pmi->states_with_exit_fns_count)
   {
	   fprintf(pcmd->cFile,"static void runAppropriateExitFunction(");
   }
   if (pmi->data)
   {
	   fprintf(pcmd->cFile,"p");
	   printAncestry(pmi, pcmw->cFile, "_", alc_upper, ai_include_self);
	   fprintf(pcmd->cFile,"_DATA_");
   }
   printAncestry(pmi, pcmw->cFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->cFile,");");
}

static void print_state_entry_or_exit_fn_switch_case(pID_INFO pstate, pITERATOR_CALLBACK_HELPER pich, ENTRY_OR_EXIT which)
{
    pID_INFO fn        = which == eoe_entry ? pstate->type_data.state_data.entry_fn : pstate->type_data.state_data.exit_fn;
    char    *no_fn_str = which == eoe_entry ? "onEntryTo_"                          : "onExitFrom_";

	pich->counter++;
	fprintf(pich->pcmw->cFile, "\tcase ");
	printAncestry(pich->pmi, pich->pcmw->cFile, "_", alc_lower, ai_include_self);
	fprintf(pich->pcmw->cFile
			, "_%s:\n\t\t"
			, pstate->name
		   );
	printAncestry(pich->pmi, pich->pcmw->cFile, "_", alc_lower, ai_include_self);

   fprintf(pich->pcmw->cFile
			, "_%s%s(%s);\n\t\tbreak;\n"
			, fn              ? fn->name : no_fn_str
			, fn              ? ""       : pstate->name
			, pich->pmi->data ? "pdata"  : ""
			);
}

static bool write_state_entry_fn_switch_case(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pstate                = (pID_INFO)pelem->mbr;
   pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

   if (pstate->type_data.state_data.state_flags & sfHasEntryFn)
   {
       print_state_entry_or_exit_fn_switch_case(pstate, pich, eoe_entry);
   }

   return false;
}

static bool write_state_exit_fn_switch_case(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pstate                = (pID_INFO)pelem->mbr;
   pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

   if (pstate->type_data.state_data.state_flags & sfHasExitFn)
   {
       print_state_entry_or_exit_fn_switch_case(pstate, pich, eoe_exit);
   }

   return false;
}

void defineStateEntryAndExitManagers(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   ITERATOR_CALLBACK_HELPER ich;

   ich.pmi  = pmi;
   ich.pcmw = pcmd;

   if (pmi->states_with_entry_fns_count)
   {
      fprintf(pcmd->cFile,"static void runAppropriateEntryFunction(");
	  if (pmi->data)
	  {
		  fprintf(pcmd->cFile,"p");
		  printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
		  fprintf(pcmd->cFile, "_DATA pdata, ");
	  }
	  printAncestry(pmi, pcmw->cFile, "_", alc_upper, ai_include_self);
      fprintf(pcmw->cFile, "_STATE s)\n{\n\tswitch(s)\n\t{\n");

      iterate_list(pmi->state_list, write_state_entry_fn_switch_case, &ich);

      if (pmi->states_with_entry_fns_count < pmi->state_list->count)
      {
         fprintf(pcmd->cFile
                 , "\tdefault:\n\t\tbreak;\n\t}\n}\n\n"
                 );
      }
   }
   if (pmi->states_with_exit_fns_count)
   {
	   fprintf(pcmd->cFile,"static void runAppropriateExitFunction(");
	   if (pmi->data)
	   {
		   fprintf(pcmd->cFile,"p");
		   printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
		   fprintf(pcmd->cFile, "_DATA pdata, ");
	   }
	   printAncestry(pmi, pcmw->cFile, "_", alc_upper, ai_include_self);
	   fprintf(pcmw->cFile, "_STATE s)\n{\n\tswitch(s)\n\t{\n");

      iterate_list(pmi->state_list, write_state_exit_fn_switch_case, &ich);

      if (pmi->states_with_exit_fns_count < pmi->state_list->count)
      {
         fprintf(pcmd->cFile
                 , "\tdefault:\n\t\tbreak;\n\t}\n}\n\n"
                 );
      }
   }
}

void printSubMachinesDeclarations(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	ITERATOR_HELPER helper;

	helper.pmi   = pmi;
	helper.fout  = pcmd->hFile;
	helper.first = true;

	fprintf(pcmd->hFile,"/* Sub Machine Declarations */\n\n");
	fprintf(pcmd->hFile
			,"%s\ntypedef enum {\n"
			,"/* enumerate sub-machines */"
			);

	iterate_list(pmi->machine_list
				 , print_sub_machine_as_enum_member
				 ,&helper
				 );

	fprintf(pcmd->hFile, "\t, ");
	streamStrCaseAware(pcmd->hFile, pmi->name->name, alc_lower);
	fprintf(pcmd->hFile, "_numSubMachines\n} ");
	printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
	fprintf(pcmd->hFile, "_SUB_MACHINES;\n\n");

	fprintf(pcmd->hFile, "typedef ");
	streamHungarianToUnderbarCaps(pcmd->hFile, ultimateAncestor(pmi)->name->name);
	fprintf(pcmd->hFile
			, "_EVENT%s (*"
			, pmi->data_block_count ? "_ENUM"  : ""
			);
	streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
	fprintf(pcmd->hFile, "_SUB_MACHINE_FN)(");
	streamHungarianToUnderbarCaps(pcmd->hFile, ultimateAncestor(pmi)->name->name);
	fprintf(pcmd->hFile
			, "_EVENT%s);\n"
			, pmi->data_block_count ? "_ENUM"  : ""
		   );

	fprintf(pcmd->hFile, "typedef struct _");
	streamStrCaseAware(pcmd->hFile, pmi->name->name, alc_lower);
	fprintf(pcmd->hFile, "_sub_fsm_if_ ");
	streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
	fprintf(pcmd->hFile, "_SUB_FSM_IF, *p");
	streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
	fprintf(pcmd->hFile, "_SUB_FSM_IF;\n");

	fprintf(pcmd->hFile,"struct _");
	streamStrCaseAware(pcmd->hFile, pmi->name->name, alc_lower);
	fprintf(pcmd->hFile, "_sub_fsm_if_\n{\n");

	fprintf(pcmd->hFile, "\t");
	printAncestry(ultimateAncestor(pmi), pcmd->hFile, "_", alc_upper, ai_include_self);
	fprintf(pcmd->hFile
			, "_EVENT%s                first_event;\n"
			, pmi->data_block_count ? "_ENUM"  : ""
			);

	fprintf(pcmd->hFile, "\t");
	printAncestry(ultimateAncestor(pmi), pcmd->hFile, "_", alc_upper, ai_include_self);
	fprintf(pcmd->hFile
			, "_EVENT%s                last_event;\n"
			, pmi->data_block_count ? "_ENUM"  : ""
			);

	fprintf(pcmd->hFile, "\t");
	streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
	fprintf(pcmd->hFile, "_SUB_MACHINE_FN       subFSM;\n");

	fprintf(pcmd->hFile
			,"};\n\n"
			);

	fprintf(pcmd->hFile, "extern p");
	streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
	fprintf(pcmd->hFile, "_SUB_FSM_IF ");
	printAncestry(pmi, pcmd->hFile, "_", alc_lower, ai_include_self);
	fprintf(pcmd->hFile, "_sub_fsm_if_array[");
	streamStrCaseAware(pcmd->hFile, pmi->name->name, alc_lower);
	fprintf(pcmd->hFile, "_numSubMachines];\n\n");

	iterate_list(pmi->machine_list, declare_sub_machine_if, &helper);

	fprintf(pcmd->hFile
			, "\n"
			);

	if (pmi->parent_event_reference_count)
	{
	   declare_parent_event_reference_data_structures(pcmd, pmi);
	}
}

void printFSMMachineDebugBlock(pCMachineData pcmd, pMACHINE_INFO pmi)
{
    fprintf(pcmd->cFile, "#ifdef %s_DEBUG\n", cp);
    fprintf(pcmd->cFile
            , "if (EVENT_IS_NOT_EXCLUDED_FROM_LOG(%s))\n{\n"
            , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
            );
    if (short_dbg_names && add_machine_name)
    {
       fprintf(pcmd->cFile, "\tDBG_PRINTF(\"%s: event: %%s; state: %%s\"\n,"
               , pmi->name->name
              );
    }
    else
    {
       fprintf(pcmd->cFile, "\tDBG_PRINTF(\"event: %%s; state: %%s\"\n,");
    }

    streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
    fprintf(pcmd->cFile
            , "_EVENT_NAMES[%s]\n,"
            , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
           );
    streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
    fprintf(pcmd->cFile, "_STATE_NAMES[pfsm->state]\n);\n}\n");

    fprintf(pcmd->cFile, "#endif\n\n");

}

void printFSMSubMachineDebugBlock(pCMachineData pcmd, pMACHINE_INFO pmi)
{
    fprintf(pcmd->cFile, "\tDBG_PRINTF(\"");

    if (short_dbg_names && add_machine_name)
    {
       fprintf(pcmd->cFile
             "%s: "
               , pmi->name->name
           );
    }

    fprintf(pcmd->cFile, "event: %%s; state: %%s\"\n,");
    printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
    fprintf(pcmd->cFile, "%s_EVENT_NAMES[%s - THIS(%s)]\n,"
          , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
          , eventNameByIndex(pmi,0)
          );
    printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
    fprintf(pcmd->cFile, "_STATE_NAMES[pfsm->state]\n);\n}\n");

    fprintf(pcmd->cFile, "#endif\n\n");

}

