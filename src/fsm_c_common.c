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
static void print_entry_or_exit_fn_body                                              (pID_INFO,pITERATOR_CALLBACK_HELPER,ENTRY_OR_EXIT);
static bool print_event_cross_reference                                              (pLIST_ELEMENT,void*);
static bool print_sub_machine_event_cross_reference                                  (pLIST_ELEMENT,void*);
static void print_event_cross_reference_entry                                        (char*,pITERATOR_HELPER);
static void print_state_only_transition_fn_declaration_for_when_actions_return_states(pMACHINE_INFO,FILE*,char*);
static void print_transition_fn_declaration_for_when_actions_return_events           (pMACHINE_INFO,FILE*,char*);
static void print_state_only_transition_fn_declaration_for_when_actions_return_events(pMACHINE_INFO,FILE*,char*);
static bool print_event_enum_member                                                  (pLIST_ELEMENT,void*);
static bool write_state_enum_member                                                  (pLIST_ELEMENT,void*);
static void print_plain_enum_member                                                  (char*,pITERATOR_HELPER);
static void print_shared_event_data_block_signature                                  (FILE*,pMACHINE_INFO,char*,bool);
static char *createHeaderCompilationGuard                                            (char*);
static void writeHeaderPreamble                                                      (char*,FILE*);

