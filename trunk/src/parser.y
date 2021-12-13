%{
	/* parser.y

		the FSM Description Language Parser.

	*/

#include <stdio.h>
#include <string.h>


#if defined (LINUX) || defined (CYGWIN)
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#endif

#if defined (LINUX)
#include <unistd.h>
#include <getopt.h>
#endif

#include "revision.h"
#include "fsm_priv.h"

#include "fsm_c.h"
#include "fsm_html.h"

int lineno=1;

//did we parse correctly?
int good=1;

extern char	*yytext;
extern FILE	*yyin, *yyout;

MACHINE_INFO  machineInfo;

pFSMOutputGenerator	pfsmog;

void yyerror(char *);

%}

%union {
	pID_INFO				pid_info;
	pACTION_SE_INFO	se_info;
	pACTION_INFO		action_info;
	pMATRIX_INFO		matrix_info;
	char *					charData;
}

%token MACHINE_KEY TRANSITION_KEY STATE_KEY EVENT_KEY ACTION_KEY ON
%token REENTRANT ACTIONS RETURN STATES EVENTS RETURNS EXTERNAL EQUALS VOID
%token <charData>	NATIVE_KEY
%token <charData>	DATA_KEY
%token <charData>	DOC_COMMENT
%token <pid_info> MACHINE
%token <pid_info> STATE
%token <pid_info> EVENT
%token <pid_info> ACTION
%token <pid_info> TRANSITION_FN
%token <pid_info> ID

%type <se_info>     event_comma_list
%type <se_info>     event_vector
%type <se_info>     state_comma_list
%type <se_info>     state_vector
%type <matrix_info> matrix
%type <pid_info>    state_decl
%type <pid_info>    event_decl
%type <pid_info>    state_decl_list
%type <pid_info>    event_decl_list
%type <pid_info>    transition
%type <pid_info>    external_designation
%type <action_info> action_matrix
%type <action_info> action
%type <action_info> action_decl
%type <action_info> action_decl_list
%type <action_info> transition_matrix
%type <action_info> transition_matrix_list
%type <charData>    doccmnt
%type <charData>    data
%type <charData>    native

%%

fsmlang: native machine 
					{ 

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a single machine\n"); 

						#else

						/* grab the native language stuff */
						machineInfo.native = $1;

						/* write the machine */
						(*pfsmog->writeMachine)(&machineInfo);

						#endif

						/* get ready for the next machine */
						freeMachineInfo(&machineInfo);

						free_ids();

					}
	| fsmlang native machine	
					{ 

						#ifdef PARSER_DEBUG
						fprintf(yyout,"making a list of machines\n"); 
						#else

						/* grab the native language stuff */
						machineInfo.native = $2;

						/* write the machine */
						(*pfsmog->writeMachine)(&machineInfo);

						#endif

						/* get ready for the next machine */
						freeMachineInfo(&machineInfo);

						free_ids();

					}
	;

