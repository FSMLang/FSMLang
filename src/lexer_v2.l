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

typedef struct _bufstack_ BUFSTACK, *pBUFSTACK;
struct _bufstack_
{
	pBUFSTACK       prev;
	YY_BUFFER_STATE bs;
	int             lineno;
	char            *filename;
	FILE            *f;
};

pBUFSTACK curbs   = NULL;
char *curfilename = NULL;

#if defined (LEX_DEBUG) || defined (PARSER_DEBUG)

#define YY_NO_UNPUT

#endif

#ifdef __cplusplus
#error nasty
#endif

#if defined (LEX_DEBUG)

#include "lexer_debug.h"

#else

#define TRANSITION LOOKUP + 1

#endif

%}

%option noyywrap yylineno

%x CMNT
%x DOCCMNT
%x NATIVE
%x NATIVE_BLCK
%x INCLUDE

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
					#ifndef LEX_DEBUG
					lineno++;
					#else
					;
					#endif
			}
<CMNT>"*/"	{ BEGIN INITIAL; }

\/\/.*	;

include[ \t]*\"? { BEGIN INCLUDE; }

<INCLUDE>[^ \t\n\"]+ {
		{
			int c;
			while ((c = input()) && c != '\n');
		}
		#ifdef LEX_DEBUG
		printf("include [%s]\n", yytext);
		#else
		lineno++;
		#endif

		if (!newfile(yytext))
		{
			yyterminate();
		}

		BEGIN INITIAL;
	}

data	{

         #ifdef LEX_DEBUG
         printf("%s\n",strings[DATA_KEY]);
         #else
         return DATA_KEY;
         #endif

		}

	/* Let's add a new rule here. */

translator {
	    
         #ifdef LEX_DEBUG
         printf("%s\n",strings[TRANSLATOR_KEY]);
         #else
         return TRANSLATOR_KEY;
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

<NATIVE>prologue {

       #ifndef LEX_DEBUG
       return PROLOGUE_KEY;
       #else
       printf("%s\n", strings[PROLOGUE_KEY]);
       #endif

    }

<NATIVE>epilogue {

       #ifndef LEX_DEBUG
       return EPILOGUE_KEY;
       #else
       printf("%s\n", strings[EPILOGUE_KEY]);
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
             yylval.charData = currDocCmnt ? currDocCmnt : NULL;
             currDocCmnt     = NULL;
							return TRANSITION_KEY;
							#else
							printf("%s\n",strings[TRANSITION_KEY]);
							#endif
						}

action	{ 
					#ifndef LEX_DEBUG
         yylval.charData = currDocCmnt ? currDocCmnt : NULL;
         currDocCmnt     = NULL;
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

struct {
					#ifndef LEX_DEBUG
					return STRUCT_KEY;
					#else
					printf("%s\n",strings[STRUCT_KEY]);
					#endif
				}

union {
					#ifndef LEX_DEBUG
					return UNION_KEY;
					#else
					printf("%s\n",strings[UNION_KEY]);
					#endif
				}

:: {
					#ifndef LEX_DEBUG
					return NAMESPACE;
					#else
					printf("%s\n",strings[NAMESPACE]);
					#endif
				}


[](){}=*] | 
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
[a-zA-Z_][a-zA-Z0-9_]*	{ 

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

 /* numeric strings */
[0-9]+ {
      #ifndef LEX_DEBUG
      yylval.charData = strdup(yytext);
      return NUMERIC_STRING;
      #else
      printf("NUMERIC_STRING: %s\n"
             , yytext
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

<<EOF>> { if (!popfile()) yyterminate(); }

		/* ignore anything else */
<*>.	;

%%

int newfile(char *filename)
{
	pBUFSTACK bs = (pBUFSTACK) malloc(sizeof(BUFSTACK));

	if (!bs)
	{
		fprintf(stderr
				, "%s: Failed to allocate BUFSTACK\n"
				, me
				);
		return 0;
	}

	if (NULL == (bs->f = fopen(filename, "r")))
	{
		fprintf(stderr
				 , "%s: Failed to open input file %s\n"
				 , me
				 , filename
				);
		return 0;
	}

	/* Remember current state. */
	if (curbs)
	{
		curbs->lineno = yylineno;
	}
	bs -> prev = curbs;

	/* Setup new entry. */
	bs->bs       = yy_create_buffer(bs->f, YY_BUF_SIZE);
	bs->filename = strdup(filename);
	
	yy_switch_to_buffer(bs->bs);

	curbs        = bs;
	yylineno     = 1;
	curfilename  = bs->filename;

	return 1;

}

int popfile(void)
{
	pBUFSTACK bs = curbs;
	pBUFSTACK prevbs;

	if (!bs)
	{
		return 0;
	}

	prevbs = curbs->prev;

	/* Get rid of current entry. */
	fclose(bs->f);
	free(bs->filename);
	yy_delete_buffer(bs->bs);
	free(bs);

	curbs = prevbs;
	if (!prevbs)
	{
		return 0;
	}

	yy_switch_to_buffer(prevbs->bs);
	yylineno    = curbs->lineno;
	curfilename = curbs->filename;

	return 1;
}

#if defined (LEX_DEBUG)

pLIST id_list;
int main (int argc, char **argv)
{
	me = argv[0];

	if (argc != 2)
	{
		fprintf(stderr
				, "%s: exactly one argument needed\n"
				, me
				);
	}

    id_list = init_list();

	if (newfile(argv[1]))
	{
		yylex();
	}

    return 0;
}
#endif