int initCMachine(pFSMOutputGenerator pfsmog, char *fileName)
{
   pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

   if (
	   (pfsmcog->pcmd = newCMachineData(fileName))
	   && (pfsmcog->pcmd->pubHName = createFileName(fileName, ".h"))
	   )
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

static char *createHeaderCompilationGuard(char *fn)
{
	char *cp, *cp1;

	cp1 = getFileNameNoDir(fn);
	for (cp = cp1; *cp; cp++)

	   /* capitalise things, and change the '.' or '-' to '_' */
	   *cp = ((*cp == '.') || (*cp == '-')) ? '_' : (char)toupper(*cp);

	return cp1;
}

static void writeHeaderPreamble(char *fn, FILE *file)
{
	char *cp = createHeaderCompilationGuard(fn);

	if (cp)
	{
		fprintf(file
				, "/**\n\t%s\n\n"
				, fn
				);
		fprintf(file
				, "\tThis file automatically generated by FSMLang\n*/\n\n"
				);

		fprintf(file, "#ifndef _%s_\n", cp);
		fprintf(file, "#define _%s_\n\n", cp);

		free(cp);
	}
}

void writeCFilePreambles(pCMachineData pcmd)
{
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
	else if (
			 pcmd->pubHName
			 && !(pcmd->pubHFile = openFile(pcmd->pubHName, "w"))
			)
	{

		fprintf(stderr
				, "%s: unable to open %s\n"
				, me
				, pcmd->pubHName
				);

	   FCLOSE_AND_CLEAR(pcmd->cFile);
	   FCLOSE_AND_CLEAR(pcmd->hFile);

	   FREE_AND_CLEAR(pcmd->cName);
	   FREE_AND_CLEAR(pcmd->hName);
	   CHECK_AND_FREE(pcmd->pubHName);

	   FREE_AND_CLEAR(pcmd);

	}
	else
	{

	   /* the header file */
	   writeHeaderPreamble(pcmd->hName, pcmd->hFile);


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

	   /* if there is a public header */
	   if (pcmd->pubHName)
	   {
		   writeHeaderPreamble(pcmd->pubHName, pcmd->pubHFile);
		   fprintf(pcmd->hFile
				   , "#include \"%s\""
				   , pcmd->pubHName
				   );
	   }
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
              , pfsmcsmog->top_level_fsmcog->pcmd->hName
              );

	  /* for sub machines, the public file contents are private */
	  pfsmcsmog->pcmd->pubHFile = pfsmcsmog->pcmd->hFile;

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

void addEventCrossReference(pCMachineData pcmd, pMACHINE_INFO pmi, pITERATOR_HELPER pih)
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

   fprintf(pcmd->pubHFile
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


   fprintf(pcmd->pubHFile
           , "\n*/\n"
          );

}

void commonHeaderStart(pCMachineData pcmd, pMACHINE_INFO pmi, char *arrayName)
{
   unsigned				i;
   ITERATOR_HELPER   helper;

   /* put the native code segment out to the header */
   if (pmi->native) fprintf(pcmd->pubHFile, "%s\n", pmi->native);

   helper.first   = false;
   helper.fout    = pcmd->hFile;
   helper.pmi     = pmi;

   fprintf(pcmd->hFile, "\n#ifdef ");
   streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
   fprintf(pcmd->hFile, "_DEBUG\n");
   fprintf(pcmd->hFile, "#include <stdio.h>\n");
   fprintf(pcmd->hFile, "#include <stdlib.h>\n");
   fprintf(pcmd->hFile, "#endif\n\n");

   if (pmi->has_single_pai_events)
   {
      fprintf(pcmd->hFile, "#include <stdbool.h>\n");
   }

   fprintf(pcmd->hFile
           , "#define FSM_VERSION \"%s\"\n"
           , rev_string
           );

   if (core_logging_only)
   {
      fprintf(pcmd->hFile, "#ifndef NON_CORE_DEBUG_PRINTF\n#define NON_CORE_DEBUG_PRINTF(...) \n#endif\n\n");
   }

   /* put the "call the state machine" macro into the header */
   fprintf(pcmd->pubHFile, "\n#define RUN_STATE_MACHINE(A,B) \\\n");
   fprintf(pcmd->pubHFile
           , "\t((*(A)->fsm)((A),((%s"
           , pmi->data_block_count ? "p" : ""
           );
   streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
   fprintf(pcmd->pubHFile, "_EVENT) B)))\n\n");

   /* put the "declare a state machine" macro into the header */
   if (pmi->data)
   {
	   fprintf(pcmd->pubHFile
			   , "#ifndef INIT_FSM_DATA\n#define INIT_FSM_DATA {0}\n#endif\n"
			  );
   }

   fprintf(pcmd->pubHFile, "#define DECLARE_");
   streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
   fprintf(pcmd->pubHFile, "_MACHINE(A) \\\n");

   streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
   fprintf(pcmd->pubHFile
           , " (A) =\\\n{\\\n%s\t"
           , pmi->data ? "\tINIT_FSM_DATA,\\\n" : ""
		  );
   fprintf(pcmd->pubHFile
		   , "%s_%s,\\\n\t"
		   , pmi->name->name
           , stateNameByIndex(pmi, 0)
          );
   fprintf(pcmd->pubHFile
		   , "%s_noEvent,\\\n\t&"
		   , pmi->name->name
		   );
   fprintf(pcmd->pubHFile
		   , "%s_%s_array,\\\n\t"
		   , pmi->name->name
           , arrayName
		   );
   fprintf(pcmd->pubHFile
		   , "%sFSM\\\n};\\\n"
		   , pmi->name->name
		   );
   streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
   fprintf(pcmd->pubHFile, " *p##A = &A;\n\n");

   fprintf(pcmd->pubHFile
           , "/* Event naming convenience macros. */\n"
           );

   fprintf(pcmd->pubHFile, "#undef THIS\n#define THIS(A) ");
   fprintf(pcmd->pubHFile
		   , "%s_##A\n"
		   , pmi->name->name
		   );

   if (pmi->machine_list)
   {
	   helper.fout = pcmd->pubHFile;
      fprintf(pcmd->pubHFile, "#undef ");
	  streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
	  fprintf(pcmd->pubHFile, "\n#define ");
	  streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
	  fprintf(pcmd->pubHFile, "(A) ");
	  fprintf(pcmd->pubHFile
			  , "%s_##A\n"
			  , pmi->name->name
			  );

      iterate_list(pmi->machine_list, print_event_macro, &helper);

      fprintf(pcmd->pubHFile
              , "\n"
              );
	  helper.fout = pcmd->hFile;
   }

   if (add_event_cross_reference)
   {
	   helper.fout = pcmd->pubHFile;
      addEventCrossReference(pcmd, pmi, &helper);
	  helper.fout = pcmd->hFile;
   }

   /* put the event enum into the header file */
   fprintf(pcmd->pubHFile, "typedef enum ");
   streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
   fprintf(pcmd->pubHFile
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
   helper.fout = pcmd->pubHFile;
   iterate_list(pmi->event_list, print_event_enum_member, &helper);
   helper.fout = pcmd->hFile;

   if (
       !(pmi->modFlags & mfActionsReturnStates)
       && !(pmi->modFlags & mfActionsReturnVoid)
      )
   {
      fprintf(pcmd->pubHFile
			  , "\t, %s_noEvent\n"
			  , pmi->name->name
			  );
   }

   fprintf(pcmd->pubHFile
		   , "\t, %s_numEvents"
		   , pmi->name->name
		   );
   if (assignExternalEventValues(pmi))
   {
      fprintf(pcmd->pubHFile
			  , " = %u"
              , pmi->event_list->count
              );
   }
   fprintf(pcmd->pubHFile, "\n");

   if (pmi->machine_list)
   {
	   helper.fout = pcmd->pubHFile;
      iterate_list(pmi->machine_list,print_sub_machine_events,&helper);
	  helper.fout = pcmd->hFile;

      fprintf(pcmd->pubHFile
              , "\t, %s_numAllEvents\n"
              , pmi->name->name
              );
   }

   fprintf(pcmd->pubHFile
           , "}%s"
           , compact_action_array ? "__attribute__((__packed__)) " : " "
          );
   streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
   fprintf(pcmd->pubHFile
		   , "_EVENT%s;\n\n"
           , pmi->data_block_count ? "_ENUM" : ""
		  );

   fprintf(pcmd->hFile, "#undef ACTION_RETURN_TYPE\n#define ACTION_RETURN_TYPE ");
   streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
   fprintf(pcmd->hFile
		   , "_EVENT%s\n"
           , pmi->data_block_count ? "_ENUM" : ""
          );

   /* if we have user data on some events, declare the event structure */
   if (pmi->data_block_count)
   {
      iterate_list(pmi->event_list, declare_event_user_data_structs, &helper);

      /* create the union of pointers */
      fprintf(pcmd->pubHFile
              , "typedef union {\n"
              );

	  helper.fout = pcmd->pubHFile;
      iterate_list(pmi->event_list, declare_event_user_data_union_mbrs, &helper);
	  helper.fout = pcmd->hFile;

      fprintf(pcmd->pubHFile, "} ");
	  streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
	  fprintf(pcmd->pubHFile, "_EVENT_DATA, *p");
	  streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
	  fprintf(pcmd->pubHFile, "_EVENT_DATA;\n\n");

      /* create the event struct */
      fprintf(pcmd->pubHFile, "typedef struct {\n\t");
	  streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
	  fprintf(pcmd->pubHFile, "_EVENT_ENUM event;\n\t");
	  streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
	  fprintf(pcmd->pubHFile, "_EVENT_DATA event_data;\n} ");
	  streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
	  fprintf(pcmd->pubHFile, "_EVENT, *p");
	  streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
	  fprintf(pcmd->pubHFile, "_EVENT;\n\n");

   }

   if (generate_run_function)
   {
      fprintf(pcmd->pubHFile, "\nvoid run_%s(%s"
              , pmi->name->name
              , pmi->data_block_count ? "p" : ""
             );
	  streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
	  fprintf(pcmd->hFile, "_EVENT);\n\n");
   }

   fprintf(pcmd->hFile, "#ifdef ");
   streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
   fprintf(pcmd->hFile, "_DEBUG\n");
   fprintf(pcmd->hFile, "extern char *");
   streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
   fprintf(pcmd->hFile, "_EVENT_NAMES[];\n");
   fprintf(pcmd->hFile, "#endif\n\n");

   /* put the state enum into the header file */
   fprintf(pcmd->pubHFile
           , "typedef enum {\n"
          );

   helper.first = true;
   helper.pmi   = pmi;
   helper.fout = pcmd->pubHFile;
   iterate_list(pmi->state_list, write_state_enum_member, &helper);
   helper.fout = pcmd->hFile;
   

   /*
     Though not a state, this needs have a value
       different from any state. Moreover, it could
       be said to bear the same relation to states
       as noEvent does to events.
   */
   if (pmi->modFlags & mfActionsReturnStates)
   {
      fprintf(pcmd->pubHFile
              , "\t, %s_noTransition\n"
              , pmi->name->name
             );
   }

   fprintf(pcmd->pubHFile
           , "\t, %s_numStates\n"
           , pmi->name->name
           );

   fprintf(pcmd->pubHFile
           , "}%s"
           , compact_action_array ? " __attribute__((__packed__))" : " "
          );
   streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
   fprintf(pcmd->pubHFile, "_STATE;\n\n");

   fprintf(pcmd->hFile, "#ifdef ");
   streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
   fprintf(pcmd->hFile, "_DEBUG\n");
   fprintf(pcmd->hFile, "extern char *");
   streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
   fprintf(pcmd->hFile, "_STATE_NAMES[];\n");
   fprintf(pcmd->hFile, "#endif\n\n");

   /* put the data struct typedef into the header */
   if (pmi->data)
   {
      fprintf(pcmd->pubHFile
              , "typedef struct _%s_data_struct_ "
              , pmi->name->name
			 );
	  streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
	  fprintf(pcmd->pubHFile, "_DATA, *p");
	  streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
	  fprintf(pcmd->pubHFile, "_DATA;\n");
   }

   /* put the machine struct typedef into the header */
   fprintf(pcmd->pubHFile
		   , "typedef struct _%s_struct_ "
           , pmi->name->name
		  );
   streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
   fprintf(pcmd->pubHFile, ", *p");
   streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
   fprintf(pcmd->pubHFile, ";\n");

   fprintf(pcmd->hFile, "#undef FSM_TYPE_PTR\n#define FSM_TYPE_PTR p");
   streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
   fprintf(pcmd->hFile, "\n");

   if (generate_instance)
   {
      fprintf(pcmd->pubHFile, "extern ");
	  streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
	  fprintf(pcmd->pubHFile
			  , " %s;\n\n"
              , pmi->name->name
			 );

      fprintf(pcmd->pubHFile, "extern p");
	  streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
	  fprintf(pcmd->pubHFile
			  , " p%s;\n\n"
              , pmi->name->name
             );
   }

   /* put the action function typedef into the header */
   if (pmi->modFlags & mfActionsReturnStates)
   {
      fprintf(pcmd->pubHFile, "typedef ");
	  streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
	  fprintf(pcmd->pubHFile, "_STATE (*");
	  streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
	  fprintf(pcmd->pubHFile, "_ACTION_FN)(p");
	  streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
	  fprintf(pcmd->pubHFile, ");\n\n");
   }
   else if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmd->pubHFile, "typedef void (*");
	  streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
	  fprintf(pcmd->pubHFile, "_ACTION_FN)(p");
	  streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
	  fprintf(pcmd->pubHFile, ");\n\n");
   }
   else
   {
      fprintf(pcmd->pubHFile, "typedef ");
	  streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
	  fprintf(pcmd->pubHFile
			  , "_EVENT%s (*"
              , pmi->data_block_count ? "_ENUM"  : ""
             );
	  streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
	  fprintf(pcmd->pubHFile, "_ACTION_FN)(p");
	  streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
	  fprintf(pcmd->pubHFile, ");\n\n");
   }

   /* typedef transition functions, if we have any */
   if (pmi->transition_fn_list->count)
   {
      fprintf(pcmd->hFile, "typedef ");
	  streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
	  fprintf(pcmd->hFile, "_STATE (*");
	  streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
	  fprintf(pcmd->hFile, "_TRANSITION_FN)(p");
	  streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
	  fprintf(pcmd->hFile, ",");
	  streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
	  fprintf(pcmd->hFile
			  , "_EVENT%s);\n\n"
              , pmi->data_block_count ? "_ENUM"  : ""
             );
   }

   /* typedef the FSM function */
   fprintf(pcmd->pubHFile, "typedef void (*");
   streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
   fprintf(pcmd->pubHFile, "_FSM)(p");
   streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
   fprintf(pcmd->pubHFile
		   , ",%s"
           , pmi->data_block_count ? "p"  : ""
           );
   streamHungarianToUnderbarCaps(pcmd->pubHFile, pmi->name->name);
   fprintf(pcmd->pubHFile, "_EVENT);\n\n");

   /* declare the fsm function */
   fprintf(pcmd->hFile
           , "void %sFSM(p"
           , pmi->name->name
           );
   streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
   fprintf(pcmd->hFile
		   , ",%s"
           , pmi->data_block_count ? "p"  : ""
		  );
   streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
   fprintf(pcmd->hFile, "_EVENT);\n\n");

   if (pmi->machine_list)
   {
	   printSubMachinesDeclarations(pcmd,pmi);
   }

   /* put the data structure definition into the header */
   if (pmi->data)
   {
      fprintf(pcmd->pubHFile
              , "struct _%s_data_struct_ {\n"
              , pmi->name->name
              );

      helper.tab_level = 1; 
	  helper.fout = pcmd->pubHFile;
      iterate_list(pmi->data, print_data_field, &helper);
	  helper.fout = pcmd->hFile;

      fprintf(pcmd->pubHFile 
              , "};\n\n"
              );
   }

}

void commonHeaderEnd(pCMachineData pcmd, pMACHINE_INFO pmi, bool needNoOp)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmd      = pcmd;
   ich.pmi       = pmi;
   ich.needNoOp  = needNoOp;

   /* declare the action functions themselves */
   iterate_list(pmi->action_list, declare_action_function, &ich);

   /* declare the dummy, or no op action */
   if (needNoOp)
   {
	   print_action_function_declaration(pmi, pcmd->hFile, "noAction");
   }

   fprintf(pcmd->hFile
           , "\n"
          );

   /* declare any machine transition function */
   if (pmi->machineTransition)
   {
	   fprintf(pcmd->hFile, "void ");
	   printAncestry(pmi, pcmd->hFile, "_", alc_lower, ai_include_self);
	   fprintf(pcmd->hFile, "_%s(p"
			   , pmi->machineTransition->name
			  );
	   printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
	   fprintf(pcmd->hFile, ",");
	   streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
	   fprintf(pcmd->hFile, "_STATE);\n\n");
   }

   /* declare any transition functions */
   if (pmi->modFlags & mfActionsReturnStates)
   {
	   print_transition_fn_declaration_for_when_actions_return_states(pmi,pcmd->hFile, "noTransition");

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

      fprintf(pcmd->hFile, "\n");

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

      fprintf(pcmd->hFile, "\n");

      if (pmi->transition_fn_list->count)
      {
		  print_transition_fn_declaration_for_when_actions_return_events(pmi
																		 , pcmd->hFile
																		 , "noTransitionFn"
																		 );
		  fprintf(pcmd->cFile, "\n");
      }
   }

   /* declare any entry or exit functions */
   if (pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
   {
      iterate_list(pmi->state_list, declare_state_entry_and_exit_functions, &ich);
      fprintf(pcmd->hFile, "\n");
   }

   /* declare needed data translators */
   if (pmi->data_block_count)
   {
      iterate_list(pmi->event_list, declare_data_translator_functions, &ich);
      fprintf(pcmd->hFile, "\n");
   }

}

void generateRunFunction(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   fprintf(pcmd->cFile
           ,"void run_%s(%s"
           , pmi->name->name
           , pmi->data_block_count ? "p" : ""
           );
   streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
   fprintf(pcmd->cFile, "_EVENT e)\n{\n");

   fprintf(pcmd->cFile
           , "\tif (p%s)\n\t{\n\t\tRUN_STATE_MACHINE(p%s,e);\n\t}\n}\n\n"
           , pmi->name->name
           , pmi->name->name
           );
}

void generateInstance(pCMachineData pcmd, pMACHINE_INFO pmi, char *arrayName)
{
   /* instantiate the machine and the pointer to it */
   /* the (empty) data struct and the state */
	printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->cFile, " ");
   printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->cFile, " = {");
   fprintf(pcmd->cFile
		   , "%s\n\t"
           , pmi->data ? "\n\tINIT_FSM_DATA," : ""
		   );
   printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->cFile
		   , "_%s,\n"
		   , stateNameByIndex(pmi, 0)
           );

   fprintf(pcmd->cFile, "\t");
   printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->cFile
		   , "_%s,\n"
           , eventNameByIndex(pmi, 0)
          );

   fprintf(pcmd->cFile, "\t&");
   printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->cFile
		   , "_%s_array,\n"
           , arrayName
          );

   if (pmi->machine_list)
   {
      fprintf(pcmd->cFile, "\t&");
	  printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
      fprintf(pcmd->cFile, "_sub_fsm_if_array,\n");
   }

   fprintf(pcmd->cFile, "\t");
   printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->cFile, "FSM\n};\n\n");

   fprintf(pcmd->cFile, "p");
   printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->cFile, " p");
   printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->cFile, " = &");
   printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->cFile, ";\n\n");

}