machine:	doccmnt machine_modifier MACHINE_KEY ID machine_qualifier '{' data statement_decl_list '}' 
					{

						#ifdef PARSER_DEBUG
						pID_INFO	pid_info;
						pACTION_SE_INFO	pase_info;
						pACTION_INFO	pai;

						fprintf(yyout,"found a machine\n");
						fprintf(yyout
								,"\twith %d events and %d states\n"
								,machineInfo.event_count
								,machineInfo.state_count
								);

						if (machineInfo.modFlags & mfReentrant) {

							fprintf(yyout,"The machine is reentrant\n");

						}

						if (machineInfo.modFlags & mfActionsReturnStates) {

							fprintf(yyout,"Actions return states\n");

						}
						else if (machineInfo.modFlags & mfActionsReturnVoid) {

							fprintf(yyout,"Actions return void\n");

						}
						else {

							fprintf(yyout,"Actions return events\n");

						}

           if (machineInfo.machineTransition)
           {
               fprintf(yyout
                       ,"on transition: %s\n"
                       ,machineInfo.machineTransition->name
                       );
           }

						fprintf(yyout,"The states :\n");
						for (pid_info = machineInfo.state_list;
									pid_info;
									pid_info = pid_info->nextState) {

								fprintf(yyout
										,"\t%d:\t%s\n%s\n"
										,pid_info->seOrder
										,pid_info->name
										,pid_info->docCmnt ? pid_info->docCmnt : ""
										);

						}

						fprintf(yyout,"The events :\n");
						for (pid_info = machineInfo.event_list;
									pid_info;
									pid_info = pid_info->nextEvent) {

								fprintf(yyout
										,"\t%d:\t%s\n%s\n"
										,pid_info->seOrder
										,pid_info->name
										,pid_info->docCmnt ? pid_info->docCmnt : ""
										);

						}

						fprintf(yyout,"The actions :\n");
						for (pid_info = machineInfo.action_list;
									pid_info;
									pid_info = pid_info->nextAction) {

								fprintf(yyout
										,"\t%s\n"
										,strlen(pid_info->name) ? pid_info->name : "noAction"
										);

								for (pai = pid_info->actionInfo;
											pai;
											pai = pai->nextAction) {

									fprintf(yyout,"\t\twhich occurs in these events\n");
									for (pase_info = pai->matrix->event_list;
											pase_info;
											pase_info = pase_info->next) {
	
										fprintf(yyout,"\t\t\t%s\n",pase_info->se->name);
	
									}
	
									fprintf(yyout,"\t\tand states\n");
									for (pase_info = pai->matrix->state_list;
											pase_info;
											pase_info = pase_info->next) {
	
										fprintf(yyout,"\t\t\t%s\n",pase_info->se->name);
	
									}
	
									if (pai->transition)
                 {
                     switch (pai->transition->type)
                     {
                         case STATE:
										         fprintf(yyout,"\t\tand transitions to state %s\n"
                            ,pai->transition->name);
                            break;
                         case TRANSITION_FN:
										         fprintf(yyout,"\t\tand transitions using function %s\n"
                            ,pai->transition->name);
                            break;
                     }
                 }
	
								}

								if (pid_info->action_returns_decl)
               {
                 fprintf(yyout,"\t\tand returns\n");

									for (pase_info = pid_info->action_returns_decl;
											 pase_info;
											 pase_info = pase_info->next) {
	
										   fprintf(yyout,"\t\t\t%s\n",pase_info->se->name);
	
									}
               }
	
								if (pid_info->docCmnt)
									fprintf(yyout,"Doc Comments:\n%s\n"
												,pid_info->docCmnt);

						}

           fprintf(yyout,"\nThe %d transitions :\n"
                   , machineInfo.transition_count
                   );
           for (pid_info = machineInfo.transition_list;
                pid_info;
                pid_info = pid_info->nextTransition)
           {
             fprintf(yyout,"\t%s\n"
                     , pid_info->name
                    );
           }

           if (machineInfo.transition_fn_count)
           {
		           fprintf(yyout,"\nThe %d transition functions :\n"
                      , machineInfo.transition_fn_count
                      );
		           for (pid_info = machineInfo.transition_fn_list;
					          pid_info;
					          pid_info = pid_info->nextTransitionFn)
                  {
                    fprintf(yyout,"\t%s\n"
                            , pid_info->name
                            );

								     if (pid_info->transition_fn_returns_decl)
                    {
                        fprintf(yyout,"\t\twhich returns\n");

									       for (pase_info = pid_info->transition_fn_returns_decl;
											       pase_info;
											       pase_info = pase_info->next) {
	
										         fprintf(yyout,"\t\t\t%s\n",pase_info->se->name);
	
									       }
                    }
	
                  }

           }

						fprintf(yyout,"\n");

						#endif

						$4->docCmnt = $1;
						machineInfo.name = $4;
						machineInfo.data = $7;

					}
	;

