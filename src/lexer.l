%{
	/*
		Implements the Lexer for the FSM Description Language.
	*/

#include "fsm_priv.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#ifndef LEX_DEBUG
#if defined (CYGWIN) || defined (LINUX)
#include "y.tab.h"
#endif 
#ifdef VS
#include "parser.h"
#endif
extern int lineno;
#endif

char	charData[4096];	          /* holder for Document comments */
char *currDocCmnt = NULL;      /* points to the most recently seen document comment. */
int	charOffset = 0;						/* the character offset */
unsigned brace_cnt = 0;



#ifdef LEX_DEBUG
#include "lexer_debug.h"
#else
#define TRANSITION LOOKUP + 1
#endif

%}

%x CMNT
%x DOCCMNT
%x DATA
%x NATIVE
%x NATIVE_BLCK

%%

	/* document comments */
"/**"	{
				BEGIN DOCCMNT;
				charOffset = 0;
			}
<DOCCMNT>.	charData[charOffset++] = *yytext;	
<DOCCMNT>\n	{
						charData[charOffset++] = *yytext;	
						#ifndef LEX_DEBUG
						lineno++;
						#else
						;
						#endif
						}
<DOCCMNT>"*/"	{

			BEGIN INITIAL;
			charData[charOffset] = 0;
			#ifdef LEX_DEBUG
			printf("Doc Comment:\n%s\n",charData);
			#else

			currDocCmnt = strdup(charData);

			#endif

		}

	/* comments */
"/*"	BEGIN CMNT;
<CMNT>.	;
<CMNT>\n	{
					charData[charOffset++] = *yytext;	
					#ifndef LEX_DEBUG
					lineno++;
					#else
					;
					#endif
					}
<CMNT>"*/"	BEGIN INITIAL;

\/\/.*	;

	/* data declarations */
data	{

				BEGIN DATA;
				charOffset = 0;

			}
<DATA>"{"	 {
       if (0 != brace_cnt++)
				   charData[charOffset++] = *yytext;
     }

<DATA>"}"	{
        
         if (--brace_cnt == 0)
         {
   					BEGIN INITIAL;
   					charData[charOffset] = 0;
   					#ifdef LEX_DEBUG
   					printf("Data:\n%s\n",charData);
   					#else
   
   					yylval.charData = strdup(charData);
   					return DATA_KEY;
   					#endif
         }
         else
         {
						charData[charOffset++] = *yytext;
         }

				}

<DATA>translator {
					BEGIN INITIAL;

         #ifdef LEX_DEBUG
         printf("%s\n",strings[TRANSLATOR_KEY]);
         #else
         return TRANSLATOR_KEY;
         #endif

       }

<DATA>.	{
       charData[charOffset++] = *yytext;
   }

<DATA>\n	{
					charData[charOffset++] = *yytext;
					#ifndef LEX_DEBUG
					lineno++;
					#else
					;
					#endif
					}

	/* native language declarations */
native	{

				BEGIN NATIVE;

       #ifndef LEX_DEBUG
       return NATIVE_KEY;
       #else
       printf("%s\n", strings[NATIVE_KEY]);
       #endif

			}

<NATIVE>"{"	{
       if (0 == brace_cnt++)
       {
				   BEGIN NATIVE_BLCK;
				   charOffset = 0;
       }
       else
       {
				   charData[charOffset++] = *yytext;
       }
    }

<NATIVE>impl(ementation)? {

       #ifndef LEX_DEBUG
       return IMPLEMENTATION_KEY;
       #else
       printf("%s\n", strings[IMPLEMENTATION_KEY]);
       #endif

    }

<NATIVE>\n {

    #ifndef LEX_DEBUG
    lineno++;
    #else
    ;
    #endif

   }

<NATIVE>. ;

<NATIVE_BLCK>"{"	{
        if (0 != brace_cnt++)
				    charData[charOffset++] = *yytext;
   }

<NATIVE_BLCK>"}"	{

         if (--brace_cnt == 0)
         {
   					BEGIN INITIAL;
   					charData[charOffset] = 0;
   					#ifdef LEX_DEBUG
   					printf("Native:\n%s\n",charData);
   					#else
   
   					yylval.charData = strdup(charData);
   					return NATIVE_BLOCK;
   					#endif
         }
         else
         {
						charData[charOffset++] = *yytext;
         }

				}
<NATIVE_BLCK>\n	{
						charData[charOffset++] = *yytext;
					#ifndef LEX_DEBUG
					lineno++;
					#else
					;
					#endif
					}
<NATIVE_BLCK>.	charData[charOffset++] = *yytext;

	/* statement end */
;	{
		#ifndef LEX_DEBUG
		return yytext[0];
		#else
		printf("%c\n",yytext[0]);
		#endif
	}

	/* reserved words */
states {
						#ifndef LEX_DEBUG
						return STATES;
						#else
						printf("%s\n",strings[STATES]);
						#endif
					}