void defineWeakActionFunctionStubs(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmd      = pcmd;
   ich.pmi       = pmi;

   iterate_list(pmi->action_list, define_weak_action_function, &ich);
}

void defineWeakNoActionFunctionStubs(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmd->cFile, "void __attribute__((weak)) ");
	  printNameWithAncestry("noAction", pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
	  fprintf(pcmd->cFile, "(p");
	  printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmd->cFile, " pfsm)\n");
      fprintf(pcmd->cFile
              , "{\n\t%s(\"weak: "
              , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
              );
	  printNameWithAncestry("noAction", pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
	  fprintf(pcmd->cFile, "\");\n");
      fprintf(pcmd->cFile , "\t(void) pfsm;\n}\n\n");
   }
   else
   {
      if (pmi->modFlags & mfActionsReturnStates)
      {
		  streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
		  fprintf(pcmd->cFile, "_STATE");
      }
      else
      {
		 streamHungarianToUnderbarCaps(pcmd->cFile, ultimateAncestor(pmi)->name->name);
         fprintf(pcmd->cFile
                 , "_EVENT%s"
                 , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
                );
      }

	  fprintf(pcmd->cFile, " __attribute__((weak)) ");
	  printNameWithAncestry("noAction", pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
	  fprintf(pcmd->cFile, "(p");
	  printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmd->cFile, " pfsm)\n");
	  fprintf(pcmd->cFile
			  , "{\n\t%s(\"weak: "
			  , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
			  );
	  printNameWithAncestry("noAction", pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
	  fprintf(pcmd->cFile, "\");\n");
	  fprintf(pcmd->cFile, "\t(void) pfsm;\n");
	  fprintf(pcmd->cFile
			  , "\treturn THIS(%s);\n}\n\n"
			  , pmi->modFlags & mfActionsReturnStates
				? "noTransition"
				: "noEvent"
			  );
   }

}

