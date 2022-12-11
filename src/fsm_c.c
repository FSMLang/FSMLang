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

typedef struct _c_machine_data_ CMachineData, *pCMachineData;

/*
  Our interface to the outside world
*/
static int  initCMachine        (char*);
static void writeCMachine       (pMACHINE_INFO);
static void writeCSwitchMachine (pMACHINE_INFO);
static void closeCMachine       (int);

FSMOutputGenerator CMachineWriter = {
   initCMachine,
   writeCMachine,
   closeCMachine
};

FSMOutputGenerator CSwitchMachineWriter = {
   initCMachine,
   writeCSwitchMachine,
   closeCMachine
};

pFSMOutputGenerator pCMachineWriter       = &CMachineWriter;
pFSMOutputGenerator pCSwitchMachineWriter = &CSwitchMachineWriter;

/*
  Our "real" internals.
*/
static void            destroyCMachineData               (pCMachineData, int);
static pCMachineData   newCMachineData                   (char *);
static int             writeCMachineInternal             (pCMachineData, pMACHINE_INFO);
static int             writeCSwitchMachineInternal       (pCMachineData, pMACHINE_INFO);
static void            writeOriginalFSM                  (pCMachineData, pMACHINE_INFO, char *);
static void            writeOriginalFSMLoop              (pCMachineData, pMACHINE_INFO, char *);
static void            writeOriginalSwitchFSM            (pCMachineData, pMACHINE_INFO, char *);
static void            writeOriginalSwitchFSMLoop        (pCMachineData, pMACHINE_INFO, char *);
static void            writeCMachineStateTransitions     (pCMachineData, pMACHINE_INFO, char *);
static void            writeNoTransition                 (pCMachineData, pMACHINE_INFO, char *);
static void            writeReentrantFSM                 (pCMachineData, pMACHINE_INFO, char *);
static void            writeReentrantSwitchFSM           (pCMachineData, pMACHINE_INFO, char *);
static void            writeActionsReturnStateFSM        (pCMachineData, pMACHINE_INFO, char *);
static void            writeActionsReturnStateSwitchFSM  (pCMachineData, pMACHINE_INFO, char *);
static void            writeDebugInfo                    (pCMachineData, pMACHINE_INFO, char *);
static char          * commonHeaderStart                 (pCMachineData, pMACHINE_INFO, char *);
static void            commonHeaderEnd                   (pCMachineData, pMACHINE_INFO, char *, bool);
static void            declareCSwitchMachineStateFnArray (pCMachineData, pMACHINE_INFO, char *);
static void            declareCMachineActionArray        (pCMachineData, pMACHINE_INFO, char *);
static void            defineCMachineActionFnArray       (pCMachineData, pMACHINE_INFO, char *);
static void            defineCMachineTransitionFnArray   (pCMachineData, pMACHINE_INFO, char *);
static void            declareCMachineActionFnEnum       (pCMachineData, pMACHINE_INFO, char *);
static void            declareCMachineTransitionFnEnum   (pCMachineData, pMACHINE_INFO, char *);
static void            declareCMachineTransitions        (pCMachineData, pMACHINE_INFO, char *);
static void            declareCSwitchMachineTransitions  (pCMachineData, pMACHINE_INFO, char *);
static void            defineCSwitchMachineTransitions   (pCMachineData, pMACHINE_INFO, char *);
static void            declareCMachineStruct             (pCMachineData, pMACHINE_INFO, char *);
static void            declareCSwitchMachineStruct       (pCMachineData, pMACHINE_INFO, char *);
static void            defineActionArray                 (pCMachineData, pMACHINE_INFO, char *);
static void            defineStateFnArray                (pCMachineData, pMACHINE_INFO, char *);
static void            generateInstance                  (pCMachineData, pMACHINE_INFO, char *, char *);
static void            defineCMachineFSM                 (pCMachineData, pMACHINE_INFO, char *);
static void            defineCSwitchMachineFSM           (pCMachineData, pMACHINE_INFO, char *);
static void            defineCSwitchMachineStateFns      (pCMachineData, pMACHINE_INFO, char *);
static void            print_complex_event_as_enum       (pID_INFO, void *);
static void            declare_complex_event_as_function (pID_INFO, void *);
static void            declare_event_discriminators      (pCMachineData, pMACHINE_INFO, char *);
static void            write_complex_event_handlers      (pCMachineData, pMACHINE_INFO, char *);

static int             write_direct_switch_case      (FILE*, pMACHINE_INFO, char *, pACTION_INFO, pID_INFO);
static int             write_switch_case_for_top_level_complex_event (FILE*, pMACHINE_INFO, pID_INFO, pID_INFO);
static void            declare_complex_event_state_handlers          (pCMachineData, pMACHINE_INFO, char *);
static void            print_complex_event_ancestry_with_underbars   (pID_INFO, void *);
static void            declare_complex_event_state_handler           (pID_INFO, void *);
static void            write_complex_event_state_handlers            (pID_INFO, pcomplex_event_cb);
static void            write_complex_event_state_handler             (pID_INFO, void *);
static void            print_complex_event_member_debug_names        (pID_INFO, void *);
static void            declare_complex_event_member_debug_names      (pID_INFO, void *);
static void            write_event_switch_case_label                 (FILE*, pID_INFO, pMACHINE_INFO);

struct _c_machine_data_
{
   FILE	*cFile
      ,			*hFile
      ;
   char	*cName
      ,			*hName
      ;
};

/**
  pcmw

    Our own private data.

*/
pCMachineData	pcmd;

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

         pcmw->cName = createFileName(baseFileName, ".c");
         pcmw->hName = createFileName(baseFileName, ".h");

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
            time(&now);

            cp1 = getFileNameNoDir(pcmw->hName);
            for (cp = cp1; *cp; cp++)

               /* capitalise things, and change the '.' or '-' to '_' */
               *cp = ((*cp == '.') || (*cp == '-')) ? '_' : toupper(*cp);

            fprintf(pcmw->hFile, "/**\n\t%s\n\n", pcmw->hName);
            fprintf(pcmw->hFile, "\tThis file automatically generated by FSMLang\n\n");
            fprintf(pcmw->hFile, "\tOn %s\n\n*/\n", ctime(&now));

            fprintf(pcmw->hFile, "#ifndef _%s_\n", cp1);
            fprintf(pcmw->hFile, "#define _%s_\n\n", cp1);

            /* the source file */
            fprintf(pcmw->cFile, "/**\n\t%s\n\n", pcmw->cName);
            fprintf(pcmw->cFile, "\tThis file automatically generated by FSMLang\n\n");
            fprintf(pcmw->cFile, "\tOn %s\n\n*/\n", ctime(&now));

            /* label the source file */
            fprintf(pcmw->cFile, "#define FSM_CODE_FILE\n");

            /* put the call to the header file into the source */
            fprintf(pcmw->cFile, "#include \"%s\"\n\n",
                    pcmw->hName);

            CHECK_AND_FREE(cp1);

         }

      }

   }

   return pcmw;

}

