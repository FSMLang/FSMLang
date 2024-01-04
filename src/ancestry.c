/**
*  ancestry.c
*
*    Manages names for variables and types.
*
*    FSMLang (fsm) - A Finite State Machine description language.
*    Copyright (C) 2023  Steven Stanton
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

#include "ancestry.h"

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

char *createAncestryFileName(pMACHINE_INFO pmi)
{
	FILE *tmp = tmpfile();
	char *cp  = NULL;
	unsigned long file_size;

	if (tmp)
	{
		printAncestry(pmi, tmp, "_", alc_lower, ai_include_self);
		file_size = ftell(tmp);
		fseek(tmp, 0, SEEK_SET);

		if ((cp = (char *) malloc(file_size+1)) != NULL)
		{
			fread(cp, 1, file_size, tmp);
			cp[file_size] = 0;
		}

		fclose(tmp);
	}

	return cp;
}

/**
	This function will take a string in hungarian notation
		and change it into "capitalized underbar" notation.
		For example, "newMachine" becomes "NEW_MACHINE", but
		testFSM will become TEST_FSM

	Moreover, escape characters are replaced by underbars.

	Memory is allocated, which must be freed by the calling 
		program.

	returns :
		pointer to the string on success,
		NULL									on failure.

*/
char *hungarianToUnderbarCaps(char *str)
{

	int 	i,consecutive;
	char	*cp, *cp1, *cp2;

	/* first, use i and cp1 to figure out how much memory to get */
	i = strlen(str);
	i++;

	for (cp = str; *cp; cp++)

		if (!(*cp & 0x20))

			i++;

	/* now, cp becomes the pointer to the new memory */
	if ((cp = (char *)malloc(i))) {

		consecutive = 0;
		for (cp1 = str, cp2 = cp; *cp1; cp1++) {

			//deal with the escapes first
			if (*cp1 == '\\') {
				*cp2++ = '_';
				continue;
			}

			if (
          !(*cp1 & 0x20)
          && (*cp1 != '_')
		  && (cp1 != str)
          )
      {

				if (!consecutive) {

					*cp2++ = '_';

					consecutive = 1;

				}

			}
			else
			{
				consecutive =  *cp & 0x20 ? 0 : 1;
			}

			*cp2++ = isalpha(*cp1) ? toupper(*cp1) : *cp1;

		}

		*cp2 = 0;

	}

	return cp;

}

/**
 *  streamHungarianToUnderbarCaps
 *
 *  Behaves similarly to hungarianToUnderbarCaps, but writes
 *  the ouput to the given stream.
*/
void streamHungarianToUnderbarCaps(FILE *fout, char *str)
{

	int 	consecutive;
	char	*cp;

	consecutive = 0;
	for (cp = str; *cp; cp++) {

		//deal with the escapes first
		if (*cp == '\\')
		{
			fputc('_',fout);
			continue;
		}

		if (
          !(*cp & 0x20)
          && (*cp != '_')
		  && (cp != str)
          )
        {

			if (!consecutive)
			{

				fputc('_',fout);
				consecutive = 1;

			}

		}
		else
		{
			consecutive =  *cp & 0x20 ? 0 : 1;
		}

		fputc((isalpha(*cp) ? toupper(*cp) : *cp), fout);

	}

}

char* actionReturnType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long file_size;

	/* only create the string once */
	if (!pcmd->action_return_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			if (pcmd->pmi->modFlags & mfActionsReturnVoid)
			{
				fprintf(tmp, "void");
			}
			else
			{
				printAncestry(pcmd->pmi, tmp, "_", alc_upper, ai_include_self | ai_stop_at_parent);
				if (pcmd->pmi->modFlags & mfActionsReturnStates)
				{
					fprintf(tmp, "_STATE");
				}
				else
				{
					fprintf(tmp
							,"_EVENT%s"
							, pcmd->pmi->data_block_count ? "_ENUM" : ""
							);
				}
			}
			file_size = ftell(tmp);
			fseek(tmp, 0, SEEK_SET);

			if ((pcmd->action_return_type = (char *)malloc(file_size + 1)) != NULL)
			{
				fread(pcmd->action_return_type, 1, file_size, tmp);
				pcmd->action_return_type[file_size] = 0;
			}

			fclose(tmp);
		}
	}

	return pcmd->action_return_type;
}

char* fsmType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long file_size;

	/* only create the string once */
	if (!pcmd->fsm_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			file_size = ftell(tmp);
			fseek(tmp, 0, SEEK_SET);

			if ((pcmd->fsm_type = (char *)malloc(file_size + 1)) != NULL)
			{
				fread(pcmd->fsm_type, 1, file_size, tmp);
				pcmd->fsm_type[file_size] = 0;
			}

			fclose(tmp);
		}
	}

	return pcmd->fsm_type;
}