void defineWeakStateEntryAndExitFunctionStubs(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   ITERATOR_CALLBACK_HELPER ich;

   ich.pmi    = pmi;
   ich.pcmd   = pcmd;

   if (pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
   {
      iterate_list(pmi->state_list, define_state_entry_and_exit_functions, &ich);
   }

}

static bool define_transition_function_from_action_array(pLIST_ELEMENT pelem, void *data)
{
	pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER)data;
	pID_INFO pstate                = (pID_INFO)pelem->mbr;

	unsigned event_ordinal         = pich->pOtherElem->ordinal;
	unsigned state_ordinal         = pelem->ordinal;

	pACTION_INFO pai               = pich->pmi->actionArray[event_ordinal][state_ordinal];

	if (pai)
	{
		if (
			(!strlen(pai->action->name))
			&& (pai->transition->type == STATE)
		   )
		{
		   fprintf(pich->pcmd->cFile, "\n");
		   streamHungarianToUnderbarCaps(pich->pcmd->cFile, pich->pmi->name->name);
		   fprintf(pich->pcmd->cFile, "_STATE __attribute__((weak)) ");
		   printAncestry(pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
		   fprintf(pich->pcmd->cFile
				   , "_transitionTo%s(p"
				   , pai->transition->name
				  );
		   printAncestry(pich->pmi, pich->pcmd->cFile, "_", alc_upper, ai_include_self);
		   fprintf(pich->pcmd->cFile, " pfsm)\n{\n");
		   fprintf(pich->pcmd->cFile
				   , "\t(void) pfsm;\n\n\t%s(\"weak: "
				   , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
				  );
		   printAncestry(pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
		   fprintf(pich->pcmd->cFile
				   , "_transitionTo%s\");\n\treturn "
				   , pai->transition->name
				   );
		   printNameWithAncestry(pai->transition->name, pich->pmi, pich->pcmd->cFile, "_", alc_upper, ai_include_self);
		   fprintf(pich->pcmd->cFile, ";\n}\n");
		}
	}

	return false;
}

static bool define_action_array_transition_functions(pLIST_ELEMENT pelem, void *data)
{
	pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER)data;

	pich->pOtherElem = pelem;

	iterate_list(pich->pmi->state_list, define_transition_function_from_action_array, pich);

	return false;
}

static bool define_transition_list_functions(pLIST_ELEMENT pelem, void *data)
{
	pITERATOR_CALLBACK_HELPER pich        = (pITERATOR_CALLBACK_HELPER) data;
	pID_INFO                  ptransition = (pID_INFO) pelem->mbr;

	if (ptransition->type == STATE)
	{
	   fprintf(pich->pcmd->cFile, "\n");
	   streamHungarianToUnderbarCaps(pich->pcmd->cFile, pich->pmi->name->name);
	   fprintf(pich->pcmd->cFile, "_STATE __attribute__((weak)) ");
	   printAncestry(pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
	   fprintf(pich->pcmd->cFile
			   , "_transitionTo%s(p"
			   , ptransition->name
			  );
	   printAncestry(pich->pmi, pich->pcmd->cFile, "_", alc_upper, ai_include_self);
	   fprintf(pich->pcmd->cFile, " pfsm,");
	   streamHungarianToUnderbarCaps(pich->pcmd->cFile, ultimateAncestor(pich->pmi)->name->name);
	   fprintf(pich->pcmd->cFile
			   , "_EVENT%s e)\n{\n"
			   , ultimateAncestor(pich->pmi)->data_block_count ? "_ENUM"  : ""
			  );
	   fprintf(pich->pcmd->cFile
			   , "\t(void) pfsm;\n\t(void) e;\n\t%s(\"weak: "
			   , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
			  );
	   printAncestry(pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
	   fprintf(pich->pcmd->cFile
			   , "_transitionTo%s\");\n\treturn "
			   , ptransition->name
			  );
	   printNameWithAncestry(ptransition->name, pich->pmi, pich->pcmd->cFile, "_", alc_upper, ai_include_self);
	   fprintf(pich->pcmd->cFile, ";\n}\n");
	}
}

void writeStateTransitions(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	ITERATOR_CALLBACK_HELPER ich = {
		.pmi = pmi
		, .pcmd = pcmd
	};

   if (pmi->modFlags & mfActionsReturnStates)
   {
      /* grab state transitions from action array */
	  iterate_list(pmi->event_list, define_action_array_transition_functions, &ich);
   }
   else
   {
	   iterate_list(pmi->transition_list, define_transition_list_functions, &ich);
   }
}

static bool print_quoted_pid_name_with_ancestry_as_list_element(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pid = (pID_INFO) pelem->mbr;
	pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

	fprintf(pih->fout
			,"%s\""
			, pih->first ? (pih->first = false, "\t ") : "\t,"
			);

	printNameWithAncestry(pid->name, pih->pmi, pih->fout, "_", alc_lower, ai_include_self);

	fprintf(pih->fout ,"\"\n");

	return false;
}

static bool print_quoted_pid_name_as_list_element(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pid = (pID_INFO) pelem->mbr;
	pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

	fprintf(pih->fout
			,"%s\"%s\"\n"
			, pih->first ? (pih->first = false, "\t ") : "\t,"
			, pid->name
			);

	return false;
}

/**
 * @brief Writes machine debug info in a short way - without name prefix
 * 
 * @param pcmd 
 * @param pmi 
 * @param cp 
 */
void writeDebugInfoShort(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   unsigned i;
   ITERATOR_HELPER helper;

   fprintf(pcmd->cFile, "\n#ifdef ");
   printNameWithAncestry("DEBUG\n", pmi, pcmd->cFile, "_", alc_upper, ai_include_self);

   fprintf(pcmd->cFile, "char *");
   printNameWithAncestry("EVENT_NAMES[] = {\n", pmi, pcmd->cFile, "_", alc_upper, ai_include_self);

   helper.fout  = pcmd->cFile;
   helper.first = true;
   iterate_list(pmi->event_list, print_quoted_pid_name_as_list_element, &helper);

   if (!(pmi->modFlags & mfActionsReturnStates) && !(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmd->cFile, "\t, \"noEvent\"\n");
   }

   fprintf(pcmd->cFile, "\t, \"numEvents\"\n");

   if (pmi->machine_list)
   {
      iterate_list(pmi->machine_list,print_sub_machine_event_names,&helper);
   }

   fprintf(pcmd->cFile, "};\n\n");

   fprintf(pcmd->cFile, "char *");
   streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
   fprintf(pcmd->cFile, "STATE_NAMES[] = {\n");

   helper.first = true;
   iterate_list(pmi->state_list, print_quoted_pid_name_as_list_element, &helper);

   if (pmi->modFlags & mfActionsReturnStates)
   {
      fprintf(pcmd->cFile, "\t,\"noTransition\"\n");
   }

   fprintf(pcmd->cFile, "};\n\n");

   fprintf(pcmd->cFile, "#endif\n");
}

/**
 * @brief Writes machine debug information with machine name prefix
 * 
 * @param pcmd 
 * @param pmi 
 * @param cp 
 */
void writeDebugInfoLong(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   unsigned i;
   ITERATOR_HELPER helper;

   fprintf(pcmd->cFile, "\n#ifdef ");
   printNameWithAncestry("DEBUG\n", pmi, pcmd->cFile, "_", alc_upper, ai_include_self);

   fprintf(pcmd->cFile, "char *");
   printNameWithAncestry("EVENT_NAMES[] = {\n", pmi, pcmd->cFile, "_", alc_upper, ai_include_self);

   helper.pmi   = pmi;
   helper.fout  = pcmd->cFile;
   helper.first = true;

   iterate_list(pmi->event_list, print_quoted_pid_name_with_ancestry_as_list_element, &helper);

   if (!(pmi->modFlags & mfActionsReturnStates) && !(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmd->cFile
			  , "\t, \"%s_noEvent\"\n"
			  , pmi->name->name
			  );
   }

   fprintf(pcmd->cFile
		   , "\t, \"%s_numEvents\"\n"
		   , pmi->name->name
		   );

   if (pmi->machine_list)
   {
      helper.first   = false;
      iterate_list(pmi->machine_list,print_sub_machine_event_names,&helper);
   }

   fprintf(pcmd->cFile, "};\n\n");

   fprintf(pcmd->cFile, "char *");
   streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
   fprintf(pcmd->cFile, "STATE_NAMES[] = {\n");

   helper.first = true;

   iterate_list(pmi->state_list, print_quoted_pid_name_with_ancestry_as_list_element, &helper);

   if (pmi->modFlags & mfActionsReturnStates)
   {
	   printNameWithAncestry("noTransition", pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
   }

   fprintf(pcmd->cFile, "};\n\n");

   fprintf(pcmd->cFile, "#endif\n");
}

void writeDebugInfo(pCMachineData pcmd, pMACHINE_INFO pmi)
{
    short_dbg_names ? writeDebugInfoShort(pcmd, pmi) : writeDebugInfoLong(pcmd, pmi);
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

   pCMachineData	pcmd;

   if ((pcmd = calloc(sizeof(CMachineData), 1)) != NULL)
   {

      if (!baseFileName)
      {

         pcmd->cFile = pcmd->hFile = stdout;

      }
      else
      {

         pcmd->cName  = createFileName(baseFileName, ".c");
         pcmd->hName  = createFileName(baseFileName, "_priv.h");

      }

   }

   return pcmd;

}

void destroyCMachineData(pCMachineData pcmd, int good)
{

   if (!pcmd) return;

   if (good)
   {

      /* close the #ifndef on the header file(s) */
      fprintf(pcmd->hFile, "\n#endif\n");

	  if (pcmd->pubHFile && (pcmd->pubHFile != pcmd->hFile))
	  {
		  fprintf(pcmd->pubHFile, "\n#endif\n");
	  }

   }

   if (pcmd->pubHFile != pcmd->hFile)
   {
	   fclose(pcmd->pubHFile);
   }
   fclose(pcmd->hFile);
   fclose(pcmd->cFile);

   if (!good)
   {

      unlink(pcmd->hName);
      unlink(pcmd->cName);

   }

   CHECK_AND_FREE(pcmd->pubHName);
   CHECK_AND_FREE(pcmd->hName);
   CHECK_AND_FREE(pcmd->cName);

   free(pcmd);

}

bool assignExternalEventValues(pMACHINE_INFO pmi)
{
   return (
           (pmi->modFlags & (mfActionsReturnStates | mfActionsReturnVoid))
           && (pmi->event_list->count == pmi->external_event_designation_count)
           && !compact_action_array
          );

}

static void print_plain_enum_member(char *name, pITERATOR_HELPER pih)
{
	fprintf(pih->fout
			, "\t%s%s_%s\n"
			, pih->first ? (pih->first = false, "") : ", "
			, pih->pmi->name->name
			, name
			);
}

static bool write_state_enum_member(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO state       = (pID_INFO)         pelem->mbr;
	pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

	print_plain_enum_member(state->name, pih);

	return false;
}

static bool print_event_enum_member(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO event       = (pID_INFO)         pelem->mbr;
	pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

	fprintf(pih->fout
			, pih->first ? (pih->first = false, "\t %s") : "\t, %s"
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

   if (pevent->type_data.event_data.puser_event_data)
   {
      if (pevent->type_data.event_data.puser_event_data->data_fields)
      {
         print_tab_levels(pih->fout, pih->tab_level);
		 printAncestry(pih->pmi, pih->fout, "_", alc_upper, ai_include_self);
		 streamHungarianToUnderbarCaps(pih->fout, pevent->name);
         fprintf(pih->fout
                 , "_DATA %s_data;\n"
                 , pevent->name
                 );
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
   fprintf(pih->fout, "_e\n");

   if (pih->first)
   {
      pih->first = false;

      fprintf(pih->fout, "\t, ");
	  printAncestry(pmi, pih->fout, "_", alc_lower, ai_omit_self);
	  fprintf(pih->fout, "_firstSubMachine = ");
	  printAncestry(pmi, pih->fout, "_", alc_lower, ai_include_self);
	  fprintf(pih->fout, "_e\n");

   }

   return false;
}

static bool declare_sub_machine_if(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO pmi    = (pMACHINE_INFO) pelem->mbr;
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   printAncestry(pih->pmi, pih->fout, "_", alc_upper, ai_include_self);
   fprintf(pih->fout, "_SUB_FSM_IF ");
   printAncestry(pih->pmi, pih->fout, "_", alc_lower, ai_include_self);
   fprintf(pih->fout, "_sub_fsm_if;\n");

   return false;
}

static void declare_parent_event_reference_data_structures(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   ITERATOR_HELPER ih;

   fprintf(pcmd->hFile
           ,"/* Some sub-machines share parent events. */\n"
           );

   fprintf(pcmd->hFile, "typedef void (*");
   streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
   fprintf(pcmd->hFile, "_DATA_TRANSLATION_FN)(p");
   printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->hFile, ");\n");

   fprintf(pcmd->hFile,"typedef struct _");
   streamStrCaseAware(pcmd->hFile, pmi->name->name, alc_lower);
   fprintf(pcmd->hFile, "_shared_event_str_ ");
   streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
   fprintf(pcmd->hFile, "_SHARED_EVENT_STR, *p");
   streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
   fprintf(pcmd->hFile, "_SHARED_EVENT_STR;\n");

   fprintf(pcmd->hFile, "struct _");
   streamStrCaseAware(pcmd->hFile, pmi->name->name, alc_lower);
   fprintf(pcmd->hFile, "_shared_event_str_\n{");

   fprintf(pcmd->hFile, "\t");
   printAncestry(ultimateAncestor(pmi), pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->hFile
		   , "_EVENT%s      event;\n"
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
           );

   fprintf(pcmd->hFile, "\t");
   streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
   fprintf(pcmd->hFile, "_DATA_TRANSLATION_FN  data_translation_fn;\n");

   fprintf(pcmd->hFile, "\tp");
   printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->hFile, "_SUB_FSM_IF          psub_fsm_if;\n");

   fprintf(pcmd->hFile
           , "};\n"
           );

   fprintf(pcmd->hFile, "extern ");
   printAncestry(ultimateAncestor(pmi), pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->hFile
		   , "_EVENT%s "
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
           );
   streamStrCaseAware(pcmd->hFile, pmi->name->name, alc_lower);
   fprintf(pcmd->hFile, "_pass_shared_event(p");
   printAncestry(ultimateAncestor(pmi), pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->hFile, ",p");
   printAncestry(ultimateAncestor(pmi), pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->hFile, "_SHARED_EVENT_STR[]);\n\n");

   ih.fout = pcmd->hFile;
   ih.pmi  = pmi;

   iterate_list(pmi->event_list,declare_shared_event_lists,&ih);

   fprintf(pcmd->hFile
           , "\n"
           );
}

static bool define_shared_event_lists(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO pevent      = (pID_INFO)pelem->mbr;
	pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

#ifdef FSMLANG_DEVELOP
	fprintf(pih->fout
			, "/* define_shared_event_lists */\n"
		   );
#endif

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
      fprintf(pich->pcmd->cFile, "\t\tcase %s_%s:\n"
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

#ifdef FSMLANG_DEVELOP
	fprintf(pih->fout
			, "/* reference_shared_event_data_blocks */\n"
		   );
#endif
   fprintf(pih->fout
           , "\t%s&"
           , pih->first ? (pih->first = false, "  ") : ", "
           );

   print_shared_event_data_block_signature(pih->fout, pmi, pih->pid->name, false /* do not include type information */);

   fprintf(pih->fout, "\n");

   return false;
}

static void define_parent_event_reference_elements(pCMachineData pcmd, pMACHINE_INFO pmi)
{
#ifdef FSMLANG_DEVELOP
	fprintf(pcmd->cFile
			, "/* define_parent_event_reference_elements */\n"
		   );
#endif
   ITERATOR_HELPER ih;

   ih.pmi   = pmi;
   ih.fout  = pcmd->cFile;
   ih.first = true;

   /* define arrays */
   iterate_list(pmi->event_list, define_shared_event_lists, &ih);

   /* passing function */
   streamHungarianToUnderbarCaps(pcmd->cFile, ultimateAncestor(pmi)->name->name);
   fprintf(pcmd->cFile
           , "_EVENT%s %s_pass_shared_event(p"
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
		   , pmi->name->name
           );
   streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
   fprintf(pcmd->cFile, " pfsm, p");
   streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
   fprintf(pcmd->cFile, "_SHARED_EVENT_STR sharer_list[])\n{\n");

   fprintf(pcmd->cFile, "\t");
   streamHungarianToUnderbarCaps(pcmd->cFile, ultimateAncestor(pmi)->name->name);
   fprintf(pcmd->cFile
		   , "_EVENT%s return_event = THIS(noEvent);\n"
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
           );

   fprintf(pcmd->cFile, "\tfor (p");
   streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
   fprintf(pcmd->cFile
		   , "_SHARED_EVENT_STR *pcurrent_sharer = sharer_list;\n\t     *pcurrent_sharer && return_event == THIS(noEvent);\n\t     pcurrent_sharer++)\n\t{\n"
           );

   fprintf(pcmd->cFile
           , "\t\tif ((*pcurrent_sharer)->data_translation_fn)\n"
           );

   fprintf(pcmd->cFile
           , "\t\t\t(*(*pcurrent_sharer)->data_translation_fn)(pfsm);\n"
           );

   fprintf(pcmd->cFile
           , "\t\treturn_event = (*(*pcurrent_sharer)->psub_fsm_if->subFSM)((*pcurrent_sharer)->event);\n"
           );

   fprintf(pcmd->cFile
           , "\t}\n\n"
           );

   fprintf(pcmd->cFile
           , "\treturn return_event;\n}\n\n"
           );

}

bool declare_transition_fn_for_when_actions_return_states(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   print_transition_fn_declaration_for_when_actions_return_states(pich->pmi, pich->pcmd->hFile, pid_info->name);

   return false;
}

bool declare_state_only_transition_functions_for_when_actions_return_states(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   print_state_only_transition_fn_declaration_for_when_actions_return_states(pich->pmi, pich->pcmd->hFile, pid_info->name);

   return false;
}

static void print_transition_fn_declaration_for_when_actions_return_events(pMACHINE_INFO pmi, FILE *fout, char *name)
{
	streamHungarianToUnderbarCaps(fout, pmi->name->name);
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
	streamHungarianToUnderbarCaps(fout, pmi->name->name);
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

   print_transition_fn_declaration_for_when_actions_return_events(pich->pmi, pich->pcmd->hFile, pid_info->name);

   return false;
}

bool declare_state_only_transition_functions_for_when_actions_return_events(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   print_state_only_transition_fn_declaration_for_when_actions_return_events(pich->pmi, pich->pcmd->hFile, pid_info->name);

   return false;
}

static void print_entry_or_exit_fn_signature(pID_INFO pstate, pITERATOR_CALLBACK_HELPER pich, ENTRY_OR_EXIT which)
{
   pID_INFO fn        = which == eoe_entry ? pstate->type_data.state_data.entry_fn : pstate->type_data.state_data.exit_fn;
   char    *no_fn_str = which == eoe_entry ? "onEntryTo_"                          : "onExitFrom_";
   FILE    *file      = pich->define       ? pich->pcmd->cFile                     : pich->pcmd->hFile;
    
   fprintf(file
           , "void %s"
           , pich->define                      ? "__attribute__((weak)) " : ""
          );
   printAncestry(pich->pmi, file, "_", alc_lower, ai_include_self);
   fprintf(file
           , "_%s%s(%s"
           , fn                                ? fn->name                 : no_fn_str
           , fn                                ? ""                       : pstate->name
           , pich->pmi->data                   ? "p"                      : "void"
		   );
   printAncestry(pich->pmi, file, "_", alc_upper, ai_include_self);
   fprintf(file
		   , "%s%s)%s\n"
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

static void print_entry_or_exit_fn_body(pID_INFO pstate, pITERATOR_CALLBACK_HELPER pich, ENTRY_OR_EXIT which)
{
	pID_INFO fn        = which == eoe_entry ? pstate->type_data.state_data.entry_fn : pstate->type_data.state_data.exit_fn;
	char    *no_fn_str = which == eoe_entry ? "onEntryTo_"                          : "onExitFrom_";

	fprintf(pich->pcmd->cFile
			,"{\n%s\tDBG_PRINTF(\"weak: "
			, pich->pmi->data
			  ? "\t(void) pdata;\n\n"
			  : ""
			);
	printNameWithAncestry(fn ? fn->name : no_fn_str, pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
	fprintf(pich->pcmd->cFile
			, "%s\");\n}\n"
			, pstate->type_data.state_data.entry_fn
			  ? ""
			  : pstate->name
			);

	fprintf(pich->pcmd->cFile, "\n");
}

bool define_state_entry_and_exit_functions(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pstate                = ((pID_INFO)pelem->mbr);

   //we are defining
   pich->define = true;

   if (pstate->type_data.state_data.state_flags & sfHasEntryFn)
   {
	  print_entry_or_exit_fn_signature(pstate, pich, eoe_entry);
	  print_entry_or_exit_fn_body(pstate, pich, eoe_entry);

   }

   if (pstate->type_data.state_data.state_flags & sfHasExitFn)
   {
      print_entry_or_exit_fn_signature(pstate, pich, eoe_exit);
	  print_entry_or_exit_fn_body(pstate, pich, eoe_exit);
   }

   return false;
}

bool declare_data_translator_functions(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pevent                = ((pID_INFO)pelem->mbr);

   if (pevent->type_data.event_data.puser_event_data)
   {
	  fprintf(pich->pcmd->hFile, "void ");
	  printAncestry(pich->pmi, pich->pcmd->hFile, "_", alc_lower, ai_include_self);

      if (pevent->type_data.event_data.puser_event_data->translator)
      {
         fprintf(pich->pcmd->hFile
                 , "_%s(p"
                 , pevent->type_data.event_data.puser_event_data->translator->name
				 );
      }
      else
      {
		  fprintf(pich->pcmd->hFile
				  , "_translate_%s_data(p"
				  , pevent->name
				  );
      }
	  printAncestry(pich->pmi, pich->pcmd->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pich->pcmd->hFile, "_DATA,p");
	  printAncestry(pich->pmi, pich->pcmd->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pich->pcmd->hFile, "_");
	  streamHungarianToUnderbarCaps(pich->pcmd->hFile, pevent->name);
	  fprintf(pich->pcmd->hFile, "_DATA);\n");
   }

   return false;
}

bool define_weak_data_translator_functions(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pevent                = ((pID_INFO)pelem->mbr);

   if (pevent->type_data.event_data.puser_event_data)
   {
      fprintf(pich->pcmd->cFile, "void __attribute__((weak)) ");
      if (pevent->type_data.event_data.puser_event_data->translator)
      {
          printNameWithAncestry(pevent->type_data.event_data.puser_event_data->translator->name, pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
          fprintf(pich->pcmd->cFile, "(p");
          printNameWithAncestry("DATA", pich->pmi, pich->pcmd->cFile, "_", alc_upper, ai_include_self);
          fprintf(pich->pcmd->cFile, " pfsm_data, p");
          printAncestry(pich->pmi, pich->pcmd->cFile, " ", alc_upper, ai_include_self);
          streamHungarianToUnderbarCaps(pich->pcmd->cFile, pevent->name);
          fprintf(pich->pcmd->cFile, " pdata)\n{\n\t(void) pfsm_data;\n\t(void) pdata;\n\t%s(\"weak: "
                  , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                 );
          printNameWithAncestry(pevent->type_data.event_data.puser_event_data->translator->name, pich->pmi, pich->pcmd->cFile, " ", alc_lower, ai_include_self);
      }
      else
      {
         printNameWithAncestry("translate", pich->pmi, pich->pcmd->cFile, " ", alc_lower, ai_include_self);
         fprintf(pich->pcmd->cFile
                 , "_%s_data(p"
                 , pevent->name
                );
         printNameWithAncestry("DATA", pich->pmi, pich->pcmd->cFile, " ", alc_upper, ai_include_self);
         fprintf(pich->pcmd->cFile, " pfsm_data, p");
         printAncestry(pich->pmi, pich->pcmd->cFile, " ", alc_upper, ai_include_self);
         streamHungarianToUnderbarCaps(pich->pcmd->cFile, pevent->name);
         fprintf(pich->pcmd->cFile, " pdata)\n{\n\t(void) pfsm_data;\n\t(void) pdata;\n\t%s(\"weak: "
                 , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                );
         printNameWithAncestry("translate", pich->pmi, pich->pcmd->cFile, " ", alc_lower, ai_include_self);
         fprintf(pich->pcmd->cFile
                 , "_%s_data(p"
                 , pevent->name
                );
      }
      fprintf(pich->pcmd->cFile, "\");\n}\n\n");
   }

   return false;
}

void subMachineHeaderStart(pCMachineData pcmd, pMACHINE_INFO pmi, char *arrayName)
{
   unsigned         i;
   ITERATOR_HELPER  helper;

   helper.fout      = pcmd->hFile;
   helper.pmi       = pmi;

   /* put the native code segment out to the header */
   if (pmi->native) fprintf(pcmd->hFile, "%s\n", pmi->native);

   fprintf(pcmd->hFile, "#ifdef ");
   printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->hFile, "_DEBUG\n");
   fprintf(pcmd->hFile, "#include <stdio.h>\n");
   fprintf(pcmd->hFile, "#include <stdlib.h>\n");
   fprintf(pcmd->hFile, "#endif\n\n");

   fprintf(pcmd->hFile
           , "/*\n\tsub-machine events are included in the top-level machine event enumeration.\n"
           );

   fprintf(pcmd->hFile
           , "\tThese macros set the appropriate names for events from THIS machine\n"
           );

   fprintf(pcmd->hFile
           , "\tand those from the PARENT machine.\n"
           );

   fprintf(pcmd->hFile
           , "\n\tThey may be turned off as needed.\n*/\n"
           );

   fprintf(pcmd->hFile
           , "#ifndef NO_EVENT_CONVENIENCE_MACROS\n"
           );

   fprintf(pcmd->hFile, "#undef THIS\n#define THIS(A) ");
   printAncestry(pmi, pcmd->hFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->hFile, "_##A\n");

   fprintf(pcmd->hFile, "#undef PARENT\n#define PARENT(A) ");
   printAncestry(pmi, pcmd->hFile, "_", alc_lower, ai_omit_self);
   fprintf(pcmd->hFile, "_##A\n");

   fprintf(pcmd->hFile
           , "#endif\n"
           );

   fprintf(pcmd->hFile, "\n#ifdef ");
   printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->hFile, "_DEBUG\n");
   fprintf(pcmd->hFile, "extern char *");
   printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->hFile, "_EVENT_NAMES[];\n");
   fprintf(pcmd->hFile, "#endif\n\n");

   /* put the state enum into the header file */
   fprintf(pcmd->hFile
           , "typedef enum {\n"
          );


   helper.first = true;
   iterate_list(pmi->state_list, write_state_enum_member, &helper);

   /*
     Though not a state, this needs have a value
       different from any state. Moreover, it could
       be said to bear the same relation to states
       as noEvent does to events.
   */
   if (pmi->modFlags & mfActionsReturnStates)
   {
	   print_plain_enum_member("noTransition", &helper);
   }

   print_plain_enum_member("numStates", &helper);

   fprintf(pcmd->hFile
           , "}%s"
           , compact_action_array ? " __attribute__((__packed__))" : " "
          );
   streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
   fprintf(pcmd->hFile, "_STATE;\n\n");

   fprintf(pcmd->hFile, "#ifdef ");
   printNameWithAncestry("DEBUG\n", pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->hFile, "extern char *");
   streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
   fprintf(pcmd->hFile, "_STATE_NAMES[];\n\n");
   fprintf(pcmd->hFile, "#endif\n\n");

   /* put the data struct typedef into the header */
   if (pmi->data)
   {
      fprintf(pcmd->hFile, "typedef struct _");
	  printAncestry(pmi, pcmd->hFile, "_", alc_lower, ai_include_self);
	  fprintf(pcmd->hFile, "_data_struct_ ");
	  printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmd->hFile, "_DATA, *p");
	  printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmd->hFile, "_DATA;\n");
   }

   /* put the machine struct typedef into the header */
   fprintf(pcmd->hFile, "typedef struct _");
   printAncestry(pmi, pcmd->hFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->hFile, "_struct_ ");
   printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->hFile, ", *p");
   printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->hFile, ";\n");

   fprintf(pcmd->hFile, "#undef FSM_TYPE_PTR\n#define FSM_TYPE_PTR p");
   printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->hFile, "\n");

   if (generate_instance)
   {
      fprintf(pcmd->hFile, "extern ");
	  printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmd->hFile, " ");
	  printAncestry(pmi, pcmd->hFile, "_", alc_lower, ai_include_self);
	  fprintf(pcmd->hFile, ";\n\n");

      fprintf(pcmd->hFile, "extern p");
	  printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmd->hFile, " p");
	  printAncestry(pmi, pcmd->hFile, "_", alc_lower, ai_include_self);
	  fprintf(pcmd->hFile, ";\n\n");
   }

   /* put the action function typedef into the header */
   fprintf(pcmd->hFile, "typedef ");
   if (pmi->modFlags & mfActionsReturnStates)
   {
	  streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
	  fprintf(pcmd->hFile, "_STATE (*");
	  printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmd->hFile, "_ACTION_FN)(p");
	  printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmd->hFile, ");\n\n");
   }
   else if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmd->hFile, "void (*");
	  printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmd->hFile, "_ACTION_FN)(p");
	  printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
	  fprintf(pcmd->hFile, ");\n\n");
   }
   else
   {
	  streamHungarianToUnderbarCaps(pcmd->hFile, ultimateAncestor(pmi)->name->name);
      fprintf(pcmd->hFile
			  , "_EVENT%s (*"
			  , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
			  );
	  printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
      fprintf(pcmd->hFile, "_ACTION_FN)(p");
	  printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
      fprintf(pcmd->hFile, ");\n\n");
   }

   /* typedef transition functions, if we have any */
   if (pmi->transition_fn_list->count)
   {
      fprintf(pcmd->hFile, "typedef ");
	  streamHungarianToUnderbarCaps(pcmd->hFile, pmi->name->name);
      fprintf(pcmd->hFile, "_STATE (*");
	  printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
      fprintf(pcmd->hFile, "_TRANSITION_FN)(p");
	  printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
      fprintf(pcmd->hFile, ",");
	  streamHungarianToUnderbarCaps(pcmd->hFile, ultimateAncestor(pmi)->name->name);
      fprintf(pcmd->hFile
			  , "_EVENT%s);\n\n"
              , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
             );

   }

   /* typedef the FSM function */
   fprintf(pcmd->hFile, "typedef ");
   streamHungarianToUnderbarCaps(pcmd->hFile, ultimateAncestor(pmi)->name->name);
   fprintf(pcmd->hFile
		   , "_EVENT%s (*"
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
		  );
   printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->hFile, "_FSM)(p");
   printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->hFile, ",");
   streamHungarianToUnderbarCaps(pcmd->hFile, ultimateAncestor(pmi)->name->name);
   fprintf(pcmd->hFile
		   , "_EVENT%s);\n\n"
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
           );

   /* declare the fsm function */
   streamHungarianToUnderbarCaps(pcmd->hFile, ultimateAncestor(pmi)->name->name);
   fprintf(pcmd->hFile
           , "_EVENT%s "
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
		  );
   printAncestry(pmi, pcmd->hFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->hFile, "FSM(p");
   printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->hFile, ",");
   streamHungarianToUnderbarCaps(pcmd->hFile, ultimateAncestor(pmi)->name->name);
   fprintf(pcmd->hFile
		   , "_EVENT%s);\n\n"
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
           );

   /* put the data structure definition into the header */
   if (pmi->data)
   {
      fprintf(pcmd->pubHFile, "struct _");
	  printAncestry(pmi, pcmd->pubHFile, "_", alc_lower, ai_include_self);
      fprintf(pcmd->pubHFile, "_data_struct_ {\n");

      helper.tab_level = 1;
	  helper.fout	   = pcmd->pubHFile;
      iterate_list(pmi->data, print_data_field, &helper);
	  helper.fout	   = pcmd->hFile;

      fprintf(pcmd->pubHFile 
              , "};\n\n"
              );
   }

   if (pmi->machine_list)
   {
	   printSubMachinesDeclarations(pcmd, pmi);
   }
   

}

