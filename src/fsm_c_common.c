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
#include "ancestry.h"

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

#if defined (LINUX) || defined (VS) || defined (CYGWIN)
	#include <time.h>
#endif
#include <string.h>
#include <stdlib.h>

#if !defined (LEX_DEBUG)
	#if defined (VS)
		#include "parser.h"
	#else
		#include "y.tab.h"
	#endif
#else
	#include "lexer_debug.h"
#endif


#include "revision.h"

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
static void print_entry_or_exit_fn_body                                              (pITERATOR_CALLBACK_HELPER);
static bool print_event_cross_reference                                              (pLIST_ELEMENT,void*);
static bool print_sub_machine_event_cross_reference                                  (pLIST_ELEMENT,void*);
static void print_event_cross_reference_entry                                        (char*,pITERATOR_CALLBACK_HELPER);
static void print_state_only_transition_fn_declaration_for_when_actions_return_states(pMACHINE_INFO,FILE*,char*);
static void print_transition_fn_declaration_for_when_actions_return_events           (pCMachineData,FILE*,char*);
static void print_state_only_transition_fn_declaration_for_when_actions_return_events(pCMachineData,FILE*,char*);
static bool print_event_enum_member                                                  (pLIST_ELEMENT,void*);
static bool write_state_enum_member                                                  (pLIST_ELEMENT,void*);
static void print_plain_enum_member                                                  (char*,pITERATOR_CALLBACK_HELPER);
static void print_shared_event_data_block_signature                                  (FILE*,pCMachineData,pMACHINE_INFO,char*,bool);
static char *createHeaderCompilationGuard                                            (char*);
static void writeHeaderPreamble                                                      (char*,FILE*);
static void print_transition_function_signature                                      (FILE*,pCMachineData,char*,char*,bool);
static void print_transition_function_body                                           (FILE*,pCMachineData,char*);
static void print_state_entry_or_exit_manager_signature                              (pCMachineData,pMACHINE_INFO,ENTRY_OR_EXIT,DECLARE_OR_DEFINE);
static void print_native_header                                                      (pCMachineData,pMACHINE_INFO);
static void print_data_translator_fn_signature                                       (FILE*,pCMachineData,pID_INFO,DECLARE_OR_DEFINE);
static void print_sub_machine_data_translator_fn_signature                                       (FILE*,pCMachineData,pID_INFO,DECLARE_OR_DEFINE);

int initCMachine(pFSMOutputGenerator pfsmog, char *fileName)
{
   pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

   if ( (pfsmcog->pcmd = newCMachineData(fileName)) )
   {
	   writeCFilePreambles(pfsmcog->pcmd, false);

	   /* include our public header */
	   fprintf(pfsmcog->pcmd->hFile
			   , "#include \"%s\"\n"
			   , pfsmcog->pcmd->pubHName
			   );

	   fprintf(pfsmcog->pcmd->subMachineHFile
			   , "#include \"%s\"\n"
			   , pfsmcog->pcmd->pubHName
			   );

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

	if (cp1)
	{
		for (cp = cp1; *cp; cp++)

		   /* capitalise things, and change the '.' or '-' to '_' */
		   *cp = ((*cp == '.') || (*cp == '-')) ? '_' : (char)toupper(*cp);
	}

	return cp1;
}

static void writeHeaderPreamble(char *fn, FILE *file)
{
	char *cp = createHeaderCompilationGuard(fn);

	if (cp && file)
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

	}

	CHECK_AND_FREE(cp);
}

void writeCFilePreambles(pCMachineData pcmd, bool sub_machine)
{
	bool          error = false;
	CREATED_FILES cf;

	/* Open (almost) all files. */
	for (cf = cf_first; cf < cf_numCreatedFiles && !error; cf++)
	{
		/* When initializing for sub-machines, do not open a public header. */
		if (!sub_machine || (sub_machine && cf != cf_pubH))
		{
			error = ((pcmd->file_array[cf] = openFile(pcmd->file_name_array[cf], "w")) == NULL);
		}
	}

	/* sub-machines use the same file for public and private matters*/
	if (sub_machine)
	{
		pcmd->file_array[cf_pubH] = pcmd->file_array[cf_h];
	}

	if (error)
	{

		fprintf(stderr
				, "%s: Unable to open file %s\n"
				, me
				, pcmd->file_name_array[cf-1]
				);
	}
	else
	{

		for (cf = cf_first; cf < cf_numCreatedFiles; cf++)
		{
			if (cf == cf_c)
			{
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

			}
			else
			{
				/* the header files */

				/* Skip the public header when it is the same as the private. */
				if (cf == cf_pubH && (pcmd->file_array[cf_pubH] == pcmd->file_array[cf_h]))
				{
					continue;
				}

				writeHeaderPreamble(pcmd->file_name_array[cf], pcmd->file_array[cf]);
				if (sub_machine && cf == cf_h)
				{
					fprintf(pcmd->hFile
							,"#include \"%s\"\n"
							, pcmd->parent_pcmd->subMachineHName
							);
				}
			}
		}

	}

}