char* actionFnType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long file_size;

	/* only create the string once */
	if (!pcmd->action_fn_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp, "_ACTION_FN");
			file_size = ftell(tmp);
			fseek(tmp, 0, SEEK_SET);

			if ((pcmd->action_fn_type = (char *)malloc(file_size + 1)) != NULL)
			{
				fread(pcmd->action_fn_type, 1, file_size, tmp);
				pcmd->action_fn_type[file_size] = 0;
			}

			if (pcmd->c_machine_struct_format_width < file_size + 2)
			{
				pcmd->c_machine_struct_format_width = file_size + 2;
			}

			fclose(tmp);
		}
	}

	return pcmd->action_fn_type;
}

char* actionTransType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long file_size;

	/* only create the string once */
	if (!pcmd->action_trans_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp, "_ACTION_TRANS");
			file_size = ftell(tmp);
			fseek(tmp, 0, SEEK_SET);

			if ((pcmd->action_trans_type = (char *)malloc(file_size + 1)) != NULL)
			{
				fread(pcmd->action_trans_type, 1, file_size, tmp);
				pcmd->action_trans_type[file_size] = 0;
			}

			if (pcmd->c_machine_struct_format_width < file_size + 2)
			{
				pcmd->c_machine_struct_format_width = file_size + 2;
			}

			fclose(tmp);
		}
	}

	return pcmd->action_trans_type;
}

char* transitionFnType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long file_size;

	/* only create the string once */
	if (!pcmd->transition_fn_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp, "_TRANSITION_FN");
			file_size = ftell(tmp);
			fseek(tmp, 0, SEEK_SET);

			if ((pcmd->transition_fn_type = (char *)malloc(file_size + 1)) != NULL)
			{
				fread(pcmd->transition_fn_type, 1, file_size, tmp);
				pcmd->transition_fn_type[file_size] = 0;
			}

			if (pcmd->c_machine_struct_format_width < file_size + 2)
			{
				pcmd->c_machine_struct_format_width = file_size + 2;
			}

			fclose(tmp);
		}
	}

	return pcmd->transition_fn_type;
}

char* fsmDataType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long file_size;

	/* only create the string once */
	if (!pcmd->fsm_data_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp,"_DATA");
			file_size = ftell(tmp);
			fseek(tmp, 0, SEEK_SET);

			if ((pcmd->fsm_data_type = (char *)malloc(file_size + 1)) != NULL)
			{
				fread(pcmd->fsm_data_type, 1, file_size, tmp);
				pcmd->fsm_data_type[file_size] = 0;
			}

			if (pcmd->sub_machine_struct_format_width < file_size + 2)
			{
				pcmd->sub_machine_struct_format_width = file_size + 2;
			}

			if (pcmd->c_machine_struct_format_width < file_size + 2)
			{
				pcmd->c_machine_struct_format_width = file_size + 2;
			}

			fclose(tmp);
		}
	}

	return pcmd->fsm_data_type;
}

char* fsmFnType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long file_size;

	/* only create the string once */
	if (!pcmd->fsm_fn_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp,"_FSM");
			file_size = ftell(tmp);
			fseek(tmp, 0, SEEK_SET);

			if ((pcmd->fsm_fn_type = (char *)malloc(file_size + 1)) != NULL)
			{
				fread(pcmd->fsm_fn_type, 1, file_size, tmp);
				pcmd->fsm_fn_type[file_size] = 0;
			}

			if (pcmd->sub_machine_struct_format_width < file_size + 2)
			{
				pcmd->sub_machine_struct_format_width = file_size + 2;
			}

			if (pcmd->c_machine_struct_format_width < file_size + 2)
			{
				pcmd->c_machine_struct_format_width = file_size + 2;
			}

			fclose(tmp);
		}
	}

	return pcmd->fsm_fn_type;
}

