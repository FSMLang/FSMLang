/**
*  util_file_inclusion.c
*
*    Utilities for tracking included files.
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
*/

#include "util_file_inclusion.h"
#include "fsm_priv.h"

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

extern void yyerror(char*);

#if defined(UNIT_TEST)
#define STATIC
#else
#define STATIC static
#endif

STATIC pLIST panchor = NULL;

void track_included_file(char *filename)
{
	FSMLANG_DEVELOP_PRINTF(stdout, "%s\n", __func__);
	FSMLANG_DEVELOP_PRINTF("%s\n", filename);

	bool good = true;
	char *fn  = NULL;

	do
	{
		if ((fn = strdup(filename)) == NULL)
		{
			good = false;
			break;
		}

		if (NULL == panchor)
		{
			panchor = init_list();
			if (!panchor)
			{
				good = false;
				break;
			}
		}

		if (add_to_list(panchor, fn) == NULL)
		{
			good = false;
			break;
		}

	} while (0);

	if (!good)
	{
		fprintf(stderr
				, "Could not store filename: %s\n"
				, filename
				);
		yyerror("Out of memory");
	}

}

static bool print_file_name(pLIST_ELEMENT pelem, void *data)
{
	FSMLANG_DEVELOP_PRINTF(stdout, "%s\n", __func__);

	(void) data;
	char *fn = (char *) pelem->mbr;

	printf("%s ", fn);

	return false;
}

void print_included_files_list(void)
{
	FSMLANG_DEVELOP_PRINTF(stdout, "%s\n", __func__);

	//It is not an error to have encountered no included files
	if (panchor)
	{
		iterate_list(panchor, print_file_name, NULL);
		free_list(panchor);
		panchor = NULL;
	}
}


