#define FSMLANG_DEVELOP
/**
  fsm_c.c

    Creates C code to implement FSM


    FSMLang (fsm) - A Finite State Machine description language.
    Copyright (C) 2002  Steven Stanton

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
    ringwinner@users.sourceforge.net

    For the latest on FSMLang: http://fsmlang.sourceforge.net

    And, finally, your possession of this source code implies nothing.

  Long Description:

  Creation: 	sstanton		Jan-21-2002
*/

#include "fsm_c.h"
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

typedef struct _fsm_c_output_generator_             FSMCOutputGenerator, *pFSMCOutputGenerator;
typedef struct _fsm_c_sub_machine_output_generator_ FSMCSubMachineOutputGenerator, *pFSMCSubMachineOutputGenerator;
typedef struct _c_machine_data_                     CMachineData,        *pCMachineData;

struct _c_machine_data_
{
   FILE	*cFile
      ,	*hFile
      ;

   char	*cName
      ,	*hName
      ;
};

struct _fsm_c_output_generator_
{
   FSMOutputGenerator fsmog;
   pCMachineData      pcmd;
};

struct _fsm_c_sub_machine_output_generator_
{
   FSMOutputGenerator   fsmog;
   pCMachineData        pcmd;
   pFSMCOutputGenerator parent_fsmcog;
};


/*
  Our interface to the outside world
*/
static int  initCMachine(pFSMOutputGenerator,char *);
static int  initCSubMachine(pFSMOutputGenerator,char *);
static void writeCMachine(pFSMOutputGenerator,pMACHINE_INFO);
static void writeCSubMachine(pFSMOutputGenerator,pMACHINE_INFO);
static void writeCSwitchMachine(pFSMOutputGenerator,pMACHINE_INFO);
static void closeCMachine(pFSMOutputGenerator,int);

FSMCOutputGenerator CMachineWriter = {
   {
      initCMachine,
      writeCMachine,
      closeCMachine
   },
   NULL
};

FSMCSubMachineOutputGenerator CSubMachineWriter = {
   {
      initCSubMachine,
      writeCSubMachine,
      closeCMachine
   },
   NULL,
   NULL
};

FSMCOutputGenerator CSwitchMachineWriter = {
   {
      initCMachine,
      writeCSwitchMachine,
      closeCMachine
   },
   NULL
};

pFSMOutputGenerator pCMachineWriter       = (pFSMOutputGenerator) &CMachineWriter;
pFSMOutputGenerator pCSubMachineWriter    = (pFSMOutputGenerator) &CSubMachineWriter;
pFSMOutputGenerator pCSwitchMachineWriter = (pFSMOutputGenerator) &CSwitchMachineWriter;

/* list iteration callbacks */
typedef struct _iterator_callback_helper_ ITERATOR_CALLBACK_HELPER, *pITERATOR_CALLBACK_HELPER;
struct _iterator_callback_helper_
{
   pCMachineData pcmw;
   pMACHINE_INFO pmi;
   char          *cp;
   char          *parent_cp;
   bool          needNoOp;
   bool          first;
};

static bool declare_action_function(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   if (pid_info->name && strlen(pid_info->name))
   {

      if (pich->pmi->modFlags & mfActionsReturnVoid)
      {
         fprintf(pich->pcmw->hFile
                 , "void %s_%s(p%s);\n"
                 , pich->pmi->name->name
                 , pid_info->name
                 , pich->cp
                );
      }
      else
      {
         if (pich->pmi->modFlags & mfActionsReturnStates)
         {
            fprintf(pich->pcmw->hFile
                    , "%s_STATE %s_%s(p%s);\n"
                    , pich->cp
                    , pich->pmi->name->name
                    , pid_info->name
                    , pich->cp
                   );
         }
         else
         {
            fprintf(pich->pcmw->hFile
                    , "%s_EVENT %s_%s(p%s);\n"
                    , pich->cp
                    , pich->pmi->name->name
                    , pid_info->name
                    , pich->cp
                   );
         }
      }

   }

   return false;
}

static bool define_weak_action_function(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   if (pid_info->name && strlen(pid_info->name))
   {

      if (pich->pmi->modFlags & mfActionsReturnVoid)
      {
         fprintf(pich->pcmw->cFile
                 , "void __attribute__((weak)) %s_%s(p%s pfsm)\n{\n"
                 , pich->pmi->name->name
                 , pid_info->name
                 , pich->cp
                );

         fprintf(pich->pcmw->cFile
                 , "\tDBG_PRINTF(\"weak: %s_%s\");\n"
                 , pich->pmi->name->name
                 , pid_info->name
                 );

      }
      else
      {
         if (pich->pmi->modFlags & mfActionsReturnStates)
         {
            fprintf(pich->pcmw->cFile
                    , "%s_STATE __attribute__((weak)) %s_%s(p%s pfsm)\n{\n"
                    , pich->cp
                    , pich->pmi->name->name
                    , pid_info->name
                    , pich->cp
                   );

            fprintf(pich->pcmw->cFile
                    , "\tDBG_PRINTF(\"weak: %s_%s\");\n"
                    , pich->pmi->name->name
                    , pid_info->name
                    );

            fprintf(pich->pcmw->cFile
                    , "\treturn %s_noTransition;\n"
                    , pich->pmi->name->name
                    );

         }
         else
         {
            fprintf(pich->pcmw->cFile
                    , "%s_EVENT __attribute__((weak)) %s_%s(p%s pfsm)\n{\n"
                    , pich->cp
                    , pich->pmi->name->name
                    , pid_info->name
                    , pich->cp
                   );

            fprintf(pich->pcmw->cFile
                    , "\tDBG_PRINTF(\"weak: %s_%s\");\n"
                    , pich->pmi->name->name
                    , pid_info->name
                    );

            fprintf(pich->pcmw->cFile
                    , "\treturn %s_noEvent;\n"
                    , pich->pmi->name->name
                    );

         }
      }

      fprintf(pich->pcmw->cFile
              , "}\n\n"
              );

   }

   return false;
}

static bool define_sub_machine_weak_action_function(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   if (pid_info->name && strlen(pid_info->name))
   {

      if (pich->pmi->modFlags & mfActionsReturnVoid)
      {
         fprintf(pich->pcmw->cFile
                 , "void __attribute__((weak)) %s_%s(p%s pfsm)\n{\n"
                 , pich->pmi->name->name
                 , pid_info->name
                 , pich->cp
                );

         fprintf(pich->pcmw->cFile
                 , "\tDBG_PRINTF(\"weak: %s_%s\");\n"
                 , pich->pmi->name->name
                 , pid_info->name
                 );

      }
      else
      {
         if (pich->pmi->modFlags & mfActionsReturnStates)
         {
            fprintf(pich->pcmw->cFile
                    , "%s_STATE __attribute__((weak)) %s_%s(p%s pfsm)\n{\n"
                    , pich->cp
                    , pich->pmi->name->name
                    , pid_info->name
                    , pich->cp
                   );

            fprintf(pich->pcmw->cFile
                    , "\tDBG_PRINTF(\"weak: %s_%s\");\n"
                    , pich->pmi->name->name
                    , pid_info->name
                    );

            fprintf(pich->pcmw->cFile
                    , "\treturn %s_noTransition;\n"
                    , pich->pmi->name->name
                    );

         }
         else
         {
            fprintf(pich->pcmw->cFile
                    , "%s_EVENT __attribute__((weak)) %s_%s(p%s pfsm)\n{\n"
                    , pich->parent_cp
                    , pich->pmi->name->name
                    , pid_info->name
                    , pich->cp
                   );

            fprintf(pich->pcmw->cFile
                    , "\tDBG_PRINTF(\"weak: %s_%s\");\n"
                    , pich->pmi->name->name
                    , pid_info->name
                    );

            fprintf(pich->pcmw->cFile
                    , "\treturn %s_noEvent;\n"
                    , pich->pmi->name->name
                    );

         }
      }

      fprintf(pich->pcmw->cFile
              , "}\n\n"
              );

   }

   return false;
}

static bool declare_sub_machine_action_function(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   if (pid_info->name && strlen(pid_info->name))
   {

      if (pich->pmi->modFlags & mfActionsReturnVoid)
      {
         fprintf(pich->pcmw->hFile
                 , "void %s_%s(p%s);\n"
                 , pich->pmi->name->name
                 , pid_info->name
                 , pich->cp
                );
      }
      else
      {
         if (pich->pmi->modFlags & mfActionsReturnStates)
         {
            fprintf(pich->pcmw->hFile
                    , "%s_STATE %s_%s(p%s);\n"
                    , pich->cp
                    , pich->pmi->name->name
                    , pid_info->name
                    , pich->cp
                   );
         }
         else
         {
            fprintf(pich->pcmw->hFile
                    , "%s_EVENT %s_%s(p%s);\n"
                    , pich->parent_cp
                    , pich->pmi->name->name
                    , pid_info->name
                    , pich->cp
                   );
         }
      }

   }

   return false;
}

static bool declare_transition_fn_for_when_actions_return_states(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   fprintf(pich->pcmw->hFile
           , "%s_STATE %s_%s(p%s);\n"
           , pich->cp
           , pich->pmi->name->name
           , pid_info->name
           , pich->cp
          );

   return false;
}

static bool declare_state_only_transition_functions_for_when_actions_return_states(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   fprintf(pich->pcmw->hFile
           , "%s_STATE %s_transitionTo%s(p%s);\n"
           , pich->cp
           , pich->pmi->name->name
           , pid_info->name
           , pich->cp
           );
   return false;
}

static bool declare_transition_fn_for_when_actions_return_events(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   fprintf(pich->pcmw->hFile
           , "%s_STATE %s_%s(p%s,%s_EVENT);\n"
           , pich->cp
           , pich->pmi->name->name
           , pid_info->name
           , pich->cp
           , pich->cp
          );

   return false;
}

static bool declare_state_only_transition_functions_for_when_actions_return_events(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   fprintf(pich->pcmw->hFile
           , "%s_STATE %s_transitionTo%s(p%s,%s_EVENT);\n"
           , pich->cp
           , pich->pmi->name->name
           , pid_info->name
           , pich->cp
           , pich->cp
          );

   return false;
}

static bool declare_action_enum_member(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER) data);
   pID_INFO pid_info              = ((pID_INFO) pelem->mbr);

   if (pid_info->name && strlen(pid_info->name))
   {

      fprintf(pich->pcmw->hFile
              , "%s%s_%s_e\n"
              , pich->first ? (pich->first = false, "  ") : ", "
              , pich->pmi->name->name
              , pid_info->name
             );

   }

   return false;
}

static bool declare_action_array_member(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER) data);
   pID_INFO pid_info              = ((pID_INFO) pelem->mbr);

   if (pid_info->name && strlen(pid_info->name))
   {

      fprintf(pich->pcmw->cFile
              , "%s%s_%s\n"
              , pich->first ? (pich->first = false, "  ") : ", "
              , pich->pmi->name->name
              , pid_info->name
             );

   }

   return false;
}

static bool declare_transition_fn_enum_member(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER) data);
   pID_INFO pid_info              = ((pID_INFO) pelem->mbr);

   fprintf(pich->pcmw->hFile
           , "%s%s_%s_e\n"
           , pich->first ? (pich->first = false, "  ") : ", "
           , pich->pmi->name->name
           , pid_info->name
          );

   return false;
}

static bool declare_transition_enum_member(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER) data);
   pID_INFO pid_info              = ((pID_INFO) pelem->mbr);

   fprintf(pich->pcmw->hFile
           , "%s%s_transitionTo%s_e\n"
           , pich->first ? (pich->first = false, "  ") : ", "
           , pich->pmi->name->name
           , pid_info->name
          );
   return false;
}

static bool define_transition_fn_array_member(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER) data);
   pID_INFO pid_info              = ((pID_INFO) pelem->mbr);

   fprintf(pich->pcmw->cFile
           , "%s%s_%s\n"
           , pich->first ? (pich->first = false, "  ") : ", "
           , pich->pmi->name->name
           , pid_info->name
          );
   return false;
}

static bool define_transition_array_member(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER) data);
   pID_INFO pid_info              = ((pID_INFO) pelem->mbr);

   fprintf(pich->pcmw->cFile
           , "%s%s_transitionTo%s\n"
           , pich->first ? (pich->first = false, "  ") : ", "
           , pich->pmi->name->name
           , pid_info->name
          );

   return false;
}