machine_qualifier:
        {
						pID_INFO pid_info;
           /* note that this is not added to the machine event list; it is here only to be
           found as an event id for return decls.
           */
						add_id(EVENT,"noEvent",&pid_info);
        }
    | machine_transition_decl
    | action_return_spec
    | action_return_spec machine_transition_decl
    | machine_transition_decl action_return_spec
    ;

machine_transition_decl: ON TRANSITION_KEY ID ';'
    {
        machineInfo.machineTransition = $3;
    }
    ;

action_return_spec: 
	ACTIONS RETURN EVENTS ';'
        {
						pID_INFO pid_info;
           /* note that this is not added to the machine event list; it is here only to be
           found as an event id for return decls.
           */
						add_id(EVENT,"noEvent",&pid_info);
        }
	| ACTIONS RETURN STATES ';'
					{
             if (machineInfo.modFlags & mfActionsReturnVoid)
                yyerror("actions return void directive already seen");

						machineInfo.modFlags |= mfActionsReturnStates;
					}
	| ACTIONS RETURN VOID ';'
					{
             if (machineInfo.modFlags & mfActionsReturnStates)
                yyerror("actions return states directive already seen");

						machineInfo.modFlags |= mfActionsReturnVoid;
					}
	;

machine_modifier:
	| machine_modifier REENTRANT
					{
						machineInfo.modFlags |= mfReentrant;
					}
	;

statement_decl_list:	state_and_event_decls actions_and_transitions
	;

actions_and_transitions: actions_or_transitions
	| actions_and_transitions actions_or_transitions
	;

actions_or_transitions: action_decl
	| transition_matrix_list
 | action_return_decl
 | transition_fn_return_decl
	;

transition_matrix_list: doccmnt transition_matrix
					{
						if ($1)
							$2->action->docCmnt = $1;
						$$ = $2;
					}
	;

transition_matrix:	TRANSITION_KEY matrix STATE ';'
					{

						pID_INFO pid_info;

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition matrix\n");
						#endif

						//first, we have to add an id_info struct to the id list
						//we treat it as a "null action"
						add_id(ACTION,"",&pid_info);

						//second, we grab a struct to hold the info
						if (($$ = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						$$->action     = pid_info;
						$$->matrix     = $2;
						$$->transition = $3;

						$$->nextAction = pid_info->actionInfo;
						pid_info->actionInfo = $$;

						addToActionList(&machineInfo,$$->action);

						if (!machineInfo.actionArray) {

							if (allocateActionArray(&machineInfo))

								yyerror("out of memory");

						}

						addToActionArray(&machineInfo,$$);

           if (!$3->nextTransition && !machineInfo.transition_count)
           {
              $3->nextTransition          = machineInfo.transition_list;
              machineInfo.transition_list = $3;
              $3->tOrder                  = machineInfo.transition_count++;
           }

					}
   | TRANSITION_KEY matrix ID ';'
					{

						pID_INFO pid_info;

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition matrix with new transition function\n");
						#endif

           set_id_type($3,TRANSITION_FN);
           $3->nextTransitionFn           = machineInfo.transition_fn_list;
           machineInfo.transition_fn_list = $3;
           $3->seOrder                    = machineInfo.transition_fn_count++;

           $3->nextTransition          = machineInfo.transition_list;
           machineInfo.transition_list = $3;
           $3->tOrder                  = machineInfo.transition_count++;

						//first, we have to add an id_info struct to the id list
						//we treat it as a "null action"
						add_id(ACTION,"",&pid_info);

						//second, we grab a struct to hold the info
						if (($$ = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						$$->action     = pid_info;
						$$->matrix     = $2;
						$$->transition = $3;

						$$->nextAction = pid_info->actionInfo;
						pid_info->actionInfo = $$;

						addToActionList(&machineInfo,$$->action);

						if (!machineInfo.actionArray) {

							if (allocateActionArray(&machineInfo))

								yyerror("out of memory");

						}

						addToActionArray(&machineInfo,$$);

					}
   | TRANSITION_KEY matrix TRANSITION_FN ';'
					{

						pID_INFO pid_info;

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition matrix with known transition function\n");
						#endif

						//first, we have to add an id_info struct to the id list
						//we treat it as a "null action"
						add_id(ACTION,"",&pid_info);

						//second, we grab a struct to hold the info
						if (($$ = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						$$->action     = pid_info;
						$$->matrix     = $2;
						$$->transition = $3;

						$$->nextAction = pid_info->actionInfo;
						pid_info->actionInfo = $$;

						addToActionList(&machineInfo,$$->action);

						if (!machineInfo.actionArray) {

							if (allocateActionArray(&machineInfo))

								yyerror("out of memory");

						}

						addToActionArray(&machineInfo,$$);
          }

	;

action_decl:	action_decl_list ';'
					{

						#ifdef PARSER_DEBUG
						/* we should now be able to print a list of the actions */
						pID_INFO	pid;

						fprintf(yyout,"The actions so far(1):\n");
						for (pid = machineInfo.action_list;
									pid;
									pid = pid->nextAction) {

								fprintf(yyout,"\t%s\n",pid->name);

						}

						#endif

						$$ = $1;

					}
	;

action_decl_list: doccmnt ACTION_KEY action 	
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"started an action declaration\n");
						#endif

						addToActionList(&machineInfo,$3->action);

						if (!machineInfo.actionArray) {

							if (allocateActionArray(&machineInfo))

								yyerror("out of memory");

						}

						addToActionArray(&machineInfo,$3);

						if ($1)
							$3->action->docCmnt = $1;

						$$ = $3;

					}

	| action_decl_list ',' action 
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"added another action declarations\n");
						#endif

						addToActionList(&machineInfo,$3->action);

						$$ = $3;

					}
	;

action: action_matrix 
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an action without transition\n");
						#endif

						$$ = $1;

					}
	| action_matrix transition 	
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an action with transition\n");
						#endif

						$1->transition = $2;

						$$ = $1;

					}
	;

