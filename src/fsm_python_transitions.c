/**
*  fsm_python_transitions.c
*
*    Creates an FSM based on the pytransitions model.
*
*    FSMLang (fsm) - A Finite State Machine description language.
*    Copyright (C) 2026  Steven Stanton
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
*    fsmlang@pesticidesoftware.com
*
*    For the latest on FSMLang: https://fsmlang.github.io
*
*    And, finally, your possession of this source code implies nothing.
*
*    File created by Steven Stanton
*
*  Long Description:
*
*  See: https://github.com/pytransitions/transitions.
*
*  Only flat machines are supported.
*
*/

#include "fsm_python_transitions.h"
#include "util_file_inclusion.h"


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

static int initPyTransitionsWriter(pFSMOutputGenerator,char *);
static void writePyTransitionsWriter(pFSMOutputGenerator,pMACHINE_INFO);
static void closePyTransitionsWriter(pFSMOutputGenerator,int);

static int initPyTransitionsFN(pFSMOutputGenerator,char *);
static void writePyTransitionsFN(pFSMOutputGenerator,pMACHINE_INFO);
static void closePyTransitionsFN(pFSMOutputGenerator,int);

typedef struct _fsm_pytransitions_output_generator_ FSMPyTransitionsOutputGenerator, *pFSMPyTransitionsOutputGenerator;
typedef struct _pytransitions_machine_data_ PyTransitionsData, *pPyTransitionsData;

struct _pytransitions_machine_data_ {
  FILE	        *file;
  char	        *fileName;
  char          *baseName;
  pMACHINE_INFO pmi;
};

struct _fsm_pytransitions_output_generator_
{
   FSMOutputGenerator        fsmog;
   pPyTransitionsData ptd;
};

static FSMPyTransitionsOutputGenerator PyTransitionsMachineWriter = {
 {
    initPyTransitionsWriter
    , writePyTransitionsWriter
    , closePyTransitionsWriter
    , generatePyTransitionsWriter
  }
  , NULL
};

pFSMOutputGenerator generatePyTransitionsWriter(pFSMOutputGenerator parent)
{
	pFSMOutputGenerator pfsmog;

	if (parent)
	{
		yyerror("PyTransitions supports only flat machines.");
	}
	else
	{
		pfsmog =  (pFSMOutputGenerator)&PyTransitionsMachineWriter;
	}

	if (output_generated_file_names_only)
	{
		pfsmog->initOutput   = initPyTransitionsFN;
		pfsmog->writeMachine = writePyTransitionsFN;
		pfsmog->closeOutput  = closePyTransitionsFN;
	}

	return pfsmog;
}

static int initPyTransitionsFN(pFSMOutputGenerator pfsmog, char *baseFileName)
{
  pFSMPyTransitionsOutputGenerator pfsmpytog
    = (pFSMPyTransitionsOutputGenerator) pfsmog;

  if (NULL != (pfsmpytog->ptd = calloc(1
                           , sizeof(PyTransitionsData)
                           )
         )
    )
  {
    if (baseFileName)
    {
      pfsmpytog->ptd->fileName = createFileName(baseFileName,".py");
      pfsmpytog->ptd->baseName = strdup(baseFileName);
    }
  }

  /* this may look funny, but it does the trick */
  return ((int) !pfsmpytog->ptd->fileName);
}

static void writePyTransitionsFN(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{ 
  pFSMPyTransitionsOutputGenerator pfsmpytog
	  = (pFSMPyTransitionsOutputGenerator)pfsmog;

  if (output_make_recipe)
  {
    printf("%s: %s.fsm "
		   , pfsmpytog->ptd->fileName
		   , pfsmpytog->ptd->baseName
         );
    
    print_included_files_list();

    printf("\n");
  }
  else
  {
    printf("%s ", pfsmpytog->ptd->fileName);
  }

  (void) pmi;
}

static void closePyTransitionsFN(pFSMOutputGenerator pfsmog, int good)
{
	pFSMPyTransitionsOutputGenerator pfsmpytog
		= (pFSMPyTransitionsOutputGenerator)pfsmog;

	if (!pfsmpytog || !pfsmpytog->ptd)
	{
		return;
	}

	CHECK_AND_FREE(pfsmpytog->ptd->fileName);

	free(pfsmpytog->ptd);

	(void) good;
}

static int initPyTransitionsWriter(pFSMOutputGenerator pfsmog, char *baseFileName)
{
	pFSMPyTransitionsOutputGenerator pfsmpytog
		= (pFSMPyTransitionsOutputGenerator) pfsmog;

	if (NULL != (pfsmpytog->ptd = calloc(1, sizeof(pPyTransitionsData))))
	{
		if (!baseFileName)
		{
			pfsmpytog->ptd->file = stdout;
		}
		else
		{
			pfsmpytog->ptd->fileName = createFileName(baseFileName, ".py");
			pfsmpytog->ptd->baseName = strdup(baseFileName);

			if (NULL == (pfsmpytog->ptd->file = openFile(pfsmpytog->ptd->fileName, "w")))
			{
				CHECK_AND_FREE(pfsmpytog->ptd->fileName);
				CHECK_AND_FREE(pfsmpytog->ptd->baseName);
			}
			else
			{
				fprintf(pfsmpytog->ptd->file
						, "\"\"\"\n\t%s\n\n"
						, pfsmpytog->ptd->fileName
						);
				fprintf(pfsmpytog->ptd->file
						, "\tThis file generated by FSMLang\n\n\"\"\""
						);
			}
		}
	}

	/* this may look funny, but it does the trick */
	return ((int) !pfsmpytog->ptd->fileName);
}

static void writePyTransitionsWriter(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
	(void) pfsmog;
	(void) pmi;
}

static void closePyTransitionsWriter(pFSMOutputGenerator pfsmog, int good)
{
	pFSMPyTransitionsOutputGenerator pfsmpytog
		= (pFSMPyTransitionsOutputGenerator) pfsmog;

	fclose(pfsmpytog->ptd->file);

	if (!good)
	{
		unlink(pfsmpytog->ptd->fileName);
	}

}