/*
  Our "real" internals.
*/
static void            destroyCMachineData(pCMachineData, int);
static pCMachineData   newCMachineData(char *);
static int             writeCMachineInternal(pCMachineData, pMACHINE_INFO);
static int             writeCSubMachineInternal(pCMachineData, pMACHINE_INFO);
static int             writeCSwitchMachineInternal(pCMachineData, pMACHINE_INFO);
static void            writeOriginalFSM(pCMachineData, pMACHINE_INFO, char *);
static void            writeOriginalSubFSM(pCMachineData, pMACHINE_INFO, char *);
static void            writeOriginalFSMLoop(pCMachineData, pMACHINE_INFO, char *);
static void            writeOriginalSubFSMLoop(pCMachineData, pMACHINE_INFO, char *);
static void            writeOriginalFSMLoopInnards(pCMachineData, pMACHINE_INFO, char *, char *);
static void            writeOriginalSubFSMLoopInnards(pCMachineData, pMACHINE_INFO, char *, char *);
static void            writeOriginalSwitchFSM(pCMachineData, pMACHINE_INFO, char *);
static void            writeOriginalSwitchFSMLoop(pCMachineData, pMACHINE_INFO, char *);
static void            writeStateTransitions(pCMachineData, pMACHINE_INFO, char *);
static void            writeNoTransition(pCMachineData, pMACHINE_INFO, char *);
static void            writeReentrantFSM(pCMachineData, pMACHINE_INFO, char *);
static void            writeReentrantSwitchFSM(pCMachineData, pMACHINE_INFO, char *);
static void            writeActionsReturnStateFSM(pCMachineData, pMACHINE_INFO, char *);
static void            writeActionsReturnStateSwitchFSM(pCMachineData, pMACHINE_INFO, char *);
static void            writeDebugInfo(pCMachineData, pMACHINE_INFO, char *);
static char          * commonHeaderStart(pCMachineData, pMACHINE_INFO, char *);
static char          * subMachineHeaderStart(pCMachineData, pMACHINE_INFO, char *);
static void            commonHeaderEnd(pCMachineData, pMACHINE_INFO, char *, bool);
static void            subMachineHeaderEnd(pCMachineData, pMACHINE_INFO, char *, bool);
static void            declareCSwitchMachineStateFnArray(pCMachineData, pMACHINE_INFO, char *);
static void            declareCMachineActionArray(pCMachineData, pMACHINE_INFO, char *);
static void            defineCMachineActionFnArray(pCMachineData, pMACHINE_INFO, char *);
static void            defineCMachineTransitionFnArray(pCMachineData, pMACHINE_INFO, char *);
static void            declareCMachineActionFnEnum(pCMachineData, pMACHINE_INFO, char *);
static void            declareCMachineTransitionFnEnum(pCMachineData, pMACHINE_INFO, char *);
static void            declareCMachineStruct(pCMachineData, pMACHINE_INFO, char *);
static void            declareCSwitchMachineStruct(pCMachineData, pMACHINE_INFO, char *);
static void            defineActionArray(pCMachineData, pMACHINE_INFO, char *);
static void            defineSubMachineIF(pCMachineData, pMACHINE_INFO, char *);
static void            defineStateFnArray(pCMachineData, pMACHINE_INFO, char *);
static void            generateInstance(pCMachineData, pMACHINE_INFO, char *, char *);
static void            defineCMachineFSM(pCMachineData, pMACHINE_INFO, char *);
static void            defineCSubMachineFSM(pCMachineData, pMACHINE_INFO, char *);
static void            defineCSwitchMachineFSM(pCMachineData, pMACHINE_INFO, char *);
static void            defineCSwitchMachineStateFns(pCMachineData, pMACHINE_INFO, char *);
static bool            assignExternalEventValues(pMACHINE_INFO);
static void            defineSubMachineArray(pCMachineData, pMACHINE_INFO, char *);
static bool            print_sub_machine_if(pLIST_ELEMENT,void*);
static bool            declare_sub_machine_if(pLIST_ELEMENT,void*);
static bool            print_sub_machine_as_enum_member(pLIST_ELEMENT,void*);
static void            defineSubMachineFinder(pCMachineData, pMACHINE_INFO, char *);
static void            defineWeakActionFunctionStubs(pCMachineData, pMACHINE_INFO, char *);
static void            defineWeakNoActionFunctionStubs(pCMachineData, pMACHINE_INFO, char *);
static void            defineSubMachineWeakActionFunctionStubs(pCMachineData, pMACHINE_INFO, char *);


/**
  newpCMachineData

  Allocates a new CMachineData struct.
  Sets cName to baseFileName + ".c"
  Sets hName to baseFileName + ".h"
  Sets cFile to openFile(cName,"w")
  Sets hFile to openFile(hName,"w")

  If baseFileName is NULL, cFile = hFile = stdout; and,
    cName = hName = NULL.

  If all ok, writes preambles to cFile and hFile.

  Returns :

    pCMachineData on success,
    NULL						on failure.

*/
static pCMachineData	newCMachineData(char *baseFileName)
{

   pCMachineData	pcmw;
   char						*cp, *cp1;
   time_t					now;

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

         if (!(pcmw->cFile = openFile(pcmw->cName, "w")))
         {

            CHECK_AND_FREE(pcmw->cName);
            CHECK_AND_FREE(pcmw->hName);

            FREE_AND_CLEAR(pcmw);

         }
         else if (!(pcmw->hFile = openFile(pcmw->hName, "w")))
         {

            FCLOSE_AND_CLEAR(pcmw->cFile);

            FREE_AND_CLEAR(pcmw->cName);
            CHECK_AND_FREE(pcmw->hName);

            FREE_AND_CLEAR(pcmw);

         }
         else
         {

            /* we're good to go; write the preambles */

            /* the header file */
            cp1 = getFileNameNoDir(pcmw->hName);
            for (cp = cp1; *cp; cp++)

               /* capitalise things, and change the '.' or '-' to '_' */
               *cp = ((*cp == '.') || (*cp == '-')) ? '_' : toupper(*cp);

            fprintf(pcmw->hFile
                    , "/**\n\t%s\n\n"
                    , pcmw->hName
                    );
            fprintf(pcmw->hFile
                    , "\tThis file automatically generated by FSMLang\n*/\n\n"
                    );

            fprintf(pcmw->hFile, "#ifndef _%s_\n", cp1);
            fprintf(pcmw->hFile, "#define _%s_\n\n", cp1);

            /* the source file */
            fprintf(pcmw->cFile
                    , "/**\n\t%s\n\n"
                    , pcmw->cName
                    );
            fprintf(pcmw->cFile
                    , "\tThis file automatically generated by FSMLang\n*/\n\n"
                    );

            /* put the call to the header file into the source */
            fprintf(pcmw->cFile
                    , "#include \"%s\"\n\n"
                    , pcmw->hName
                    );

            CHECK_AND_FREE(cp1);

         }

      }

   }

   return pcmw;

}

static char* subMachineHeaderStart(pCMachineData pcmw, pMACHINE_INFO pmi, char *arrayName)
{
   pID_INFO			   pid_info;
   pACTION_INFO	   pai;
   char					   *cp;
   int						 i, j;
   bool            canAssignExternals;
   ITERATOR_HELPER helper;
   char            *parent_cp = hungarianToUnderbarCaps(pmi->parent->name->name);

   /* put the native code segment out to the header */
   if (pmi->native) fprintf(pcmw->hFile, "%s\n", pmi->native);

   if (!(cp = hungarianToUnderbarCaps(pmi->name->name))) return NULL;

   fprintf(pcmw->hFile, "#ifdef %s_DEBUG\n", cp);
   fprintf(pcmw->hFile, "#include <stdio.h>\n");
   fprintf(pcmw->hFile, "#include <stdlib.h>\n");
   fprintf(pcmw->hFile, "#endif\n\n");

   /* put the "declare a state machine" macro into the header */
   fprintf(pcmw->hFile, "#define DECLARE_%s_MACHINE(A) \\\n"
           , cp
          );
   fprintf(pcmw->hFile
           , "%s A =\\\n{\\\n%s\t%s_%s,\\\n\t&%s_%s_array,\\\n\t%sFSM\\\n};\\\n%s *p##A = &A;\n\n"
           , cp
           , pmi->data ? "\t{0},\\\n" : ""
           , pmi->name->name
           , stateNameByIndex(pmi, 0)
           , pmi->name->name
           , arrayName
           , pmi->name->name
           , cp
          );

   /* put the event enum into the header file */
   fprintf(pcmw->hFile, "typedef enum _%s_EVENT_ %s_EVENT;\n"
           , cp
           , cp
          );
   fprintf(pcmw->hFile
           , "enum _%s_EVENT_\n{\n"
           , cp
          );


   if ((canAssignExternals = (pmi->external_event_designation_count > 0)) == true)
   {
      if ((canAssignExternals = assignExternalEventValues(pmi)) == false)
      {
         printf("warning: cannot use external event designations\n");
      }
   }

   fprintf(pcmw->hFile 
           , "\t %s_%s = %s_%s_%s\n"
           , pmi->name->name
           , eventNameByIndex(pmi, 0)
           , pmi->parent->name->name
           , pmi->name->name
           , eventNameByIndex(pmi, 0)
          );

   for (i = 1; i < pmi->event_list->count; i++)
   {
      fprintf(pcmw->hFile
              , assignExternalEventValues(pmi)
              ? "\t, %s_%s = %s\n"
              : "\t, %s_%s%s\n"
              , pmi->name->name
              , eventNameByIndex(pmi, i)
              , assignExternalEventValues(pmi)
              ? (eventPidByIndex(pmi, i))->externalDesignation->name
              : ""
             );
   }

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

   if (pmi->machine_list)
   {
      helper.first   = false;
      helper.pparent = pmi;
      helper.fout    = pcmw->hFile;

      iterate_list(pmi->machine_list,print_sub_machine_events,&helper);
   }

   if (assignExternalEventValues(pmi))
   {
      fprintf(pcmw->hFile
              , "\t, %s_numEvents = %u\n"
              , pmi->name->name
              , pmi->event_list->count
              );
   }
   else
   {
      fprintf(pcmw->hFile
              , "\t, %s_numEvents\n"
              , pmi->name->name
              );
   }

   fprintf(pcmw->hFile
           , "}%s;\n\n"
           , compact_action_array ? " __attribute__((__packed__))" : ""
          );

   fprintf(pcmw->hFile, "#ifdef %s_DEBUG\n", cp);
   fprintf(pcmw->hFile, "extern char *%s_EVENT_NAMES[];\n", cp);
   fprintf(pcmw->hFile, "#endif\n\n");

   /* put the state enum into the header file */
   fprintf(pcmw->hFile
           , "typedef enum _%s_STATE_ %s_STATE;\n"
           , cp
           , cp
          );
   fprintf(pcmw->hFile
           , "enum _%s_STATE_\n{\n"
           , cp
          );

   fprintf(pcmw->hFile, "\t %s_%s\n",
           pmi->name->name,
           stateNameByIndex(pmi, 0));

   for (i = 1; i < pmi->state_list->count; i++)
   {
      fprintf(pcmw->hFile
              , "\t, %s_%s\n"
              , pmi->name->name
              , stateNameByIndex(pmi, i)
             );
   }

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
           , "}%s;\n\n"
           , compact_action_array ? " __attribute__((__packed__))" : ""
          );

   fprintf(pcmw->hFile, "#ifdef %s_DEBUG\n", cp);
   fprintf(pcmw->hFile, "extern char *%s_STATE_NAMES[];\n", cp);
   fprintf(pcmw->hFile, "#endif\n\n");

   /* put the data struct typedef into the header */
   if (pmi->data)
   {
      fprintf(pcmw->hFile
              , "typedef struct _%s_data_struct_ %s_DATA, *p%s_DATA;\n"
              , pmi->name->name
              , cp
              , cp);
   }

   /* put the machine struct typedef into the header */
   fprintf(pcmw->hFile, "typedef struct _%s_struct_ %s, *p%s;\n",
           pmi->name->name,
           cp,
           cp);

   if (generate_instance)
   {
      fprintf(pcmw->hFile, "extern %s %s;\n\n",
              cp,
              pmi->name->name);

      fprintf(pcmw->hFile, "extern p%s p%s;\n\n"
              , cp
              , pmi->name->name
             );
   }

   /* put the action function typedef into the header */
   if (pmi->modFlags & mfActionsReturnStates)
   {
      fprintf(pcmw->hFile
              , "typedef %s_STATE (*%s_ACTION_FN)(p%s);\n\n"
              , cp
              , cp
              , cp
             );

   }
   else if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmw->hFile
              , "typedef void (*%s_ACTION_FN)(p%s);\n\n"
              , cp
              , cp
             );

   }
   else
   {
      fprintf(pcmw->hFile
              , "typedef %s_EVENT (*%s_ACTION_FN)(p%s);\n\n"
              , parent_cp
              , cp
              , cp
             );
   }

   /* typedef transition functions, if we have any */
   if (pmi->transition_fn_list->count)
   {
      fprintf(pcmw->hFile
              , "typedef %s_STATE (*%s_TRANSITION_FN)(p%s,%s_EVENT);\n\n"
              , cp
              , cp
              , cp
              , cp
             );

   }

   /* typedef the FSM function */
   fprintf(pcmw->hFile
           , "typedef %s_EVENT (*%s_FSM)(p%s,%s_EVENT);\n\n"
           , parent_cp
           , cp
           , cp
           , parent_cp
           );

   /* declare the fsm function */
   fprintf(pcmw->hFile
           , "%s_EVENT %sFSM(p%s,%s_EVENT);\n\n"
           , parent_cp
           , pmi->name->name
           , cp
           , parent_cp
           );

   /* put the data structure definition into the header */
   if (pmi->data)
   {
      fprintf(pcmw->hFile
              , "struct _%s_data_struct_ {%s};\n\n"
              , pmi->name->name
              , pmi->data
              );
   }

   if (pmi->machine_list)
   {
      yyerror("only one level of sub-machines are currently supported");

      #if 0
      fprintf(pcmw->hFile,"/* Sub Machine Declarations */\n\n");
      fprintf(pcmw->hFile
              , "typedef %s_EVENT (*%s_SUB_MACHINE_FN)(%s_EVENT);\n"
              , cp
              , cp
              , cp
             );

      fprintf(pcmw->hFile
              , "typedef struct _%s_sub_fsm_if_ %s_SUB_FSM_IF, *p%s_SUB_FSM_IF;\n"
              , pmi->name->name
              , cp
              , cp
             );

      fprintf(pcmw->hFile
              ,"struct _%s_sub_fsm_if_\n{\n\t%s_SUB_MACHINE_FN subFSM;\n};\n\n"
              , pmi->name->name
              , cp
              );

      fprintf(pcmw->hFile
              , "extern p%s_SUB_FSM_IF %s_sub_fsm_if_array[%u];\n\n"
              , cp
              , pmi->name->name
              , pmi->machine_list->count
             );

      helper.cp = cp;
      iterate_list(pmi->machine_list, declare_sub_machine_if, &helper);
      #endif
   }

   free (parent_cp);

   return cp;

}