static char * commonHeaderStart(pCMachineData pcmw, pMACHINE_INFO pmi, char *arrayName)
{
   pID_INFO			pid_info;
   pACTION_INFO	pai;
   char					*cp;
   int						i, j;
   bool          canAssignExternals;
   bool             need_comma;
   complex_event_cb cecb;

   /* put the native code segment out to the header */
   if (pmi->native) fprintf(pcmw->hFile, "%s\n", pmi->native);

   if (!(cp = hungarianToUnderbarCaps(pmi->name->name))) return NULL;

   fprintf(pcmw->hFile, "#ifdef %s_DEBUG\n", cp);
   fprintf(pcmw->hFile, "#include <stdio.h>\n");
   fprintf(pcmw->hFile, "#include <stdlib.h>\n");
   fprintf(pcmw->hFile, "#endif\n");

   /* put the "call the state machine" macro into the header */
   fprintf(pcmw->hFile, "\n#define RUN_STATE_MACHINE(A,B) \\\n");
   fprintf(pcmw->hFile, "\t((*A->fsm)(A,B))\n\n");

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
           , "enum _%s_EVENT_ {\n"
           , cp
           );


   if ((canAssignExternals = (pmi->external_event_designation_count > 0)) == true)
   {
      if ((canAssignExternals = assignExternalEventValues(pmi)) == false)
      {
         printf("warning: cannot use external event designations\n");
      }
   }

   //basic events
   need_comma = false;
   for (i = 0; i < pmi->event_count; i++) 
   {
      pid_info = eventPidByIndex(pmi, i);
      if (get_id_type(pid_info) == EVENT)
      {
         fprintf(pcmw->hFile, canAssignExternals 
                     ? "\t%c %s_%s = %s%s\n"
               : "\t%c %s_%s%s%s\n"
                 , need_comma ? ',' : ' '
                 ,
           pmi->name->name, pid_info->name
                 , get_event_name_prefix(pid_info, pmi)
                 , canAssignExternals
                     ? pid_info->externalDesignation->name
               : ""
           );
         need_comma = true;
      }
   }

   //complex events
   if (pmi->complex_event_count)
   {
      for (pid_info = pmi->complex_event_list;
            pid_info;
            pid_info = pid_info->nextEvent
          ) 
   {
         cecb.fp  = pcmw->hFile;
         cecb.pmi = pmi;

         fprintf(pcmw->hFile
              , "\t%c %s_%s = %s%s\n"
                 , need_comma ? ',' : ' '
                 , pmi->name->name
                 , pid_info->name
                 , pid_info->externalDesignation->name_prefix
                     ? pid_info->externalDesignation->name_prefix 
                     : ""
                 , pid_info->externalDesignation->name
                 );
         iterate_complex_event(pid_info
                               , print_complex_event_as_enum
                               , &cecb
                               );
         need_comma = true;
      }
   }

   if (
       !(pmi->modFlags & mfActionsReturnStates)
       && !(pmi->modFlags & mfActionsReturnVoid)
       )
   {
      fprintf(pcmw->hFile
              , "\t,%s_noEvent\n"
              , pmi->name->name
              );
   }

   fprintf(pcmw->hFile
           , "}%s;\n\n"
           , compact_action_array ? " __attribute__((__packed__))" : ""
           );

   fprintf(pcmw->hFile, "#ifdef %s_DEBUG\n", cp);
   fprintf(pcmw->hFile, "extern char *%s_EVENT_NAMES[];\n", cp);

   /* Now, if there are complex events, we need names for each one. */
   if (pmi->complex_event_list)
   {
      complex_event_cb cecb = {.fp = pcmw->hFile, .pmi = pmi, .ucMachineName = cp };

      for (pid_info = pmi->complex_event_list; pid_info; pid_info = pid_info->nextEvent)
      {
         declare_complex_event_member_debug_names(pid_info, &cecb);
         iterate_complex_event(pid_info,declare_complex_event_member_debug_names,&cecb);
      }
   }

   fprintf(pcmw->hFile, "#endif\n\n");

   /* put the state enum into the header file */
   fprintf(pcmw->hFile
           , "typedef enum _%s_STATE_ %s_STATE;\n"
           , cp
           , cp
           );
   fprintf(pcmw->hFile
           , "enum _%s_STATE_ {\n"
           , cp
           );

   fprintf(pcmw->hFile, "\t %s_%s\n",
           pmi->name->name,
           stateNameByIndex(pmi, 0));

   for (i = 1; i < pmi->state_count; i++)
   {
      fprintf(pcmw->hFile
              , "\t,%s_%s\n"
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
              , "\t,%s_noTransition\n"
              , pmi->name->name
              );
   }

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
   if (pmi->transition_fn_count)
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

   return cp;

}