int initCSubMachine(pFSMOutputGenerator pfsmog, char *fileName)
{
   pFSMCSubMachineOutputGenerator pfsmcsmog = (pFSMCSubMachineOutputGenerator) pfsmog;

   if ((pfsmcsmog->pcmd = newCMachineData(fileName)))
   {

	  /* Only data (not pointers to the writers) are passed around; 
	  this gives the data block access to the parent.  writeCFilePreables
	  uses parent_pcmd. */
	  pfsmcsmog->pcmd->parent_pcmd = pfsmcsmog->parent_fsmcog->pcmd;

	  /* With parent_pcmd assigned, we can call writeCFilePreambles. */
	  writeCFilePreambles(pfsmcsmog->pcmd, true);

	  /* The parent has encountered a sub-machine. This affects how
	  the data block is destroyed.
	  */
	  pfsmcsmog->parent_fsmcog->pcmd->a_sub_machine_was_encountered = true;

	  /* for sub machines, some output strings are taken from the parent */
	  pfsmcsmog->pcmd->action_return_type = pfsmcsmog->parent_fsmcog->pcmd->action_return_type;
	  pfsmcsmog->pcmd->fsm_fn_event_type  = pfsmcsmog->parent_fsmcog->pcmd->action_return_type;
	  pfsmcsmog->pcmd->event_type         = pfsmcsmog->parent_fsmcog->pcmd->event_type;

	 /* Put the call to the top-level header file into the header. */
	 fprintf(pfsmcsmog->pcmd->hFile
			 , "#include \"%s\"\n\n"
			 , pfsmcsmog->top_level_fsmcog->pcmd->pubHName
			 );

	 fprintf(pfsmcsmog->pcmd->subMachineHFile
			 , "#include \"%s\"\n\n"
			 , pfsmcsmog->top_level_fsmcog->pcmd->pubHName
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

   (void) how;

}

void addEventCrossReference(pCMachineData pcmd, pMACHINE_INFO pmi, pITERATOR_CALLBACK_HELPER pich)
{

   if ( pmi->external_event_designation_count > 0 )
   {
      if (assignExternalEventValues(pmi) == false)
      {
         printf("warning: cannot print cross reference with external designations\n");
         return;
      }
   }

   fprintf(pcmd->pubHFile
           , "/*\n\tEvent Cross Reference:\n\n"
          );

   pich->ih.first     = true;
   pich->ih.tab_level  = 0;
   pich->ih.pmi       = pmi;

   iterate_list(pmi->event_list, print_event_cross_reference, pich);

   if (
       !(pmi->modFlags & mfActionsReturnStates)
       && !(pmi->modFlags & mfActionsReturnVoid)
      )
   {
      print_event_cross_reference_entry("noEvent", pich);
   }

   print_event_cross_reference_entry("numEvents", pich);

   if (pmi->machine_list)
   {
      iterate_list(pmi->machine_list, print_sub_machine_event_cross_reference, pich);

	  pich->ih.pmi = pmi;
	  print_event_cross_reference_entry("numAllEvents", pich);
   }


   fprintf(pcmd->pubHFile
           , "\n*/\n"
          );

}

void commonHeaderStart(pCMachineData pcmd, pMACHINE_INFO pmi, char *arrayName)
{
	//TODO: remove from signature
	(void) arrayName;

   ITERATOR_CALLBACK_HELPER   ich = {
	   .ih = {
		   .first     = false
		   , .fout    = pcmd->hFile
		   , .pmi     = pmi
	   }
	   , .pcmd    = pcmd
   };

   /* put the native code segment out to the header */
   if (pmi->native) print_native_header(pcmd, pmi);

   fprintf(pcmd->hFile
		   , "\n#ifdef %s_DEBUG\n"
		   , fsmType(pcmd)
		   );
   fprintf(pcmd->hFile, "#include <stdio.h>\n");
   fprintf(pcmd->hFile, "#include <stdlib.h>\n");
   fprintf(pcmd->hFile, "#endif\n\n");

   if (pmi->has_single_pai_events)
   {
      fprintf(pcmd->hFile, "#include <stdbool.h>\n");
   }

   fprintf(pcmd->pubHFile
           , "#define FSM_VERSION \"%s\"\n\n"
           , rev_string
           );

   if (core_logging_only)
   {
      fprintf(pcmd->hFile, "#ifndef NON_CORE_DEBUG_PRINTF\n#define NON_CORE_DEBUG_PRINTF(...) \n#endif\n\n");
   }

   fprintf(pcmd->pubHFile
		   , "#undef THIS\n#define THIS(A) %s_##A\n"
		   , machineName(pcmd)
		   );

   if (pmi->machine_list)
   {
      fprintf(pcmd->pubHFile
			  , "#undef %s\n#define %s(A) %s_##A\n"
			  , fsmType(pcmd)
			  , fsmType(pcmd)
			  , machineName(pcmd)
			  );

	  ich.ih.fout = pcmd->pubHFile;
      iterate_list(pmi->machine_list, print_event_macro, &ich);
	  ich.ih.fout = pcmd->hFile;
   }
   fprintf(pcmd->pubHFile, "\n");

   if (add_event_cross_reference)
   {
	   ich.ih.fout = pcmd->pubHFile;
	   addEventCrossReference(pcmd, pmi, &ich);
	   ich.ih.fout = pcmd->hFile;
   }

   /* put the event enum into the header file */
   fprintf(pcmd->pubHFile
		   , "typedef enum %s {\n"
		   , eventType(pcmd)
		   );

   if (pmi->external_event_designation_count > 0 )
   {
      if (assignExternalEventValues(pmi) == false)
      {
         printf("warning: cannot use external event designations\n");
      }
   }


   ich.ih.first = true;
   ich.ih.fout = pcmd->pubHFile;
   iterate_list(pmi->event_list, print_event_enum_member, &ich);
   ich.ih.fout = pcmd->hFile;

   if (
       !(pmi->modFlags & mfActionsReturnStates)
       && !(pmi->modFlags & mfActionsReturnVoid)
      )
   {
      fprintf(pcmd->pubHFile
			  , "\t, %s_noEvent\n"
			  , machineName(pcmd)
			  );
   }

   fprintf(pcmd->pubHFile
		   , "\t, %s_numEvents"
		   , machineName(pcmd)
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
	   ich.ih.fout = pcmd->pubHFile;
      iterate_list(pmi->machine_list,print_sub_machine_events,&ich);
	  ich.ih.fout = pcmd->hFile;
   }

   fprintf(pcmd->pubHFile
		   , "\t, %s_numAllEvents"
		   , machineName(pcmd)
		   );

   fprintf(pcmd->pubHFile
		   , pmi->machine_list ? "%s\n" : " = %s_numEvents\n" 
		   , pmi->machine_list ? ""     : machineName(pcmd)
		   );

   fprintf(pcmd->pubHFile
           , "}%s %s;\n\n"
           , compact_action_array ? "__attribute__((__packed__)) " : " "
		   , eventType(pcmd)
          );

   fprintf(pcmd->pubHFile
		   , "#undef ACTION_RETURN_TYPE\n#define ACTION_RETURN_TYPE %s\n"
		   , actionReturnType(pcmd)
		   );

   /* if we have user data on some events, declare the event structure */
   if (pmi->data_block_count)
   {
	   ich.ih.pmi = pmi;
	   ich.ih.fout = pcmd->pubHFile;
      iterate_list(pmi->event_list, declare_event_user_data_structs, &ich);

      /* create the union of pointers */
      fprintf(pcmd->pubHFile
              , "typedef union {\n"
              );

      iterate_list(pmi->event_list, declare_event_user_data_union_mbrs, &ich);

      fprintf(pcmd->pubHFile
			  , "} %s_EVENT_DATA, *p%s_EVENT_DATA;\n\n"
			  , fsmType(pcmd)
			  , fsmType(pcmd)
			  );

      /* create the event struct */
	  fprintf(pcmd->pubHFile
			  , "typedef struct {\n\t%s_EVENT_ENUM event;\n\t%s_EVENT_DATA event_data;\n} %s_EVENT, *p%s_EVENT;\n\n"
			  , fsmType(pcmd)
			  , fsmType(pcmd)
			  , fsmType(pcmd)
			  , fsmType(pcmd)
			  );
	  ich.ih.fout = pcmd->hFile;
   }

   if (generate_run_function)
   {
      fprintf(pcmd->pubHFile, "\nvoid run_%s(%s);\n\n"
			  , machineName(pcmd)
			  , fsmFnEventType(pcmd)
             );
   }

   fprintf(pcmd->hFile
		   , "\n#ifdef %s_DEBUG\n"
		   , fsmType(pcmd)
		   );
   fprintf(pcmd->hFile
		   , "extern char *%s_EVENT_NAMES[];\n"
		   , fsmType(pcmd)
		   );
   fprintf(pcmd->hFile
		   , "extern char *%s_STATE_NAMES[];\n"
		   , fsmType(pcmd)
		   );
   fprintf(pcmd->hFile, "#endif\n\n");

   /* put the state enum into the header file */
   fprintf(pcmd->hFile
           , "typedef enum {\n"
          );

   ich.ih.first = true;
   ich.ih.pmi   = pmi;
   iterate_list(pmi->state_list, write_state_enum_member, &ich);
   

   /*
     Though not a state, this needs have a value
       different from any state. Moreover, it could
       be said to bear the same relation to states
       as noEvent does to events.
   */
   if (pmi->modFlags & mfActionsReturnStates)
   {
      fprintf(pcmd->hFile
              , "\t, %s_noTransition\n"
			  , machineName(pcmd)
             );
   }

   fprintf(pcmd->hFile
           , "\t, %s_numStates\n"
		   , machineName(pcmd)
           );

   fprintf(pcmd->hFile
           , "}%s %s;\n\n"
           , compact_action_array ? " __attribute__((__packed__))" : " "
		   , stateType(pcmd)
          );

   /* Put the data struct typedef into the header file. */
   if (pmi->data)
   {
	   /* Which header depends on whether we have sub-machines. */
      fprintf(pmi->machine_list ? pcmd->subMachineHFile : pcmd->hFile
              , "typedef struct _%s_data_struct_ %s, *p%s;\n"
			  , machineName(pcmd)
			  , fsmDataType(pcmd)
			  , fsmDataType(pcmd)
			 );

   }

   /* forward declare the machine type */
   fprintf(pcmd->pubHFile
		   , "typedef struct _%s_struct_ *p%s;\n"
		   , machineName(pcmd)
		   , fsmType(pcmd)
		  );

   /* put the machine struct typedef into the header */
   fprintf(pcmd->hFile
		   , "typedef struct _%s_struct_ %s;\n"
		   , machineName(pcmd)
		   , fsmType(pcmd)
		  );

   fprintf(pcmd->hFile
		   , "#undef FSM_TYPE_PTR\n#define FSM_TYPE_PTR p%s\n"
		   , fsmType(pcmd)
		   );

   if (generate_instance)
   {
      fprintf(pcmd->hFile
			  , "extern %s %s;\n\n"
			  , fsmType(pcmd)
			  , machineName(pcmd)
			  );

      fprintf(pcmd->pubHFile
			  , "extern p%s p%s;\n\n"
			  , fsmType(pcmd)
			  , machineName(pcmd)
			  );

   }

   /* put the action function typedef into the header */
   fprintf(pcmd->hFile
		   , "typedef %s (*%s)(FSM_TYPE_PTR);\n\n"
		   , actionReturnType(pcmd)
		   , actionFnType(pcmd)
		   );

   /* typedef transition functions, if we have any */
   if (pmi->transition_fn_list->count)
   {
	   fprintf(pcmd->hFile
			   , "typedef %s (*%s)(FSM_TYPE_PTR,%s);\n\n"
			   , stateType(pcmd)
			   , transitionFnType(pcmd)
			   , eventType(pcmd)
			   );
   }

   /* typedef the FSM function */
   fprintf(pcmd->hFile
		   , "typedef void (*%s_FSM)(FSM_TYPE_PTR,%s);\n\n"
		   , fsmType(pcmd)
		   , fsmFnEventType(pcmd)
		   );

   /* declare the fsm function */
   fprintf(pcmd->hFile
           , "void %sFSM(FSM_TYPE_PTR,%s);\n\n"
		   , machineName(pcmd)
		   , fsmFnEventType(pcmd)
           );

   if (pmi->machine_list)
   {
	   /* The sub-machine header will hold things needed by the private header. */
	   fprintf(pcmd->hFile
			   ,"#include \"%s\"\n"
			   , pcmd->subMachineHName
			   );

	   printSubMachinesDeclarations(pcmd,pmi);
   }

   /* put the data structure definition into the header */
   if (pmi->data)
   {
	   /* Which header depends on whether we have sub-machines. */
      fprintf(pmi->machine_list ? pcmd->subMachineHFile : pcmd->hFile
              , "struct _%s_data_struct_ {\n"
			  , machineName(pcmd)
              );

      ich.ih.tab_level = 1; 
	  ich.ih.fout      = pmi->machine_list ? pcmd->subMachineHFile : pcmd->hFile;
      iterate_list(pmi->data, print_data_field, &ich);

      fprintf(pmi->machine_list ? pcmd->subMachineHFile : pcmd->hFile
              , "};\n\n"
              );
   }

}

void commonHeaderEnd(pCMachineData pcmd, pMACHINE_INFO pmi, bool needNoOp)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->hFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmd      = pcmd;
   ich.ih.pmi       = pmi;
   ich.needNoOp  = needNoOp;

   /* declare the action functions themselves */
   iterate_list(pmi->action_list, declare_action_function, &ich);

   /* declare the dummy, or no op action */
   if (needNoOp)
   {
	   print_action_function_declaration(pcmd, "noAction");
   }

   fprintf(pcmd->hFile
           , "\n"
          );

   /* declare any machine transition function */
   if (pmi->machineTransition)
   {
	   fprintf(pcmd->hFile
			   , "void %s_%s(FSM_TYPE_PTR,%s);\n\n"
			   , fqMachineName(pcmd)
			   , pmi->machineTransition->name
			   , stateType(pcmd)
			   );
   }


   /* declare any transition functions */
   if (pmi->modFlags & mfActionsReturnStates)
   {
	   print_transition_fn_declaration_for_when_actions_return_states(pmi,pcmd->hFile, "noTransitionFn");

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
		  print_transition_fn_declaration_for_when_actions_return_events(pcmd
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
	   /* for top-level machines */
      iterate_list(pmi->event_list
				   , declare_data_translator_functions
				   , &ich
				   );
      fprintf(pcmd->hFile, "\n");
   }
   else if (pmi->parent)
   {
	   /* for sub-machines */
	   iterate_list(pmi->event_list
					, sub_machine_declare_data_translator_functions
					, &ich
					);
   }

}

void generateRunFunction(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   fprintf(pcmd->cFile
           ,"void run_%s(%s e)\n{\n"
           , pmi->name->name
		   , fsmFnEventType(pcmd)
           );

   fprintf(pcmd->cFile
           , "\tif (p%s)\n\t{\n\t\tp%s->fsm(p%s,e);\n\t}\n}\n\n"
           , pmi->name->name
           , pmi->name->name
           , pmi->name->name
           );
}

void generateInstance(pCMachineData pcmd, pMACHINE_INFO pmi, char *arrayName)
{
   FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   /* instantiate the machine and the pointer to it */
   /* the (empty) data struct and the state */
   fprintf(pcmd->cFile
		   , "%s %s = {\n"
		   , fsmType(pcmd)
		   , machineName(pcmd)
		   );
   fprintf(pcmd->cFile
		   , "%s"
           , pmi->data ? "\n\tINIT_FSM_DATA,\n" : ""
		   );
   fprintf(pcmd->cFile
		   , "\t%s_%s,\n"
		   , machineName(pcmd)
		   , stateNameByIndex(pmi, 0)
           );

   fprintf(pcmd->cFile
		   , "\tTHIS(%s),\n"
           , eventNameByIndex(pmi, 0)
          );

   fprintf(pcmd->cFile
		   , "\t&%s_%s_array,\n"
		   , machineName(pcmd)
           , arrayName
          );

   if (pmi->machine_list)
   {
      fprintf(pcmd->cFile
			  , "\t&%s_sub_fsm_if_array,\n"
			  , machineName(pcmd)
			  );
   }

   fprintf(pcmd->cFile
		   , "\t%sFSM\n};\n\n"
		   , machineName(pcmd)
		   );

   fprintf(pcmd->cFile
		   , "p%s p%s = &%s;\n\n"
		   , fsmType(pcmd)
		   , machineName(pcmd)
		   , machineName(pcmd)
		   );

}

void defineWeakActionFunctionStubs(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmd      = pcmd;
   ich.ih.pmi       = pmi;

   iterate_list(pmi->action_list, define_weak_action_function, &ich);
}

void defineWeakNoActionFunctionStubs(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	print_weak_action_function_body_omitting_return_statement(pcmd, "noAction");

	if (!(pcmd->pmi->modFlags & mfActionsReturnVoid))
	{
		fprintf(pcmd->cFile
				, "\treturn THIS(%s);"
				, pmi->modFlags & mfActionsReturnStates
				  ? "noTransition"
				  : "noEvent"
				);
	}

	fprintf(pcmd->cFile
			, "\n}\n\n"
			);
}

void defineWeakStateEntryAndExitFunctionStubs(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.ih.pmi    = pmi;
   ich.pcmd   = pcmd;

   if (pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
   {
      iterate_list(pmi->state_list, define_state_entry_and_exit_functions, &ich);
   }

}

static void print_transition_function_signature(FILE *fout, pCMachineData pcmd, char *name_prefix, char *name, bool define)
{
	FSMLANG_DEVELOP_PRINTF(fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	fprintf(fout
			, "\n%s __attribute__((weak)) %s_%s%s(FSM_TYPE_PTR%s%s%s%s)%s\n"
			, stateType(pcmd)
			, fqMachineName(pcmd)
			, name_prefix ? name_prefix : ""
			, name
			, define ? " pfsm" : ""
			, pcmd->pmi->modFlags & mfActionsReturnStates ? "" : ", "
			, pcmd->pmi->modFlags & mfActionsReturnStates ? "" : eventType(pcmd)
			, define && pcmd->pmi->modFlags & mfActionsReturnStates ? "" : " e"
			, define ? "\n{" : ";"
			);
}

static void print_transition_function_body(FILE *fout, pCMachineData pcmd, char *name)
{
	FSMLANG_DEVELOP_PRINTF(fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	if (!(pcmd->pmi->modFlags & mfActionsReturnStates))
	{
		fprintf(fout, "\t(void) e;\n");
	}

	fprintf(fout
			, "\t(void) pfsm;\n\n\t%s(\"weak: %%s\", __func__);\n\treturn %s_%s;\n}\n"
			, core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
			, machineName(pcmd)
			, name
		   );
}

static bool define_transition_function_from_action_array(pLIST_ELEMENT pelem, void *data)
{
	pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER)data;

	unsigned event_ordinal         = pich->pOtherElem->ordinal;
	unsigned state_ordinal         = pelem->ordinal;

	pACTION_INFO pai               = pich->ih.pmi->actionArray[event_ordinal][state_ordinal];

	if (pai)
	{
		if (
			(!strlen(pai->action->name))
			&& (pai->transition->type == STATE)
		   )
		{
			print_transition_function_signature(pich->pcmd->cFile, pich->pcmd, "transitionTo", pai->transition->name, true);
			print_transition_function_body(pich->pcmd->cFile, pich->pcmd, pai->transition->name);
		}
	}

	return false;
}

static bool define_action_array_transition_functions(pLIST_ELEMENT pelem, void *data)
{
	pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER)data;

	pich->pOtherElem = pelem;

	iterate_list(pich->ih.pmi->state_list
				 , define_transition_function_from_action_array
				 , pich
				 );

	return false;
}

static bool define_transition_list_functions(pLIST_ELEMENT pelem, void *data)
{
	pITERATOR_CALLBACK_HELPER pich        = (pITERATOR_CALLBACK_HELPER) data;
	pID_INFO                  ptransition = (pID_INFO) pelem->mbr;

	if (ptransition->type == STATE)
	{
		print_transition_function_signature(pich->pcmd->cFile, pich->pcmd, "transitionTo", ptransition->name, true);
		print_transition_function_body(pich->pcmd->cFile, pich->pcmd, ptransition->name);
	}

	return false;
}

void writeStateTransitions(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	ITERATOR_CALLBACK_HELPER ich = {
		. ih = {
			.pmi = pmi
		}
		, .pcmd = pcmd
	};

   if (pmi->modFlags & mfActionsReturnStates)
   {
      /* grab state transitions from action array */
	  iterate_list(pmi->event_list
				   , define_action_array_transition_functions
				   , &ich
				   );
   }
   else
   {
	   iterate_list(pmi->transition_list
					, define_transition_list_functions
					, &ich
					);
   }
}

static bool print_quoted_pid_name_with_ancestry_as_list_element(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pid = (pID_INFO) pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

	fprintf(pich->ih.fout
			,"%s\""
			, pich->ih.first ? (pich->ih.first = false, "\t ") : "\t,"
			);

	printNameWithAncestry(pid->name, pich->ih.pmi, pich->ih.fout, "_", alc_lower, ai_include_self);

	fprintf(pich->ih.fout ,"\"\n");

	return false;
}

static bool print_quoted_pid_name_as_list_element(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pid = (pID_INFO) pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

	fprintf(pich->ih.fout
			,"%s\"%s\"\n"
			, pich->ih.first ? (pich->ih.first = false, "\t ") : "\t,"
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
	ITERATOR_CALLBACK_HELPER ich = { 0 };

   fprintf(pcmd->cFile
		   , "\n#ifdef %s_DEBUG\nchar *%s_EVENT_NAMES[] = {\n"
		   , fsmType(pcmd)
		   , fsmType(pcmd)
		   );

   ich.ih.fout  = pcmd->cFile;
   ich.ih.first = true;
   iterate_list(pmi->event_list, print_quoted_pid_name_as_list_element, &ich);

   if (!(pmi->modFlags & mfActionsReturnStates) && !(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmd->cFile, "\t, \"noEvent\"\n");
   }

   fprintf(pcmd->cFile, "\t, \"numEvents\"\n");

   if (pmi->machine_list)
   {
      iterate_list(pmi->machine_list,print_sub_machine_event_names,&ich);
   }

   fprintf(pcmd->cFile, "};\n\n");

   fprintf(pcmd->cFile
		   , "char *%s_STATE_NAMES[%s_numStates] = {\n"
		   , fsmType(pcmd)
		   , pmi->name->name
		   );

   ich.ih.first = true;
   ich.ih.pmi   = pmi;   //this got changed in the earlier list iterations
   iterate_list(pmi->state_list, print_quoted_pid_name_as_list_element, &ich);

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
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   fprintf(pcmd->cFile
		   , "\n#ifdef %s_DEBUG\nchar *%s_EVENT_NAMES[] = {\n"
		   , fsmType(pcmd)
		   , fsmType(pcmd)
		   );

   ich.ih.pmi   = pmi;
   ich.ih.fout  = pcmd->cFile;
   ich.ih.first = true;

   iterate_list(pmi->event_list
				, print_quoted_pid_name_with_ancestry_as_list_element
				, &ich
				);

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
      ich.ih.first   = false;
      iterate_list(pmi->machine_list
				   , print_sub_machine_event_names
				   , &ich
				   );
   }

   fprintf(pcmd->cFile, "};\n\n");

   fprintf(pcmd->cFile
		   , "char *%s_STATE_NAMES[%s_numStates] = {\n"
		   , fsmType(pcmd)
		   , pmi->name->name
		   );

   ich.ih.first = true;
   ich.ih.pmi   = pmi;   //this got changed in the earlier list iterations

   iterate_list(pmi->state_list, print_quoted_pid_name_with_ancestry_as_list_element, &ich);

   if (pmi->modFlags & mfActionsReturnStates)
   {
	   fprintf(pcmd->cFile
			   , "\t, \"%s_noTransition\"\n"
			   , fqMachineName(pcmd)
			   );
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

         pcmd->pubHFile = pcmd->subMachineHFile = stdout;
		 pcmd->cFile    = pcmd->hFile           = stdout;

      }
      else
      {

		  /* 
		  Create all file names now, even ones we may not use,
		  because we want to put initialization pieces before other
		  material.
		  */
         pcmd->cName            = createFileName(baseFileName, ".c");
         pcmd->pubHName         = createFileName(baseFileName, ".h");
         pcmd->hName            = createFileName(baseFileName, "_priv.h");
         pcmd->subMachineHName  = createFileName(baseFileName, "_submach.h");

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
      fprintf(pcmd->subMachineHFile, "\n#endif\n");

	  if (pcmd->pubHFile && (pcmd->pubHFile != pcmd->hFile))
	  {
		  fprintf(pcmd->pubHFile, "\n#endif\n");
	  }

	  if (!pcmd->a_sub_machine_was_encountered)
	  {
		  FCLOSE_AND_CLEAR(pcmd->subMachineHFile);
		  (void) unlink(pcmd->subMachineHName);
	  }

   }

   /* A special case */
   if (pcmd->pubHFile == pcmd->hFile)
   {
	   pcmd->pubHFile = NULL;
   }
   for (CREATED_FILES cf = cf_first; cf < cf_numCreatedFiles; cf++)
   {
	   if (pcmd->file_array[cf])
	   {
		   FCLOSE_AND_CLEAR(pcmd->file_array[cf]);
	   }
   }

   /* Delete the files if something went wrong somewhere. */
   if (!good)
   {
	   for (CREATED_FILES cf = cf_first; cf < cf_numCreatedFiles; cf++)
	   {
		   (void) unlink(pcmd->file_name_array[cf]);
	   }
   }

   /* Free the file names */
   for (CREATED_FILES cf = cf_first; cf < cf_numCreatedFiles; cf++)
   {
	   CHECK_AND_FREE(pcmd->file_name_array[cf]);
   }

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

static void print_plain_enum_member(char *name, pITERATOR_CALLBACK_HELPER pich)
{
	fprintf(pich->ih.fout
			, "\t%s%s_%s\n"
			, pich->ih.first ? (pich->ih.first = false, "") : ", "
			, pich->ih.pmi->name->name
			, name
			);
}

static bool write_state_enum_member(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO state       = (pID_INFO)         pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

	print_plain_enum_member(state->name, pich);

	return false;
}

static bool print_event_enum_member(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO event       = (pID_INFO)         pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

	fprintf(pich->ih.fout
			, pich->ih.first ? (pich->ih.first = false, "\t %s") : "\t, %s"
			, pich->ih.pmi->name->name
			);
	fprintf(pich->ih.fout
			, assignExternalEventValues(pich->ih.pmi)
			  ? "_%s = %s\n"
			  : "_%s%s\n"
			, event->name
			, assignExternalEventValues(pich->ih.pmi)
			  ? event->type_data.event_data.externalDesignation->name
			  : ""
		   );
	return false;
}
static bool print_event_cross_reference(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO event       = (pID_INFO)         pelem->mbr;
   pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

   print_event_cross_reference_entry(event->name, pich);

   return false;
}

static bool print_sub_machine_event_cross_reference(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO pmi    = (pMACHINE_INFO)    pelem->mbr;
   pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

   pich->ih.pmi     = pmi;

   iterate_list(pmi->event_list, print_event_cross_reference, pich);

   if (
       !(pmi->modFlags & mfActionsReturnStates)
       && !(pmi->modFlags & mfActionsReturnVoid)
      )
   {
      print_event_cross_reference_entry("noEvent", pich);
   }

   if (pmi->machine_list)
   {
	   iterate_list(pmi->machine_list, print_sub_machine_event_cross_reference, pich);
	   pich->ih.pmi = pmi;
   }

   return false;
}

static void print_event_cross_reference_entry(char *event_name, pITERATOR_CALLBACK_HELPER pich)
{
   fprintf(pich->ih.fout
           , eventXRefFormat0Str
           , pich->ih.tab_level++
		   );
   printAncestry(pich->ih.pmi, pich->ih.fout, "_", alc_lower, ai_include_self);
   fprintf(pich->ih.fout
		   , eventXRefFormat1Str
           , event_name
           );
}

static bool declare_event_user_data_union_mbrs(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO         pevent = (pID_INFO) pelem->mbr;
   pITERATOR_CALLBACK_HELPER pich    = (pITERATOR_CALLBACK_HELPER) data;

   if (pevent->type_data.event_data.puser_event_data)
   {
      if (pevent->type_data.event_data.puser_event_data->data_fields)
      {
         print_tab_levels(pich->ih.fout, pich->ih.tab_level);
		 printAncestry(pich->ih.pmi, pich->ih.fout, "_", alc_upper, ai_include_self);
         fprintf(pich->ih.fout, "_");
		 streamHungarianToUnderbarCaps(pich->ih.fout, pevent->name);
         fprintf(pich->ih.fout
                 , "_DATA %s_data;\n"
                 , pevent->name
                 );
      }
   }

   return false;
}

static bool declare_event_user_data_structs(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO                  pevent  = (pID_INFO) pelem->mbr;
   pITERATOR_CALLBACK_HELPER pich    = (pITERATOR_CALLBACK_HELPER) data;

   FSMLANG_DEVELOP_PRINTF(pich->ih.fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   if (pevent->type_data.event_data.puser_event_data)
   {
      if (pevent->type_data.event_data.puser_event_data->data_fields)
      {
         fprintf(pich->ih.fout
				 , "typedef struct _%s_%s_data_ {\n"
				 , machineName(pich->pcmd)
				 , pevent->name
				 );

         pich->ih.tab_level = 1;
         iterate_list(pevent->type_data.event_data.puser_event_data->data_fields
                      , print_data_field
                      , pich
                      );

         fprintf(pich->ih.fout, "} ");
		 streamHungarianToUnderbarCaps(pich->ih.fout, pich->ih.pmi->name->name);
         fprintf(pich->ih.fout, "_");
		 streamHungarianToUnderbarCaps(pich->ih.fout, pevent->name);
         fprintf(pich->ih.fout, "_DATA, *p");
		 streamHungarianToUnderbarCaps(pich->ih.fout, pich->ih.pmi->name->name);
         fprintf(pich->ih.fout, "_");
		 streamHungarianToUnderbarCaps(pich->ih.fout, pevent->name);
         fprintf(pich->ih.fout, "_DATA;\n\n");
      }
   }

   return false;
}

static bool print_sub_machine_as_enum_member(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO pmi    = (pMACHINE_INFO) pelem->mbr;
   pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

   fprintf(pich->ih.fout
		   , "\t%s %s_e\n"
           , pich->ih.first ? "" : ", "
		   , pmi->name->name
		   );

   if (pich->ih.first)
   {
      pich->ih.first = false;

      fprintf(pich->ih.fout
			  , "\t, %s_firstSubMachine = %s_e\n"
			  , fqMachineName(pich->pcmd)
			  , pmi->name->name
			  );
   }

   return false;
}

static bool declare_sub_machine_if(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO             pmi  = (pMACHINE_INFO) pelem->mbr;
   pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;
   char                      *cp  = NULL;

   FSMLANG_DEVELOP_PRINTF(pich->ih.fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   fprintf(pich->ih.fout
		   , "extern %s %s_sub_fsm_if;\n"
		   , subFsmIfType(pich->pcmd)
		   , nfMachineNamePmi(pmi, &cp)
		   );

   if (cp)
   {
	   free (cp);
   }

   return false;
}

static void declare_parent_event_reference_data_structures(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   FSMLANG_DEVELOP_PRINTF(pcmd->subMachineHFile, "/* FSMLANG DEVELOP: %s */\n", __func__);

   if (pmi->data)
   {
	   fprintf(pcmd->subMachineHFile
			   , "typedef void (*%s)(p%s);\n"
			   , dataTranslationFnType(pcmd)
			   , fsmDataType(pcmd)
			   );
   }

   fprintf(pcmd->subMachineHFile
           ,"/* Some sub-machines share parent events. */\n"
           );

   fprintf(pcmd->subMachineHFile
		   , "typedef struct _%s_shared_event_str_ %s, *p%s;\n"
		   , machineName(pcmd)
		   , sharedEventStrType(pcmd)
		   , sharedEventStrType(pcmd)
		   );

   fprintf(pcmd->subMachineHFile
		   , "struct _%s_shared_event_str_\n{\n"
		   , machineName(pcmd)
		   );

   fprintf(pcmd->subMachineHFile
		   , "\t%-*sevent;\n"
		   , (int)pcmd->shared_event_str_format_width
		   , eventType(pcmd)
           );

   if (pmi->data)
   {
	   fprintf(pcmd->subMachineHFile
			   , "\t%-*sdata_translation_fn;\n"
			   , (int)pcmd->shared_event_str_format_width
			   , dataTranslationFnType(pcmd)
			   );
   }

   fprintf(pcmd->subMachineHFile
		   , "\tp%-*spsub_fsm_if;\n"
		   , (int)pcmd->shared_event_str_format_width - 1
		   , subFsmIfType(pcmd)
		   );

   fprintf(pcmd->subMachineHFile
           , "};\n"
           );

   fprintf(pcmd->subMachineHFile
		   , "extern %s %s_pass_shared_event(%s%s%sp%s[]);\n\n"
		   , eventType(pcmd)
		   , machineName(pcmd)
		   , pmi->data ? "p" : ""
		   , pmi->data ? fsmType(pcmd) : ""
		   , pmi->data ? "," : ""
		   , sharedEventStrType(pcmd)
           );

   ich.ih.fout = pcmd->subMachineHFile;
   ich.ih.pmi  = pmi;
   ich.pcmd = pcmd;

   iterate_list(pmi->event_list,declare_shared_event_lists,&ich);

   fprintf(pcmd->subMachineHFile
           , "\n"
           );
}

static bool define_shared_event_lists(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO pevent      = (pID_INFO)pelem->mbr;
	pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

	FSMLANG_DEVELOP_PRINTF(pich->ih.fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   if (pevent->type_data.event_data.psharing_sub_machines)
   {
      pich->ih.pid   = pevent;
      pich->ih.first = true;

      fprintf(pich->ih.fout, "p");
	  streamHungarianToUnderbarCaps(pich->ih.fout, pich->ih.pmi->name->name);
      fprintf(pich->ih.fout
			  , "_SHARED_EVENT_STR sharing_%s_%s[] =\n{\n"
              , pich->ih.pmi->name->name
              , pevent->name
              );

      iterate_list(pevent->type_data.event_data.psharing_sub_machines
				   , reference_shared_event_data_blocks
				   , pich
				   );

      fprintf(pich->ih.fout
              , "\t, NULL\n"
              );

      fprintf(pich->ih.fout
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
             , pich->ih.pmi->name->name
             , pid->name
             );
   }

   return false;
}

static bool reference_shared_event_data_blocks(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO             pmi  = (pMACHINE_INFO)             pelem->mbr;
   pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

   FSMLANG_DEVELOP_PRINTF(pich->ih.fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   fprintf(pich->ih.fout
           , "\t%s&"
           , pich->ih.first ? (pich->ih.first = false, "  ") : ", "
           );

   print_shared_event_data_block_signature(pich->ih.fout, pich->pcmd, pmi, pich->ih.pid->name, false /* do not include type information */);

   fprintf(pich->ih.fout, "\n");

   return false;
}

static void define_parent_event_reference_elements(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmd  = pcmd;
   ich.ih.pmi   = pmi;
   ich.ih.fout  = pcmd->cFile;
   ich.ih.first = true;

   /* define arrays */
   iterate_list(pmi->event_list, define_shared_event_lists, &ich);

   /* passing function */
   fprintf(pcmd->cFile
		   , "%s %s_pass_shared_event(%s%s%sp%s sharer_list[])\n{\n"
		   , eventType(pcmd)
		   , machineName(pcmd)
		   , pmi->data ? "p" : ""
		   , pmi->data ? fsmType(pcmd) : ""
		   , pmi->data ? " pfsm," : ""
		   , sharedEventStrType(pcmd)
           );

   fprintf(pcmd->cFile
		   , "\t%s return_event = THIS(noEvent);\n"
		   , eventType(pcmd)
           );

   fprintf(pcmd->cFile, "\tfor (p");
   streamHungarianToUnderbarCaps(pcmd->cFile, pmi->name->name);
   fprintf(pcmd->cFile
		   , "_SHARED_EVENT_STR *pcurrent_sharer = sharer_list;\n\t     *pcurrent_sharer && return_event == THIS(noEvent);\n\t     pcurrent_sharer++)\n\t{\n"
           );

   if (pmi->data)
   {
	   fprintf(pcmd->cFile
			   , "\t\tif ((*pcurrent_sharer)->data_translation_fn)\n"
			  );

	   fprintf(pcmd->cFile
			   , "\t\t\t(*(*pcurrent_sharer)->data_translation_fn)(&pfsm->data);\n"
			  );
   }

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

   print_transition_fn_declaration_for_when_actions_return_states(pich->ih.pmi, pich->pcmd->hFile, pid_info->name);

   return false;
}

bool declare_state_only_transition_functions_for_when_actions_return_states(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   print_state_only_transition_fn_declaration_for_when_actions_return_states(pich->ih.pmi, pich->pcmd->hFile, pid_info->name);

   return false;
}

static void print_transition_fn_declaration_for_when_actions_return_events(pCMachineData pcmd, FILE *fout, char *name)
{
	FSMLANG_DEVELOP_PRINTF(fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	fprintf(fout
			, "%s THIS(%s)(p%s,%s);\n"
			, stateType(pcmd)
			, name
			, fsmType(pcmd)
			, eventType(pcmd)
			);
}

static void print_state_only_transition_fn_declaration_for_when_actions_return_events(pCMachineData pcmd, FILE *fout, char *name)
{
	FSMLANG_DEVELOP_PRINTF(fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	fprintf(fout
			, "%s %s_transitionTo%s(p%s,%s);\n"
			, stateType(pcmd)
			, fqMachineName(pcmd)
			, name
			, fsmType(pcmd)
			, eventType(pcmd)
			);
}

bool declare_transition_fn_for_when_actions_return_events(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   print_transition_fn_declaration_for_when_actions_return_events(pich->pcmd, pich->pcmd->hFile, pid_info->name);

   return false;
}

bool declare_state_only_transition_functions_for_when_actions_return_events(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   print_state_only_transition_fn_declaration_for_when_actions_return_events(pich->pcmd, pich->pcmd->hFile, pid_info->name);

   return false;
}

static void print_entry_or_exit_fn_signature(pID_INFO pstate, pITERATOR_CALLBACK_HELPER pich, ENTRY_OR_EXIT which)
{
   pID_INFO fn        = which == eoe_entry ? pstate->type_data.state_data.entry_fn : pstate->type_data.state_data.exit_fn;
   char    *no_fn_str = which == eoe_entry ? "onEntryTo_"                          : "onExitFrom_";
   FILE    *file      = pich->define       ? pich->pcmd->cFile                     : pich->pcmd->hFile;
    
   FSMLANG_DEVELOP_PRINTF(file, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   fprintf(file
           , "void %s%s_%s%s(%s%s%s)%s\n"
           , pich->define                         ? "__attribute__((weak)) " : ""
		   , fqMachineName(pich->pcmd)
           , fn                                   ? fn->name                 : no_fn_str
           , fn                                   ? ""                       : pstate->name
           , pich->ih.pmi->data                   ? "p"                      : "void"
           , pich->ih.pmi->data                   ? fsmDataType(pich->pcmd)  : ""
           , (pich->ih.pmi->data && pich->define) ? " pdata"                 : ""
           , pich->define                         ? ""                       : ";"
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

static void print_entry_or_exit_fn_body(pITERATOR_CALLBACK_HELPER pich)
{

	FSMLANG_DEVELOP_PRINTF(pich->pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	fprintf(pich->pcmd->cFile
			,"{\n%s\tDBG_PRINTF(\"weak: %%s\", __func__);\n}\n\n"
			, pich->ih.pmi->data
			  ? "\t(void) pdata;\n\n"
			  : ""
			);
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
	  print_entry_or_exit_fn_body(pich);

   }

   if (pstate->type_data.state_data.state_flags & sfHasExitFn)
   {
      print_entry_or_exit_fn_signature(pstate, pich, eoe_exit);
	  print_entry_or_exit_fn_body(pich);
   }

   return false;
}

static void print_data_translator_fn_signature(FILE *fout, pCMachineData pcmd, pID_INFO pevent, DECLARE_OR_DEFINE dod)
{
	char *event_name_cp            = hungarianToUnderbarCaps(pevent->name);

	FSMLANG_DEVELOP_PRINTF(fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	if (pevent->type_data.event_data.puser_event_data && event_name_cp)
	{
		fprintf(fout
				, "void%sTHIS("
				, dod == dod_declare ? " " : " __attribute__((weak)) "
				);

		if (pevent->type_data.event_data.puser_event_data->translator)
		{
		   fprintf(fout
				   , "%s"
				   , pevent->type_data.event_data.puser_event_data->translator->name
				  );
		}
		else
		{
		   fprintf(fout
				   , "translate_%s_data"
				   , pevent->name
				   );
		}
		fprintf(fout
				, ")(p%s%s,p%s_%s_DATA%s)%s\n"
				, fsmDataType(pcmd)
				, dod == dod_declare ? "" : " pfsm_data"
				, fsmType(pcmd)
				, event_name_cp
				, dod == dod_declare ? "" : " pdata"
				, dod == dod_declare ? ";" : "\n{"
				);
	}

	CHECK_AND_FREE(event_name_cp);
}

/**
 * The data translator functions declared here are used by
 * a top-level machine which has data-bearing events.
 * 
 * @author sstan (12/10/2023)
 * 
 * @param pelem  the mbr field must point to an ID_INFO struct,
 *  			 which itself must represent an event
 * @param data   this must point to an ITERATOR_CALLBACK_HELPER
 *  			 struct
 * 
 * @return bool  always "false"
 */
bool declare_data_translator_functions(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pevent                = ((pID_INFO)pelem->mbr);

   FSMLANG_DEVELOP_PRINTF(pich->pcmd->hFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   print_data_translator_fn_signature(pich->pcmd->hFile, pich->pcmd, pevent, dod_declare);

   return false;
}

static void print_sub_machine_data_translator_fn_signature(FILE* file, pCMachineData pcmd, pID_INFO pevent, DECLARE_OR_DEFINE dod)
{
	if (pevent->type_data.event_data.puser_event_data)
	{
	   if (pevent->type_data.event_data.puser_event_data->translator)
	   {
		  fprintf(file
				  , "void%sTHIS(%s)(p%s%s)%s\n"
				   , dod == dod_declare ? " __attribute__((weak)) " : " "
				  , pevent->type_data.event_data.puser_event_data->translator->name
				  , fsmDataType(pcmd->parent_pcmd)
				  , dod == dod_declare ? "" : " pdata"
				  , dod == dod_declare ? ";" : "\n{"
				 );
	   }
	}
}

bool sub_machine_declare_data_translator_functions(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pevent                = ((pID_INFO)pelem->mbr);

   FSMLANG_DEVELOP_PRINTF(pich->pcmd->hFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   print_sub_machine_data_translator_fn_signature(pich->pcmd->hFile
												  , pich->pcmd
												  , pevent
												  , dod_declare
												  );

   return false;
}

bool define_weak_data_translator_functions(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pevent                = ((pID_INFO)pelem->mbr);

   FSMLANG_DEVELOP_PRINTF(pich->pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   if (pevent->type_data.event_data.puser_event_data)
   {
	   print_data_translator_fn_signature(pich->pcmd->cFile, pich->pcmd, pevent, dod_define);

	   fprintf(pich->pcmd->cFile
			   , "\t(void) pfsm_data;\n\t(void) pdata;\n\n\t%s(\"weak: %%s\", __func__);\n}\n\n"
			   , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
			  );
   }

   return false;
}

bool sub_machine_define_weak_data_translator_functions(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pevent                = ((pID_INFO)pelem->mbr);

   FSMLANG_DEVELOP_PRINTF(pich->pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   if (pevent->type_data.event_data.puser_event_data)
   {
	   print_sub_machine_data_translator_fn_signature(pich->pcmd->cFile, pich->pcmd, pevent, dod_define);

	   fprintf(pich->pcmd->cFile
			   , "\t(void) pdata;\n\n\t%s(\"weak: %%s\", __func__);\n}\n\n"
			   , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
			  );
   }

   return false;
}

static void print_native_header(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	FILE *f_array[] = {
		pcmd->hFile
		, pcmd->subMachineHFile
		, pcmd->pubHFile
	};

	for (unsigned long f_iterator = 0; f_iterator < sizeof(f_array)/sizeof(f_array[0]); f_iterator++)
	{
		fprintf(f_array[f_iterator]
				, "#ifndef %s_NATIVE\n#define %s_NATIVE\n%s\n#endif\n"
				, fsmType(pcmd)
				, fsmType(pcmd)
				, pmi->native
				);
	}
}

void subMachineHeaderStart(pCMachineData pcmd, pMACHINE_INFO pmi, char *arrayName)
{
	(void) arrayName;
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.ih.fout      = pcmd->hFile;
   ich.ih.pmi       = pmi;

   /* put the native code segment out to the header */
   if (pmi->native) print_native_header(pcmd, pmi);

   fprintf(pcmd->hFile
		   , "#ifdef %s_DEBUG\n"
		   , fsmType(pcmd)
		   );
   fprintf(pcmd->hFile, "#include <stdio.h>\n");
   fprintf(pcmd->hFile, "#include <stdlib.h>\n");
   fprintf(pcmd->hFile, "#endif\n\n");

   if (pmi->has_single_pai_events)
   {
      fprintf(pcmd->hFile, "#include <stdbool.h>\n");
   }

   if (core_logging_only)
   {
      fprintf(pcmd->hFile, "#ifndef NON_CORE_DEBUG_PRINTF\n#define NON_CORE_DEBUG_PRINTF(...) \n#endif\n\n");
   }

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

   fprintf(pcmd->hFile
		   , "#undef THIS\n#define THIS(A) %s_##A\n"
		   , fqMachineName(pcmd)
		   );

   fprintf(pcmd->hFile
		   , "#undef PARENT\n#define PARENT(A) %s_##A\n"
		   , fqMachineName(pcmd->parent_pcmd)
		   );

   fprintf(pcmd->hFile
           , "#endif\n"
           );

   fprintf(pcmd->hFile
		   , "\n#ifdef %s_DEBUG\n"
		   , fsmType(pcmd)
		   );
   fprintf(pcmd->hFile
		   , "extern char *%s_EVENT_NAMES[];\n"
		   , fsmType(pcmd)
		   );
   fprintf(pcmd->hFile
		   , "extern char *%s_STATE_NAMES[];\n"
		   , fsmType(pcmd)
		   );
   fprintf(pcmd->hFile, "#endif\n\n");

   /* put the state enum into the header file */
   fprintf(pcmd->hFile
           , "typedef enum {\n"
          );


   ich.ih.first = true;
   iterate_list(pmi->state_list, write_state_enum_member, &ich);

   /*
     Though not a state, this needs have a value
       different from any state. Moreover, it could
       be said to bear the same relation to states
       as noEvent does to events.
   */
   if (pmi->modFlags & mfActionsReturnStates)
   {
	   print_plain_enum_member("noTransition", &ich);
   }

   print_plain_enum_member("numStates", &ich);

   fprintf(pcmd->hFile
           , "}%s %s_STATE;\n\n"
           , compact_action_array ? " __attribute__((__packed__))" : " "
		   , fsmType(pcmd)
          );

   /* put the data struct typedef into the header file */
   if (pmi->data)
   {
      fprintf(pcmd->hFile
			  , "typedef struct _%s_data_struct_ %s, *p%s;\n"
			  , pmi->name->name
			  , fsmDataType(pcmd)
			  , fsmDataType(pcmd)
			  );

   }

   /* put the machine struct typedef into the header */
   fprintf(pcmd->hFile
		   , "typedef struct _%s_struct_ %s, *p%s;\n"
		   , pmi->name->name
		   , fsmType(pcmd)
		   , fsmType(pcmd)
		   );

   fprintf(pcmd->hFile
		   , "#undef FSM_TYPE_PTR\n#define FSM_TYPE_PTR p%s\n"
		   , fsmType(pcmd)
		   );

   if (generate_instance)
   {
      fprintf(pcmd->hFile
			  , "extern %s %s;\n\n"
			  , fsmType(pcmd)
			  , machineName(pcmd)
			  );

      fprintf(pcmd->hFile
			  , "extern p%s p%s;\n\n"
			  , fsmType(pcmd)
			  , machineName(pcmd)
			  );

   }

   /* put the action function typedef into the header */
   fprintf(pcmd->hFile
		   , "typedef %s (*%s)(FSM_TYPE_PTR);\n\n"
		   , actionReturnType(pcmd)
		   , actionFnType(pcmd)
		   );

   /* typedef transition functions, if we have any */
   if (pmi->transition_fn_list->count)
   {
	   fprintf(pcmd->hFile
			   , "typedef %s (*%s)(FSM_TYPE_PTR,%s);\n\n"
			   , stateType(pcmd)
			   , transitionFnType(pcmd)
			   , eventType(pcmd)
			   );
   }

   /* typedef the FSM function */
   fprintf(pcmd->hFile
		   , "typedef %s (*%s_FSM)(FSM_TYPE_PTR,%s);\n\n"
		   , actionReturnType(pcmd)
		   , fsmType(pcmd)
		   , fsmFnEventType(pcmd)
		   );

   /* TODO: move this to source file */
   fprintf(pcmd->cFile
           , "static %s %sFSM(FSM_TYPE_PTR,%s);\n\n"
		   , actionReturnType(pcmd)
		   , machineName(pcmd)
		   , fsmFnEventType(pcmd)
           );

   /* put the data structure definition into the header */
   if (pmi->data)
   {
      fprintf(pcmd->hFile
			  , "struct _%s_data_struct_ {\n"
			  , machineName(pcmd)
			  );

      ich.ih.tab_level = 1;
      iterate_list(pmi->data, print_data_field, &ich);

      fprintf(pcmd->hFile 
              , "};\n\n"
              );
   }

   if (pmi->machine_list)
   {
	   /* The sub-machine header will hold things needed by the private header. */
	   fprintf(pcmd->hFile
			   ,"#include \"%s\"\n"
			   , pcmd->subMachineHName
			   );

	   printSubMachinesDeclarations(pcmd, pmi);
   }
   

}

void print_transition_fn_declaration_for_when_actions_return_states(pMACHINE_INFO pmi, FILE *fout, char *name)
{
	FSMLANG_DEVELOP_PRINTF(fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

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
	FSMLANG_DEVELOP_PRINTF(fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	streamHungarianToUnderbarCaps(fout, pmi->name->name);
	fprintf(fout, "_STATE ");
	printAncestry(pmi, fout, "_", alc_lower, ai_include_self);
	fprintf(fout, "_transitionTo%s(p"
			, name
			);
	printAncestry(pmi, fout, "_", alc_upper, ai_include_self);
	fprintf(fout, ");\n");
}

/**
 * This function defines "upward looking" structures used by a
 * parent machine to share events to a sub-machine.
 * 
 * @author sstan (12/10/2023)
 * 
 * @param pelem  this must point to a mbr element of type
 *  			 pID_INFO, which itself points to an event.
 * @param data   this must point to an ITERATOR_CALLBACK_HELPER
 *  			 structure.
 * 
 * @return bool always "false"
 */
static bool define_needed_shared_event_structures(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pevent      = (pID_INFO) pelem->mbr;
   pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

   FSMLANG_DEVELOP_PRINTF(pich->ih.fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   if (pevent->type_data.event_data.shared_with_parent)
   {

	   print_shared_event_data_block_signature(pich->ih.fout
											   , pich->pcmd->parent_pcmd
											   , pich->ih.pmi
											   , pevent->name
											   , true /* include type information */
											   );

	   fprintf(pich->ih.fout, " = {\n");

      fprintf(pich->ih.fout
              , "\t  .event               = THIS(%s)\n"
              , pevent->name
              );

	  if (pevent->type_data.event_data.puser_event_data)
	  {
		  fprintf(pich->ih.fout
				  , "\t, .data_translation_fn = THIS(%s)\n"
				  , pevent->type_data.event_data.puser_event_data->translator->name
				  );

	  }

      fprintf(pich->ih.fout
			  , "\t, .psub_fsm_if         = &%s_sub_fsm_if\n"
			  , nfMachineName(pich->pcmd)
			  );

      fprintf(pich->ih.fout, "};\n\n");

   }

   return false;
}

void possiblyDefineSubMachineSharedEventStructures (pCMachineData pcmd, pMACHINE_INFO pmi)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   if (pmi->shared_event_count)
   {
      ich.ih.pmi  = pmi;
	  ich.pcmd = pcmd;
      ich.ih.fout = pcmd->cFile;

      iterate_list(pmi->event_list, define_needed_shared_event_structures, &ich);
   }

}

void defineSubMachineIF (pCMachineData pcmd, pMACHINE_INFO pmi)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   fprintf(pcmd->cFile
           , "%s "
		   , eventType(pcmd)
 		  );
   printAncestry(pmi, pcmd->cFile, "_", alc_lower, ai_include_self);
   fprintf(pcmd->cFile, "_sub_machine_fn(");
   fprintf(pcmd->cFile
 		  , "%s e)\n{\n"
		   , eventType(pcmd)
           );

   fprintf(pcmd->cFile
		   , "\treturn %sFSM(p%s,e);\n}\n\n"
		   , machineName(pcmd)
		   , machineName(pcmd)
		   );

   fprintf(pcmd->cFile
		   , "%s %s_sub_fsm_if =\n{\n\t"
		   , subFsmIfType(pcmd->parent_pcmd)
		   , nfMachineName(pcmd)
		   );

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
	FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   ITERATOR_CALLBACK_HELPER ich = { 0 };

   if (pmi->machine_list)
   {
      fprintf(pcmd->cFile
			   , "\nconst p%s %s_sub_fsm_if_array[THIS(numSubMachines)] =\n{\n"
			   , subFsmIfType(pcmd)
			   , machineName(pcmd)
			  );

      ich.pcmd  = pcmd;
      ich.ih.pmi   = pmi;
      ich.ih.first = true;
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

void print_action_function_declaration(pCMachineData pcmd, char *name)
{
	fprintf(pcmd->hFile
			, "ACTION_RETURN_TYPE %s_%s(FSM_TYPE_PTR);\n"
			, fqMachineName(pcmd)
			, name
			);
}

bool declare_action_function(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   if (pid_info->name && strlen(pid_info->name))
   {
	   print_action_function_declaration(pich->pcmd, pid_info->name);
   }

   return false;
}

bool print_sub_machine_if(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO pmi              = (pMACHINE_INFO) pelem->mbr;
   pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;
   char                      *cp  = NULL;

   FSMLANG_DEVELOP_PRINTF(pich->pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   fprintf(pich->pcmd->cFile
           , "\t%s&%s_sub_fsm_if\n"
           , pich->ih.first ? (pich->ih.first = false, "") : ", "
		   , nfMachineNamePmi(pmi, &cp)
          );

   if (cp)
   {
	   free(cp);
   }

   return false;
}

void print_weak_action_function_body_omitting_return_statement(pCMachineData pcmd, char *name)
{
	fprintf(pcmd->cFile
			, "%s __attribute__((weak)) THIS(%s)(FSM_TYPE_PTR pfsm)\n{\n"
			, actionReturnType(pcmd)
			, name
		   );

	fprintf(pcmd->cFile
			, "\t%s(\"weak: %%s\", __func__);\n"
			, core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
			);

	fprintf(pcmd->cFile
			, "\t(void) pfsm;\n"
			);

}

bool define_weak_action_function(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   FSMLANG_DEVELOP_PRINTF(pich->pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   if (pid_info->name && strlen(pid_info->name))
   {
	   print_weak_action_function_body_omitting_return_statement(pich->pcmd, pid_info->name);

       if (pich->ih.pmi->modFlags & mfActionsReturnStates)
       {

          fprintf(pich->pcmd->cFile
				  , "\treturn %s_noTransition;\n"
				  , machineName(pich->pcmd)
				  );
       }
       else
       {

          /* if this action is associated with a shared event, it will have exactly one event */
          pID_INFO pevent = (pID_INFO)find_nth_list_member(pid_info->actionInfo->matrix->event_list,0);

          /* and, that event will have a list of sharing machines */
          if (pevent->type_data.event_data.psharing_sub_machines)
          {
             fprintf(pich->pcmd->cFile
                     , "\t%s%s_pass_shared_event(%ssharing_%s_%s);\n"
					 , pich->ih.pmi->modFlags & mfActionsReturnVoid ? "" : "return "
					 , machineName(pich->pcmd)
					 , pich->ih.pmi->data ? "pfsm, " : ""
					 , machineName(pich->pcmd)
					 , pevent->name
					 );
          }
          else
          {
			  if (!(pich->ih.pmi->modFlags & mfActionsReturnVoid))
			  {
				  fprintf(pich->pcmd->cFile
						  , "\treturn THIS(noEvent);\n"
						 );
			  }
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

   FSMLANG_DEVELOP_PRINTF(pich->pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   if (pid_info->name && strlen(pid_info->name))
   {
      /* if this action is associated with a shared event, it will have exactly one event */
      pID_INFO pevent = (pID_INFO)find_nth_list_member(pid_info->actionInfo->matrix->event_list,0);

      /* and, that event will have a list of sharing machines */
      if (pevent->type_data.event_data.psharing_sub_machines)
      {
         fprintf(pich->pcmd->cFile
                 , "%s THIS(%s)(p%s pfsm)\n\{\n"
				 , eventType(pich->pcmd)
				 , pid_info->name
				 , fsmType(pich->pcmd)
                );

         fprintf(pich->pcmd->cFile
                 , "\t%s(\"weak: %%s\", __func__);\n"
                 , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                );

         fprintf(pich->pcmd->cFile
                 , "\treturn %s_pass_shared_event(%ssharing_%s_%s);\n}\n\n"
                 , machineName(pich->pcmd)
				 , pich->ih.pmi->data ? "pfsm, " : ""
                 , machineName(pich->pcmd)
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
   ich.ih.pmi       = pmi;

   iterate_list(pmi->action_list
                , define_event_passing_actions
                , &ich
                );
}

void defineWeakDataTranslatorStubs(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	ITERATOR_CALLBACK_HELPER ich = { 0 };

	ich.pcmd      = pcmd;
	ich.ih.pmi       = pmi;

	iterate_list(pmi->event_list
				 , define_weak_data_translator_functions
				 , &ich
				 );

	fprintf(pcmd->hFile, "\n");
}

void defineSubMachineWeakDataTranslatorStubs(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	ITERATOR_CALLBACK_HELPER ich = { 0 };

	ich.pcmd      = pcmd;
	ich.ih.pmi       = pmi;

	iterate_list(pmi->event_list
				 , sub_machine_define_weak_data_translator_functions
				 , &ich
				 );

	fprintf(pcmd->hFile, "\n");
}

static void defineSubMachineInhibitor(pCMachineData pcmd, pMACHINE_INFO pmi)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmd      = pcmd;
   ich.ih.pmi       = pmi;
   ich.counter   = 0;

   FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

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

   FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   fprintf(pcmd->cFile
           , "\nstatic %s findAndRunSubMachine(p%s pfsm, %s e)\n{\n"
		   , eventType(pcmd)
		   , fsmType(pcmd)
		   , eventType(pcmd)
          );

   fprintf(pcmd->cFile
		   , "\tfor (%s_SUB_MACHINES machineIterator = THIS(firstSubMachine);\n\t     machineIterator < THIS(numSubMachines);\n\t     machineIterator++\n\t    )\n\t{\n"
		   , fsmType(pcmd)
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
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER) data      );

   FSMLANG_DEVELOP_PRINTF(pich->ih.fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   fprintf(pich->ih.fout, "#undef ");
   printAncestry(pmi, pich->ih.fout, "_", alc_upper, ai_include_self | ai_omit_ultimate);
   fprintf(pich->ih.fout, "\n#define ");
   printAncestry(pmi, pich->ih.fout, "_", alc_upper, ai_include_self | ai_omit_ultimate);
   fprintf(pich->ih.fout, "(A) ");
   printAncestry(pmi, pich->ih.fout, "_", alc_lower, ai_include_self);
   fprintf(pich->ih.fout, "_##A\n");

   if (pmi->machine_list)
   {
	   iterate_list(pmi->machine_list, print_event_macro, data);
   }

   return false;

}

static void print_shared_event_data_block_signature(FILE *file, pCMachineData pcmd, pMACHINE_INFO pmi, char *event_name, bool include_type)
{
	FSMLANG_DEVELOP_PRINTF(file, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	if (include_type)
	{
		fprintf(file
				, "%s "
				, sharedEventStrType(pcmd)
				);
	}

	fprintf(file
			, "%s_share_%s_%s_str"
			, pmi->name->name
			, machineName(pcmd)
			, event_name
			);

}

static bool declare_shared_event_data_blocks(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO pmi    = (pMACHINE_INFO)pelem->mbr;
   pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

   fprintf(pich->ih.fout, "extern ");

   print_shared_event_data_block_signature(pich->ih.fout, pich->pcmd, pmi, pich->ih.pid->name, true /* include type information */);

   fprintf(pich->ih.fout, ";\n");

   return false;
}

static bool declare_shared_event_lists(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO                  pevent = (pID_INFO)pelem->mbr;
   pITERATOR_CALLBACK_HELPER pich    = (pITERATOR_CALLBACK_HELPER) data;

   FSMLANG_DEVELOP_PRINTF(pich->ih.fout, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   if (pevent->type_data.event_data.psharing_sub_machines)
   {

      pich->ih.pid = pevent;
      iterate_list(pevent->type_data.event_data.psharing_sub_machines
				   , declare_shared_event_data_blocks
				   , pich
				  );

      fprintf(pich->ih.fout
			  , "extern p%s sharing_%s_%s[];\n\n"
			  , sharedEventStrType(pich->pcmd)
			  , machineName(pich->pcmd)
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
              , "%s\n\n"
              , pmi->native_impl
              );
   }
}

void declareSubMachineManagers(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   fprintf(pcmd->cFile
		   , "static %s findAndRunSubMachine(p%s,%s);\n"
		   , eventType(pcmd)
		   , fsmType(pcmd)
		   , eventType(pcmd)
		  );

   if (pmi->submachine_inhibitor_count)
   {
      fprintf(pcmd->cFile
			  , "static bool doNotInhibitSubMachines(%s);\n"
			  , stateType(pcmd)
			  );
   }

   fprintf(pcmd->cFile, "\n");
}

void declareEventDataManager(pCMachineData pcmd)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   fprintf(pcmd->cFile
		   , "static void translateEventData(p%s,%s);\n\n"
		   , fsmDataType(pcmd)
		   , fsmFnEventType(pcmd)
		   );
}

static bool write_event_data_manager_switch_case(pLIST_ELEMENT pelem, void *data)
{
   pID_INFO pevent                = (pID_INFO)pelem->mbr;
   pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

   FSMLANG_DEVELOP_PRINTF(pich->pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   if (pevent->type_data.event_data.puser_event_data)
   {
      pich->counter++;
      fprintf(pich->pcmd->cFile
              , "\tcase %s_%s:\n"
              , pich->ih.pmi->name->name
              , pevent->name
              );

      fprintf(pich->pcmd->cFile
              , pevent->type_data.event_data.puser_event_data->translator
                 ? "\t\tTHIS(%s)(pfsm_data, &pevent->event_data.%s_data);\n\t\tbreak;\n"
                 : "\t\tTHIS(translate_%s_data)(pfsm_data, &pevent->event_data.%s_data);\n\t\tbreak;\n"
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
   ich.ih.pmi     = pmi;

   FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   fprintf(pcmd->cFile
		   , "static void translateEventData(p%s pfsm_data, %s pevent)\n{\n"
		   , fsmDataType(pcmd)
		   , fsmFnEventType(pcmd)
		   );

   fprintf(pcmd->cFile
           , "\tswitch(pevent->event)\n\t{\n"
           );

   iterate_list(pmi->event_list, write_event_data_manager_switch_case, &ich);

   fprintf(pcmd->cFile
           , "\tdefault:\n\t\tbreak;\n\t}\n\n}\n\n"
           );
}

static void print_state_entry_or_exit_manager_signature(pCMachineData pcmd, pMACHINE_INFO pmi,ENTRY_OR_EXIT eoe, DECLARE_OR_DEFINE dod)
{
	fprintf(pcmd->cFile
			,"static void runAppropriate%sFunction(%s%s%s%s%s%s)%s\n"
			, eoe == eoe_entry ? "Entry" : "Exit"
			, pmi->data ? "p" : ""
			, pmi->data ? fsmDataType(pcmd) : ""
			, ((dod == dod_define) && pmi->data) ? " pdata" : ""
			, pmi->data ? "," : ""
			, stateType(pcmd)
			, dod == dod_define ? " s" : ""
			, dod == dod_define ? "\n{\n\tswitch(s)\n\t{" : ";"
			);
}

void declareStateEntryAndExitManagers(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   if (pmi->states_with_entry_fns_count)
   {
	   print_state_entry_or_exit_manager_signature(pcmd, pmi, eoe_entry, dod_declare);
   }
   if (pmi->states_with_exit_fns_count)
   {
	   print_state_entry_or_exit_manager_signature(pcmd, pmi, eoe_exit, dod_declare);
   }
}

static void print_state_entry_or_exit_fn_switch_case(pID_INFO pstate, pITERATOR_CALLBACK_HELPER pich, ENTRY_OR_EXIT which)
{
    pID_INFO fn        = which == eoe_entry ? pstate->type_data.state_data.entry_fn : pstate->type_data.state_data.exit_fn;
    char    *no_fn_str = which == eoe_entry ? "onEntryTo_"                          : "onExitFrom_";

	FSMLANG_DEVELOP_PRINTF(pich->pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

	pich->counter++;
	fprintf(pich->pcmd->cFile
			, "\tcase %s_%s:\n\t\t"
			, machineName(pich->pcmd)
			, pstate->name
			);

	fprintf(pich->pcmd->cFile
			, "%s_%s%s(%s);\n\t\tbreak;\n"
			, fqMachineName(pich->pcmd)
			, fn              ? fn->name : no_fn_str
			, fn              ? ""       : pstate->name
			, pich->ih.pmi->data ? "pdata"  : ""
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

   ich.ih.pmi  = pmi;
   ich.pcmd = pcmd;

   FSMLANG_DEVELOP_PRINTF(pcmd->cFile, "/* FSMLANG_DEVELOP: %s */\n", __func__);

   if (pmi->states_with_entry_fns_count)
   {
	   print_state_entry_or_exit_manager_signature(pcmd, pmi, eoe_entry, dod_define);

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
	   print_state_entry_or_exit_manager_signature(pcmd, pmi, eoe_exit, dod_define);

      iterate_list(pmi->state_list, write_state_exit_fn_switch_case, &ich);

      if (pmi->states_with_exit_fns_count < pmi->state_list->count)
      {
         fprintf(pcmd->cFile
                 , "\tdefault:\n\t\tbreak;\n\t}\n}\n\n"
                 );
      }
   }
}

/**
 * These are downward looking
 * 
 * @author sstan (12/3/2023)
 * 
 * @param pcmd   the current writer's data structure
 * @param pmi    the current machine's data structure
 */
void printSubMachinesDeclarations(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->subMachineHFile , "/* FSMLANG_DEVELOP: %s */\n", __func__ );

	ITERATOR_CALLBACK_HELPER ich = { 0 };

	ich.pcmd  = pcmd;
	ich.ih.pmi   = pmi;
	ich.ih.first = true;

	fprintf(pcmd->subMachineHFile,"/* Sub Machine Declarations */\n\n");
	fprintf(pcmd->subMachineHFile, "typedef enum {\n");

	ich.ih.fout  = pcmd->subMachineHFile;
	iterate_list(pmi->machine_list
				 , print_sub_machine_as_enum_member
				 ,&ich
				 );

	fprintf(pcmd->subMachineHFile
			, "\t, %s_numSubMachines\n} %s_SUB_MACHINES;\n\n"
			, fqMachineName(pcmd)
			, fsmType(pcmd)
			);

	fprintf(pcmd->subMachineHFile
			, "typedef %s (*%s)(%s);\n"
			, actionReturnType(pcmd)
			, subMachineFnType(pcmd)
			, actionReturnType(pcmd)
			);

	fprintf(pcmd->subMachineHFile
			, "typedef struct _%s_sub_fsm_if_ %s, *p%s;\n"
			, nfMachineName(pcmd)
			, subFsmIfType(pcmd)
			, subFsmIfType(pcmd)
			);

	fprintf(pcmd->subMachineHFile
			,"struct _%s_sub_fsm_if_\n{\n"
			, nfMachineName(pcmd)
			);

	fprintf(pcmd->subMachineHFile
			, "\t%-*sfirst_event;\n"
			, (int)pcmd->sub_fsm_if_format_width
			, eventType(pcmd)
			);

	fprintf(pcmd->subMachineHFile
			, "\t%-*slast_event;\n"
			, (int)pcmd->sub_fsm_if_format_width
			, eventType(pcmd)
			);

	fprintf(pcmd->subMachineHFile
			, "\t%-*ssubFSM;\n"
			, (int)pcmd->sub_fsm_if_format_width
			, subMachineFnType(pcmd)
			);

	fprintf(pcmd->subMachineHFile
			,"};\n\n"
			);

	ich.ih.fout  = pcmd->cFile;
	iterate_list(pmi->machine_list, declare_sub_machine_if, &ich);

	fprintf(pcmd->cFile
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

    fprintf(pcmd->cFile
            , "%s_EVENT_NAMES[%s]\n,%s_STATE_NAMES[pfsm->state]\n);\n}\n#endif\n\n"
			, fsmType(pcmd)
            , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
			, fsmType(pcmd)
           );

}

void printFSMSubMachineDebugBlock(pCMachineData pcmd, pMACHINE_INFO pmi)
{
	fprintf(pcmd->cFile
			, "#ifdef %s_DEBUG\n"
			, fsmType(pcmd)
			);
	fprintf(pcmd->cFile
			, "if (EVENT_IS_NOT_EXCLUDED_FROM_LOG(%s))\n{\n"
			, (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
			);

    fprintf(pcmd->cFile, "\tDBG_PRINTF(\"");

    if (short_dbg_names && add_machine_name)
    {
       fprintf(pcmd->cFile
			   , "%s: "
               , pmi->name->name
           );
    }

    fprintf(pcmd->cFile, "event: %%s; state: %%s\"\n,");
    fprintf(pcmd->cFile
			, "%s_EVENT_NAMES[%s - THIS(%s)]\n,%s_STATE_NAMES[pfsm->state]\n);\n}\n#endif\n\n"
			, fsmType(pcmd)
			, (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
			, eventNameByIndex(pmi,0)
			, fsmType(pcmd)
			);

}