static char* commonHeaderStart(pCMachineData pcmw, pMACHINE_INFO pmi, char *arrayName)
{
   pID_INFO			   pid_info;
   pACTION_INFO	   pai;
   char					   *cp;
   int						 i, j;
   bool            canAssignExternals;
   ITERATOR_HELPER helper;

   /* put the native code segment out to the header */
   if (pmi->native) fprintf(pcmw->hFile, "%s\n", pmi->native);

   if (!(cp = hungarianToUnderbarCaps(pmi->name->name))) return NULL;

   fprintf(pcmw->hFile, "#ifdef %s_DEBUG\n", cp);
   fprintf(pcmw->hFile, "#include <stdio.h>\n");
   fprintf(pcmw->hFile, "#include <stdlib.h>\n");
   fprintf(pcmw->hFile, "#endif\n\n");

   /* put the "call the state machine" macro into the header */
   fprintf(pcmw->hFile, "\n#define RUN_STATE_MACHINE(A,B) \\\n");
   fprintf(pcmw->hFile, "\t((*A->fsm)((A),(B)))\n\n");

   /* put the "declare a state machine" macro into the header */
   fprintf(pcmw->hFile, "#define DECLARE_%s_MACHINE(A) \\\n"
           , cp
          );
   fprintf(pcmw->hFile
           , "%s (A) =\\\n{\\\n%s\t%s_%s,\\\n\t&%s_%s_array,\\\n\t%sFSM\\\n};\\\n%s *p##A = &A;\n\n"
           , cp
           , pmi->data ? "\t{0},\\\n" : ""
           , pmi->name->name
           , stateNameByIndex(pmi, 0)
           , pmi->name->name
           , arrayName
           , pmi->name->name
           , cp
          );

   /* put the event enum into the header file */
   fprintf(pcmw->hFile, "typedef enum _%s_EVENT_ %s_EVENT;\n"
           , cp
           , cp
          );
   fprintf(pcmw->hFile
           , "enum _%s_EVENT_\n{\n"
           , cp
          );


   if ((canAssignExternals = (pmi->external_event_designation_count > 0)) == true)
   {
      if ((canAssignExternals = assignExternalEventValues(pmi)) == false)
      {
         printf("warning: cannot use external event designations\n");
      }
   }

   fprintf(pcmw->hFile, assignExternalEventValues(pmi)
           ? "\t %s_%s = %s\n"
           : "\t %s_%s%s\n"
           ,
           pmi->name->name,
           eventNameByIndex(pmi, 0)
           , assignExternalEventValues(pmi)
           ? (eventPidByIndex(pmi, 0))->externalDesignation->name
           : ""
          );

   for (i = 1; i < pmi->event_list->count; i++)
   {
      fprintf(pcmw->hFile
              , assignExternalEventValues(pmi)
              ? "\t, %s_%s = %s\n"
              : "\t, %s_%s%s\n"
              , pmi->name->name
              , eventNameByIndex(pmi, i)
              , assignExternalEventValues(pmi)
              ? (eventPidByIndex(pmi, i))->externalDesignation->name
              : ""
             );
   }

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

   if (pmi->machine_list)
   {
      helper.first   = false;
      helper.pparent = pmi;
      helper.fout    = pcmw->hFile;

      iterate_list(pmi->machine_list,print_sub_machine_events,&helper);
   }

   if (assignExternalEventValues(pmi))
   {
      fprintf(pcmw->hFile
              , "\t, %s_numEvents = %u\n"
              , pmi->name->name
              , pmi->event_list->count
              );
   }
   else
   {
      fprintf(pcmw->hFile
              , "\t, %s_numEvents\n"
              , pmi->name->name
              );
   }

   fprintf(pcmw->hFile
           , "}%s;\n\n"
           , compact_action_array ? " __attribute__((__packed__))" : ""
          );

   fprintf(pcmw->hFile, "#ifdef %s_DEBUG\n", cp);
   fprintf(pcmw->hFile, "extern char *%s_EVENT_NAMES[];\n", cp);
   fprintf(pcmw->hFile, "#endif\n\n");

   /* put the state enum into the header file */
   fprintf(pcmw->hFile
           , "typedef enum _%s_STATE_ %s_STATE;\n"
           , cp
           , cp
          );
   fprintf(pcmw->hFile
           , "enum _%s_STATE_\n{\n"
           , cp
          );

   fprintf(pcmw->hFile, "\t %s_%s\n",
           pmi->name->name,
           stateNameByIndex(pmi, 0));

   for (i = 1; i < pmi->state_list->count; i++)
   {
      fprintf(pcmw->hFile
              , "\t, %s_%s\n"
              , pmi->name->name
              , stateNameByIndex(pmi, i)
             );
   }

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
           , "}%s;\n\n"
           , compact_action_array ? " __attribute__((__packed__))" : ""
          );

   fprintf(pcmw->hFile, "#ifdef %s_DEBUG\n", cp);
   fprintf(pcmw->hFile, "extern char *%s_STATE_NAMES[];\n", cp);
   fprintf(pcmw->hFile, "#endif\n\n");

   /* put the data struct typedef into the header */
   if (pmi->data)
   {
      fprintf(pcmw->hFile
              , "typedef struct _%s_data_struct_ %s_DATA, *p%s_DATA;\n"
              , pmi->name->name
              , cp
              , cp);
   }

   /* put the machine struct typedef into the header */
   fprintf(pcmw->hFile, "typedef struct _%s_struct_ %s, *p%s;\n",
           pmi->name->name,
           cp,
           cp);

   if (generate_instance)
   {
      fprintf(pcmw->hFile, "extern %s %s;\n\n",
              cp,
              pmi->name->name);

      fprintf(pcmw->hFile, "extern p%s p%s;\n\n"
              , cp
              , pmi->name->name
             );
   }

   /* put the action function typedef into the header */
   if (pmi->modFlags & mfActionsReturnStates)
   {
      fprintf(pcmw->hFile
              , "typedef %s_STATE (*%s_ACTION_FN)(p%s);\n\n"
              , cp
              , cp
              , cp
             );

   }
   else if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmw->hFile
              , "typedef void (*%s_ACTION_FN)(p%s);\n\n"
              , cp
              , cp
             );

   }
   else
   {
      fprintf(pcmw->hFile
              , "typedef %s_EVENT (*%s_ACTION_FN)(p%s);\n\n"
              , cp
              , cp
              , cp
             );
   }

   /* typedef transition functions, if we have any */
   if (pmi->transition_fn_list->count)
   {
      fprintf(pcmw->hFile
              , "typedef %s_STATE (*%s_TRANSITION_FN)(p%s,%s_EVENT);\n\n"
              , cp
              , cp
              , cp
              , cp
             );

   }

   /* typedef the FSM function */
   fprintf(pcmw->hFile, "typedef void (*%s_FSM)(p%s,%s_EVENT);\n\n",
           cp,
           cp,
           cp);

   /* declare the fsm function */
   fprintf(pcmw->hFile, "void %sFSM(p%s,%s_EVENT);\n\n",
           pmi->name->name,
           cp,
           cp);

   /* put the data structure definition into the header */
   if (pmi->data)
   {
      fprintf(pcmw->hFile
              , "struct _%s_data_struct_ {%s};\n\n"
              , pmi->name->name
              , pmi->data);
   }

   if (pmi->machine_list)
   {
      helper.first = true;
      helper.cp    = cp;

      fprintf(pcmw->hFile,"/* Sub Machine Declarations */\n\n");
      fprintf(pcmw->hFile
              ,"%s\ntypedef enum _%s_SUB_MACHINES_ %s_SUB_MACHINES;\nenum _%s_SUB_MACHINES_\n{\n"
              ,"/* enumerate sub-machines */"
              , cp
              , cp
              , cp
              );

      iterate_list(pmi->machine_list
                   , print_sub_machine_as_enum_member
                   ,&helper
                   );

      fprintf(pcmw->hFile
              , "\t, %s_numSubMachines\n};\n\n"
              , pmi->name->name
              );

      fprintf(pcmw->hFile
              , "typedef %s_EVENT (*%s_SUB_MACHINE_FN)(%s_EVENT);\n"
              , cp
              , cp
              , cp
             );

      fprintf(pcmw->hFile
              , "typedef struct _%s_sub_fsm_if_ %s_SUB_FSM_IF, *p%s_SUB_FSM_IF;\n"
              , pmi->name->name
              , cp
              , cp
             );

      fprintf(pcmw->hFile
              ,"struct _%s_sub_fsm_if_\n{\n"
              , pmi->name->name
              );

      fprintf(pcmw->hFile
              , "\t%s_EVENT                first_event;\n"
              , cp
              );

      fprintf(pcmw->hFile
              , "\t%s_EVENT                last_event;\n"
              , cp
              );

      fprintf(pcmw->hFile
              , "\t%s_SUB_MACHINE_FN       subFSM;\n"
              , cp
              );

      fprintf(pcmw->hFile
              ,"};\n\n"
              );

      fprintf(pcmw->hFile
              , "extern p%s_SUB_FSM_IF %s_sub_fsm_if_array[%s_numSubMachines];\n\n"
              , cp
              , pmi->name->name
              , pmi->name->name
             );

      iterate_list(pmi->machine_list, declare_sub_machine_if, &helper);

      fprintf(pcmw->hFile
              , "\n"
              );

   }

   return cp;

}

