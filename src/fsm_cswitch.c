#define FSMLANG_DEVELOP
/**
*  fsm_cswitch.c
*
*    Creates a Switch style C Machine
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

#include "fsm_cswitch.h"
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

static void            writeCSwitchMachine(pFSMOutputGenerator,pMACHINE_INFO);
static void            writeCSwitchSubMachine(pFSMOutputGenerator,pMACHINE_INFO);
static void            defineCSwitchMachineFSM(pCMachineData, pMACHINE_INFO, char *);
static void            defineCSwitchSubMachineFSM(pCMachineData, pMACHINE_INFO, char *);
static void            declareCSwitchMachineStruct(pCMachineData, pMACHINE_INFO, char *);
static void            declareCSwitchMachineStateFnArray(pCMachineData, pMACHINE_INFO, char *);
static void            declareCSwitchSubMachineStateFnArray(pCMachineData, pMACHINE_INFO, char *);
static void            writeActionsReturnStateSwitchFSM(pCMachineData, pMACHINE_INFO, char *);
static void            writeReentrantSwitchFSM(pCMachineData, pMACHINE_INFO, char *);
static void            writeOriginalSwitchFSM(pCMachineData, pMACHINE_INFO, char *);
static void            writeOriginalSwitchSubFSM(pCMachineData, pMACHINE_INFO, char *);
static int             writeCSwitchMachineInternal(pCMachineData, pMACHINE_INFO);
static int             writeCSwitchSubMachineInternal(pCMachineData, pMACHINE_INFO);
static void            writeOriginalSwitchFSMLoop(pCMachineData, pMACHINE_INFO, char *);
static void            writeOriginalSwitchFSMLoopInnards(pCMachineData, pMACHINE_INFO, char *, char *);
static void            writeOriginalSwitchSubFSMLoop(pCMachineData, pMACHINE_INFO, char *);
static void            defineCSwitchMachineStateFns(pCMachineData, pMACHINE_INFO, char *);
static void            defineCSwitchSubMachineStateFns(pCMachineData, pMACHINE_INFO, char *);
static void            defineStateFnArray(pCMachineData, pMACHINE_INFO, char *);
static void            cswitchHeaderEnd(pCMachineData, pMACHINE_INFO, char *, bool);
static void            cswitchSubMachineHeaderEnd(pCMachineData, pMACHINE_INFO, char *, bool);
static bool            cswitch_sub_machine_declare_transition_fn_for_when_actions_return_events(pLIST_ELEMENT,void*);

FSMCOutputGenerator CSwitchMachineWriter = {
   {
      initCMachine,
      writeCSwitchMachine,
      closeCMachine
   },
   NULL
};

FSMCSubMachineOutputGenerator CSwitchSubMachineWriter = {
   {
      initCSubMachine,
      writeCSwitchSubMachine,
      closeCMachine
   },
   NULL,
   NULL
};

pFSMOutputGenerator pCSwitchMachineWriter    = (pFSMOutputGenerator) &CSwitchMachineWriter;
pFSMOutputGenerator pCSwitchSubMachineWriter = (pFSMOutputGenerator) &CSwitchSubMachineWriter;

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

static void writeCSwitchMachine(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
   pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

   writeCSwitchMachineInternal(pfsmcog->pcmd, pmi);

   if (pmi->machine_list)
   {
      CSwitchSubMachineWriter.parent_fsmcog = pfsmcog;
      write_machines(pmi->machine_list, pCSwitchSubMachineWriter);
   }
}

static void writeCSwitchSubMachine(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{

   pFSMCOutputGenerator pfsmcog = (pFSMCOutputGenerator) pfsmog;

   writeCSwitchSubMachineInternal(pfsmcog->pcmd, pmi);
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

   cswitchHeaderEnd(pcmw, pmi, cp, false);

   /*
     Source File
   */

   addNativeImplementationIfThereIsAny(pmi, pcmw->cFile);

   defineStateFnArray(pcmw, pmi, cp);

   defineSubMachineArray(pcmw, pmi, cp);

   if (generate_instance)
   {
      generateInstance(pcmw, pmi, cp, "state_fn");
   }

   defineCSwitchMachineFSM(pcmw, pmi, cp);

   /* write our sub-machine lookup, if needed */
   if (pmi->machine_list)
   {
      defineSubMachineFinder(pcmw, pmi, cp);
   }

   defineCSwitchMachineStateFns(pcmw, pmi, cp);

   if (generate_weak_fns)
   {
      defineWeakActionFunctionStubs(pcmw, pmi, cp);

   }
   else if (force_generation_of_event_passing_actions)
   {
      defineEventPassingActions(pcmw, pmi, cp);
   }

   writeDebugInfo(pcmw, pmi, cp);

   FREE_AND_CLEAR(cp);

   return 0;
}