char* eventType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long file_size;

	/* only create the string once */
	if (!pcmd->event_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp
					,"_EVENT%s"
					, pcmd->pmi->data_block_count ? "_ENUM" : ""
					);
			file_size = ftell(tmp);
			fseek(tmp, 0, SEEK_SET);

			if ((pcmd->event_type = (char *)malloc(file_size + 1)) != NULL)
			{
				fread(pcmd->event_type, 1, file_size, tmp);
				pcmd->event_type[file_size] = 0;
			}

			if (pcmd->sub_fsm_if_format_width < file_size + 2)
			{
				pcmd->sub_fsm_if_format_width = file_size + 2;
			}

			if (pcmd->shared_event_str_format_width < file_size + 2)
			{
				pcmd->shared_event_str_format_width = file_size + 2;
			}

			if (pcmd->sub_machine_struct_format_width < file_size + 2)
			{
				pcmd->sub_machine_struct_format_width = file_size + 2;
			}

			if (pcmd->c_machine_struct_format_width < file_size + 2)
			{
				pcmd->c_machine_struct_format_width = file_size + 2;
			}

			fclose(tmp);
		}
	}

	return pcmd->event_type;
}

char* fsmFnEventType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long file_size;

	/* only create the string once */
	if (!pcmd->fsm_fn_event_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			if (pcmd->pmi->data_block_count)
			{
				fputc('p', tmp);
			}
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp
					,"_EVENT"
					);
			file_size = ftell(tmp);
			fseek(tmp, 0, SEEK_SET);

			if ((pcmd->fsm_fn_event_type = (char *)malloc(file_size + 1)) != NULL)
			{
				fread(pcmd->fsm_fn_event_type, 1, file_size, tmp);
				pcmd->fsm_fn_event_type[file_size] = 0;
			}

			if (pcmd->sub_fsm_if_format_width < file_size + 2)
			{
				pcmd->sub_fsm_if_format_width = file_size + 2;
			}

			fclose(tmp);
		}
	}

	return pcmd->fsm_fn_event_type;
}

char* subFsmIfType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long file_size;

	/* only create the string once */
	if (!pcmd->sub_fsm_if_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp ,"_SUB_FSM_IF");
			file_size = ftell(tmp);
			fseek(tmp, 0, SEEK_SET);

			if ((pcmd->sub_fsm_if_type = (char *)malloc(file_size + 1)) != NULL)
			{
				fread(pcmd->sub_fsm_if_type, 1, file_size, tmp);
				pcmd->sub_fsm_if_type[file_size] = 0;
			}

			if (pcmd->sub_machine_struct_format_width < file_size + 2)
			{
				pcmd->sub_machine_struct_format_width = file_size + 2;
			}

			if (pcmd->c_machine_struct_format_width < file_size + 2)
			{
				pcmd->c_machine_struct_format_width = file_size + 2;
			}

			fclose(tmp);
		}
	}

	return pcmd->sub_fsm_if_type;
}

char* stateType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long file_size;

	/* only create the string once */
	if (!pcmd->state_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp ,"_STATE");
			file_size = ftell(tmp);
			fseek(tmp, 0, SEEK_SET);

			if ((pcmd->state_type = (char *)malloc(file_size + 1)) != NULL)
			{
				fread(pcmd->state_type, 1, file_size, tmp);
				pcmd->state_type[file_size] = 0;
			}

			if (pcmd->sub_machine_struct_format_width < file_size + 2)
			{
				pcmd->sub_machine_struct_format_width = file_size + 2;
			}

			if (pcmd->c_machine_struct_format_width < file_size + 2)
			{
				pcmd->c_machine_struct_format_width = file_size + 2;
			}

			fclose(tmp);
		}
	}

	return pcmd->state_type;
}

char* stateFnType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long file_size;

	/* only create the string once */
	if (!pcmd->state_fn_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp ,"_STATE_FN");
			file_size = ftell(tmp);
			fseek(tmp, 0, SEEK_SET);

			if ((pcmd->state_fn_type = (char *)malloc(file_size + 1)) != NULL)
			{
				fread(pcmd->state_fn_type, 1, file_size, tmp);
				pcmd->state_fn_type[file_size] = 0;
			}

			if (pcmd->sub_machine_struct_format_width < file_size + 2)
			{
				pcmd->sub_machine_struct_format_width = file_size + 2;
			}

			fclose(tmp);
		}
	}

	return pcmd->state_fn_type;
}

char* subMachineFnType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long file_size;

	/* only create the string once */
	if (!pcmd->sub_machine_fn_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp ,"_SUB_MACHINE_FN");
			file_size = ftell(tmp);
			fseek(tmp, 0, SEEK_SET);

			if ((pcmd->sub_machine_fn_type = (char *)malloc(file_size + 1)) != NULL)
			{
				fread(pcmd->sub_machine_fn_type, 1, file_size, tmp);
				pcmd->sub_machine_fn_type[file_size] = 0;
			}

			fclose(tmp);

			if (pcmd->sub_fsm_if_format_width < file_size + 2)
			{
				pcmd->sub_fsm_if_format_width = file_size + 2;
			}

		}
	}

	return pcmd->sub_machine_fn_type;
}