void print_transition_fn_declaration_for_when_actions_return_states(pMACHINE_INFO pmi, FILE *fout, char *name)
{
	streamHungarianToUnderbarCaps(fout, pmi->name->name);
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
	streamHungarianToUnderbarCaps(fout, pmi->name->name);
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

	   print_shared_event_data_block_signature(pih->fout, pih->pmi, pevent->name, true /* include type information */);

	   fprintf(pih->fout, " = {\n");

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
	  fprintf(pih->fout, "_sub_fsm_if\n");

      fprintf(pih->fout, "};\n\n");

   }

   return false;
}

void possiblyDefineSubMachineSharedEventStructures (pCMachineData pcmd, pMACHINE_INFO pmi)
{
   ITERATOR_HELPER ih;

   if (pmi->shared_event_count)
   {
      ih.pmi  = pmi;
      ih.fout = pcmd->cFile;

      iterate_list(pmi->event_list, define_needed_shared_event_structures, &ih);
   }

}

void defineSubMachineIF (pCMachineData pcmd, pMACHINE_INFO pmi)
{
	streamHungarianToUnderbarCaps(pcmd->cFile, ultimateAncestor(pmi)->name->name);
   fprintf(pcmd->cFile
           , "_EVENT%s "
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
 		  );
   printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->cFile, "_sub_machine_fn(");
   streamHungarianToUnderbarCaps(pcmd->cFile, ultimateAncestor(pmi)->name->name);
   fprintf(pcmd->cFile
 		  , "_EVENT%s e)\n{\n"
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
           );

   fprintf(pcmd->cFile, "\treturn ");
   printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->cFile, "FSM(p");
   printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->cFile, ", e);\n}\n\n\n");

   printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->cFile, "_SUB_FSM_IF ");
   printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->cFile, "_sub_fsm_if =\n{\n\t");

  fprintf(pcmd->cFile, "\t.subFSM = ");
  printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
  fprintf(pcmd->cFile, "_sub_machine_fn\n");

  fprintf(pcmd->cFile, "\t, .first_event = ");
  printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
  fprintf(pcmd->cFile
 		 , "_%s\n"
          , eventNameByIndex(pmi, 0)
          );

  fprintf(pcmd->cFile, "\t, .last_event = ");
  printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
  fprintf(pcmd->cFile
 		 , "_%s\n"
          , eventNameByIndex(pmi, pmi->event_list->count - 1)
          );

  fprintf(pcmd->cFile
          ,"};\n\n"
          );

}

