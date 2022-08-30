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
#include "fsm_c_common.h"

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


/*
  Our interface to the outside world
*/
static void writeCMachine(pFSMOutputGenerator,pMACHINE_INFO);
static void writeCSubMachine(pFSMOutputGenerator,pMACHINE_INFO);

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

pFSMOutputGenerator pCMachineWriter       = (pFSMOutputGenerator) &CMachineWriter;
pFSMOutputGenerator pCSubMachineWriter    = (pFSMOutputGenerator) &CSubMachineWriter;

/* list iteration callbacks */

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
static int             writeCMachineInternal(pCMachineData, pMACHINE_INFO);
static int             writeCSubMachineInternal(pCMachineData, pMACHINE_INFO);
static void            writeOriginalFSM(pCMachineData, pMACHINE_INFO, char *);
static void            writeOriginalSubFSM(pCMachineData, pMACHINE_INFO, char *);
static void            writeOriginalFSMLoop(pCMachineData, pMACHINE_INFO, char *);
static void            writeOriginalSubFSMLoop(pCMachineData, pMACHINE_INFO, char *);
static void            writeOriginalFSMLoopInnards(pCMachineData, pMACHINE_INFO, char *, char *);
static void            writeOriginalSubFSMLoopInnards(pCMachineData, pMACHINE_INFO, char *, char *);
static void            writeNoTransition(pCMachineData, pMACHINE_INFO, char *);
static void            writeReentrantFSM(pCMachineData, pMACHINE_INFO, char *);
static void            writeActionsReturnStateFSM(pCMachineData, pMACHINE_INFO, char *);
static void            declareCMachineActionArray(pCMachineData, pMACHINE_INFO, char *);
static void            defineCMachineActionFnArray(pCMachineData, pMACHINE_INFO, char *);
static void            defineCMachineTransitionFnArray(pCMachineData, pMACHINE_INFO, char *);
static void            declareCMachineActionFnEnum(pCMachineData, pMACHINE_INFO, char *);
static void            declareCMachineTransitionFnEnum(pCMachineData, pMACHINE_INFO, char *);
static void            declareCMachineStruct(pCMachineData, pMACHINE_INFO, char *);
static void            defineActionArray(pCMachineData, pMACHINE_INFO, char *);
static void            defineCMachineFSM(pCMachineData, pMACHINE_INFO, char *);
static void            defineCSubMachineFSM(pCMachineData, pMACHINE_INFO, char *);


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

static void defineCMachineFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{

   if (pmi->machine_list)
   {
      fprintf(pcmw->cFile
              , "static %s_EVENT findAndRunSubMachine(p%s, %s_EVENT);\n\n"
              , cp
              , cp
              , cp
              );
   }

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