static int writeCSwitchSubMachineInternal(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   char     *cp;

   if (!pmi || !pcmw) return 1;

   char *parent_cp = hungarianToUnderbarCaps(pmi->parent->name->name);

   if ((cp = subMachineHeaderStart(pcmw, pmi, "state_fn")) == NULL) return 1;

   declareCSwitchSubMachineStateFnArray(pcmw, pmi, cp);

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
                 , parent_cp
                );
      }
      else
      {
         fprintf(pcmw->hFile
                 , "%s_%s %s_%s_stateFn(p%s,%s_EVENT);\n"
                 , parent_cp
                 , (pmi->modFlags & mfActionsReturnStates) ? "STATE" : "EVENT"
                 , pmi->name->name
                 , stateNameByIndex(pmi, i)
                 , cp
                 , parent_cp
                );
      }
   }

   fprintf(pcmw->hFile, "\n");

   cswitchSubMachineHeaderEnd(pcmw, pmi, cp, false);

   /*
     Source File
   */

   addNativeImplementationIfThereIsAny(pmi, pcmw->cFile);

   defineStateFnArray(pcmw, pmi, cp);

   defineSubMachineIF(pcmw, pmi, cp);

   possiblyDefineSubMachineSharedEventStructures(pcmw, pmi, cp);

   defineSubMachineArray(pcmw, pmi, cp);

   if (generate_instance)
   {
      generateInstance(pcmw, pmi, cp, "state_fn");
   }

   defineCSwitchSubMachineFSM(pcmw, pmi, cp);

   defineCSwitchSubMachineStateFns(pcmw, pmi, cp);

   if (generate_weak_fns)
   {
      defineSubMachineWeakActionFunctionStubs(pcmw, pmi, cp);

      defineSubMachineWeakDataTranslatorStubs(pcmw, pmi, cp);
   }
   else if (force_generation_of_event_passing_actions)
   {
      defineEventPassingActions(pcmw, pmi, cp);
   }

   writeDebugInfo(pcmw, pmi, cp);

   FREE_AND_CLEAR(cp);
   FREE_AND_CLEAR(parent_cp);

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

static void declareCSwitchSubMachineStateFnArray(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   char *parent_cp = hungarianToUnderbarCaps(pmi->parent->name->name);

   if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmw->hFile
              , "typedef void (*%s_STATE_FN)(p%s,%s_EVENT);\n\n"
              , cp
              , cp
              , parent_cp
             );
   }
   else
   {
      fprintf(pcmw->hFile
              , "typedef %s_%s (*%s_STATE_FN)(p%s,%s_EVENT);\n\n"
              , parent_cp
              , (pmi->modFlags & mfActionsReturnStates) ? "STATE" : "EVENT"
              , cp
              , cp
              , parent_cp
             );
   }

   fprintf(pcmw->hFile
           , "extern const %s_STATE_FN %s_state_fn_array[%s_numStates];\n\n"
           , cp
           , pmi->name->name
           , pmi->name->name
          );

   FREE_AND_CLEAR(parent_cp);
}