void defineSubMachineArray(pCMachineData pcmd, pMACHINE_INFO pmi)
{
#ifdef FSMLANG_DEVELOP
	fprintf(pcmd->cFile
			, "/* defineSubMachineArray */\n"
		   );
#endif
   ITERATOR_CALLBACK_HELPER ich;

   if (pmi->machine_list)
   {
      fprintf(pcmd->cFile, "\np");
	  printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
      fprintf(pcmd->cFile, "_SUB_FSM_IF ");
	  printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
      fprintf(pcmd->cFile, "_sub_fsm_if_array[");
	  printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
      fprintf(pcmd->cFile, "_numSubMachines] =\n{\n");

      ich.pcmd  = pcmd;
      ich.pmi   = pmi;
      ich.first = true;
      iterate_list(pmi->machine_list, print_sub_machine_if,&ich);

      fprintf(pcmd->cFile
              ,"};\n\n"
              );

      if (pmi->parent_event_reference_count)
      {
         define_parent_event_reference_elements(pcmd, pmi);
      }

   }
}

void print_action_function_declaration(pMACHINE_INFO pmi, FILE *fout, char *name)
{
	if (pmi->modFlags & mfActionsReturnVoid)
	{
	   fprintf(fout, "void ");
	}
	else
	{
	   if (pmi->modFlags & mfActionsReturnStates)
	   {
		  streamHungarianToUnderbarCaps(fout, pmi->name->name);
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
	fprintf(fout
			, "_%s(p"
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
	   print_action_function_declaration(pich->pmi, pich->pcmd->hFile, pid_info->name);
   }

   return false;
}

bool print_sub_machine_if(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO pmi              = (pMACHINE_INFO) pelem->mbr;
   pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

#ifdef FSMLANG_DEVELOP
	fprintf(pich->pcmd->cFile
			, "/* print_sub_machine_if */\n"
		   );
#endif
   fprintf(pich->pcmd->cFile
           ,"\t%s&"
           ,pich->first ? (pich->first = false, "") : ", "
          );
   printNameWithAncestry("sub_fsm_if\n", pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);

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
         fprintf(pich->pcmd->cFile, "void __attribute__((weak)) ");
		 printAncestry(pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
		 fprintf(pich->pcmd->cFile
				 , "_%s(p"
                 , pid_info->name
                );
		 printAncestry(pich->pmi, pich->pcmd->cFile, "_", alc_upper, ai_include_self);
		 fprintf(pich->pcmd->cFile, " pfsm)\n{\n");

         fprintf(pich->pcmd->cFile
                 , "\t%s(\"weak: "
                 , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
				 );
		 printAncestry(pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
		 fprintf(pich->pcmd->cFile
				 , "_%s\");\n"
                 , pid_info->name
                 );

         fprintf(pich->pcmd->cFile
                 , "\t(void) pfsm;\n"
                 );

      }
      else
      {
         if (pich->pmi->modFlags & mfActionsReturnStates)
         {
			streamHungarianToUnderbarCaps(pich->pcmd->cFile, pich->pmi->name->name);
            fprintf(pich->pcmd->cFile, "_STATE __attribute__((weak)) ");
			printAncestry(pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
			fprintf(pich->pcmd->cFile
					, "_%s(p"
                    , pid_info->name
					);
			printAncestry(pich->pmi, pich->pcmd->cFile, "_", alc_upper, ai_include_self);
			fprintf(pich->pcmd->cFile, " pfsm)\n{\n");

            fprintf(pich->pcmd->cFile
                    , "\t%s(\"weak: "
                    , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                    );
			printAncestry(pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
			fprintf(pich->pcmd->cFile
					, "_%s\");\n"
                    , pid_info->name
					);

            fprintf(pich->pcmd->cFile
                    , "\t(void) pfsm;\n"
                    );

            fprintf(pich->pcmd->cFile, "\treturn ");
			printNameWithAncestry("noTransition;\n", pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);

         }
         else
         {
			 printAncestry(pich->pmi, pich->pcmd->cFile, "_", alc_upper, ai_include_self);
            fprintf(pich->pcmd->cFile
                    , "_EVENT%s __attribute__((weak)) "
                    , ultimateAncestor(pich->pmi)->data_block_count ? "_ENUM"  : ""
                   );
			printAncestry(pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
			fprintf(pich->pcmd->cFile
					, "_%s(p"
                    , pid_info->name
					);
			printAncestry(pich->pmi, pich->pcmd->cFile, "_", alc_upper, ai_include_self);
			fprintf(pich->pcmd->cFile, " pfsm)\n{\n");

            fprintf(pich->pcmd->cFile
                    , "\t%s(\"weak: "
                    , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                    );
			printAncestry(pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
			fprintf(pich->pcmd->cFile
					, "_%s\");\n"
                    , pid_info->name
					);

            fprintf(pich->pcmd->cFile
                    , "\t(void) pfsm;\n"
                    );

            fprintf(pich->pcmd->cFile
                    , "\treturn THIS(noEvent);\n"
                    );

         }
      }

      fprintf(pich->pcmd->cFile
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
          streamHungarianToUnderbarCaps(pich->pcmd->cFile, ultimateAncestor(pich->pmi)->name->name);
         fprintf(pich->pcmd->cFile
                 , "_EVENT%s "
                 , ultimateAncestor(pich->pmi)->data_block_count ? "_ENUM"  : ""
                );
         printNameWithAncestry(pid_info->name, pich->pmi, pich->pcmd->cFile, " ", alc_lower, ai_include_self);
         fprintf(pich->pcmd->cFile, "(p");
         printAncestry(pich->pmi, pich->pcmd->cFile, "_", alc_upper, ai_include_self);
         fprintf(pich->pcmd->cFile, " pfsm)\n{\n");

         fprintf(pich->pcmd->cFile
                 , "\t%s(\"weak: "
                 , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                );
         printNameWithAncestry(pid_info->name, pich->pmi, pich->pcmd->cFile, " ", alc_lower, ai_include_self);
         fprintf(pich->pcmd->cFile, "\");\n");

         fprintf(pich->pcmd->cFile
                 , "\treturn %s_pass_shared_event(pfsm, sharing_%s_%s);\n}\n\n"
                 , pich->pmi->name->name
                 , pich->pmi->name->name
                 , pevent->name
                 );
      }

   }

   return false;
}

void defineEventPassingActions(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmd      = pcmd;
   ich.pmi       = pmi;

   iterate_list(pmi->action_list
                , define_event_passing_actions
                , &ich
                );
}

void defineWeakDataTranslatorStubs(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmd      = pcmd;
   ich.pmi       = pmi;

   iterate_list(pmi->event_list
                , define_weak_data_translator_functions
                , &ich
                );

   fprintf(pcmd->hFile, "\n");
}

static void defineSubMachineInhibitor(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmd      = pcmd;
   ich.pmi       = pmi;
   ich.counter   = 0;

   fprintf(pcmd->cFile, "\nstatic bool doNotInhibitSubMachines(");
   streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
   fprintf(pcmd->cFile, "_STATE s)\n{\n");

   fprintf(pcmd->cFile, "\tswitch (s)\n\t{\n");

   iterate_list(pmi->state_list
                , print_inhibited_state_case
                , &ich
                );

   fprintf(pcmd->cFile
           , "\t\t\treturn false;\n%s\n\t}\n"
           , ich.counter < pmi->state_list->count ? "\n\t\tdefault:\n\t\t\treturn true;" : ""
           );

   fprintf(pcmd->cFile
           , "}\n\n"
           );
 
   if (pmi->submachine_inhibitor_count == pmi->state_list->count)
   {
      printf("warning: (%s) all states inhibit sub machines\n"
             , pmi->name->name
             );
   }
}

void defineSubMachineFinder(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   if (pmi->submachine_inhibitor_count)
   {
      defineSubMachineInhibitor(pcmd, pmi);
   }

   fprintf(pcmd->cFile, "\nstatic ");
   streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
   fprintf(pcmd->cFile
           , "_EVENT%s findAndRunSubMachine(p"
           , pmi->data_block_count ? "_ENUM"  : ""
          );
   streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
   fprintf(pcmd->cFile, " pfsm, ");
   streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
   fprintf(pcmd->cFile
           , "_EVENT%s e)\n{\n"
           , pmi->data_block_count ? "_ENUM"  : ""
          );

   fprintf(pcmd->cFile, "\tfor (");
   streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
   fprintf(pcmd->cFile
           , "_SUB_MACHINES machineIterator = %s_firstSubMachine;\n\t     machineIterator < %s_numSubMachines;\n\t     machineIterator++\n\t    )\n\t{\n"
           , pmi->name->name
           , pmi->name->name
           );

   fprintf(pcmd->cFile
           , "\t\t\tif (\n\t\t\t   ((*pfsm->subMachineArray)[machineIterator]->first_event <= e)\n"
           );

   fprintf(pcmd->cFile
           , "\t\t\t   && ((*pfsm->subMachineArray)[machineIterator]->last_event >= e)\n\t\t\t    )\n"
           );

   fprintf(pcmd->cFile
           , "\t\t\t{\n"
           );

   fprintf(pcmd->cFile
           , "\t\t\t\treturn ((*(*pfsm->subMachineArray)[machineIterator]->subFSM)(e));\n"
          );

   fprintf(pcmd->cFile
           , "\t\t\t}\n"
           );

   fprintf(pcmd->cFile, "\t}\n\n\treturn ");
   printNameWithAncestry("noEvent", pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->cFile, ";\n\n}\n\n");
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

static void print_shared_event_data_block_signature(FILE *file, pMACHINE_INFO pmi, char *event_name, bool include_type)
{
#ifdef FSMLANG_DEVELOP
	fprintf(file
			, "/* print_shared_event_data_block_signature */\n"
		   );
#endif

	if (include_type)
	{
		streamHungarianToUnderbarCaps(file, pmi->parent->name->name);
		fprintf(file, "_SHARED_EVENT_STR ");
	}

	fprintf(file
			, "%s_share_%s_%s_str"
			, pmi->name->name
			, pmi->parent->name->name
			, event_name
			);

}

static bool declare_shared_event_data_blocks(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO pmi    = (pMACHINE_INFO)pelem->mbr;
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   fprintf(pih->fout, "extern ");

   print_shared_event_data_block_signature(pih->fout, pmi, pih->pid->name, true /* include type information */);

   fprintf(pih->fout, ";\n");

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
   printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->cFile, ",");
   streamHungarianToUnderbarCaps(pcmd->cFile, ultimateAncestor(pmi)->name->name);
   fprintf(pcmd->cFile
		   , "_EVENT%s);\n"
           , ultimateAncestor(pmi)->data_block_count ? "_ENUM"  : ""
           );

   if (pmi->submachine_inhibitor_count)
   {
      fprintf(pcmd->cFile, "static bool doNotInhibitSubMachines(");
	  streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
	  fprintf(pcmd->cFile, "_STATE);\n");
   }

   fprintf(pcmd->cFile, "\n");
}

void declareEventDataManager(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   fprintf(pcmd->cFile, "static void translateEventData(p");
   printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->cFile, "_DATA,p");
   printAncestry(pmi, pcmd->cFile, "_",  alc_upper, ai_include_self);
   fprintf(pcmd->cFile, "_EVENT);\n\n");
}

static bool write_event_data_manager_switch_case(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pevent                = (pID_INFO)pelem->mbr;
   pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

   if (pevent->type_data.event_data.puser_event_data)
   {
      pich->counter++;
      fprintf(pich->pcmd->cFile
              , "\tcase %s_%s:\n"
              , pich->pmi->name->name
              , pevent->name
              );

      fprintf(pich->pcmd->cFile
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

   ich.pcmd    = pcmd;
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
#ifdef FSMLANG_DEVELOP
	fprintf(pcmd->cFile
			, "/* declareStateEntryAndExitManagers */\n"
		   );
#endif
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
	   printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
	   fprintf(pcmd->cFile,"_DATA_");
   }
   printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
   fprintf(pcmd->cFile,");");
}

static void print_state_entry_or_exit_fn_switch_case(pID_INFO pstate, pITERATOR_CALLBACK_HELPER pich, ENTRY_OR_EXIT which)
{
    pID_INFO fn        = which == eoe_entry ? pstate->type_data.state_data.entry_fn : pstate->type_data.state_data.exit_fn;
    char    *no_fn_str = which == eoe_entry ? "onEntryTo_"                          : "onExitFrom_";

	pich->counter++;
	fprintf(pich->pcmd->cFile, "\tcase ");
	printAncestry(pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);
	fprintf(pich->pcmd->cFile
			, "_%s:\n\t\t"
			, pstate->name
		   );
	printAncestry(pich->pmi, pich->pcmd->cFile, "_", alc_lower, ai_include_self);

   fprintf(pich->pcmd->cFile
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
   ich.pcmd = pcmd;

   if (pmi->states_with_entry_fns_count)
   {
      fprintf(pcmd->cFile,"static void runAppropriateEntryFunction(");
	  if (pmi->data)
	  {
		  fprintf(pcmd->cFile,"p");
		  printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
		  fprintf(pcmd->cFile, "_DATA pdata, ");
	  }
	  streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
      fprintf(pcmd->cFile, "_STATE s)\n{\n\tswitch(s)\n\t{\n");

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
	   streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
	   fprintf(pcmd->cFile, "_STATE s)\n{\n\tswitch(s)\n\t{\n");

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
#ifdef FSMLANG_DEVELOP
	fprintf(pcmd->cFile
			, "/* printSubMachinesDeclarations */\n"
		   );
#endif
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
	printAncestry(pmi, pcmd->hFile, "_", alc_lower, ai_include_self);
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
	printAncestry(pmi, pcmd->hFile, "_", alc_lower, ai_include_self);
	fprintf(pcmd->hFile, "_sub_fsm_if_ ");
	printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
	fprintf(pcmd->hFile, "_SUB_FSM_IF, *p");
	printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
	fprintf(pcmd->hFile, "_SUB_FSM_IF;\n");

	fprintf(pcmd->hFile,"struct _");
	printAncestry(pmi, pcmd->hFile, "_", alc_lower, ai_include_self);
	fprintf(pcmd->hFile, "_sub_fsm_if_\n{\n");

	fprintf(pcmd->hFile, "\t");
	streamHungarianToUnderbarCaps(pcmd->hFile, ultimateAncestor(pmi)->name->name);
	fprintf(pcmd->hFile
			, "_EVENT%s                first_event;\n"
			, pmi->data_block_count ? "_ENUM"  : ""
			);

	fprintf(pcmd->hFile, "\t");
	streamHungarianToUnderbarCaps(pcmd->hFile, ultimateAncestor(pmi)->name->name);
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
	printAncestry(pmi, pcmd->hFile, "_", alc_upper, ai_include_self);
	fprintf(pcmd->hFile, "_SUB_FSM_IF ");
	printAncestry(pmi, pcmd->hFile, "_", alc_lower, ai_include_self);
	fprintf(pcmd->hFile, "_sub_fsm_if_array[");
	printAncestry(pmi, pcmd->hFile, "_", alc_lower, ai_include_self);
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
    fprintf(pcmd->cFile, "#ifdef ");
	printNameWithAncestry("DEBUG\n", pmi, pcmd->cFile, "_", alc_upper, ai_include_self);

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
			   , "%s: "
               , pmi->name->name
           );
    }

    fprintf(pcmd->cFile, "event: %%s; state: %%s\"\n,");
    printAncestry(pmi, pcmd->cFile, "_", alc_upper, ai_include_self);
    fprintf(pcmd->cFile, "_EVENT_NAMES[%s - THIS(%s)]\n,"
          , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
          , eventNameByIndex(pmi,0)
          );
	streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
    fprintf(pcmd->cFile, "_STATE_NAMES[pfsm->state]\n);\n}\n");

    fprintf(pcmd->cFile, "#endif\n\n");

}