static void commonHeaderEnd(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp, bool needNoOp)
{
   pID_INFO pid_info;
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmw      = pcmw;
   ich.pmi       = pmi;
   ich.cp        = cp;
   ich.needNoOp  = needNoOp;

   /* declare the action functions themselves */
   iterate_list(pmi->action_list, declare_action_function, &ich);

   /* declare the dummy, or no op action */
   if (needNoOp)
   {
      if (pmi->modFlags & mfActionsReturnVoid)
      {
         fprintf(pcmw->hFile
                 , "void %s_noAction(p%s);\n\n"
                 , pmi->name->name
                 , cp
                );
      }
      else
      {
         if (pmi->modFlags & mfActionsReturnStates)
         {
            fprintf(pcmw->hFile
                    , "%s_STATE %s_noAction(p%s);\n\n"
                    , cp
                    , pmi->name->name
                    , cp
                   );
         }
         else
         {
            fprintf(pcmw->hFile
                    , "%s_EVENT %s_noAction(p%s);\n\n"
                    , cp
                    , pmi->name->name
                    , cp
                   );
         }
      }
   }

   fprintf(pcmw->hFile
           , "\n"
          );

   /* declare any machine transition function */
   if (pmi->machineTransition)
   {
      fprintf(pcmw->hFile
              , "void %s_%s(p%s,%s_STATE);\n\n"
              , pmi->name->name
              , pmi->machineTransition->name
              , cp
              , cp
             );
   }

   /* declare any transition functions */
   if (pmi->modFlags & mfActionsReturnStates)
   {
      fprintf(pcmw->hFile
              , "%s_STATE %s_noTransitionFn(p%s);\n"
              , cp
              , pmi->name->name
              , cp
             );

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
         fprintf(pcmw->hFile
                    , "%s_STATE %s_noTransitionFn(p%s,%s_EVENT);\n\n"
                    , cp
                    , pmi->name->name
                    , cp
                    , cp
                   );
      }
   }


   /* if the machine has data, declare the data init function
 
     This is pre-mature.  We need to parse the data structure
       to know what parameters this function needs.
 
   if (pmi->data)
     fprintf(pcmw->hFile,"void %s_initData(p%s);\n",
       pmi->name->name,
       cp);
   */

}

static void subMachineHeaderEnd(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp, bool needNoOp)
{
   pID_INFO pid_info;
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmw      = pcmw;
   ich.pmi       = pmi;
   ich.cp        = cp;
   ich.parent_cp = hungarianToUnderbarCaps(pmi->parent->name->name);
   ich.needNoOp  = needNoOp;

   /* declare the action functions themselves */
   iterate_list(pmi->action_list, declare_sub_machine_action_function, &ich);

   /* declare the dummy, or no op action */
   if (needNoOp)
   {
      if (pmi->modFlags & mfActionsReturnVoid)
      {
         fprintf(pcmw->hFile
                 , "void %s_noAction(p%s);\n"
                 , pmi->name->name
                 , cp
                );
      }
      else
      {
         if (pmi->modFlags & mfActionsReturnStates)
         {
            fprintf(pcmw->hFile
                    , "%s_STATE %s_noAction(p%s);\n\n"
                    , cp
                    , pmi->name->name
                    , cp
                   );
         }
         else
         {
            fprintf(pcmw->hFile
                    , "%s_EVENT %s_noAction(p%s);\n\n"
                    , ich.parent_cp
                    , pmi->name->name
                    , cp
                   );
         }
      }
   }

   fprintf(pcmw->hFile
           , "\n"
          );

   /* declare any machine transition function */
   if (pmi->machineTransition)
   {
      fprintf(pcmw->hFile
              , "void %s_%s(p%s,%s_STATE);\n\n"
              , pmi->name->name
              , pmi->machineTransition->name
              , cp
              , cp
             );
   }

   /* declare any transition functions */
   if (pmi->modFlags & mfActionsReturnStates)
   {
      fprintf(pcmw->hFile
              , "%s_STATE %s_noTransitionFn(p%s);\n"
              , cp
              , pmi->name->name
              , cp
             );

      iterate_list(pmi->transition_fn_list
                   , declare_transition_fn_for_when_actions_return_states
                   , &ich
                   );

      iterate_list(pmi->transition_list
                   , declare_state_only_transition_functions_for_when_actions_return_states
                   , &ich
                   );

      fprintf(pcmw->cFile, "\n");

   }
   else
   {

      iterate_list(pmi->transition_fn_list
                   , declare_transition_fn_for_when_actions_return_events
                   , &ich
                   );
      iterate_list(pmi->transition_list
                   , declare_state_only_transition_functions_for_when_actions_return_events
                   , &ich
                   );

      fprintf(pcmw->cFile, "\n");

      if (pmi->transition_fn_list->count)
      {
         fprintf(pcmw->hFile
                    , "%s_STATE %s_noTransitionFn(p%s,%s_EVENT);\n\n"
                    , cp
                    , pmi->name->name
                    , cp
                    , cp
                   );
      }
   }


   /* if the machine has data, declare the data init function
 
     This is pre-mature.  We need to parse the data structure
       to know what parameters this function needs.
 
   if (pmi->data)
     fprintf(pcmw->hFile,"void %s_initData(p%s);\n",
       pmi->name->name,
       cp);
   */

}

static int writeCSubMachineInternal(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   char     *cp;
   pID_INFO pid_info;

   if (!pmi || !pcmw) return 1;

   if ((cp = subMachineHeaderStart(pcmw, pmi, "action")) == NULL) return 1;

   declareCMachineActionArray(pcmw, pmi, cp);

   declareCMachineStruct(pcmw, pmi, cp);

   subMachineHeaderEnd(pcmw, pmi, cp, true);

   /*
     Source File
   */

   defineActionArray(pcmw, pmi, cp);

   defineSubMachineIF(pcmw, pmi, cp);

   defineSubMachineArray(pcmw, pmi, cp);

   if (generate_instance)
   {
      generateInstance(pcmw, pmi, cp, "action");
   }

   defineCSubMachineFSM(pcmw, pmi, cp);

   /* write weak stubs for our action functions */
   defineSubMachineWeakActionFunctionStubs(pcmw, pmi, cp);

   /* write our transition functions, if needed */
   if (pmi->transition_fn_list->count)
   {
      writeStateTransitions(pcmw, pmi, cp);
      writeNoTransition(pcmw, pmi, cp);
   }

   writeDebugInfo(pcmw, pmi, cp);

   FREE_AND_CLEAR(cp);

   return 0;

}

static int writeCMachineInternal(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   char     *cp;
   pID_INFO pid_info;

   if (!pmi || !pcmw) return 1;

   if ((cp = commonHeaderStart(pcmw, pmi, "action")) == NULL) return 1;

   declareCMachineActionArray(pcmw, pmi, cp);

   declareCMachineStruct(pcmw, pmi, cp);

   commonHeaderEnd(pcmw, pmi, cp, true);

   /*
     Source File
   */

   defineActionArray(pcmw, pmi, cp);

   defineSubMachineArray(pcmw, pmi, cp);

   if (generate_instance)
   {
      generateInstance(pcmw, pmi, cp, "action");
   }

   defineCMachineFSM(pcmw, pmi, cp);

   /* write our sub-machine lookup, if needed */
   if (pmi->machine_list)
   {
      defineSubMachineFinder(pcmw, pmi, cp);
   }

   /* write weak stubs for our action functions */
   defineWeakActionFunctionStubs(pcmw, pmi, cp);

   /* ... and for the noAction case */
   defineWeakNoActionFunctionStubs(pcmw, pmi, cp);

   /* write our transition functions, if needed */
   if (pmi->transition_fn_list->count)
   {
      writeStateTransitions(pcmw, pmi, cp);
      writeNoTransition(pcmw, pmi, cp);
   }

   writeDebugInfo(pcmw, pmi, cp);

   FREE_AND_CLEAR(cp);

   return 0;

}

static void destroyCMachineData(pCMachineData pcmw, int good)
{

   if (!pcmw) return;

   if (good)
   {

      /* close the #ifndef on the header file */
      fprintf(pcmw->hFile, "#endif\n");

   }

   fclose(pcmw->hFile);
   fclose(pcmw->cFile);
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

static int initCMachine(pFSMOutputGenerator pfsmog, char *fileName)
{
   pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

   if (pfsmcog->pcmd = newCMachineData(fileName))
   {
      return 0;
   }

   return 1;

}

static int initCSubMachine(pFSMOutputGenerator pfsmog, char *fileName)
{
   pFSMCSubMachineOutputGenerator pfsmcsmog = (pFSMCSubMachineOutputGenerator) pfsmog;

   if (pfsmcsmog->pcmd = newCMachineData(fileName))
   {
      /* put the call to the parent header file into the header */
      fprintf(pfsmcsmog->pcmd->hFile
              , "#include \"%s\"\n\n"
              , pfsmcsmog->parent_fsmcog->pcmd->hName
              );

      return 0;
   }

   return 1;

}

static void writeCSubMachine(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{

   pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

   writeCSubMachineInternal(pfsmcog->pcmd, pmi);

   #if 0
   if (pmi->machine_list)
   {
      write_machines(pmi->machine_list, pCSubMachineWriter);
   }
   #endif

}

static void writeCMachine(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{

   pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

   writeCMachineInternal(pfsmcog->pcmd, pmi);

   if (pmi->machine_list)
   {
      CSubMachineWriter.parent_fsmcog = pfsmcog;
      write_machines(pmi->machine_list, pCSubMachineWriter);
   }

}

static void closeCMachine(pFSMOutputGenerator pfsmog, int how)
{
   pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

   destroyCMachineData(pfsmcog->pcmd, how);
}

/**
  This function writes the original FSM
*/
static void writeOriginalFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   #ifdef FSMLANG_DEVELOP
   fprintf(pcmw->cFile
           , "/* writeOriginalFSM */\n"
          );
   #endif

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile, "\t%s_EVENT new_e;\n\n"
              , cp
             );

      fprintf(pcmw->cFile, "\t%s_EVENT e = event;\n\n"
              , cp
             );
   }

   if (pmi->machineTransition)
   {
      fprintf(pcmw->cFile, "\t%s_STATE new_s;\n\n"
              , cp
             );
   }

   writeOriginalFSMLoop(pcmw, pmi, cp);

   fprintf(pcmw->cFile, "\n\n}\n\n");
}

/**
  This function writes the original FSM
*/
static void writeOriginalSubFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   #ifdef FSMLANG_DEVELOP
   fprintf(pcmw->cFile
           , "/* writeOriginalSubFSM */\n"
          );
   #endif

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile, "\t%s_EVENT new_e;\n\n"
              , cp
             );

      fprintf(pcmw->cFile, "\t%s_EVENT e = event;\n\n"
              , cp
             );
   }

   if (pmi->machineTransition)
   {
      fprintf(pcmw->cFile, "\t%s_STATE new_s;\n\n"
              , cp
             );
   }

   writeOriginalSubFSMLoop(pcmw, pmi, cp);

   fprintf(pcmw->cFile
           , "\n\treturn e == %s_noEvent ? %s_noEvent : e;"
           , pmi->name->name
           , pmi->parent->name->name
           );

   fprintf(pcmw->cFile, "\n\n}\n\n");
}

/**
 *This function writes an FSM suitable for shared use in ISRs  
 *and non-ISR settings.  
*/
static void writeReentrantFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   fprintf(pcmw->cFile, "\t%s_EVENT new_e;\n\n"
           , cp
          );

   fprintf(pcmw->cFile, "\t%s_EVENT e = event;\n",
           cp);

   fprintf(pcmw->cFile, "#ifdef FSM_START_CRITICAL\n");
   fprintf(pcmw->cFile, "\tFSM_START_CRITICAL;\n");
   fprintf(pcmw->cFile, "#endif\n\n");

   writeOriginalFSMLoop(pcmw, pmi, cp);

   fprintf(pcmw->cFile, "\n\n#ifdef FSM_END_CRITICAL\n");
   fprintf(pcmw->cFile, "\tFSM_END_CRITICAL;\n");
   fprintf(pcmw->cFile, "#endif\n\n");

   fprintf(pcmw->cFile, "}\n\n");

}

static void writeDebugInfo(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   int i;
   pID_INFO pid_info;
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
              , pmi->name->name,
              eventNameByIndex(pmi, i)
              );
   }

   if (!(pmi->modFlags & mfActionsReturnStates) && !(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile, "\t, \"%s_noEvent\"\n",
              pmi->name->name);
   }

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