events {
						#ifndef LEX_DEBUG
						return EVENTS;
						#else
						printf("%s\n",strings[EVENTS]);
						#endif
					}

return {
						#ifndef LEX_DEBUG
						return RETURN;
						#else
						printf("%s\n",strings[RETURN]);
						#endif
					}

actions {
						#ifndef LEX_DEBUG
						return ACTIONS;
						#else
						printf("%s\n",strings[ACTIONS]);
						#endif
					}

reentrant {
						#ifndef LEX_DEBUG
						return REENTRANT;
						#else
						printf("%s\n",strings[REENTRANT]);
						#endif
					}

machine	{ 
					#ifndef LEX_DEBUG
					return MACHINE_KEY;
					#else
					printf("%s\n",strings[MACHINE_KEY]);
					#endif
				}

state	{ 
					#ifndef LEX_DEBUG
					return STATE_KEY;
					#else
					printf("%s\n",strings[STATE_KEY]);
					#endif
				}

event	{ 
					#ifndef LEX_DEBUG
					return EVENT_KEY;
					#else
					printf("%s\n",strings[EVENT_KEY]);
					#endif
				}

transition	{ 
							#ifndef LEX_DEBUG
							return TRANSITION_KEY;
							#else
							printf("%s\n",strings[TRANSITION_KEY]);
							#endif
						}

action	{ 
					#ifndef LEX_DEBUG
					return ACTION_KEY;
					#else
					printf("%s\n",strings[ACTION_KEY]);
					#endif
				}

returns {
					#ifndef LEX_DEBUG
					return RETURNS;
					#else
					printf("%s\n",strings[RETURNS]);
					#endif
				}

external {
					#ifndef LEX_DEBUG
					return EXTERNAL;
					#else
					printf("%s\n",strings[EXTERNAL]);
					#endif
				}

on {
					#ifndef LEX_DEBUG
					return ON;
					#else
					printf("%s\n",strings[ON]);
					#endif
				}

void {
					#ifndef LEX_DEBUG
					return VOID;
					#else
					printf("%s\n",strings[VOID]);
					#endif
				}

parent {
					#ifndef LEX_DEBUG

         yylval.charData = currDocCmnt ? currDocCmnt : NULL;
         currDocCmnt     = NULL;

					return PARENT;
					#else
					printf("%s\n",strings[PARENT]);
					#endif
				}

inhibits {
					#ifndef LEX_DEBUG
					return INHIBITS;
					#else
					printf("%s\n",strings[INHIBITS]);
					#endif
				}

submachines {
					#ifndef LEX_DEBUG
					return SUBMACHINES;
					#else
					printf("%s\n",strings[SUBMACHINES]);
					#endif
				}

all {
					#ifndef LEX_DEBUG
					return ALL;
					#else
					printf("%s\n",strings[ALL]);
					#endif
				}

entry {
					#ifndef LEX_DEBUG
					return ENTRY;
					#else
					printf("%s\n",strings[ENTRY]);
					#endif
				}

exit {
					#ifndef LEX_DEBUG
					return EXIT;
					#else
					printf("%s\n",strings[EXIT]);
					#endif
				}

:: {
					#ifndef LEX_DEBUG
					return NAMESPACE;
					#else
					printf("%s\n",strings[NAMESPACE]);
					#endif
				}


[](){}=] | 
[[]		{ 
				#ifndef LEX_DEBUG
				return yytext[0];
				#else
				printf("%c\n",yytext[0]);
				#endif
			}

,		{ 
				#ifndef LEX_DEBUG
				return yytext[0]; 
				#else
				printf("%c\n",yytext[0]); 
				#endif
		}

	/* identifiers */
[a-zA-Z_][a-zA-Z0-9_]+	{ 

					int				wordType;
					pID_INFO	pid_info;

         wordType = add_id(id_list,ID,yytext,&pid_info);

					#ifndef LEX_DEBUG

         if (currDocCmnt)
         {
             pid_info->docCmnt = currDocCmnt;
             currDocCmnt       = NULL;
         }

					yylval.pid_info = pid_info;

         #ifdef PARSE_DEBUG
					printf("found: %s : %s\n"
                ,yytext
                ,strings[wordType]
                );
         #endif

					return(wordType);

         #else
					printf("%s : %s\n"
                ,yytext
                ,strings[wordType]
                );
					#endif

		} 

\n	{
			#ifndef LEX_DEBUG
			lineno++;
			#else
			;
			#endif
		}

		/* ignore anything else */
.	;

%%

#if defined (VS) || defined (MINGW) || defined (CYGWIN) || defined (LINUX)
int yywrap(void)
{
	return 1;
}
#endif

#if defined (LEX_DEBUG)
pLIST id_list;
int main (int argc, char **argv)
{
    id_list = init_list();

    yylex();

    return 0;
}
#endif