static void commonHeaderEnd(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp, bool needNoOp)
{
   pID_INFO pid_info;

   /* declare the action functions themselves */
   for (pid_info = pmi->action_list; pid_info; pid_info = pid_info->nextAction)
   {

      if (pid_info->name && strlen(pid_info->name))
      {

         if (pmi->modFlags & mfActionsReturnVoid)
         {
            fprintf(pcmw->hFile
                    , "void %s_%s(p%s);\n"
                    , pmi->name->name
                    , pid_info->name
                    , cp
                    );
         }
         else
         {
            fprintf(pcmw->hFile
                    , (pmi->modFlags & mfActionsReturnStates)
                        ? "%s_STATE %s_%s(p%s);\n"
                        : "%s_EVENT %s_%s(p%s);\n"
                    , cp
                    , pmi->name->name
                    , pid_info->name
                    , cp
                    );
         }

      }

   }

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
         fprintf(pcmw->hFile
                 , (pmi->modFlags & mfActionsReturnStates)
                    ? "%s_STATE %s_noAction(p%s);\n\n"
                    : "%s_EVENT %s_noAction(p%s);\n\n"
                 , cp
                 , pmi->name->name
                 , cp
                 );
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
      
      if (pmi->transition_fn_count)
   {
      for (int i = 0; i < pmi->transition_count; i++)
      {
         pid_info = transitionPidByIndex(pmi, i);
         fprintf(pcmw->hFile
                 , (pid_info->type == TRANSITION_FN)
                     ? "%s_STATE %s_%s(p%s);\n"
                        : "%s_STATE %s_transitionTo%s(p%s);\n"
                    , cp
                    , pmi->name->name
                    , pid_info->name
                    , cp
                   );
         }
      }

      /* look for state only transitions */
      for (int e = 0; e < pmi->event_count; e++)
      {
         for (int s = 0; s < pmi->state_count; s++)
         {
            if (pmi->actionArray[e][s])
            {
               if (
                   (!strlen(pmi->actionArray[e][s]->action->name))
                   && (pmi->actionArray[e][s]->transition->type == STATE)
                   )
               {
                  fprintf(pcmw->hFile
                          , "%s_STATE %s_transitionTo%s(p%s);\n"
                          , cp
                          , pmi->name->name
                          , pmi->actionArray[e][s]->transition->name
                          , cp
                          );
               }
            }
         }
      }
      fprintf(pcmw->hFile, "\n");
   }
   else
   {
      if (pmi->transition_fn_count)
      {
         for (int i = 0; i < pmi->transition_count; i++)
         {
            pid_info = transitionPidByIndex(pmi, i);
            fprintf(pcmw->hFile
                    , (pid_info->type == TRANSITION_FN)
                        ? "%s_STATE %s_%s(p%s,%s_EVENT);\n"
                     : "%s_STATE %s_transitionTo%s(p%s,%s_EVENT);\n"
                 , cp
                 , pmi->name->name
                 , pid_info->name
                 , cp
                 , cp
                );
      }

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

static int writeCMachineInternal(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   char     *cp;
   pID_INFO pid_info;

   if (!pmi || !pcmw) return 1;

   if ((cp = commonHeaderStart(pcmw, pmi, "action")) == NULL) return 1;

   declareCMachineActionArray(pcmw, pmi, cp);

   declareCMachineStruct(pcmw, pmi, cp);

   commonHeaderEnd(pcmw, pmi, cp, true);
 
   declareCMachineTransitions(pcmw, pmi, cp);

   /*
     Source File
   */

   defineActionArray(pcmw, pmi, cp);

   if (generate_instance)
   {
      generateInstance(pcmw, pmi, cp, "action");
   }

   defineCMachineFSM(pcmw, pmi, cp);

   /* write our transition functions, if needed */
   if (pmi->transition_fn_count)
   {
      writeCMachineStateTransitions(pcmw, pmi, cp);
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

   if (!good)
   {

      unlink(pcmw->hName);
      unlink(pcmw->cName);

   }

   CHECK_AND_FREE(pcmw->hName);
   CHECK_AND_FREE(pcmw->cName);

   free(pcmw);

}

static int initCMachine(char *fileName)
{

   if (pcmd = newCMachineData(fileName)) return 0;

   return 1;

}

static void writeCMachine(pMACHINE_INFO pmi)
{

   writeCMachineInternal(pcmd, pmi);

}

static void closeCMachine(int how)
{

   destroyCMachineData(pcmd, how);

}

/**
  This function writes the original FSM
*/
static void writeOriginalFSM(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   #ifdef FSMLANG_DEVELOP
   fprintf(pcmw->cFile
           ,"/* writeOriginalFSM */\n"
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

   fprintf(pcmw->cFile, "\n\n}\n");
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

   fprintf(pcmw->cFile, "}\n");

}

static void writeDebugInfo(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   int i;
   pID_INFO pid_info;
   char comma = ' ';

   fprintf(pcmw->cFile, "\n#ifdef %s_DEBUG\n", cp);

   fprintf(pcmw->cFile, "char *%s_EVENT_NAMES[] = {\n", cp);

   /* These are "top level" events, when there are complex events; otherwise, they are just the events. */
   for (i = 0; i < pmi->event_count; i++)
   {
      pid_info = eventPidByIndex(pmi, i);
      if (get_id_type(pid_info = eventPidByIndex(pmi,i)) != COMPLEX_EVENT_MEMBER)
      {
         fprintf(pcmw->cFile, "\t%c[%s_%s] = \""
                 , comma
                 , pmi->name->name
           , pid_info->name
                 );

         comma = ',';

         if (assignExternalEventValues(pmi))
         {
            print_external_event_name_with_prefix(pcmw->cFile, pid_info);
         }
         else
         {
            fprintf(pcmw->cFile
                    , "%s_%s"
                    , pmi->name->name, pid_info->name
                    );
         }

         fprintf(pcmw->cFile
                 , "\"\n"
                 );
      }
   }

   for (pid_info = pmi->complex_event_list; pid_info; pid_info = pid_info->nextEvent)
   {
       fprintf(pcmw->cFile, "\t,[%s_%s] = \""
               , pmi->name->name
               , pid_info->name
              );

      if (assignExternalEventValues(pmi))
      {
         print_external_event_name_with_prefix(pcmw->cFile, pid_info);
      }
      else
      {
         fprintf(pcmw->cFile
                 , "%s_%s"
                 , pmi->name->name, pid_info->name
                 );
      }

      fprintf(pcmw->cFile
              , "\"\n"
              );
   }

   if (!(pmi->modFlags & mfActionsReturnStates) && !(pmi->modFlags & mfActionsReturnVoid))
   {
       fprintf(pcmw->cFile, "\t,\"%s_noEvent\"\n",
                                                                                                   pmi->name->name);
   }

   fprintf(pcmw->cFile, "};\n\n");

   /* Now, if there are complex events, we need names for each one. */
   if (pmi->complex_event_list)
   {
      complex_event_cb cecb = {.fp = pcmw->cFile, .pmi = pmi, .ucMachineName = cp };

      for (pid_info = pmi->complex_event_list; pid_info; pid_info = pid_info->nextEvent)
      {
         print_complex_event_member_debug_names(pid_info, &cecb);
         iterate_complex_event(pid_info,print_complex_event_member_debug_names,&cecb);
      }
   }

   fprintf(pcmw->cFile, "char *%s_STATE_NAMES[] = {\n", cp);

   fprintf(pcmw->cFile, "\t \"%s_%s\"\n",
           pmi->name->name,
           stateNameByIndex(pmi, 0));

   for (i = 1; i < pmi->state_count; i++) fprintf(pcmw->cFile, "\t,\"%s_%s\"\n",
                                                  pmi->name->name,
                                                  stateNameByIndex(pmi, i));

   if (pmi->modFlags & mfActionsReturnStates) fprintf(pcmw->cFile, "\t,\"%s_noTransition\"\n",
                                                      pmi->name->name);

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
           , "\n\tDBG_PRINTF(\"event: %%s; start state: %%s\\n\"\n\t\t,%s_EVENT_NAMES[event]\n\t\t,%s_STATE_NAMES[pfsm->state]\n\t\t);\n"
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
           , "\n\tDBG_PRINTF(\"end state: %%s\\n\"\n\t\t,%s_STATE_NAMES[pfsm->state]\n\t\t);\n"
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
           cp
           ,pmi->name->name
           );

   fprintf(pcmw->cFile
           , "\n\tDBG_PRINTF(\"event: %%s; start state: %%s\\n\"\n\t\t,%s_EVENT_NAMES[event]\n\t\t,%s_STATE_NAMES[pfsm->state]\n\t\t);\n\n"
           , cp
           , cp
           );

   fprintf(pcmw->cFile, "\ts = ((* (*pfsm->statesArray)[pfsm->state])(pfsm,e));\n\n");

   fprintf(pcmw->cFile, "\tif (s != %s_noTransition)\n\t{\n",
           pmi->name->name);

   if (pmi->machineTransition)
   {
      fprintf(pcmw->cFile
              , "\t\tif (s != pfsm->state)\n\t\t{\n"
              );

      fprintf(pcmw->cFile
           ,"\t\t\t%s_%s(pfsm,s);\n\t\t}\n"
              , pmi->name->name
              , pmi->machineTransition->name
              );

   }

   fprintf(pcmw->cFile
           , "\t\tpfsm->state = s;\n\t}\n\n"
           );

   fprintf(pcmw->cFile
           , "\n\tDBG_PRINTF(\"end state: %%s\\n\"\n\t\t,%s_STATE_NAMES[pfsm->state]\n\t\t);\n"
           , cp
           );

}

static void writeCMachineStateTransitions(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   pID_INFO pid_info;

   if (pmi->modFlags & mfActionsReturnStates)
   {
      /* grab state transitions from action array */
      for (int e = 0; e < pmi->event_count; e++)
      {
         for (int s = 0; s < pmi->state_count; s++)
         {
            if (pmi->actionArray[e][s])
            {
               if (
                   (!strlen(pmi->actionArray[e][s]->action->name))
                   && (pmi->actionArray[e][s]->transition->type == STATE)
                   )
               {
                  fprintf(pcmw->cFile
                          , "\n%s_STATE %s_transitionTo%s(p%s pfsm)\n{\n"
                          , cp
                          , pmi->name->name
                          , pmi->actionArray[e][s]->transition->name
                          , cp
                         );
                  fprintf(pcmw->cFile
                          , "\t(void) pfsm;\n\n\tDBG_PRINTF(\"%s_transitionTo%s\\n\");\n\treturn %s_%s;\n}\n\n"
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
      for (int i = 0; i < pmi->transition_count; i++)
   {
      pid_info = transitionPidByIndex(pmi, i);
      if (pid_info->type == STATE)
      {
         fprintf(pcmw->cFile
                 , "\n%s_STATE %s_transitionTo%s(p%s pfsm,%s_EVENT e)\n{\n"
                 , cp
                 , pmi->name->name
                 , pid_info->name
                 , cp
                 , cp
                );
         fprintf(pcmw->cFile
                 , "\t(void) pfsm;\n\t(void) e;\n\tDBG_PRINTF(\"%s_transitionTo%s\\n\");\n\treturn %s_%s;\n}\n\n"
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
              , "\tDBG_PRINTF(\"%s_noTransitionFn\\n\");\n\treturn %s_noTransition;\n}\n\n"
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
           , "\t(void) e;\n\tDBG_PRINTF(\"%s_noTransitionFn\\n\");\n\treturn pfsm->state;\n}\n\n"
           , pmi->name->name
          );
   }
}

static void writeOriginalFSMLoop(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile, "\twhile (e != %s_noEvent) {\n\n",
              pmi->name->name);
   }

   fprintf(pcmw->cFile, "#ifdef %s_DEBUG\n", cp);
   fprintf(pcmw->cFile, "DBG_PRINTF(\"event: %%s\\nstate: %%s\\n\"\n,%s_EVENT_NAMES[%s]\n,%s_STATE_NAMES[pfsm->state]\n);\n"
           , cp
           , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
           , cp
          );
   fprintf(pcmw->cFile, "#endif\n\n");

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      if (compact_action_array)
      {
         fprintf(pcmw->cFile
                 , "\t\tnew_e = (*%s_action_fns[(*pfsm->actionArray)[e][pfsm->state].action])(pfsm);\n\n"
                 , pmi->name->name
                 );
      }
      else
      {
         fprintf(pcmw->cFile, "\t\tnew_e = ((* (*pfsm->actionArray)[e][pfsm->state].action)(pfsm));\n\n");
      }
   }
   else
   {
      if (compact_action_array)
      {
         fprintf(pcmw->cFile
                 , "\t(*%s_action_fns[(*pfsm->actionArray)[event][pfsm->state].action])(pfsm);\n\n"
                 , pmi->name->name
                 );
      }
      else
      {
         fprintf(pcmw->cFile, "\t((* (*pfsm->actionArray)[event][pfsm->state].action)(pfsm));\n\n");
      }
   }

   if (!pmi->transition_fn_count)
   {
      fprintf(pcmw->cFile
              , "\t\t%s = (*pfsm->actionArray)[%s][pfsm->state].transition;\n\n"
              , pmi->machineTransition ? "new_s" : "pfsm->state"
              , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
              );
   }
   else
   {
      if (compact_action_array)
      {
         fprintf(pcmw->cFile
                 , "\t\t%s = (*%s_transition_fns[(*pfsm->actionArray)[%s][pfsm->state].transition])(pfsm,%s);\n\n"
                 , pmi->machineTransition ? "new_s" : "pfsm->state"
                 , pmi->name->name
                 , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
                 , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
                 );
      }
      else
      {
         fprintf(pcmw->cFile
                 , "\t\t%s = ((* (*pfsm->actionArray)[%s][pfsm->state].transition)(pfsm,%s));\n\n"
                 , pmi->machineTransition ? "new_s" : "pfsm->state"
                 , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
                 , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
                 );
      }
   }

   if (pmi->machineTransition)
   {
      fprintf(pcmw->cFile
              , "\t\tif (pfsm->state != new_s)\n\t\t{\n"
              );

      fprintf(pcmw->cFile
              ,"\t\t\t%s_%s(pfsm,new_s);\n"
              , pmi->name->name
              , pmi->machineTransition->name
              );

      fprintf(pcmw->cFile
              ,"\t\t\tpfsm->state = new_s;\n\n"
              );

      fprintf(pcmw->cFile
              , "\t\t}\n\n"
              );
   }

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile, "\t\te = new_e;\n\n");

      fprintf(pcmw->cFile, "\t}");
   }
}

static void writeCSwitchMachine(pMACHINE_INFO pmi)
{
   writeCSwitchMachineInternal(pcmd, pmi);
}

static int writeCSwitchMachineInternal(pCMachineData pcmw, pMACHINE_INFO pmi)
{
   char     *cp;

   if (!pmi || !pcmw) return 1;

   if ((cp = commonHeaderStart(pcmw, pmi, "state_fn")) == NULL) return 1;

   declareCSwitchMachineStateFnArray(pcmw, pmi, cp);

   declareCSwitchMachineStruct(pcmw, pmi, cp);

   commonHeaderEnd(pcmw, pmi, cp, false);

   declareCSwitchMachineTransitions(pcmw, pmi, cp);

   /* declare state fns */
   for (unsigned i = 0; i < pmi->state_count; i++)
   {
      if (pmi->modFlags & mfActionsReturnVoid)
      {
         fprintf(pcmw->hFile
                 , "void %s_%s_stateFn(p%s,%s_EVENT"
                 , pmi->name->name
                 , stateNameByIndex(pmi, i)
                 , cp
                 , cp
                 );

         if (pmi->machineTransition)
         {
            fprintf(pcmw->hFile
                    , ",%s_STATE*"
                    , cp
                    );
         }

         fprintf(pcmw->hFile
                 , ");\n"
                 );
      }
      else
      {
         fprintf(pcmw->hFile
                 , "%s_%s %s_%s_stateFn(p%s,%s_EVENT"
                 , cp
                 , (pmi->modFlags & mfActionsReturnStates) ? "STATE" : "EVENT"
                 , pmi->name->name
                 , stateNameByIndex(pmi, i)
                 , cp
                 , cp
                 );

         if ((pmi->machineTransition) && !(pmi->modFlags & mfActionsReturnStates))
         {
            fprintf(pcmw->hFile
                    , ",%s_STATE*"
                    , cp
                 );
      }

         fprintf(pcmw->hFile
                 , ");\n"
                 );

      }
   }

   fprintf(pcmw->hFile,"\n");

   if (pmi->complex_event_count > 0)
   {
      declare_event_discriminators(pcmw, pmi, cp);
      declare_complex_event_state_handlers(pcmw, pmi, cp);
   }

   /*
     Source File
   */

   writeDebugInfo(pcmw, pmi, cp);

   defineStateFnArray(pcmw, pmi, cp);

   if (generate_instance)
   {
      generateInstance(pcmw, pmi, cp, "state_fn");
   }

   defineCSwitchMachineFSM(pcmw, pmi, cp);

   defineCSwitchMachineStateFns(pcmw, pmi, cp);

   /* no need to write transition functions */

   /* write complex event handlers */
   if (pmi->complex_event_count)
   {
      write_complex_event_handlers(pcmw, pmi, cp);
   }

   FREE_AND_CLEAR(cp);

   return 0; 
}

static void declareCSwitchMachineStateFnArray(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmw->hFile
              ,"typedef void (*%s_STATE_FN)(p%s,%s_EVENT"
              , cp
              , cp
              , cp
              );

      if (pmi->machineTransition)
      {
         fprintf(pcmw->hFile
                 , ",%s_STATE*"
                 , cp
                 );
      }

      fprintf(pcmw->hFile
              , ");\n\n"
              );
   }
   else
   {
      fprintf(pcmw->hFile
              , "typedef %s_%s (*%s_STATE_FN)(p%s,%s_EVENT"
              , cp
              , (pmi->modFlags & mfActionsReturnStates) ? "STATE" : "EVENT"
              , cp
              , cp
              , cp
              );

      if ((pmi->machineTransition) && !(pmi->modFlags & mfActionsReturnStates))
      {
         fprintf(pcmw->hFile
                 , ", %s_STATE*"
                 , cp
              );
   }

   fprintf(pcmw->hFile
           , ");\n\n"
              );
   }

   fprintf(pcmw->hFile
           , "extern const %s_STATE_FN %s_state_fn_array[%d];\n\n"
           , cp
           , pmi->name->name
           , pmi->state_count
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
      fprintf(pcmw->hFile, "extern const %s_ACTION_FN %s_ACTION_ARRAY[%d][%d];\n\n",
              cp,
              cp,
              pmi->event_count,
              pmi->state_count
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
         if (pmi->transition_fn_count)
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
              , pmi->transition_fn_count ? "TRANSITION_FN" : "STATE"
              , (pmi->transition_fn_count && compact_action_array) ? "_E" : ""
              , cp
              , cp
              );

      /* publish the array */
      fprintf(pcmw->hFile, "extern const %s_ACTION_TRANS %s_action_array[%d][%d];\n\n"
              , cp
              , pmi->name->name
              , pmi->event_count
              , pmi->state_count
             );

   }

}

static void declareCMachineActionFnEnum(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   pID_INFO pid_info;
   bool     first;

   /* enum */
   fprintf(pcmw->hFile
           , "typedef enum _%s_action_fn_ %s_ACTION_FN_E;\nenum _%s_action_fn_\n{\n"
           , pmi->name->name
           , cp
           , pmi->name->name
           );

   for (pid_info = pmi->action_list, first = true; pid_info; pid_info = pid_info->nextAction)
   {

      if (pid_info->name && strlen(pid_info->name))
      {

         fprintf(pcmw->hFile
                 , "%s%s_%s_e\n"
                 , first ? (first = false, "  ") : ", "
                 , pmi->name->name
                 , pid_info->name
                 );

      }

   }

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
   bool     first;


   /* array */
   fprintf(pcmw->cFile
           , "const %s_ACTION_FN %s_action_fns[] = \n{\n"
           , cp
           , pmi->name->name
           );

   for (pid_info = pmi->action_list, first = true; pid_info; pid_info = pid_info->nextAction)
   {

      if (pid_info->name && strlen(pid_info->name))
      {

         fprintf(pcmw->cFile
                 , "%s%s_%s\n"
                 , first ? (first = false, "  ") : ", "
                 , pmi->name->name
                 , pid_info->name
                 );

      }

   }

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
   unsigned i;
   bool     first;
   pID_INFO pid_info;

   /* enum */
   fprintf(pcmw->hFile
           , "typedef enum _%s_transition_fn_ %s_TRANSITION_FN_E;\nenum _%s_transition_fn_\n{\n"
           , pmi->name->name
           , cp
           , pmi->name->name
           );

   for (i = 0, first = true; i < pmi->transition_count; i++)
   {
      pid_info = transitionPidByIndex(pmi, i);
      fprintf(pcmw->hFile
              , (pid_info->type == TRANSITION_FN)
              ? "%s%s_%s_e\n"
              : "%s%s_transitionTo%s_e\n"
              , first ? (first = false, "  ") : ", "
              , pmi->name->name
              , pid_info->name
             );
   }

   fprintf(pcmw->hFile
           , ", %s_noTransition_e\n"
           , pmi->name->name
           , cp
           , cp
          );

   fprintf(pcmw->hFile
           , "} __attribute__ (( __packed__ ));\n\n"
           );

}

static void defineCMachineTransitionFnArray(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   unsigned i;
   bool     first;
   pID_INFO pid_info;

   /* array */
   fprintf(pcmw->cFile
           , "const %s_TRANSITION_FN %s_transition_fns[] = \n{\n"
           , cp
           , pmi->name->name
           );

   for (i = 0, first = true; i < pmi->transition_count; i++)
   {
      pid_info = transitionPidByIndex(pmi, i);
      fprintf(pcmw->cFile
              , (pid_info->type == TRANSITION_FN)
              ? "%s%s_%s\n"
              : "%s%s_transitionTo%s\n"
              , first ? (first = false, "  ") : ", "
              , pmi->name->name
              , pid_info->name
             );
   }

   fprintf(pcmw->cFile
           , ", %s_noTransitionFn\n"
           , pmi->name->name
           , cp
           , cp
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

   fprintf(pcmw->hFile, "\t%s_ACTION_%s const\t(*actionArray)[%d][%d];\n"
           , cp
           , (pmi->modFlags & mfActionsReturnStates) ? "FN" : "TRANS"
           , pmi->event_count
           , pmi->state_count
          );

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

   fprintf(pcmw->hFile, "\t%s_STATE_FN const\t(*statesArray)[%d];\n"
           , cp
           , pmi->state_count
          );

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
      if (pmi->transition_fn_count)
      {
         defineCMachineTransitionFnArray(pcmw, pmi, cp);
      }
   }

   fprintf(pcmw->cFile
           , "const %s_ACTION_%s %s_action_array[%d][%d] =\n{\n"
           , cp
           , (pmi->modFlags & mfActionsReturnStates) ? "FN" : "TRANS"
           , pmi->name->name
           , pmi->event_count
           , pmi->state_count
          );
   for (int i = 0; i < pmi->event_count; i++)
   {

      fprintf(pcmw->cFile, "\t{\n");

      fprintf(pcmw->cFile, "\t\t/* -- %s -- */\n\n",
              eventNameByIndex(pmi, i));

      for (int j = 0; j < pmi->state_count; j++)
      {

         fprintf(pcmw->cFile, "\t\t/* -- %s -- */\t",
                 stateNameByIndex(pmi, j)
                 );

         if (j) fprintf(pcmw->cFile, ",");

         if (pmi->actionArray[i][j])
         {

            if (pmi->modFlags & mfActionsReturnStates)
            {
               if ( strlen(pmi->actionArray[i][j]->action->name) )
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
            else
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


               if (pmi->transition_fn_count)
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
                             , compact_action_array ? "_e" : "Fn"
                            );
                  }
               }
               else
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
         else
         {

            /* we need to insert a dummy here */
            if (pmi->modFlags & mfActionsReturnStates)
            {

               fprintf(pcmw->cFile, "%s_noTransition%s\n",
                       pmi->name->name
                       , compact_action_array ? "_e" : "Fn"
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
                       , (pmi->transition_fn_count == 0)
                       ? stateNameByIndex(pmi, j)
                       : "noTransition"
                       , compact_action_array ? "_e" : "Fn"
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
           , "const %s_STATE_FN %s_state_fn_array[%d] = \n{\n"
           , cp
           , pmi->name->name
           , pmi->state_count
           );


   /* declare state fns */
   for (unsigned i = 0; i < pmi->state_count; i++)
   {
      fprintf(pcmw->cFile
              , "\t%s%s_%s_stateFn\n"
              , i ? ", " : "  "
              , pmi->name->name
              , stateNameByIndex(pmi, i)
              );
   }

   fprintf(pcmw->cFile,"};\n\n");
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
           , "void %sFSM(p%s pfsm, %s_EVENT event)\n{\n"
           , pmi->name->name
           , cp
           , cp
          );

   if      (pmi->modFlags & mfReentrant)           writeReentrantFSM          (pcmw, pmi, cp);
   else if (pmi->modFlags & mfActionsReturnStates) writeActionsReturnStateFSM (pcmw, pmi, cp);
   else                                            writeOriginalFSM           (pcmw, pmi, cp);

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

   if      (pmi->modFlags & mfReentrant)           writeReentrantSwitchFSM (pcmw, pmi, cp);
   else if (pmi->modFlags & mfActionsReturnStates) writeActionsReturnStateSwitchFSM (pcmw, pmi, cp);
   else                                            writeOriginalSwitchFSM  (pcmw, pmi, cp);

   fprintf(pcmw->cFile
           , "\n}\n\n"
           );

}

static void defineCSwitchMachineStateFns(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   unsigned events_handled;

   for (unsigned i = 0; i < pmi->state_count; i++)
   {
      if (pmi->modFlags & mfActionsReturnVoid)
      {
         fprintf(pcmw->cFile
                 , "void %s_%s_stateFn(p%s pfsm, %s_EVENT e"
                 , pmi->name->name
                 , stateNameByIndex(pmi, i)
                 , cp
                 , cp
                 );

         if (pmi->machineTransition)
         {
            fprintf(pcmw->cFile
                    , ", %s_STATE *new_s"
                    , cp
                 );
      }

         fprintf(pcmw->cFile
                 , ")\n{\n"
                 );
      }
      else
      {
         fprintf(pcmw->cFile
              , "%s_%s %s_%s_stateFn(p%s pfsm, %s_EVENT e"
              , cp
              , (pmi->modFlags & mfActionsReturnStates) ? "STATE" : "EVENT"
              , pmi->name->name
              , stateNameByIndex(pmi, i)
              , cp
              , cp
                 );

         if ((pmi->machineTransition) && !(pmi->modFlags & mfActionsReturnStates))
         {
            fprintf(pcmw->cFile
                    , ", %s_STATE *new_s"
                    , cp
                    );
         }

         fprintf(pcmw->cFile
                 , ")\n{\n"
                 );

      fprintf(pcmw->cFile
              , "\t%s_%s retVal = %s_no%s;\n"
              , cp
              , (pmi->modFlags & mfActionsReturnStates) ? "STATE" : "EVENT"
              , pmi->name->name
              , (pmi->modFlags & mfActionsReturnStates) ? "Transition" : "Event"
                 );
      }

      fprintf(pcmw->cFile
              , "\n\tswitch(e)\n\t{\n"
              );

      events_handled = 0;
      #if 0
      for (int j = 0; j < pmi->event_count; j++)
      {
         if (get_id_type(eventPidByIndex(pmi, j)) != COMPLEX_EVENT_MEMBER)
         {
            events_handled += write_direct_switch_case(pcmw->cFile
                    , pmi, cp, j, i);
         }
      }

      if (pmi->complex_event_count)
      {
         for (pID_INFO ce = pmi->complex_event_list; ce; ce = ce->nextEvent)
         {
            events_handled += write_switch_case_for_top_level_complex_event(pcmw->cFile
                                                                            , pmi
                                                                            , ce
                                                                            , statePidByIndex(pmi,i)
                    );
         }
      }
      #endif

      for (pID_INFO action = pmi->action_list; action; action = action->nextAction)
            {
         for (pACTION_INFO pai = action->actionInfo; pai; pai = pai->nextAction)
         {
            for (pACTION_SE_INFO pasiS = pai->matrix->state_list; pasiS; pasiS = pasiS->next)
            {
               if (pasiS->se->seOrder == i)
               {
                  pID_INFO        lastBasicEvent = NULL;
                  pACTION_SE_INFO pasiE          = pai->matrix->event_list;

                  do
                  {
                     if (get_id_type(pasiE->se) == EVENT)
               {
                        events_handled++;
                        lastBasicEvent = pasiE->se;
                        if (pasiE->next)
                        {
                           write_event_switch_case_label(pcmw->cFile
                          , pasiE->se,
                       pmi);
                        }
                     }

                     pasiE = pasiE->next;

                  } while (pasiE);

                  if (lastBasicEvent)
                  {
                     write_direct_switch_case(pcmw->cFile, pmi, cp, pai, lastBasicEvent);
                  }
               }
            }

         }
      }

      if (pmi->complex_event_count)
      {
         for (pID_INFO ce = pmi->complex_event_list; ce; ce = ce->nextEvent)
         {
            events_handled += write_switch_case_for_top_level_complex_event(pcmw->cFile
                          , pmi
                                                                            , ce
                                                                            , statePidByIndex(pmi,i)
                                                                            );
         }
      }

      if (events_handled < pmi->event_count)
      {
         fprintf(pcmw->cFile
                 , "\tdefault:\n\t\tDBG_PRINTF(\"%s_noAction\\n\");\n\t\tbreak;\n"
                          , pmi->name->name
                 );
      }

      fprintf(pcmw->cFile
              , "\t}\n\n"
              );

#if 0 
      I do not think this belongs here; it is in the main machine loop
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
                 ,"\t\tpfsm->state = new_s;\n\n"
                 );

         fprintf(pcmw->cFile
                 , "\t}\n\n"
                 );
      }
#endif

      if (!(pmi->modFlags & mfActionsReturnVoid))
      {
         fprintf(pcmw->cFile
                 , "\n\treturn retVal;\n"
                 );
      }

      fprintf(pcmw->cFile,"}\n\n");
               }

               fprintf(pcmw->cFile
                       , "\n");

            }

static void writeOriginalSwitchFSMLoop(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
               if (pmi->machineTransition)
   {
      fprintf(pcmw->cFile
              , "\t%s_STATE new_s = pfsm->state;\n\n"
              , cp
              );
   }

   if (!(pmi->modFlags & mfActionsReturnVoid))
   {
      fprintf(pcmw->cFile, "\twhile (e != %s_noEvent) {\n\n",
              pmi->name->name);
   }

   fprintf(pcmw->cFile, "#ifdef %s_DEBUG\n", cp);
   fprintf(pcmw->cFile
                          , "DBG_PRINTF(\"event: %%s\\nstate: %%s\\n\"\n,%s_EVENT_NAMES[%s]\n,%s_STATE_NAMES[pfsm->state]\n);\n"
                          , cp
           , (pmi->modFlags & mfActionsReturnVoid) ? "event" : "e"
                          , cp
          );
   fprintf(pcmw->cFile, "#endif\n\n");

   if (pmi->modFlags & mfActionsReturnVoid)
   {
      fprintf(pcmw->cFile
              , "\t((* (*pfsm->statesArray)[pfsm->state])(pfsm,event%s));\n\n"
              , pmi->machineTransition ? ",&new_s" : ""
              );
   }
   else
   {
      fprintf(pcmw->cFile
              , "\t\te = ((* (*pfsm->statesArray)[pfsm->state])(pfsm,e%s));\n\n"
              , pmi->machineTransition ? ",&new_s" : ""
              );
               }

   if (pmi->machineTransition)
   {
                  fprintf(pcmw->cFile
                          , "\n\t\tif (pfsm->state != new_s)\n\t\t{\n"
             );

      fprintf(pcmw->cFile
              , "\t\t\t%s_%s(pfsm,new_s);\n"
              , pmi->name->name
                          , pmi->machineTransition->name
             );

      fprintf(pcmw->cFile
              , "\t\t\tpfsm->state = new_s;\n\n"
             );

      fprintf(pcmw->cFile
              , "\t\t}\n\n"
             );
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
   fprintf(pcmw->cFile
                          , "#endif\n\n");

   writeOriginalSwitchFSMLoop(pcmw, pmi, cp);

   fprintf(pcmw->cFile, "\n\n#ifdef FSM_END_CRITICAL\n");
   fprintf(pcmw->cFile, "\tFSM_END_CRITICAL;\n"
                          );
   fprintf(pcmw->cFile, "#endif\n\n");

}

static void print_complex_event_ancestry_with_underbars(pID_INFO ce, void *p)
                  {
   pcomplex_event_cb pcecb = p;

   fprintf(pcecb->fp
           , "%s%s"
           , ce->complexInfo->parent ? "_" : ""
           , ce->name
           );
}

static void print_complex_event_as_enum(pID_INFO ce, void *p)
{
   pcomplex_event_cb pcecb = p;

   fprintf(pcecb->fp, "\t, %s_"
                             , pcecb->pmi->name->name
                             );
   iterate_complex_event_ancestors(ce
                                   , print_complex_event_ancestry_with_underbars
                                   , p
                                   );
   fprintf(pcecb->fp
           , " = %s%s\n"
           , ce->complexInfo->parent->complexInfo->name_prefix
               ? ce->complexInfo->parent->complexInfo->name_prefix
               : ""
                             , ce->externalDesignation->name
           );

}

static void declare_complex_event_as_function(pID_INFO pid, void *p)
{
   pcomplex_event_cb pcecb = p;

   if (get_id_type(pid) == COMPLEX_EVENT_KEY)
   {
      fprintf(pcecb->fp
              , "%s_EVENT "
              , pcecb->ucMachineName
              );

      print_complex_event_discriminator_name(pid, pcecb);

      fprintf(pcecb->fp
              ,"(p%s);\n"
              , pcecb->ucMachineName
              );
                  }
}

void print_complex_event_state_handler_name(pID_INFO pid, pcomplex_event_cb pcecb)
{
                     fprintf(pcecb->fp
           , "%s_handle_"
                             , pcecb->pmi->name->name
           );

   if (assignExternalEventValues(pcecb->pmi))
   {
      fprintf(pcecb->fp
              , "%s"
              , pid->externalDesignation->name
                             );
                  }
   else
   {
      iterate_complex_event_ancestors(pid
                                      , print_complex_event_ancestry_with_underbars
                                      , pcecb
                                      );
   }

            fprintf(pcecb->fp
           , "_in_%s"
           , pcecb->state->name
           );
         }

void print_complex_event_discriminator_name(pID_INFO pid, pcomplex_event_cb pcecb)
{
            fprintf(pcecb->fp
           , "%s_discriminate_"
                    , pcecb->pmi->name->name
           );

   if (assignExternalEventValues(pcecb->pmi))
   {
      fprintf(pcecb->fp
              , "%s"
              , pid->externalDesignation->name
              );
   }
   else
   {
      iterate_complex_event_ancestors(pid
                                      , print_complex_event_ancestry_with_underbars
                                      , pcecb
                                      );
         }

      }

static void declare_complex_event_state_handler(pID_INFO pid, void *p)
{
   pcomplex_event_cb pcecb = p;

   if (get_id_type(pid) == COMPLEX_EVENT_KEY)
   {
      if (pcecb->pmi->modFlags & mfActionsReturnVoid)
      {
         fprintf(pcecb->fp, "void ");
      }
      else
      {
         fprintf(pcecb->fp
                 , "%s_EVENT "
                 , pcecb->ucMachineName
                 );
      }

      print_complex_event_state_handler_name(pid, pcecb);

      fprintf(pcecb->fp
              , "(p%s"
              , pcecb->ucMachineName
              );

      if ((pcecb->pmi->machineTransition) && !(pcecb->pmi->modFlags & mfActionsReturnStates))
      {
         fprintf(pcecb->fp
                 , ",%s_STATE*"
                 , pcecb->ucMachineName
                 );
      }

      fprintf(pcecb->fp
              , ");\n"
              );

   }
}

static void write_complex_event_state_handler(pID_INFO pid, void *p)
{
   pcomplex_event_cb pcecb = p;

   if (get_id_type(pid) == COMPLEX_EVENT_KEY)
   {
      if (pid->complexInfo->states[pcecb->state->seOrder])
      {
         if (pcecb->pmi->modFlags & mfActionsReturnVoid)
         {
            fprintf(pcecb->fp, "void ");
         }
         else
         {
            fprintf(pcecb->fp
                    , "%s_%s "
                    , pcecb->ucMachineName
                    , (pcecb->pmi->modFlags & mfActionsReturnStates)
                    ? "STATE"
                    : "EVENT"
                   );
         }

         print_complex_event_state_handler_name(pid, pcecb);

         fprintf(pcecb->fp
                 , "(p%s pfsm"
                 , pcecb->ucMachineName
                );

         if (pcecb->pmi->machineTransition)
         {
            fprintf(pcecb->fp
                    , ", %s_STATE *new_s"
                    , pcecb->ucMachineName
                    );
         }

         fprintf(pcecb->fp
                 ,")\n{\n"
                 );

         fprintf(pcecb->fp
                 , "\t%s_EVENT e = "
                 , pcecb->ucMachineName
                 );

         print_complex_event_discriminator_name(pid, pcecb);

         fprintf(pcecb->fp
                 , "(pfsm);\n\n"
                 );

         if (!(pcecb->pmi->modFlags & mfActionsReturnVoid))
      {
         fprintf(pcecb->fp
                    , "\t%s_%s retVal;\n\n"
              );
      }

      fprintf(pcecb->fp, "#ifdef %s_DEBUG\n", pcecb->ucMachineName);
         fprintf(pcecb->fp
                 ,"if (e < sizeof(%s_"
                 , pcecb->ucMachineName
                 );
         iterate_complex_event_ancestors(pid, print_complex_event_ancestry_with_underbars, pcecb);
         fprintf(pcecb->fp
                 , "_EVENT_NAMES)/sizeof(char*))\n{\n"
                 );

         fprintf(pcecb->fp
                 ,"\tif (%s_"
                 , pcecb->ucMachineName
                 );
         iterate_complex_event_ancestors(pid, print_complex_event_ancestry_with_underbars, pcecb);
         fprintf(pcecb->fp
                 , "_EVENT_NAMES[e] != NULL)\n\t{\n"
                 );
         
         fprintf(pcecb->fp
                 ,"\t\tDBG_PRINTF(\"discriminated event: %%s\\n\"\n,%s_"
                 , pcecb->ucMachineName
                 );
         iterate_complex_event_ancestors(pid, print_complex_event_ancestry_with_underbars, pcecb);
         fprintf(pcecb->fp, "_EVENT_NAMES[e]);\n");
         fprintf(pcecb->fp, "\t}\n}\n#endif\n\n");

         fprintf(pcecb->fp
                 , "\tswitch (e)\n\t{\n"
                );


         /* tackle the items with children of their own */
         for (pID_INFO ce = pid->complexInfo->members; ce; ce = ce->complexInfo->nextEvent)
         {
            if (get_id_type(ce) == COMPLEX_EVENT_KEY)
            {
               write_switch_case_for_top_level_complex_event(pcecb->fp, pcecb->pmi, ce, pcecb->state);
            }
         }

         /* tackle the items with direct actions */
         for (pID_INFO action = pcecb->pmi->action_list; action; action = action->nextAction)
         {
            for (pACTION_INFO pai = action->actionInfo; pai; pai = pai->nextAction)
            {
               for (pACTION_SE_INFO pasiS = pai->matrix->state_list; pasiS; pasiS = pasiS->next)
               {
                  bool case_label_written = false;
                  if (pasiS->se == pcecb->state)
                  {

                     for (pACTION_SE_INFO pasiE = pai->matrix->event_list; pasiE; pasiE = pasiE->next)
                     {
                        if (pasiE->se->complexInfo)
                        {
                           if (pasiE->se->complexInfo->parent == pid)
                           {
                              if (get_id_type(pasiE->se) == COMPLEX_EVENT_MEMBER)
                              {
                                 write_event_switch_case_label(pcecb->fp, pasiE->se, pcecb->pmi);
                                 case_label_written = true;
                              }
                           }
                        }
                     }

                     if (case_label_written)
                     {
                        write_direct_switch_case(pcecb->fp
                                                    , pcecb->pmi
                                                    , pcecb->ucMachineName
                                                    , pai
                                                    , NULL
                                                    );
                     }
                  }
               }
            }
         }

         fprintf(pcecb->fp
                 , "\tdefault:\n\t\tDBG_PRINTF(\"discriminated event: %%d\\n%s_noAction\\n\",e);\n\t\tbreak;\n\t}\n"
                 , pcecb->pmi->name->name
                );

         fprintf(pcecb->fp
                 , "}\n\n");

      }
   }
}

void declare_event_discriminators(pCMachineData pcmw, pMACHINE_INFO pmi, char * cp)
{
   complex_event_cb cecb = { .fp = pcmw->hFile
      , .pmi = pmi
      , .ucMachineName = cp
   };

   for (pID_INFO pid = pmi->complex_event_list; pid; pid = pid->nextEvent)
   {
      declare_complex_event_as_function(pid, &cecb);
      iterate_complex_event(pid, declare_complex_event_as_function, &cecb);
   }

   fprintf(pcmw->hFile
           ,"\n");
}

void declare_complex_event_state_handlers(pCMachineData pcmw, pMACHINE_INFO pmi, char * cp)
{
   complex_event_cb cecb = { .fp = pcmw->hFile
      , .pmi = pmi
      , .ucMachineName = cp
   };

   for (pID_INFO pid = pmi->complex_event_list; pid; pid = pid->nextEvent)
   {
      for (int i = 0; i < pmi->state_count; i++)
      {
         if (pid->complexInfo->states[i])
         {
            cecb.state = pid->complexInfo->states[i];
            declare_complex_event_state_handler(pid, &cecb);
            iterate_complex_event(pid, declare_complex_event_state_handler, &cecb);
         }
      }
   }

   fprintf(pcmw->hFile
           , "\n"
          );

}

static int write_switch_case_for_top_level_complex_event(FILE *fp, pMACHINE_INFO pmi, pID_INFO ce, pID_INFO state)
{
   int events_handled = 0;
   complex_event_cb cecb = {.fp = fp
                            , .pmi = pmi
                            , .state = state 
                           };

   if (ce->complexInfo->states[state->seOrder])
   {
      events_handled = 1;

      write_event_switch_case_label(fp, ce, pmi);

      fprintf(fp
              , "\t\t"
              );

      print_complex_event_state_handler_name(ce, &cecb);

      fprintf(fp
              , "(pfsm%s);\n\t\tbreak;\n"
              , (pmi->machineTransition) ? ",new_s" : ""
              );

   }

   return events_handled;
}

static int  write_direct_switch_case(FILE *fp
                                     , pMACHINE_INFO pmi, char *cp
                                     , pACTION_INFO pai
                                     , pID_INFO event
                                     )
{
   if (event)
   {
      write_event_switch_case_label(fp, event, pmi);
   }

   if (pmi->modFlags & mfActionsReturnStates)
   {
      fprintf(fp
              , "\t\tretVal = "
             );

      if (strlen(pai->action->name))
      {
         fprintf(fp
                 , "%s_%s(pfsm);\n",
              pmi->name->name
                 , pai->action->name
                );
   }
      else
      {
         fprintf(fp
                 , (pai->transition->type == STATE)
                 ? "%s_%s;\n"
                 : "%s_%s(pfsm);\n"
                 , pmi->name->name
                 , pai->transition->name
                );
      }

   }
   else
   {
      if (strlen(pai->action->name))
      {
         fprintf(fp
                 , "\t\t%s%s_%s(pfsm);\n"
                 , (pmi->modFlags & mfActionsReturnVoid) ? "" : "retVal = "
           , pmi->name->name
                 , pai->action->name
                );
      }

      if (pai->transition)
      {
         fprintf(fp
                 , "\t\t%s = "
                 , pmi->machineTransition ? "*new_s" : "pfsm->state"
                );

   if (pmi->transition_fn_count)
   {
      fprintf(fp
                    , "%s_%s"
                    , pmi->name->name
                    , pai->transition->name
                   );
            if (pai->transition->type == TRANSITION_FN)
            {
               fprintf(fp
                       , "(pfsm, e)"
                      );
            }
            fprintf(fp
                    , ";\n"
                   );
   }
   else
   {
      fprintf(fp
                    , "%s_%s;\n"
                    , pmi->name->name
                    , pai->transition->name
                   );
         }
      }
   }

   fprintf(fp
           , "\t\tbreak;\n");

   return 1;
}

static void write_complex_event_handlers(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   complex_event_cb cecb = {
      .fp = pcmw->cFile
      , .pmi = pmi
      , .ucMachineName = cp
      };

   for (pID_INFO pid = pmi->complex_event_list; pid; pid = pid->nextEvent)
   {
      write_complex_event_state_handlers(pid, &cecb);
   }
}

static void write_complex_event_state_handlers(pID_INFO pid, pcomplex_event_cb pcecb)
{
   for (int s = 0; s < pcecb->pmi->state_count; s++)
   {
      if ((pcecb->state = pid->complexInfo->states[s]) != NULL)
      {
         write_complex_event_state_handler(pid, pcecb);
         iterate_complex_event(pid, write_complex_event_state_handler, pcecb);
      }
   }
}

static void print_complex_event_member_debug_names(pID_INFO ce, void *p)
{
   pcomplex_event_cb pcecb = p;
   char              comma = ' ';

   if (get_id_type(ce) == COMPLEX_EVENT_KEY)
   {
      fprintf(pcecb->fp
              , "char *%s_"
              , pcecb->ucMachineName
              );

      iterate_complex_event_ancestors(ce, print_complex_event_ancestry_with_underbars, pcecb);

      fprintf(pcecb->fp
              , "_EVENT_NAMES[] = {\n"
              );

      for (pID_INFO child = ce->complexInfo->members; child; child = child->complexInfo->nextEvent)
      {
         fprintf(pcecb->fp
                 , "\t%c [%s_"
                 , comma
                 , pcecb->pmi->name->name
                 );

         iterate_complex_event_ancestors(child, print_complex_event_ancestry_with_underbars, pcecb);

         fprintf(pcecb->fp
                 , "] = \""
                 );

         print_external_event_name_with_prefix(pcecb->fp
                                               , child
                                               );

         fprintf(pcecb->fp
                 , "\"\n"
                 );

         comma = ',';
      }

      fprintf(pcecb->fp
              , "};\n\n"
              );

   }
}

static void declare_complex_event_member_debug_names(pID_INFO ce, void *p)
   {
   pcomplex_event_cb pcecb = p;

   if (get_id_type(ce) == COMPLEX_EVENT_KEY)
   {
      fprintf(pcecb->fp
              , "extern char *%s_"
              , pcecb->ucMachineName
              );

      iterate_complex_event_ancestors(ce, print_complex_event_ancestry_with_underbars, pcecb);

      fprintf(pcecb->fp
              , "_EVENT_NAMES[];\n"
              );
   }

}

static void write_event_switch_case_label(FILE *fp, pID_INFO event, pMACHINE_INFO pmi)
{
   fprintf(fp
           , "\tcase "
           );

   if (assignExternalEventValues(pmi))
{
      /* this handles both complex and basic events */
      print_external_event_name_with_prefix(fp, event);
   }
   else
   {
      fprintf(fp
              , "%s_"
              , pmi->name->name
              );

      if (get_id_type(event) == EVENT)
      {
         fprintf(fp
                 , "%s"
                 , event->name
                 );
      }
      else
      {
         complex_event_cb cecb = {.fp = fp
                                  , .pmi = pmi
                                 };

         iterate_complex_event_ancestors(event
                                         , print_complex_event_ancestry_with_underbars
                                         , &cecb
                                         );
}
   }
   fprintf(fp
           , ":\n"
           );

}

static void declareCMachineTransitions(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   pID_INFO pid_info;

   /* declare any transition functions */
   if (pmi->modFlags & mfActionsReturnStates)
   {
      fprintf(pcmw->hFile
              , "%s_STATE %s_noTransitionFn(p%s);\n"
              , cp
              , pmi->name->name
              , cp
             );
      
      if (pmi->transition_fn_count)
      {
         for (int i = 0; i < pmi->transition_count; i++)
         {
            pid_info = transitionPidByIndex(pmi, i);
   fprintf(pcmw->hFile
                    , (pid_info->type == TRANSITION_FN)
                        ? "%s_STATE %s_%s(p%s);\n"
                        : "%s_STATE %s_transitionTo%s(p%s);\n"
                    , cp
                    , pmi->name->name
                    , pid_info->name
                    , cp
                   );
         }
      }

      /* look for state only transitions */
      for (int e = 0; e < pmi->event_count; e++)
      {
         for (int s = 0; s < pmi->state_count; s++)
         {
            if (pmi->actionArray[e][s])
            {
               if (
                   (!strlen(pmi->actionArray[e][s]->action->name))
                   && (pmi->actionArray[e][s]->transition->type == STATE)
                   )
               {
                  fprintf(pcmw->hFile
                          , "%s_STATE %s_transitionTo%s(p%s);\n"
                          , cp
                          , pmi->name->name
                          , pmi->actionArray[e][s]->transition->name
                          , cp);
               }
            }
         }
      }
      fprintf(pcmw->hFile, "\n");
   }
   else
   {
      if (pmi->transition_fn_count)
      {
         for (int i = 0; i < pmi->transition_count; i++)
         {
            pid_info = transitionPidByIndex(pmi, i);
   fprintf(pcmw->hFile
                    , (pid_info->type == TRANSITION_FN)
                        ? "%s_STATE %s_%s(p%s,%s_EVENT);\n"
                        : "%s_STATE %s_transitionTo%s(p%s,%s_EVENT);\n"
                    , cp
                    , pmi->name->name
                    , pid_info->name
                    , cp
                    , cp
                   );
         }

         fprintf(pcmw->hFile
                 , "%s_STATE %s_noTransitionFn(p%s,%s_EVENT);\n\n"
                 , cp
                 , pmi->name->name
                 , cp
                 , cp
                );
      }
   }

}

static void declareCSwitchMachineTransitions(pCMachineData pcmw, pMACHINE_INFO pmi, char *cp)
{
   pID_INFO pid_info;

   /* declare any transition functions */
   if (pmi->modFlags & mfActionsReturnStates)
   {
      fprintf(pcmw->hFile
              , "%s_STATE %s_noTransitionFn(p%s);\n"
              , cp
              , pmi->name->name
              , cp
             );
      
      if (pmi->transition_fn_count)
      {
         for (int i = 0; i < pmi->transition_count; i++)
         {
            pid_info = transitionPidByIndex(pmi, i);
            if (pid_info->type == TRANSITION_FN)
            {
               fprintf(pcmw->hFile
                       , "%s_STATE %s_%s(p%s);\n"
                       , cp
                       , pmi->name->name
                       , pid_info->name
                       , cp
                      );
            }
         }
      }

      fprintf(pcmw->hFile, "\n");
   }
   else
   {
      if (pmi->transition_fn_count)
      {
         for (int i = 0; i < pmi->transition_count; i++)
         {
            pid_info = transitionPidByIndex(pmi, i);
            if (pid_info->type == TRANSITION_FN)
            {
               fprintf(pcmw->hFile
                       , "%s_STATE %s_%s(p%s,%s_EVENT);\n"
                       , cp
                       , pmi->name->name
                       , pid_info->name
                       , cp
                       , cp
                      );
            }
         }

         fprintf(pcmw->hFile
                 , "%s_STATE %s_noTransitionFn(p%s,%s_EVENT);\n\n"
                 , cp
                 , pmi->name->name
                 , cp
                 , cp
                );

      }
   }

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