/**
  This function writes the ActionsReturnState FSM
*/
static void writeActionsReturnStateFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   fprintf(pcmw->cFile, "\t%s_STATE s;\n",
           cp);

   fprintf(pcmw->cFile
           , "\n\tDBG_PRINTF(\"event: %%s; start state: %%s\"\n\t\t,%s_EVENT_NAMES[event]\n\t\t,%s_STATE_NAMES[pfsm->state]\n\t\t);\n"
           , cp
           , cp
          );

   fprintf(pcmw->cFile, "\n\ts = (*(*pfsm->actionArray)[event][pfsm->state])(pfsm);\n\n");

   fprintf(pcmw->cFile, "\tif (s != %s_noTransition)\n\t{\n",
           pmi->name->name
          );

   if (pmi->machineTransition)
   {
      fprintf(pcmw->cFile
              , "\t\tif (s != pfsm->state)\n\t\t{\n"
             );

      fprintf(pcmw->cFile
              , "\t\t\t%s_%s(pfsm,s);\n\t\t}\n"
              , pmi->name->name
              , pmi->machineTransition->name
             );

   }

   fprintf(pcmw->cFile
           , "\t\tpfsm->state = s;\n\t}\n\n"
          );

   fprintf(pcmw->cFile
           , "\n\tDBG_PRINTF(\"end state: %%s\"\n\t\t,%s_STATE_NAMES[pfsm->state]\n\t\t);\n"
           , cp
          );

   fprintf(pcmw->cFile, "}\n");

}

/**
  This function writes the ActionsReturnState Switch FSM
*/
static void writeActionsReturnStateSwitchFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   fprintf(pcmw->cFile, "\t%s_STATE s;\n",
           cp);

   fprintf(pcmw->cFile
           , "\n\tDBG_PRINTF(\"event: %%s; start state: %%s\"\n\t\t,%s_EVENT_NAMES[event]\n\t\t,%s_STATE_NAMES[pfsm->state]\n\t\t);\n"
           , cp
           , cp
          );

   fprintf(pcmw->cFile, "\t\ts = ((* (*pfsm->statesArray)[pfsm->state])(pfsm,e));\n\n");

   fprintf(pcmw->cFile, "\tif (s != %s_noTransition)\n\t{\n",
           pmi->name->name);

   if (pmi->machineTransition)
   {
      fprintf(pcmw->cFile
              , "\t\tif (s != pfsm->state)\n\t\t{\n"
             );

      fprintf(pcmw->cFile
              , "\t\t\t%s_%s(pfsm,s);\n\t\t}\n"
              , pmi->name->name
              , pmi->machineTransition->name
             );

   }

   fprintf(pcmw->cFile
           , "\t\tpfsm->state = s;\n\t}\n\n"
          );

   fprintf(pcmw->cFile
           , "\n\tDBG_PRINTF(\"end state: %%s\"\n\t\t,%s_STATE_NAMES[pfsm->state]\n\t\t);\n"
           , cp
          );

}

static void writeStateTransitions(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   pID_INFO pid_info;

   if (pmi->modFlags & mfActionsReturnStates)
   {
      /* grab state transitions from action array */
      for (int e = 0; e < pmi->event_list->count; e++)
      {
         for (int s = 0; s < pmi->state_list->count; s++)
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
                          , "\t(void) pfsm;\n\n\tDBG_PRINTF(\"weak: %s_transitionTo%s\");\n\treturn %s_%s;\n}\n"
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
      for (int i = 0; i < pmi->transition_list->count; i++)
      {
         pid_info = transitionPidByIndex(pmi, i);
         if (pid_info->type == STATE)
         {
            fprintf(pcmw->cFile
                    , "\n%s_STATE __attribute__((weak)) %s_transitionTo%s(p%s pfsm,%s_EVENT e)\n{\n"
                    , cp
                    , pmi->name->name
                    , pid_info->name
                    , cp
                    , cp
                   );
            fprintf(pcmw->cFile
                    , "\t(void) pfsm;\n\t(void) e;\n\tDBG_PRINTF(\"weak: %s_transitionTo%s\");\n\treturn %s_%s;\n}\n"
                    , pmi->name->name
                    , pid_info->name
                    , pmi->name->name
                    , pid_info->name
                   );
         }
      }
   }
}

static void writeNoTransition(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   if (pmi->modFlags & mfActionsReturnStates)
   {
      fprintf(pcmw->cFile
              , "\n%s_STATE %s_noTransitionFn(p%s pfsm)\n{\n"
              , cp
              , pmi->name->name
              , cp
             );
      fprintf(pcmw->cFile
              , "\tDBG_PRINTF(\"%s_noTransitionFn\");\n\treturn %s_noTransition;\n}\n\n"
              , pmi->name->name
              , pmi->name->name
             );
   }
   else
   {
      fprintf(pcmw->cFile
              , "\n%s_STATE %s_noTransitionFn(p%s pfsm,%s_EVENT e)\n{\n"
              , cp
              , pmi->name->name
              , cp
              , cp
             );
      fprintf(pcmw->cFile
              , "\t(void) e;\n\tDBG_PRINTF(\"%s_noTransitionFn\");\n\treturn pfsm->state;\n}\n\n"
              , pmi->name->name
             );
   }
}

static void writeOriginalFSMLoopInnards(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp, char *tabstr)
{
   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      if (compact_action_array)
      {
         fprintf(pcmw->cFile
                 , "%s\tnew_e = (*%s_action_fns[(*pfsm->actionArray)[e][pfsm->state].action])(pfsm);\n\n"
                 , tabstr
                 , pmi->name->name
                );
      }
      else
      {
         fprintf(pcmw->cFile
                 , "%s\tnew_e = ((* (*pfsm->actionArray)[e][pfsm->state].action)(pfsm));\n\n"
                 , tabstr
                 );
      }
   }
   else
   {
      if (compact_action_array)
      {
         fprintf(pcmw->cFile
                 , "%s(*%s_action_fns[(*pfsm->actionArray)[event][pfsm->state].action])(pfsm);\n\n"
                 , tabstr
                 , pmi->name->name
                );
      }
      else
      {
         fprintf(pcmw->cFile
                 , "%s((* (*pfsm->actionArray)[event][pfsm->state].action)(pfsm));\n\n"
                 , tabstr
                 );
      }
   }

   if (!pmi->transition_fn_list->count)
   {
      fprintf(pcmw->cFile
              , "%s\t%s = (*pfsm->actionArray)[%s][pfsm->state].transition;\n\n"
              , tabstr
              , pmi->machineTransition ? "new_s" : "pfsm->state"
              , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
             );
   }
   else
   {
      if (compact_action_array)
      {
         fprintf(pcmw->cFile
                 , "%s\t%s = (*%s_transition_fns[(*pfsm->actionArray)[%s][pfsm->state].transition])(pfsm,%s);\n\n"
                 , tabstr
                 , pmi->machineTransition ? "new_s" : "pfsm->state"
                 , pmi->name->name
                 , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
                 , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
                );
      }
      else
      {
         fprintf(pcmw->cFile
                 , "%s\t%s = ((* (*pfsm->actionArray)[%s][pfsm->state].transition)(pfsm,%s));\n\n"
                 , tabstr
                 , pmi->machineTransition ? "new_s" : "pfsm->state"
                 , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
                 , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
                );
      }
   }

   if (pmi->machineTransition)
   {
      fprintf(pcmw->cFile
              , "%s\tif (pfsm->state != new_s)\n\t\t{\n"
              , tabstr
             );

      fprintf(pcmw->cFile
              , "%s\t\t%s_%s(pfsm,new_s);\n"
              , tabstr
              , pmi->name->name
              , pmi->machineTransition->name
             );

      fprintf(pcmw->cFile
              , "%s\t\tpfsm->state = new_s;\n\n"
              , tabstr
             );

      fprintf(pcmw->cFile
              , "%s\t}\n\n"
              , tabstr
             );
   }

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile
              , "%s\te = new_e;\n\n"
              , tabstr
              );

      fprintf(pcmw->cFile
              , "%s} "
              , tabstr
              );
   }
}

static void writeOriginalSubFSMLoopInnards(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp, char *tabstr)
{
   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      if (compact_action_array)
      {
         fprintf(pcmw->cFile
                 , "%s\tnew_e = (*%s_action_fns[(*pfsm->actionArray)[e - %s_%s][pfsm->state].action])(pfsm);\n\n"
                 , tabstr
                 , pmi->name->name
                 , pmi->name->name
                 , eventNameByIndex(pmi,0)
                );
      }
      else
      {
         fprintf(pcmw->cFile
                 , "%s\tnew_e = ((* (*pfsm->actionArray)[e - %s_%s][pfsm->state].action)(pfsm));\n\n"
                 , tabstr
                 , pmi->name->name
                 , eventNameByIndex(pmi,0)
                 );
      }
   }
   else
   {
      if (compact_action_array)
      {
         fprintf(pcmw->cFile
                 , "%s(*%s_action_fns[(*pfsm->actionArray)[event - %s_%s][pfsm->state].action])(pfsm);\n\n"
                 , tabstr
                 , pmi->name->name
                 , pmi->name->name
                 , eventNameByIndex(pmi,0)
                );
      }
      else
      {
         fprintf(pcmw->cFile
                 , "%s((* (*pfsm->actionArray)[event - %s_%s][pfsm->state].action)(pfsm));\n\n"
                 , tabstr
                 , pmi->name->name
                 , eventNameByIndex(pmi,0)
                 );
      }
   }

   if (!pmi->transition_fn_list->count)
   {
      fprintf(pcmw->cFile
              , "%s\t%s = (*pfsm->actionArray)[%s - %s_%s][pfsm->state].transition;\n\n"
              , tabstr
              , pmi->machineTransition ? "new_s" : "pfsm->state"
              , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
              , pmi->name->name
              , eventNameByIndex(pmi,0)
             );
   }
   else
   {
      if (compact_action_array)
      {
         fprintf(pcmw->cFile
                 , "%s\t%s = (*%s_transition_fns[(*pfsm->actionArray)[%s - %s_%s][pfsm->state].transition])(pfsm,%s);\n\n"
                 , tabstr
                 , pmi->machineTransition ? "new_s" : "pfsm->state"
                 , pmi->name->name
                 , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
                 , pmi->name->name
                 , eventNameByIndex(pmi,0)
                 , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
                );
      }
      else
      {
         fprintf(pcmw->cFile
                 , "%s\t%s = ((* (*pfsm->actionArray)[%s - %s_%s][pfsm->state].transition)(pfsm,%s));\n\n"
                 , tabstr
                 , pmi->machineTransition ? "new_s" : "pfsm->state"
                 , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
                 , pmi->name->name
                 , eventNameByIndex(pmi,0)
                 , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
                );
      }
   }

   if (pmi->machineTransition)
   {
      fprintf(pcmw->cFile
              , "%s\tif (pfsm->state != new_s)\n\t\t{\n"
              , tabstr
             );

      fprintf(pcmw->cFile
              , "%s\t\t%s_%s(pfsm,new_s);\n"
              , tabstr
              , pmi->name->name
              , pmi->machineTransition->name
             );

      fprintf(pcmw->cFile
              , "%s\t\tpfsm->state = new_s;\n\n"
              , tabstr
             );

      fprintf(pcmw->cFile
              , "%s\t}\n\n"
              , tabstr
             );
   }

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile
              , "%s\te = new_e;\n\n"
              , tabstr
              );

      fprintf(pcmw->cFile
              , "%s} "
              , tabstr
              );
   }
}

static void writeOriginalFSMLoop(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   char *tabstr = "\t";

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile, "\twhile (e != %s_noEvent) {\n\n",
              pmi->name->name);
   }

   fprintf(pcmw->cFile, "#ifdef %s_DEBUG\n", cp);
   fprintf(pcmw->cFile, "DBG_PRINTF(\"event: %%s; state: %%s\"\n,%s_EVENT_NAMES[%s]\n,%s_STATE_NAMES[pfsm->state]\n);\n"
           , cp
           , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
           , cp
          );
   fprintf(pcmw->cFile, "#endif\n\n");

   if (pmi->machine_list)
   {
      fprintf(pcmw->cFile
              , "\t\tif (e < %s_noEvent)\n\t\t{\n\n"
              , pmi->name->name
              );
      tabstr = "\t\t";
   }

   writeOriginalFSMLoopInnards(pcmw, pmi, cp, tabstr);

   if (pmi->machine_list)
   {
      fprintf(pcmw->cFile
              , "\n\t\telse\n\t\t{\n"
              );

      fprintf(pcmw->cFile
              , "\t\t\te = findAndRunSubMachine(pfsm, e);\n\t\t}\n\n\t}"
              );
   }

}