static void declareCSwitchMachineStruct(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   char *parent_cp = NULL;

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

   fprintf(pcmw->hFile
           , "\t%s_EVENT\t\t\t\t\tevent;\n"
           , pmi->parent 
             ? (parent_cp = hungarianToUnderbarCaps(pmi->parent->name->name))
             : cp
           );

   CHECK_AND_FREE(parent_cp);

   fprintf(pcmw->hFile, "\t%s_STATE_FN const\t(*statesArray)[%s_numStates];\n"
           , cp
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

static void defineCSwitchMachineFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   if (pmi->machine_list)
   {
      fprintf(pcmw->cFile
              , "static %s_EVENT findAndRunSubMachine(p%s, %s_EVENT);\n"
              , cp
              , cp
              , cp
              );

      if (pmi->submachine_inhibitor_count)
      {
         fprintf(pcmw->cFile
                 , "static bool doNotInhibitSubMachines(%s_STATE);\n"
                 , cp
                 );

      }

      fprintf(pcmw->cFile, "\n");
   }

   fprintf(pcmw->cFile
           , "#ifndef EVENT_IS_NOT_EXCLUDED_FROM_LOG\n"
           );

   fprintf(pcmw->cFile
           , "#define EVENT_IS_NOT_EXCLUDED_FROM_LOG(e) (e == e)\n"
           );

   fprintf(pcmw->cFile
           , "#endif\n"
           );

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

static void defineCSwitchSubMachineFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   char *parent_cp = hungarianToUnderbarCaps(pmi->parent->name->name);

   fprintf(pcmw->cFile
           , "#ifndef EVENT_IS_NOT_EXCLUDED_FROM_LOG\n"
           );

   fprintf(pcmw->cFile
           , "#define EVENT_IS_NOT_EXCLUDED_FROM_LOG(e) (e == e)\n"
           );

   fprintf(pcmw->cFile
           , "#endif\n"
           );

   fprintf(pcmw->cFile
           , "%s_EVENT %sFSM(p%s pfsm, %s_EVENT event)\n{\n"
           , parent_cp
           , pmi->name->name
           , cp
           , parent_cp
          );

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile, "\t%s_EVENT e = event;\n\n"
              , parent_cp
             );
   }

   if      (pmi->modFlags & mfReentrant)           writeReentrantSwitchFSM(pcmw, pmi, cp);
   else if (pmi->modFlags & mfActionsReturnStates) writeActionsReturnStateSwitchFSM(pcmw, pmi, cp);
   else                                            writeOriginalSwitchSubFSM(pcmw, pmi, cp);

   fprintf(pcmw->cFile
           , "\n}\n\n"
          );

   FREE_AND_CLEAR(parent_cp);
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
                          , "%s_%s(pfsm);",
                          pmi->name->name, pmi->actionArray[j][i]->action->name);
               }
               else
               {
                  fprintf(pcmw->cFile
                          , (pmi->actionArray[j][i]->transition->type == STATE)
                          ? "%s_%s;"
                          : "%s_%s(pfsm);"
                          , pmi->name->name
                          , pmi->actionArray[j][i]->transition->name
                         );
               }


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
                          , "#ifdef %s_DEBUG\n\t\t%s(\"%s_noAction\");\n#endif\n"
                          , cp
                          , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
                          , pmi->name->name
                         );
               }

               if (pmi->actionArray[j][i]->transition)
               {
                  fprintf(pcmw->cFile
                          , "\t\t%s = "
                          , pmi->machineTransition ? "new_s" : "pfsm->state"
                         );

                  fprintf(pcmw->cFile, "%s_%s%s;\n"
                          , pmi->name->name
                          , pmi->actionArray[j][i]->transition->name
                          , pmi->actionArray[j][i]->transition->type == STATE
                             ? ""
                             : "(pfsm,e)"
                          );
               }
            }

            fprintf(pcmw->cFile
                    , "\t\tbreak;\n"
                   );
         }

      }

      if (events_handled < pmi->event_list->count)
      {
         fprintf(pcmw->cFile
                 , "\tdefault:\n\t\t%s(\"%s_noAction\");\n\t\tbreak;\n"
                 , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
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

static void defineCSwitchSubMachineStateFns(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   unsigned events_handled;
   char     *parent_cp = hungarianToUnderbarCaps(pmi->parent->name->name);

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
                 , parent_cp
                 , (pmi->modFlags & mfActionsReturnStates) ? "STATE" : "EVENT"
                 , pmi->name->name
                 , stateNameByIndex(pmi, i)
                 , cp
                 , parent_cp
                );

         fprintf(pcmw->cFile
                 , "\t%s_%s retVal = THIS(no%s);\n"
                 , parent_cp
                 , (pmi->modFlags & mfActionsReturnStates) ? "STATE" : "EVENT"
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
                    , "\tcase THIS(%s):\n"
                    , eventNameByIndex(pmi, j)
                   );

            if (pmi->modFlags & mfActionsReturnStates)
            {

               fprintf(pcmw->cFile, "\t\tretVal = "
                      );

               if (strlen(pmi->actionArray[j][i]->action->name))
               {
                  fprintf(pcmw->cFile
                          , "%s_%s(pfsm);\n",
                          pmi->name->name, pmi->actionArray[j][i]->action->name);
               }
               else
               {
                  fprintf(pcmw->cFile
                          , (pmi->actionArray[j][i]->transition->type == STATE)
                          ? "%s_%s\n"
                          : "%s_%s(pfsm);\n"
                          , pmi->name->name
                          , pmi->actionArray[j][i]->transition->name
                         );
               }

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

                  fprintf(pcmw->cFile, "%s_%s%s;\n"
                          , pmi->name->name
                          , pmi->actionArray[j][i]->transition->name
                          , pmi->actionArray[j][i]->transition->type == STATE
                             ? ""
                             : "(pfsm,e)"
                          );
               }
            }

            fprintf(pcmw->cFile
                    , "\t\tbreak;\n"
                   );
         }

      }

      if (events_handled < pmi->event_list->count)
      {
         fprintf(pcmw->cFile
                 , "\tdefault:\n\t\t%s(\"%s_noAction\");\n\t\tbreak;\n"
                 , core_logging_only ? "NON_CORE_DEBUG_PRINTF" : "DBG_PRINTF"
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

   FREE_AND_CLEAR(parent_cp);

}

static void writeOriginalSwitchFSMLoopInnards(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp, char *tabstr)
{
   if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmw->cFile
              , "%s\t((* (*pfsm->statesArray)[pfsm->state])(pfsm,event));\n"
              , tabstr
              );
   }
   else
   {
      fprintf(pcmw->cFile
              , "%s\te = ((* (*pfsm->statesArray)[pfsm->state])(pfsm,e));\n"
              , tabstr
              );
   }

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile
              , "%s}\n"
              , tabstr
              );
   }
}