char* dataTranslationFnType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long file_size;

	/* only create the string once */
	if (!pcmd->data_translation_fn_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp ,"_DATA_TRANSLATION_FN");
			file_size = ftell(tmp);
			fseek(tmp, 0, SEEK_SET);

			if ((pcmd->data_translation_fn_type = (char *)malloc(file_size + 1)) != NULL)
			{
				fread(pcmd->data_translation_fn_type, 1, file_size, tmp);
				pcmd->data_translation_fn_type[file_size] = 0;
			}

			fclose(tmp);

			if (pcmd->shared_event_str_format_width < file_size + 2)
			{
				pcmd->shared_event_str_format_width = file_size + 2;
			}
		}
	}

	return pcmd->data_translation_fn_type;
}

char* sharedEventStrType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long file_size;

	/* only create the string once */
	if (!pcmd->shared_event_str_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp, "_SHARED_EVENT_STR");
			file_size = ftell(tmp);
			fseek(tmp, 0, SEEK_SET);

			if ((pcmd->shared_event_str_type = (char *)malloc(file_size + 1)) != NULL)
			{
				fread(pcmd->shared_event_str_type, 1, file_size, tmp);
				pcmd->shared_event_str_type[file_size] = 0;
			}

			fclose(tmp);

			if (pcmd->shared_event_str_format_width < file_size + 2)
			{
				pcmd->shared_event_str_format_width = file_size + 2;
			}
		}
	}

	return pcmd->shared_event_str_type;
}

char *machineName(pCMachineData pcmd)
{
	return pcmd->pmi->name->name;
}

/**
 * Returns the "fully qualified" machine name. "Fully qualified"
 * means that the full ancestry is prepended to the name.
 * 
 * @author Steven Stanton (12/14/2023)
 * 
 * @param pcmd   
 * 
 * @return char* 
 */
char *fqMachineName(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long file_size;

	if (!pcmd->fq_machine_name)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			printAncestry(pcmd->pmi, tmp, "_", alc_lower, ai_include_self);
			file_size = ftell(tmp);
			fseek(tmp, 0, SEEK_SET);

			if ((pcmd->fq_machine_name = (char *)malloc(file_size + 1)) != NULL)
			{
				fread(pcmd->fq_machine_name, 1, file_size, tmp);
				pcmd->fq_machine_name[file_size] = 0;
			}

			fclose(tmp);

		}
	}

	return pcmd->fq_machine_name;

}

/**
 * Returns a string composed of the current machine name and the
 * immediate parent.  If no immediate parent, then only machine
 * name is returned.
 * 
 * @author Steven Stanton (12/18/2023)
 * 
 * @param pcmd   The machine data
 * 
 * @return char* Ancestry string, "_" separated.
 */
char *nfMachineName(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long file_size;

	if (!pcmd->nf_machine_name)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			printAncestry(pcmd->pmi, tmp, "_", alc_lower, ai_include_self | ai_stop_at_parent);
			file_size = ftell(tmp);
			fseek(tmp, 0, SEEK_SET);

			if ((pcmd->nf_machine_name = (char *)malloc(file_size + 1)) != NULL)
			{
				fread(pcmd->nf_machine_name, 1, file_size, tmp);
				pcmd->nf_machine_name[file_size] = 0;
			}

			fclose(tmp);

		}
	}

	return pcmd->nf_machine_name;

}


/**
 * Returns a string composed of the machine name prepended with
 * the parent name, separated by an underscore.
 *
 * Memory will be allocated for the string; this must be freed
 * by the caller.
 * 
 * @author Steven Stanton (12/18/2023)
 * 
 * @param pmi    The machine to name
 * @param pcp    pointer to a character pointer; this will point
 *  			 to the allocated memory
 * 
 * @return char* *pcp.
 */
char *nfMachineNamePmi(pMACHINE_INFO pmi, char **pcp)
{
	FILE          *tmp;
	unsigned long file_size;

	*pcp = NULL;

	/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
	if (NULL != (tmp = tmpfile()))
	{
		printAncestry(pmi, tmp, "_", alc_lower, ai_include_self | ai_stop_at_parent);
		file_size = ftell(tmp);
		fseek(tmp, 0, SEEK_SET);

		if ((*pcp = (char *)malloc(file_size + 1)) != NULL)
		{
			fread(*pcp, 1, file_size, tmp);
			(*pcp)[file_size] = 0;
		}

		fclose(tmp);

	}

	return *pcp;

}