static void writeOriginalSubFSMLoop(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   char *tabstr = "\t";

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile
              , "\twhile (\n\t\t(e != %s_noEvent)\n\t\t&& (e >= %s_%s)\n\t)\n\t{\n\n"
              , pmi->name->name
              , pmi->name->name
              , eventNameByIndex(pmi, 0)
              );
   }

   fprintf(pcmw->cFile, "#ifdef %s_DEBUG\n", cp);
   fprintf(pcmw->cFile, "DBG_PRINTF(\"event: %%s; state: %%s\"\n,%s_EVENT_NAMES[%s - %s_%s]\n,%s_STATE_NAMES[pfsm->state]\n);\n"
           , cp
           , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
           , pmi->name->name
           , eventNameByIndex(pmi, 0)
           , cp
          );
   fprintf(pcmw->cFile, "#endif\n\n");

   if (pmi->machine_list)
   {
      fprintf(pcmw->cFile
              , "\t\tif (e < %s_noEvent)\n\t\t{\n\n"
              , pmi->name->name
              );
      tabstr = "\t\t";
   }

   writeOriginalSubFSMLoopInnards(pcmw, pmi, cp, tabstr);

   if (pmi->machine_list)
   {
      fprintf(pcmw->cFile
              , "\n\t\telse\n\t\t{\n"
              );

      fprintf(pcmw->cFile
              , "\t\t\tnew_e = ((* (*pfsm->subMachineArray)[pfsm->active_sub_machine]->subFSM)(e));\n\n\t\t}\n\n\t}"
              );
   }

}

static void writeCSwitchMachine(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
   pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

   writeCSwitchMachineInternal(pfsmcog->pcmd, pmi);
}

static int writeCSwitchMachineInternal(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   char     *cp;

   if (!pmi || !pcmw) return 1;

   if ((cp = commonHeaderStart(pcmw, pmi, "state_fn")) == NULL) return 1;

   declareCSwitchMachineStateFnArray(pcmw, pmi, cp);

   declareCSwitchMachineStruct(pcmw, pmi, cp);

   /* declare state fns */
   for (unsigned i = 0; i < pmi->state_list->count; i++)
   {
      if (pmi->modFlags & mfActionsReturnVoid)
      {
         fprintf(pcmw->hFile
                 , "void %s_%s_stateFn(p%s,%s_EVENT);\n"
                 , pmi->name->name
                 , stateNameByIndex(pmi, i)
                 , cp
                 , cp
                );
      }
      else
      {
         fprintf(pcmw->hFile
                 , "%s_%s %s_%s_stateFn(p%s,%s_EVENT);\n"
                 , cp
                 , (pmi->modFlags & mfActionsReturnStates) ? "STATE" : "EVENT"
                 , pmi->name->name
                 , stateNameByIndex(pmi, i)
                 , cp
                 , cp
                );
      }
   }

   fprintf(pcmw->hFile, "\n");

   commonHeaderEnd(pcmw, pmi, cp, false);

   /*
     Source File
   */

   defineStateFnArray(pcmw, pmi, cp);

   if (generate_instance)
   {
      generateInstance(pcmw, pmi, cp, "state_fn");
   }

   defineCSwitchMachineFSM(pcmw, pmi, cp);

   defineCSwitchMachineStateFns(pcmw, pmi, cp);

   defineWeakActionFunctionStubs(pcmw, pmi, cp);

   /* write our transition functions, if needed */
   if (pmi->transition_fn_list->count)
   {
      writeStateTransitions(pcmw, pmi, cp);
   }

   writeDebugInfo(pcmw, pmi, cp);

   FREE_AND_CLEAR(cp);

   return 0;
}

static void declareCSwitchMachineStateFnArray(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmw->hFile
              , "typedef void (*%s_STATE_FN)(p%s,%s_EVENT);\n\n"
              , cp
              , cp
              , cp
             );
   }
   else
   {
      fprintf(pcmw->hFile
              , "typedef %s_%s (*%s_STATE_FN)(p%s,%s_EVENT);\n\n"
              , cp
              , (pmi->modFlags & mfActionsReturnStates) ? "STATE" : "EVENT"
              , cp
              , cp
              , cp
             );
   }

   fprintf(pcmw->hFile
           , "extern const %s_STATE_FN %s_state_fn_array[%s_numStates];\n\n"
           , cp
           , pmi->name->name
           , pmi->name->name
          );
}