static void writeOriginalSwitchFSMLoop(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   char *tabstr = "\t";

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile, "\twhile (e != %s_noEvent) {\n\n",
              pmi->name->name);
   }

   fprintf(pcmw->cFile, "#ifdef %s_DEBUG\n", cp);
   fprintf(pcmw->cFile
           , "if (EVENT_IS_NOT_EXCLUDED_FROM_LOG(%s))\n{\n"
           , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
           );
   fprintf(pcmw->cFile, "\tDBG_PRINTF(\"event: %%s; state: %%s\"\n,%s_EVENT_NAMES[%s]\n,%s_STATE_NAMES[pfsm->state]\n);\n}\n"
           , cp
           , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
           , cp
          );
   fprintf(pcmw->cFile, "#endif\n\n");

   fprintf(pcmw->cFile
           , "\t/* This is read-only data to facilitate error reporting in action functions */\n"
           );

   fprintf(pcmw->cFile
           , "\tpfsm->event = %s;\n\n"
           , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
           );

   if (pmi->machine_list)
   {
      fprintf(pcmw->cFile
              , "\t\tif (e < %s_noEvent)\n\t\t{\n"
              , pmi->name->name
              );
      tabstr = "\t\t";
   }

   writeOriginalSwitchFSMLoopInnards(pcmw,pmi,cp,tabstr);

   if (pmi->machine_list)
   {
      fprintf(pcmw->cFile
              , "\t\telse\n\t\t{\n"
              );

      if (pmi->submachine_inhibitor_count)
      {
         fprintf(pcmw->cFile
                 , "\t\t\tif (doNotInhibitSubMachines(pfsm->state))\n\t"
                 );
      }
      fprintf(pcmw->cFile
              , "\t\t\te = findAndRunSubMachine(pfsm, e);"
              );

      fprintf(pcmw->cFile
              , "\n\t\t}\n\n\t}"
              );
   }

}