transition: TRANSITION_KEY STATE	
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition to known state\n");
						#endif

           if (!$2->nextTransition && !machineInfo.transition_count)
           {
              $2->nextTransition          = machineInfo.transition_list;
              machineInfo.transition_list = $2;
              $2->tOrder                  = machineInfo.transition_count++;
           }

						$$ = $2;

					}
  | TRANSITION_KEY ID
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition with new function\n");
						#endif

           set_id_type($2,TRANSITION_FN);
           $2->nextTransitionFn           = machineInfo.transition_fn_list;
           machineInfo.transition_fn_list = $2;
           $2->seOrder                    = machineInfo.transition_fn_count++;

           $2->nextTransition          = machineInfo.transition_list;
           machineInfo.transition_list = $2;
           $2->tOrder                  = machineInfo.transition_count++;

						$$ = $2;

					}
  | TRANSITION_KEY TRANSITION_FN
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition with known function\n");
						#endif

						$$ = $2;

					}
	;

action_matrix: ID matrix
					{

						#ifdef PARSER_DEBUG
						volatile pID_INFO pid_info = $1;

						fprintf(yyout,"found an action matrix\n");
						#endif

           set_id_type($1,ACTION);

						/* 
							grab an ACTION_INFO struct
						*/
						if (($$ = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						$$->action     = $1;
						$$->matrix     = $2;
						$$->transition = NULL;

						$$->nextAction = $1->actionInfo;
						$1->actionInfo = $$;

					}
   | ACTION matrix
					{

						#ifdef PARSER_DEBUG
						volatile pID_INFO pid_info = $1;

						fprintf(yyout,"found an action matrix\n");
						#endif

						/* 
							grab an ACTION_INFO struct
						*/
						if (($$ = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						$$->action     = $1;
						$$->matrix     = $2;
						$$->transition = NULL;

						$$->nextAction = $1->actionInfo;
						$1->actionInfo = $$;

					}
	;

matrix: '[' event_vector ',' state_vector ']'
     {
         #ifdef PARSER_DEBUG
         fprintf(yyout,"found a matrix\n");
         #endif

         /*
            grab a MATRIX_INFO struct
         */
						if (($$ = (pMATRIX_INFO)malloc(sizeof(MATRIX_INFO))) == NULL)

							yyerror("out of memory");

            $$->event_list = $2;
            $$->state_list = $4;

     }
 ;

state_vector: '(' state_comma_list STATE ')' 
					{

						#ifdef PARSER_DEBUG
						pACTION_SE_INFO	pse_info;
						#endif

						/* grab a state/event info record */
						if (($$ = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

							yyerror("out of memory");

						#ifdef MEM_DEBUG
						fprintf(yyout,"Adding: state ACTION_SE_INFO 0x%x\n",$$);
						#endif

						$$->next = $2;
						$$->se = $3;

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a state vector\n");
						for (pse_info = $$; pse_info; pse_info = pse_info->next)
							fprintf(yyout,"\t%s\n",pse_info->se->name); 
						#endif

					}
	| STATE
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a state scalar\n");
						#endif

						/* grab a state/event info record */
						if (($$ = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

							yyerror("out of memory");

						#ifdef MEM_DEBUG
						fprintf(yyout,"Adding: state ACTION_SE_INFO 0x%x\n",$$);
						#endif

						$$->se = $1;
						$$->next = NULL;

					}
	;

state_comma_list:	STATE ','
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the beginning of a state comma list\n");
						#endif

						/* grab a state/event info record */
						if (($$ = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

							yyerror("out of memory");

						#ifdef MEM_DEBUG
						fprintf(yyout,"Adding: state ACTION_SE_INFO 0x%x\n",$$);
						#endif

						$$->se = $1;
						$$->next = NULL;

					}
	| state_comma_list STATE ','
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the continuation of a state comma list\n");
						#endif

						/* grab a state/event info record */
						if (($$ = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

							yyerror("out of memory");

						#ifdef MEM_DEBUG
						fprintf(yyout,"Adding: state ACTION_SE_INFO 0x%x\n",$$);
						#endif

						$$->next = $1;
						$$->se = $2;

					}
	;

event_vector: '(' event_comma_list EVENT ')' 
					{

						#ifdef PARSER_DEBUG
						pACTION_SE_INFO	pse_info;
						#endif

						/* grab a state/event info record */
						if (($$ = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

							yyerror("out of memory");

						#ifdef MEM_DEBUG
						fprintf(yyout,"Adding: event ACTION_SE_INFO 0x%x\n",$$);
						#endif

						$$->next = $2;
						$$->se = $3;

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an event vector\n");
						for (pse_info = $$; pse_info; pse_info = pse_info->next)
							fprintf(yyout,"\t%s\n",pse_info->se->name); 
						#endif

					}
	| EVENT
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a scalar event : %s\n",$1->name);
						#endif

						/* grab a state/event info record */
						if (($$ = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

							yyerror("out of memory");

						#ifdef MEM_DEBUG
						fprintf(yyout,"Adding: event ACTION_SE_INFO 0x%x\n",$$);
						#endif

						$$->se = $1;
						$$->next = NULL;

					}
	;

event_comma_list:	EVENT ',' 	
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the begining of an event comma list: %s\n",$1->name);
						#endif

						/* grab a state/event info record */
						if (($$ = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

							yyerror("out of memory");

						#ifdef MEM_DEBUG
						fprintf(yyout,"Adding: event ACTION_SE_INFO 0x%x\n",$$);
						#endif

						$$->se = $1;
						$$->next = NULL;

					}
	| event_comma_list EVENT ','
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the continuation of an event comma list. adding %s\n",$2->name);
						#endif

						/* grab a state/event info record */
						if (($$ = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

							yyerror("out of memory");

						#ifdef MEM_DEBUG
						fprintf(yyout,"Adding: event ACTION_SE_INFO 0x%x\n",$$);
						#endif

						$$->next = $1;
						$$->se = $2;

					}
	;

state_and_event_decls: state_or_event_decl
	| state_and_event_decls state_or_event_decl
	;

state_or_event_decl: state_decl
	| event_decl
	;

state_decl:	state_decl_list ';'
					{

						#ifdef PARSER_DEBUG
						/* we should now be able to print a list of the states */
						pID_INFO	pid_info;

						fprintf(yyout,"The %d states so far :\n",machineInfo.state_count);
						for (pid_info = machineInfo.state_list;
									pid_info;
									pid_info = pid_info->nextState) {

								fprintf(yyout,"\t%s",pid_info->name);
               if (pid_info->externalDesignation)
                fprintf(yyout," = %s", pid_info->externalDesignation->name);
								fprintf(yyout,"\n");

						}
						#endif

						$$ = $1;

					}
	;

state_decl_list:	doccmnt STATE_KEY ID	
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the start of a state declaration list\n");
						#endif

           set_id_type($3,STATE);

						$3->nextState = machineInfo.state_list;
						machineInfo.state_list = $3;

						$3->seOrder = machineInfo.state_count++;

						$3->docCmnt             = $1;

						$$ = $3;

					}
	| state_decl_list ',' ID 
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"added another id to the state declaration list\n");
						#endif

           set_id_type($3,STATE);

						$3->nextState = machineInfo.state_list;
						machineInfo.state_list = $3;
						$3->seOrder = machineInfo.state_count++;

						$$ = $3;

					}
	;
 
event_decl:	event_decl_list ';'
					{

						#ifdef PARSER_DEBUG
						/* we should now be able to print a list of the events */
						pID_INFO	pid_info;

						fprintf(yyout,"The %d events so far :\n",machineInfo.event_count);
						for (pid_info = machineInfo.event_list;
									pid_info;
									pid_info = pid_info->nextEvent) {

								fprintf(yyout,"\t%s",pid_info->name);
               if (pid_info->externalDesignation)
                fprintf(yyout," = %s", pid_info->externalDesignation->name);
								fprintf(yyout,"\n");

						}
						#endif

						$$ = $1;

					}
	;

event_decl_list:	doccmnt EVENT_KEY ID external_designation
					{

           set_id_type($3,EVENT);

						$3->nextEvent = machineInfo.event_list;
						machineInfo.event_list = $3;

						$3->seOrder = machineInfo.event_count++;

 					$3->docCmnt             = $1;
           $3->externalDesignation = $4;
           if ($4)
           {
						   machineInfo.external_event_designation_count++;
           }

						$$ = $3;

					}
	| event_decl_list ',' ID external_designation
					{

						#ifdef PARSER_DEBUG
						fprintf(yyout,"added another id to the event declaration list\n");
						#endif

           set_id_type($3,EVENT);

						$3->nextEvent = machineInfo.event_list;
						machineInfo.event_list = $3;
						$3->seOrder = machineInfo.event_count++;
           $3->externalDesignation = $4;
           if ($4)
           {
						   machineInfo.external_event_designation_count++;
           }

						$$ = $3;

					}
	;

external_designation : {
           $$ = NULL;
     }
 | '=' ID
 {
           #ifdef PARSER_DEBUG
           fprintf(yyout,"External designation = %s\n",$2->name);
           #endif
            $$ = $2;
 }
 ;

native:	{
						$$ = NULL;
			}
	| NATIVE_KEY
					{
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native\n%s\n",$1);
						#else
						$$ = $1;
						#endif
					}

	;
 
data:	{
						$$ = NULL;
			}
	| DATA_KEY
					{
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Data\n%s\n",$1);
						#else
						$$ = $1;
						#endif
					}

	;
 
doccmnt:	{
						$$ = NULL;
					}
	| DOC_COMMENT
					{
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Document Comment\n%s\n",$1);
						#else
						$$ = $1;
						#endif
					}

	;

action_return_decl: 
  doccmnt ACTION RETURNS event_comma_list EVENT ';'
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

    if (machineInfo.modFlags & mfActionsReturnStates)
        yyerror("action returning event statement found after actions declared to return states");

			/* grab a state/event info record */
			if (($2->action_returns_decl = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

				yyerror("out of memory");

			#ifdef MEM_DEBUG
			fprintf(yyout,"Adding: event ACTION_SE_INFO 0x%x\n",$1->actionInfo->returns_decl);
			#endif

			$2->action_returns_decl->se = $5;
			$2->action_returns_decl->next = $4;

     if ($1 && !$2->docCmnt)
        $2->docCmnt = $1;

  }
  | doccmnt ACTION RETURNS EVENT ';'
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

    if (machineInfo.modFlags & mfActionsReturnStates)
        yyerror("action returning event statement found after actions declared to return states");

    if (machineInfo.modFlags & mfActionsReturnVoid)
        yyerror("action returning event statement found after actions declared to return void");

		 /* grab a state/event info record */
		 if (($2->action_returns_decl = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

				yyerror("out of memory");

			#ifdef MEM_DEBUG
			fprintf(yyout,"Adding: state ACTION_SE_INFO 0x%x\n",$1->action_returns_decl);
			#endif

			$2->action_returns_decl->se = $4;
			$2->action_returns_decl->next = NULL;

     if ($1 && !$2->docCmnt)
        $2->docCmnt = $1;

  }
  | doccmnt ACTION RETURNS state_comma_list STATE ';'
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

    if (machineInfo.modFlags & mfActionsReturnVoid)
        yyerror("action returning state statement found after actions declared to return void");

    if (!(machineInfo.modFlags & mfActionsReturnStates))
        yyerror("action returning state statement found after actions declared to return events");

			/* grab a state/event info record */
			if (($2->action_returns_decl = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

				yyerror("out of memory");

			#ifdef MEM_DEBUG
			fprintf(yyout,"Adding: event ACTION_SE_INFO 0x%x\n",$1->actionInfo->returns_decl);
			#endif

			$2->action_returns_decl->se = $5;
			$2->action_returns_decl->next = $4;

     if ($1 && !$2->docCmnt)
        $2->docCmnt = $1;

  }
  | doccmnt ACTION RETURNS STATE ';'
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

    if (machineInfo.modFlags & mfActionsReturnVoid)
        yyerror("action returning state statement found after actions declared to return void");

    if (!(machineInfo.modFlags & mfActionsReturnStates))
        yyerror("action returning state statement found after actions declared to return events");

		 /* grab a state/event info record */
		 if (($2->action_returns_decl = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

				yyerror("out of memory");

			#ifdef MEM_DEBUG
			fprintf(yyout,"Adding: state ACTION_SE_INFO 0x%x\n",$1->action_returns_decl);
			#endif

			$2->action_returns_decl->se = $4;
			$2->action_returns_decl->next = NULL;

     if ($1 && !$2->docCmnt)
        $2->docCmnt = $1;

  }
  ;
 
transition_fn_return_decl:
  doccmnt TRANSITION_FN RETURNS state_comma_list STATE ';'
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an transition_fn return declaration\n");
    #endif

		 /* grab a state/event info record */
		 if (($2->transition_fn_returns_decl = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

				yyerror("out of memory");

			#ifdef MEM_DEBUG
			fprintf(yyout,"Adding: state ACTION_SE_INFO 0x%x\n",$1->transition_fn_returns_decl);
			#endif

			$2->transition_fn_returns_decl->se = $5;
			$2->transition_fn_returns_decl->next = $4;

     if ($1 && !$2->docCmnt)
        $2->docCmnt = $1;

  }
  | doccmnt TRANSITION_FN RETURNS STATE ';'
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an transition_fn return declaration\n");
    #endif

			/* grab a state/event info record */
			if (($2->transition_fn_returns_decl = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

				yyerror("out of memory");

			#ifdef MEM_DEBUG
			fprintf(yyout,"Adding: state ACTION_SE_INFO 0x%x\n",$1->transition_fn_returns_decl);
			#endif

			$2->transition_fn_returns_decl->se = $4;
			$2->transition_fn_returns_decl->next = NULL;

     if ($1 && !$2->docCmnt)
        $2->docCmnt = $1;

  }
  ;
 
%%

#if defined(CYGWIN) || defined (LINUX)
#include <unistd.h>
#include <string.h>
#endif

#ifdef VS
#include "vsstd.h"
#include "vsstring.h"
#endif

void usage(void);
char *dotfsm = ".fsm";

int main(int argc, char **argv)
{

	int		c;
	char	*cp,*cp1;
	char  *outFileBase = 0;

	me = argv[0];

	/* special case the single '?' */
	if (argc == 2 && argv[1][0] == '?') {

		usage();
		return (1);

	}

	/* default to writing a c machine */
	pfsmog = pCMachineWriter;

	while ((c = getopt(argc,argv,"vht:o:i:c")) != -1) {

		switch(c) {

			case 'h':
				usage();
				return (1);

			case 't':

				switch (optarg[0]) {

					case 'c':

						pfsmog = pCMachineWriter;
						break;

					case 'h':

						pfsmog = pHTMLMachineWriter;
						break;

         case 's':
           pfsmog = pCSwitchMachineWriter;
						break;

					default:

						usage();
						return (1);

				}
				break;

     case 'i':
        if ('0' == optarg[0])
        {
            generate_instance = false;
        }
        break;

     case 'c':
        compact_action_array = true;
        break;

			case 'o':

				outFileBase = optarg;
				break;

     case 'v':

        fprintf(stdout,"%s\n",rev_string);
        return (0);

			case '?':
			case ':':

				usage();
				return (1);

		}

	}

	if (argv[optind]) {

		cp = strdup(argv[optind]);

		/* find the extension */
		cp1 = rindex(cp,'.');
		if (!cp1) {

			usage();
			exit(1);

		}
		else {

			if (strcmp(cp1,dotfsm))	{

				usage();
				return (1);

			}

		}

		if ((yyin = openFile(argv[optind],"r")) == NULL) {

			return 1;

		}

		/* get the base file name */
		if (!outFileBase) {
			/* use the base input file name */
			*cp1 = 0;
			outFileBase = cp;
		}

		memset(&machineInfo,0,sizeof(MACHINE_INFO));

		#ifndef PARSER_DEBUG
		if (!(*pfsmog->initOutput)(outFileBase)) {
		#endif

			yyparse();

		#ifndef PARSER_DEBUG
			(*pfsmog->closeOutput)(good);

		}
		#endif

		fclose(yyin);

		return (!good);

	}
	else {

		usage();
		return (!good);

	}

}

void yyerror(char *s)
{

  fprintf(stderr,"%s: %s\n",me,s);
	fprintf(stderr,"\tline %d : %s\n",lineno,yytext);

	good = 0;

}

void usage(void)
{

	fprintf(stdout,"Usage : %s [-tc|s|h] filename, where filename ends with '.fsm'\n",me);
	fprintf(stdout,"\t and where 'c' gets you c code output based on an event/state table,\n");
	fprintf(stdout,"\t 's' gets you c code output with individual state functions using switch constructions,\n");
	fprintf(stdout,"\t and 'h' gets you html output\n");
 fprintf(stdout,"\t%s -i0 inhibits the creation of a machine instance\n",me);
 fprintf(stdout,"\t\tany other argument to 'i' allows the creation of an instance;\n",me);
 fprintf(stdout,"\t\tthis is the default\n",me);
 fprintf(stdout,"\t%s -c will create a more compact event/state table when -tc is used\n",me);
 fprintf(stdout,"\t\twith machines having actions which return states\n",me);
 fprintf(stdout,"\t%s -v prints the version and exits\n",me);
	
}