static void declareCMachineActionArray(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{

   /*
     Actions which return events or void have state transitions
       stored in a struct with the function pointer.
 
     Actions which return states do not.
   */
   if (pmi->modFlags & mfActionsReturnStates)
   {

      /* publish the array */
      fprintf(pcmw->hFile
              , "extern const %s_ACTION_FN %s_ACTION_ARRAY[%s_numEvents][%s_numStates];\n\n",
              cp,
              cp,
              pmi->name->name,
              pmi->name->name
             );

   }
   else
   {

      /* build the structure for the action array */

      /* 
         if compacting, we will use an array for the action functions,
         and the transition functions, should they exist
       */
      if (compact_action_array)
      {
         declareCMachineActionFnEnum(pcmw, pmi, cp);
         if (pmi->transition_fn_list->count)
         {
            declareCMachineTransitionFnEnum(pcmw, pmi, cp);
         }
      }

      /* now do the action/transition array */
      fprintf(pcmw->hFile, "typedef struct _%s_action_trans_struct_ {\n",
              pmi->name->name);

      fprintf(pcmw->hFile
              , "\t%s_ACTION_FN%s\taction;\n"
              , cp
              , compact_action_array ? "_E" : ""
             );

      fprintf(pcmw->hFile
              , "\t%s_%s%s\ttransition;\n} %s_ACTION_TRANS, *p%s_ACTION_TRANS;\n\n"
              , cp
              , pmi->transition_fn_list->count ? "TRANSITION_FN" : "STATE"
              , (pmi->transition_fn_list->count && compact_action_array) ? "_E" : ""
              , cp
              , cp
             );

      /* publish the array */
      fprintf(pcmw->hFile, "extern const %s_ACTION_TRANS %s_action_array[%s_numEvents][%s_numStates];\n\n"
              , cp
              , pmi->name->name
              , pmi->name->name
              , pmi->name->name
             );

   }

}

static void declareCMachineActionFnEnum(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   pID_INFO pid_info;
   ITERATOR_CALLBACK_HELPER ich = {0};

   ich.first = true;;
   ich.pcmw  = pcmw;
   ich.pmi   = pmi;
   ich.cp    = cp;

   /* enum */
   fprintf(pcmw->hFile
           , "typedef enum _%s_action_fn_ %s_ACTION_FN_E;\nenum _%s_action_fn_\n{\n"
           , pmi->name->name
           , cp
           , pmi->name->name
          );

   iterate_list(pmi->action_list, declare_action_enum_member,&ich);

   /* declare the dummy, or no op action */
   fprintf(pcmw->hFile
           , ", %s_noAction_e\n"
           , pmi->name->name
          );

   fprintf(pcmw->hFile
           , "} __attribute__((__packed__));\n\n"
          );

}

static void defineCMachineActionFnArray(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   pID_INFO pid_info;
   ITERATOR_CALLBACK_HELPER ich = {0};

   ich.first = true;
   ich.pcmw  = pcmw;
   ich.pmi   = pmi;
   ich.cp    = cp;


   /* array */
   fprintf(pcmw->cFile
           , "const %s_ACTION_FN %s_action_fns[] = \n{\n"
           , cp
           , pmi->name->name
          );

   iterate_list(pmi->action_list,  declare_action_array_member, &ich);

   /* declare the dummy, or no op action */
   fprintf(pcmw->cFile
           , ",  %s_noAction\n"
           , pmi->name->name
          );

   fprintf(pcmw->cFile
           , "};\n\n"
          );

}

static void declareCMachineTransitionFnEnum(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   pID_INFO pid_info;
   ITERATOR_CALLBACK_HELPER ich = {0};

   ich.first = true;
   ich.pcmw  = pcmw;
   ich.pmi   = pmi;
   ich.cp    = cp;

   /* enum */
   fprintf(pcmw->hFile
           , "typedef enum _%s_transition_fn_ %s_TRANSITION_FN_E;\nenum _%s_transition_fn_\n{\n"
           , pmi->name->name
           , cp
           , pmi->name->name
          );

   iterate_list(pmi->transition_fn_list,declare_transition_fn_enum_member,&ich);
   iterate_list(pmi->transition_list,declare_transition_enum_member,&ich);

   fprintf(pcmw->hFile
           , ", %s_noTransition_e\n"
           , pmi->name->name
          );

   fprintf(pcmw->hFile
           , "} __attribute__ (( __packed__ ));\n\n"
          );

}

static void defineCMachineTransitionFnArray(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   pID_INFO pid_info;
   ITERATOR_CALLBACK_HELPER ich = {0};

   ich.first = true;
   ich.pcmw  = pcmw;
   ich.pmi   = pmi;
   ich.cp    = cp;


   /* array */
   fprintf(pcmw->cFile
           , "const %s_TRANSITION_FN %s_transition_fns[] = \n{\n"
           , cp
           , pmi->name->name
          );

   iterate_list(pmi->transition_fn_list
                ,define_transition_fn_array_member
                ,&ich
                );
   iterate_list(pmi->transition_list
                ,define_transition_array_member
                ,&ich
                );

   fprintf(pcmw->cFile
           , ", %s_noTransitionFn\n"
           , pmi->name->name
          );

   fprintf(pcmw->cFile
           , "};\n\n"
          );

}


static void declareCMachineStruct(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   /* put the machine structure definition into the header */
   fprintf(pcmw->hFile, "struct _%s_struct_ {\n",
           pmi->name->name);

   if (pmi->data)
   {
      fprintf(pcmw->hFile, "\t%s_DATA\t\t\t\t\tdata;\n",
              cp);
   }

   fprintf(pcmw->hFile, "\t%s_STATE\t\t\t\t\tstate;\n",
           cp);

   fprintf(pcmw->hFile, "\t%s_ACTION_%s const\t(*actionArray)[%s_numEvents][%s_numStates];\n"
           , cp
           , (pmi->modFlags & mfActionsReturnStates) ? "FN" : "TRANS"
           , pmi->name->name
           , pmi->name->name
          );

   if (pmi->machine_list)
   {
      fprintf(pcmw->hFile
              , "\tp%s_SUB_FSM_IF\t(*subMachineArray)[%s_numSubMachines];\n"
              , cp
              , pmi->name->name
             );
   }

   fprintf(pcmw->hFile, "\t%s_FSM\t\t\t\t\t\tfsm;\n};\n\n",
           cp);

}

static void declareCSwitchMachineStruct(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   /* put the machine structure definition into the header */
   fprintf(pcmw->hFile, "struct _%s_struct_ {\n",
           pmi->name->name);

   if (pmi->data)
   {
      fprintf(pcmw->hFile, "\t%s_DATA\t\t\t\t\tdata;\n",
              cp);
   }

   fprintf(pcmw->hFile, "\t%s_STATE\t\t\t\t\tstate;\n",
           cp);

   fprintf(pcmw->hFile, "\t%s_STATE_FN const\t(*statesArray)[%s_numStates];\n"
           , cp
           , pmi->name->name
          );

   fprintf(pcmw->hFile, "\t%s_FSM\t\t\t\t\t\tfsm;\n};\n\n",
           cp);

}

static bool print_sub_machine_if(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO pmi              = (pMACHINE_INFO) pelem->mbr;
   pITERATOR_CALLBACK_HELPER pich = (pITERATOR_CALLBACK_HELPER) data;

   fprintf(pich->pcmw->cFile
           ,"\t%s&%s_sub_fsm_if\n"
           ,pich->first ? (pich->first = false, "") : ", "
           ,pmi->name->name
          );

   return false;
}

static bool print_sub_machine_as_enum_member(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO pmi    = (pMACHINE_INFO) pelem->mbr;
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   fprintf(pih->fout
           , "\t%s %s_%s\n"
           , pih->first ? "" : ", "
           , pih->pparent->name->name
           , pmi->name->name
          );

   if (pih->first)
   {
      pih->first = false;

      fprintf(pih->fout
              , "\t, %s_firstSubMachine = %s_%s\n"
              , pih->pparent->name->name
              , pih->pparent->name->name
              , pmi->name->name
              );

   }

   return false;
}

static bool declare_sub_machine_if(pLIST_ELEMENT pelem, void *data)
{
   pMACHINE_INFO pmi    = (pMACHINE_INFO) pelem->mbr;
   pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

   fprintf(pih->fout
           , "extern %s_SUB_FSM_IF %s_sub_fsm_if;\n"
           , pih->cp
           , pmi->name->name
          );

   return false;
}

static void defineSubMachineArray(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   ITERATOR_CALLBACK_HELPER ich;

   if (pmi->machine_list)
   {
      fprintf(pcmw->cFile
              , "\np%s_SUB_FSM_IF %s_sub_fsm_if_array[%s_numSubMachines] =\n{\n"
              , cp
              , pmi->name->name
              , pmi->name->name
             );

      ich.pcmw  = pcmw;
      ich.pmi   = pmi;
      ich.first = true;
      ich.cp    = cp;
      iterate_list(pmi->machine_list,print_sub_machine_if,&ich);

      fprintf(pcmw->cFile
              ,"};\n\n"
              );
   }
}

static void defineSubMachineIF(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   char *parent_cp;

   if (pmi->parent)
   {
      parent_cp = hungarianToUnderbarCaps(pmi->parent->name->name);

      /* the function */
      fprintf(pcmw->cFile
              , "%s_EVENT %s_sub_machine_fn(%s_EVENT e)\n{\n"
              , parent_cp
              , pmi->name->name
              , parent_cp
              );

      fprintf(pcmw->cFile
              , "\treturn %sFSM(p%s, e);\n}\n\n"
              , pmi->name->name
              , pmi->name->name
              );

      fprintf(pcmw->cFile
              , "\n%s_SUB_FSM_IF %s_sub_fsm_if =\n{\n\t"
              , parent_cp
              , pmi->name->name
             );

     fprintf(pcmw->cFile
             , "\t.subFSM = %s_sub_machine_fn\n"
             , pmi->name->name
             );

     fprintf(pcmw->cFile
             , "\t, .first_event = %s_%s_%s\n"
             , pmi->parent->name->name
             , pmi->name->name
             , eventNameByIndex(pmi, 0)
             );

     fprintf(pcmw->cFile
             , "\t, .last_event = %s_%s_%s\n"
             , pmi->parent->name->name
             , pmi->name->name
             , eventNameByIndex(pmi, pmi->event_list->count - 1)
             );

     fprintf(pcmw->cFile
             ,"};\n\n"
             );
   }
}

static void defineActionArray(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   /* 
      if compacting, we will use an array for the action functions,
      and the transition functions, should they exist
    */
   if (compact_action_array)
   {
      defineCMachineActionFnArray(pcmw, pmi, cp);
      if (pmi->transition_fn_list->count)
      {
         defineCMachineTransitionFnArray(pcmw, pmi, cp);
      }
   }

   fprintf(pcmw->cFile
           , "const %s_ACTION_%s %s_action_array[%s_numEvents][%s_numStates] =\n{\n"
           , cp
           , (pmi->modFlags & mfActionsReturnStates) ? "FN" : "TRANS"
           , pmi->name->name
           , pmi->name->name
           , pmi->name->name
          );
   for (int i = 0; i < pmi->event_list->count; i++)
   {

      fprintf(pcmw->cFile, "\t{\n");

      fprintf(pcmw->cFile, "\t\t/* -- %s -- */\n\n",
              eventNameByIndex(pmi, i));

      for (int j = 0; j < pmi->state_list->count; j++)
      {

         fprintf(pcmw->cFile, "\t\t/* -- %s -- */\t",
                 stateNameByIndex(pmi, j)
                );

         if (j) fprintf(pcmw->cFile, ", ");

         if (pmi->actionArray[i][j])
         {

            if (pmi->modFlags & mfActionsReturnStates)
            {
               if (strlen(pmi->actionArray[i][j]->action->name))
               {

                  fprintf(pcmw->cFile, "%s_%s\n",
                          pmi->name->name,
                          pmi->actionArray[i][j]->action->name);
               }
               else
               {
                  fprintf(pcmw->cFile
                          , (pmi->actionArray[i][j]->transition->type == STATE)
                          ? "%s_transitionTo%s\n"
                          :
                          "%s_%s\n"
                          , pmi->name->name
                          , pmi->actionArray[i][j]->transition->name
                         );

               }

            }
            else //if (pmi->modFlags & mfActionsReturnStates)
            {

               fprintf(pcmw->cFile, "{");

               /* also handle the transition only case */
               fprintf(pcmw->cFile
                       , "%s_%s%s,"
                       , pmi->name->name
                       , strlen(pmi->actionArray[i][j]->action->name)
                       ? pmi->actionArray[i][j]->action->name
                       : "noAction"
                       , compact_action_array ? "_e" : ""
                      );


               if (pmi->transition_fn_list->count)
               {
                  if (pmi->actionArray[i][j]->transition)
                  {
                     fprintf(pcmw->cFile
                             , pmi->actionArray[i][j]->transition->type == STATE
                             ? "%s_transitionTo%s%s"
                             : "%s_%s%s"
                             , pmi->name->name
                             , pmi->actionArray[i][j]->transition->name
                             , compact_action_array ? "_e" : ""
                            );
                  }
                  else
                  {
                     fprintf(pcmw->cFile
                             , "%s_noTransition%s"
                             , pmi->name->name
                             , compact_action_array ? "_e"
                             : pmi->transition_fn_list->count ? "Fn"
                             : ""
                            );
                  }
               }
               else // if (pmi->transition_fn_list->count)
               {
                  fprintf(pcmw->cFile, "%s_%s",
                          pmi->name->name,
                          pmi->actionArray[i][j]->transition ?
                          pmi->actionArray[i][j]->transition->name :
                          stateNameByIndex(pmi, j));
               }

               fprintf(pcmw->cFile, "}\n");

            }

         }
         else // if (pmi->actionArray[i][j])
         {

            /* we need to insert a dummy here */
            if (pmi->modFlags & mfActionsReturnStates)
            {

               fprintf(pcmw->cFile, "%s_noTransition%s\n",
                       pmi->name->name
                       , compact_action_array
                         ? "_e"
                         : pmi->transition_fn_list->count
                           ? "Fn"
                           : ""
                      );

            }
            else
            {

               fprintf(pcmw->cFile, "{");

               fprintf(pcmw->cFile
                       , "%s_noAction%s, %s_%s%s"
                       , pmi->name->name
                       , compact_action_array ? "_e" : ""
                       , pmi->name->name
                       , (pmi->transition_fn_list->count == 0)
                         ? stateNameByIndex(pmi, j)
                         : "noTransition"
                       , pmi->transition_fn_list->count
                         ? compact_action_array 
                           ? "_e"
                           : "Fn"
                         : ""
                      );

               fprintf(pcmw->cFile, "}\n");

            }

         }

      }
      fprintf(pcmw->cFile, "\t},\n");

   }
   fprintf(pcmw->cFile, "};\n");

}

static void defineStateFnArray(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   fprintf(pcmw->cFile
           , "const %s_STATE_FN %s_state_fn_array[%s_numStates] = \n{\n"
           , cp
           , pmi->name->name
           , pmi->name->name
          );


   /* declare state fns */
   for (unsigned i = 0; i < pmi->state_list->count; i++)
   {
      fprintf(pcmw->cFile
              , "\t%s%s_%s_stateFn\n"
              , i ? ", " : "  "
              , pmi->name->name
              , stateNameByIndex(pmi, i)
             );
   }

   fprintf(pcmw->cFile, "};\n\n");
}

static void generateInstance(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp, char *arrayName)
{
   /* instantiate the machine and the pointer to it */
   /* the (empty) data struct and the state */
   fprintf(pcmw->cFile, "%s %s = {%s\n\t%s_%s,\n",
           cp,
           pmi->name->name,
           pmi->data ? "\n\t{0}," : "",
           pmi->name->name,
           stateNameByIndex(pmi, 0));

   fprintf(pcmw->cFile, "\t&%s_%s_array,\n"
           , pmi->name->name
           , arrayName
          );
   if (pmi->machine_list)
   {
      fprintf(pcmw->cFile
              , "\t&%s_sub_fsm_if_array,\n"
              , pmi->name->name
              );
   }

   fprintf(pcmw->cFile, "\t%sFSM\n};\n\n",
           pmi->name->name);

   fprintf(pcmw->cFile, "p%s p%s = &%s;\n\n",
           cp,
           pmi->name->name,
           pmi->name->name);
}

static void defineCMachineFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{

   fprintf(pcmw->cFile
           , "static %s_EVENT findAndRunSubMachine(p%s, %s_EVENT);\n\n"
           , cp
           , cp
           , cp
           );

   fprintf(pcmw->cFile
           , "void %sFSM(p%s pfsm, %s_EVENT event)\n{\n"
           , pmi->name->name
           , cp
           , cp
          );

   if      (pmi->modFlags & mfReentrant)           writeReentrantFSM(pcmw, pmi, cp);
   else if (pmi->modFlags & mfActionsReturnStates) writeActionsReturnStateFSM(pcmw, pmi, cp);
   else                                            writeOriginalFSM(pcmw, pmi, cp);

}

static void defineCSubMachineFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   char *parent_cp = hungarianToUnderbarCaps(pmi->parent->name->name);

   fprintf(pcmw->cFile
           , "%s_EVENT %sFSM(p%s pfsm, %s_EVENT event)\n{\n"
           , parent_cp
           , pmi->name->name
           , cp
           , parent_cp
          );

   if      (pmi->modFlags & mfReentrant)           writeReentrantFSM(pcmw, pmi, cp);
   else if (pmi->modFlags & mfActionsReturnStates) writeActionsReturnStateFSM(pcmw, pmi, cp);
   else                                            writeOriginalSubFSM(pcmw, pmi, cp);

}

static void defineCSwitchMachineFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   fprintf(pcmw->cFile
           , "void %sFSM(p%s pfsm, %s_EVENT event)\n{\n"
           , pmi->name->name
           , cp
           , cp
          );

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile, "\t%s_EVENT e = event;\n\n"
              , cp
             );
   }

   if      (pmi->modFlags & mfReentrant)           writeReentrantSwitchFSM(pcmw, pmi, cp);
   else if (pmi->modFlags & mfActionsReturnStates) writeActionsReturnStateSwitchFSM(pcmw, pmi, cp);
   else                                            writeOriginalSwitchFSM(pcmw, pmi, cp);

   fprintf(pcmw->cFile
           , "\n}\n\n"
          );

}