static void writeOriginalSwitchSubFSMLoop(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile
              , "\twhile (\n\t\t(e != THIS(noEvent))\n\t\t&& (e >= THIS(%s))\n\t)\n\t{\n\n"
              , eventNameByIndex(pmi, 0)
              );
   }

   fprintf(pcmw->cFile, "#ifdef %s_DEBUG\n", cp);
   fprintf(pcmw->cFile
           , "if (EVENT_IS_NOT_EXCLUDED_FROM_LOG(%s))\n{\n"
           , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
           );
   fprintf(pcmw->cFile, "\tDBG_PRINTF(\"event: %%s; state: %%s\"\n,%s_EVENT_NAMES[%s - THIS(%s)]\n,%s_STATE_NAMES[pfsm->state]\n);\n}\n"
           , cp
           , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
           , eventNameByIndex(pmi,0)
           , cp
          );
   fprintf(pcmw->cFile, "#endif\n\n");

   fprintf(pcmw->cFile
           , "\t/* This is read-only data to facilitate error reporting in action functions */\n"
           );

   fprintf(pcmw->cFile
           , "\tpfsm->event = %s;\n\n"
           , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
           );

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

      fprintf(pcmw->cFile
              , "\n\treturn e == THIS(noEvent) ? PARENT(noEvent) : e;"
              );
   }

}

static void writeOriginalSwitchFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   writeOriginalSwitchFSMLoop(pcmw, pmi, cp);
}

static void writeOriginalSwitchSubFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   writeOriginalSwitchSubFSMLoop(pcmw, pmi, cp);
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

void cswitchHeaderEnd(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp, bool needNoOp)
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
      if (pmi->transition_fn_list->count)
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

      }

      fprintf(pcmw->hFile, "\n");

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

void cswitchSubMachineHeaderEnd(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp, bool needNoOp)
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
      if (pmi->transition_fn_list->count)
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

      }

   }
   else
   {

      if (pmi->transition_fn_list->count)
      {
         iterate_list(pmi->transition_fn_list
                      , cswitch_sub_machine_declare_transition_fn_for_when_actions_return_events
                      , &ich
                      );

      }

   }

   iterate_list(pmi->event_list, declare_data_translator_functions, &ich);
   fprintf(pcmw->cFile, "\n");


   FREE_AND_CLEAR(ich.parent_cp);

   /* if the machine has data, declare the data init function
 
     This is pre-mature.  We need to parse the data structure
       to know what parameters this function needs.
 
   if (pmi->data)
     fprintf(pcmw->hFile,"void %s_initData(p%s);\n",
       pmi->name->name,
       cp);
   */

}

bool cswitch_sub_machine_declare_transition_fn_for_when_actions_return_events(pLIST_ELEMENT pelem, void *data)
{
   pITERATOR_CALLBACK_HELPER pich = ((pITERATOR_CALLBACK_HELPER)data);
   pID_INFO pid_info              = ((pID_INFO)pelem->mbr);

   fprintf(pich->pcmw->hFile
           , "%s_STATE %s_%s(p%s,%s_EVENT);\n"
           , pich->cp
           , pich->pmi->name->name
           , pid_info->name
           , pich->cp
           , pich->parent_cp
          );

   return false;
}