static void defineCSwitchMachineStateFns(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   unsigned events_handled;

   for (unsigned i = 0; i < pmi->state_list->count; i++)
   {
      if (pmi->modFlags & mfActionsReturnVoid)
      {
         fprintf(pcmw->cFile
                 , "void %s_%s_stateFn(p%s pfsm, %s_EVENT e)\n{"
                 , pmi->name->name
                 , stateNameByIndex(pmi, i)
                 , cp
                 , cp
                );
      }
      else
      {
         fprintf(pcmw->cFile
                 , "%s_%s %s_%s_stateFn(p%s pfsm, %s_EVENT e)\n{\n"
                 , cp
                 , (pmi->modFlags & mfActionsReturnStates) ? "STATE" : "EVENT"
                 , pmi->name->name
                 , stateNameByIndex(pmi, i)
                 , cp
                 , cp
                );

         fprintf(pcmw->cFile
                 , "\t%s_%s retVal = %s_no%s;\n"
                 , cp
                 , (pmi->modFlags & mfActionsReturnStates) ? "STATE" : "EVENT"
                 , pmi->name->name
                 , (pmi->modFlags & mfActionsReturnStates) ? "Transition" : "Event"
                );
      }

      if (pmi->machineTransition)
      {
         fprintf(pcmw->cFile
                 , "\t%s_STATE new_s  = pfsm->state;\n"
                 , cp
                );

      }

      fprintf(pcmw->cFile
              , "\n\tswitch(e)\n\t{\n"
             );

      events_handled = 0;
      for (int j = 0; j < pmi->event_list->count; j++)
      {
         if (pmi->actionArray[j][i])
         {
            events_handled++;

            fprintf(pcmw->cFile
                    , "\tcase %s_%s:\n"
                    , pmi->name->name
                    , eventNameByIndex(pmi, j)
                   );

            if (pmi->modFlags & mfActionsReturnStates)
            {

               fprintf(pcmw->cFile, "\t\tretVal = "
                      );

               if (strlen(pmi->actionArray[j][i]->action->name))
               {
                  fprintf(pcmw->cFile
                          , "%s_%s",
                          pmi->name->name, pmi->actionArray[j][i]->action->name);
               }
               else
               {
                  fprintf(pcmw->cFile
                          , (pmi->actionArray[j][i]->transition->type == STATE)
                          ? "%s_transitionTo%s"
                          : "%s_%s"
                          , pmi->name->name
                          , pmi->actionArray[j][i]->transition->name
                         );
               }

               fprintf(pcmw->cFile
                       , "(pfsm);\n");

            }
            else
            {
               if (strlen(pmi->actionArray[j][i]->action->name))
               {
                  fprintf(pcmw->cFile
                          , "\t\t%s%s_%s(pfsm);\n"
                          , (pmi->modFlags & mfActionsReturnVoid) ? "" : "retVal = "
                          , pmi->name->name
                          , pmi->actionArray[j][i]->action->name
                         );
               }
               else
               {
                  fprintf(pcmw->cFile
                          , "#ifdef %s_DEBUG\n\t\tDBG_PRINTF(\"%s_noAction\");\n#endif\n"
                          , cp
                          , pmi->name->name
                         );
               }

               if (pmi->actionArray[j][i]->transition)
               {
                  fprintf(pcmw->cFile
                          , "\t\t%s = "
                          , pmi->machineTransition ? "new_s" : "pfsm->state"
                         );

                  if (pmi->transition_fn_list->count)
                  {
                     fprintf(pcmw->cFile
                             , "%s_"
                             , pmi->name->name
                            );
                     fprintf(pcmw->cFile
                             , pmi->actionArray[j][i]->transition->type == STATE
                             ? "transitionTo%s"
                             : "%s"
                             , pmi->actionArray[j][i]->transition->name
                            );
                     fprintf(pcmw->cFile
                             , "(pfsm,e);\n"
                            );
                  }
                  else
                  {
                     fprintf(pcmw->cFile, "%s_%s;\n"
                             , pmi->name->name
                             , pmi->actionArray[j][i]->transition->name
                            );
                  }
               }
            }

            fprintf(pcmw->cFile
                    , "\t\tbreak;\n"
                   );
         }
         else
         {
            fprintf(pcmw->cFile
                    , "#ifdef %s_DEBUG\n\tcase %s_%s:\n\t\tDBG_PRINTF(\"%s_noAction\");\n\t\tbreak;\n#endif\n"
                    , cp
                    , pmi->name->name
                    , eventNameByIndex(pmi, j)
                    , pmi->name->name
                   );
         }

      }

      if (events_handled < pmi->event_list->count)
      {
         fprintf(pcmw->cFile
                 , "\tdefault:\n\t\tDBG_PRINTF(\"%s_noAction\");\n\t\tbreak;\n"
                 , pmi->name->name
                );
      }

      fprintf(pcmw->cFile
              , "\t}\n"
             );

      if (pmi->machineTransition)
      {
         fprintf(pcmw->cFile
                 , "\n\tif (pfsm->state != new_s)\n\t{\n"
                );

         fprintf(pcmw->cFile
                 , "\t\t%s_%s(pfsm,new_s);\n"
                 , pmi->name->name
                 , pmi->machineTransition->name
                );

         fprintf(pcmw->cFile
                 , "\t\tpfsm->state = new_s;\n\n"
                );

         fprintf(pcmw->cFile
                 , "\t}\n\n"
                );
      }

      if (!(pmi->modFlags & mfActionsReturnVoid))
      {
         fprintf(pcmw->cFile
                 , "\n\treturn retVal;\n"
                );
      }

      fprintf(pcmw->cFile, "}\n\n");

   }

   fprintf(pcmw->cFile, "\n");
}

static void defineSubMachineFinder(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   fprintf(pcmw->cFile
           , "\nstatic %s_EVENT findAndRunSubMachine(p%s pfsm, %s_EVENT e)\n{\n"
           , cp
           , cp
           , cp
          );

   fprintf(pcmw->cFile
           , "\tfor (%s_SUB_MACHINES machineIterator = %s_firstSubMachine;\n\t     machineIterator < %s_numSubMachines;\n\t     machineIterator++\n\t    )\n\t{\n"
           , cp
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

   fprintf(pcmw->cFile
           , "\t}\n\n\treturn %s_noEvent;\n\n}\n\n"
           , pmi->name->name
          );
}

static void writeOriginalSwitchFSMLoop(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile, "\twhile (e != %s_noEvent) {\n\n",
              pmi->name->name);
   }

   fprintf(pcmw->cFile, "#ifdef %s_DEBUG\n", cp);
   fprintf(pcmw->cFile, "DBG_PRINTF(\"event: %%s; state: %%s\"\n,%s_EVENT_NAMES[%s]\n,%s_STATE_NAMES[pfsm->state]\n);\n"
           , cp
           , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
           , cp
          );
   fprintf(pcmw->cFile, "#endif\n\n");

   if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmw->cFile, "\t((* (*pfsm->statesArray)[pfsm->state])(pfsm,event));\n\n");
   }
   else
   {
      fprintf(pcmw->cFile, "\t\te = ((* (*pfsm->statesArray)[pfsm->state])(pfsm,e));\n\n");
   }

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile, "\t}\n");
   }

}

static void writeOriginalSwitchFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   writeOriginalSwitchFSMLoop(pcmw, pmi, cp);
}

static void writeReentrantSwitchFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   fprintf(pcmw->cFile, "#ifdef FSM_START_CRITICAL\n");
   fprintf(pcmw->cFile, "\tFSM_START_CRITICAL;\n");
   fprintf(pcmw->cFile, "#endif\n\n");

   writeOriginalSwitchFSMLoop(pcmw, pmi, cp);

   fprintf(pcmw->cFile, "\n\n#ifdef FSM_END_CRITICAL\n");
   fprintf(pcmw->cFile, "\tFSM_END_CRITICAL;\n");
   fprintf(pcmw->cFile, "#endif\n\n");

}

static bool assignExternalEventValues(pMACHINE_INFO pmi)
{
   return (
           (pmi->modFlags & (mfActionsReturnStates | mfActionsReturnVoid))
           && (pmi->event_list->count == pmi->external_event_designation_count)
           && !compact_action_array
          );

}

static void defineWeakActionFunctionStubs(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmw      = pcmw;
   ich.pmi       = pmi;
   ich.cp        = cp;

   iterate_list(pmi->action_list, define_weak_action_function, &ich);
}

static void defineWeakNoActionFunctionStubs(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmw->cFile
              , "void __attribute__((weak)) %s_noAction(p%s pfsm)\n"
              , pmi->name->name
              , cp
             );
      fprintf(pcmw->cFile
              , "{\n\tDBG_PRINTF(\"weak: %s_noAction\");\n}\n\n"
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
                 , "{\n\tDBG_PRINTF(\"weak: %s_noAction\");\n"
                 , pmi->name->name
                 );
         fprintf(pcmw->cFile
                 , "\treturn %s_noTransition;\n}\n\n"
                 , pmi->name->name
                 );
      }
      else
      {
         fprintf(pcmw->cFile
                 , "%s_EVENT __attribute__((weak)) %s_noAction(p%s pfsm)\n"
                 , cp
                 , pmi->name->name
                 , cp
                );
         fprintf(pcmw->cFile
                 , "{\n\tDBG_PRINTF(\"weak: %s_noAction\");\n"
                 , pmi->name->name
                 );
         fprintf(pcmw->cFile
                 , "\treturn %s_noEvent;\n}\n\n"
                 , pmi->name->name
                 );
      }
   }
}

static void defineSubMachineWeakActionFunctionStubs(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   ITERATOR_CALLBACK_HELPER ich = { 0 };

   ich.pcmw      = pcmw;
   ich.pmi       = pmi;
   ich.cp        = cp;
   ich.parent_cp = hungarianToUnderbarCaps(pmi->parent->name->name);

   iterate_list(pmi->action_list
                , define_sub_machine_weak_action_function
                , &ich
                );
}

#ifdef FSM_C_TEST

#include "y.tab.h"

ID_INFO id_info8;
ID_INFO id_info7;
ID_INFO id_info6;
ID_INFO id_info5;
ID_INFO id_info4;
ID_INFO id_info3;
ID_INFO id_info2;
ID_INFO id_info1;

ACTION_SE_INFO a_se_info11 = {
   &id_info6,				NULL
};

ACTION_SE_INFO a_se_info10 = {
   &id_info5,				&a_se_info11
};

ACTION_SE_INFO a_se_info9 = {
   &id_info4,				NULL
};

ACTION_SE_INFO a_se_info8 = {
   &id_info8,				NULL
};

ACTION_SE_INFO a_se_info7 = {
   &id_info7,				&a_se_info8
};

ACTION_SE_INFO a_se_info6 = {
   &id_info4,				NULL
};

ACTION_SE_INFO a_se_info5 = {
   &id_info3,				&a_se_info6
};

ACTION_SE_INFO a_se_info4 = {
   &id_info6,				NULL
};

ACTION_SE_INFO a_se_info3 = {
   &id_info3,				NULL
};

ACTION_SE_INFO a_se_info2 = {
   &id_info2,				&a_se_info3
};

ACTION_SE_INFO a_se_info1 = {
   &id_info5,				NULL
};

ACTION_SE_INFO a_se_info0 = {
   &id_info1,				NULL
};

ACTION_INFO	action_info3 = {
   &a_se_info9,				&a_se_info10,				NULL
};

ACTION_INFO	action_info2 = {
   &a_se_info5,				&a_se_info7,				&id_info1
};

ACTION_INFO	action_info1 = {
   &a_se_info2,				&a_se_info4,				&id_info2
};

ACTION_INFO	action_info0 = {
   &a_se_info0,				&a_se_info1,				NULL
};

ID_INFO id_info12 = {
   "a4",					ACTION,		NULL,				NULL,				NULL,				NULL,					&action_info3,			0
};

ID_INFO id_info11 = {
   "a3",					ACTION,		NULL,				NULL,				NULL,				&id_info12,		&action_info2,			0
};

ID_INFO id_info10 = {
   "a2",					ACTION,		NULL,				NULL,				NULL,				&id_info11,		&action_info1,			0
};

ID_INFO id_info9 = {
   "a1",					ACTION,		NULL,				NULL,				NULL,				&id_info10,		&action_info0,			0
};

ID_INFO id_info8 = {
   "e4",					EVENT,		NULL,				NULL,				NULL,				NULL,					NULL,								3
};

ID_INFO id_info7 = {
   "e3",					EVENT,		&id_info8,	NULL,				&id_info8,	NULL,					NULL,								2
};

ID_INFO id_info6 = {
   "e2",					EVENT,		&id_info7,	NULL,				&id_info7,	NULL,					NULL,								1
};

ID_INFO id_info5 = {
   "e1",					EVENT,		&id_info6,	NULL,				&id_info6,	NULL,					NULL,								0
};

ID_INFO id_info4 = {
   "s4",					STATE,		&id_info5,	NULL,				NULL,				NULL,					NULL,								3
};

ID_INFO id_info3 = {
   "s3",					STATE,		&id_info4,	&id_info4,	NULL,				NULL,					NULL,								2
};

ID_INFO id_info2 = {
   "s2",					STATE,		&id_info3,	&id_info3,	NULL,				NULL,					NULL,								1
};

ID_INFO id_info1 = {
   "s1",					STATE,		&id_info2,	&id_info2,	NULL,				NULL,					NULL,								0
};

ID_INFO id_info0 = {
   "newMachine",	MACHINE,	&id_info1,	NULL,				NULL,				NULL,					NULL,								0
};

MACHINE_INFO machine = {
   &id_info1, 4, &id_info5, 4, &id_info9, &id_info0
};

int main(int argc, char **argv)
{

   pCMachineData	pcmw = newCMachineData("foo");

   if (!populateActionArray(&machine)) writeCMachine(pcmw, &machine);

/*
  id_info0.name = "anotherMachine";

  writeCMachine(pcmw,&machine);
*/
   destroyCMachineData(pcmw, 1);

   return (0);

}
#endif

