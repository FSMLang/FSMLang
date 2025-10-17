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

#include <stdio.h>
#if defined (CYGWIN) || defined (LINUX)
	#include <ctype.h>
	#include <unistd.h>
#endif
#include <time.h>
#include <string.h>
#include <stdlib.h>

char *createAncestryFileName(pMACHINE_INFO pmi)
{
	FILE *tmp = tmpfile();
	char *cp  = NULL;

	if (tmp)
	{
		printAncestry(pmi, tmp, "_", alc_lower, ai_include_self);
		cp = create_string_from_file(tmp, NULL);
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
		pointer to the string on success, NULL on failure.

*/
char *hungarianToUnderbarCaps(char *str)
{

	int 	i = strlen(str) + 1;
	int     consecutive = 0;

	char	*cp, *cp1, *cp2;

	/* first, use i and cp1 to figure out how much memory to get */
	i = strlen(str) + 1;

	for (cp = str; *cp; cp++) if (!(*cp & 0x20)) i++;

	/* now, cp becomes the pointer to the new memory */
	if ((cp = (char *)malloc(i)))
	{

		consecutive = 0;
		for (cp1 = str, cp2 = cp; *cp1; cp1++)
		{

			//deal with the escapes first
			if (*cp1 == '\\')
			{
				*cp2++ = '_';
				continue;
			}

			if (
				!(*cp1 & 0x20)
				&& (*cp1 != '_')
				&& (cp1 != str)
			   )
			{

				if (!consecutive)
				{
					*cp2++ = '_';
					consecutive = 1;
				}

			}
			else
			{
				consecutive =  *cp1 & 0x20 ? 0 : 1;
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

	int 	consecutive = 0;
	char	*cp;

	for (cp = str; *cp; cp++)
	{

		//deal with the escapes first
		if (*cp == '\\')
		{
			fputc('_', fout);
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
				fputc('_', fout);
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
				if (pcmd->pmi->modFlags & mfActionsReturnStates)
				{
					streamStrCaseAware(tmp, pcmd->pmi->name->name, alc_upper);
					fprintf(tmp, "_STATE");
				}
				else
				{
					printAncestry(pcmd->pmi
								  , tmp
								  , "_"
								  , alc_upper
								  , ai_include_self | ai_stop_at_parent
								  );
					fprintf(tmp
							,"_EVENT%s"
							, pcmd->pmi->data_block_count ? "_ENUM" : ""
							);
				}
			}

			pcmd->action_return_type = create_string_from_file(tmp, NULL);
		}
	}

	return pcmd->action_return_type;
}

char* fsmType(pCMachineData pcmd)
{
	FILE          *tmp;

	/* only create the string once */
	if (!pcmd->fsm_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			pcmd->fsm_type = create_string_from_file(tmp, NULL);
		}
	}

	return pcmd->fsm_type;
}

char* actionFnType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long str_len;

	/* only create the string once */
	if (!pcmd->action_fn_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp, "_ACTION_FN");

			pcmd->action_fn_type = create_string_from_file(tmp, &str_len);

			if (pcmd->c_machine_struct_format_width < str_len + 2)
			{
				pcmd->c_machine_struct_format_width = str_len + 2;
			}

		}
	}

	return pcmd->action_fn_type;
}

char* actionTransType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long str_len;

	/* only create the string once */
	if (!pcmd->action_trans_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp, "_ACTION_TRANS");

			pcmd->action_trans_type = create_string_from_file(tmp, &str_len);

			if (pcmd->c_machine_struct_format_width < str_len + 2)
			{
				pcmd->c_machine_struct_format_width = str_len + 2;
			}

		}
	}

	return pcmd->action_trans_type;
}

char* transitionFnType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long str_len;

	/* only create the string once */
	if (!pcmd->transition_fn_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp, "_TRANSITION_FN");

			pcmd->transition_fn_type = create_string_from_file(tmp, &str_len);

			if (pcmd->c_machine_struct_format_width < str_len + 2)
			{
				pcmd->c_machine_struct_format_width = str_len + 2;
			}

		}
	}

	return pcmd->transition_fn_type;
}

char* fsmDataType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long str_len;

	/* only create the string once */
	if (!pcmd->fsm_data_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp,"_DATA");

			pcmd->fsm_data_type = create_string_from_file(tmp, &str_len);

			if (pcmd->sub_machine_struct_format_width < str_len + 2)
			{
				pcmd->sub_machine_struct_format_width = str_len + 2;
			}

			if (pcmd->c_machine_struct_format_width < str_len + 2)
			{
				pcmd->c_machine_struct_format_width = str_len + 2;
			}

		}
	}

	return pcmd->fsm_data_type;
}

char* fsmFnType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long str_len;

	/* only create the string once */
	if (!pcmd->fsm_fn_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp,"_FSM");

			pcmd->fsm_fn_type = create_string_from_file(tmp, &str_len);

			if (pcmd->sub_machine_struct_format_width < str_len + 2)
			{
				pcmd->sub_machine_struct_format_width = str_len + 2;
			}

			if (pcmd->c_machine_struct_format_width < str_len + 2)
			{
				pcmd->c_machine_struct_format_width = str_len + 2;
			}

		}
	}

	return pcmd->fsm_fn_type;
}

char* eventType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long str_len;

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

			pcmd->event_type = create_string_from_file(tmp, &str_len);

			if (pcmd->sub_fsm_if_format_width < str_len + 2)
			{
				pcmd->sub_fsm_if_format_width = str_len + 2;
			}

			if (pcmd->shared_event_str_format_width < str_len + 2)
			{
				pcmd->shared_event_str_format_width = str_len + 2;
			}

			if (pcmd->sub_machine_struct_format_width < str_len + 2)
			{
				pcmd->sub_machine_struct_format_width = str_len + 2;
			}

			if (pcmd->c_machine_struct_format_width < str_len + 2)
			{
				pcmd->c_machine_struct_format_width = str_len + 2;
			}

		}
	}

	return pcmd->event_type;
}

char* fsmFnEventType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long str_len;

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

			pcmd->fsm_fn_event_type = create_string_from_file(tmp, &str_len);

			if (pcmd->sub_fsm_if_format_width < str_len + 2)
			{
				pcmd->sub_fsm_if_format_width = str_len + 2;
			}
		}
	}

	return pcmd->fsm_fn_event_type;
}

char* subFsmFnEventType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long str_len;

	/* only create the string once */
	if (!pcmd->sub_fsm_fn_event_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp
					,"_EVENT%s"
					, pcmd->pmi->data_block_count ? "_ENUM" : ""
					);

			pcmd->sub_fsm_fn_event_type = create_string_from_file(tmp, &str_len);

		}
	}

	return pcmd->sub_fsm_fn_event_type;
}

char* subFsmFnReturnType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long str_len;

	/* only create the string once */
	if (!pcmd->sub_fsm_fn_return_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			if (pcmd->pmi->modFlags & ACTIONS_RETURN_FLAGS)
			{
				fprintf(tmp, "void");
			}
			else
			{
				streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
				fprintf(tmp
						,"_EVENT%s"
						, pcmd->pmi->data_block_count ? "_ENUM" : ""
						);
			}

			pcmd->sub_fsm_fn_return_type = create_string_from_file(tmp, &str_len);

		}
	}

	return pcmd->sub_fsm_fn_return_type;
}

char* subFsmIfType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long str_len;

	/* only create the string once */
	if (!pcmd->sub_fsm_if_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp ,"_SUB_FSM_IF");

			pcmd->sub_fsm_if_type = create_string_from_file(tmp, &str_len);

			if (pcmd->sub_machine_struct_format_width < str_len + 2)
			{
				pcmd->sub_machine_struct_format_width = str_len + 2;
			}

			if (pcmd->c_machine_struct_format_width < str_len + 2)
			{
				pcmd->c_machine_struct_format_width = str_len + 2;
			}

		}
	}

	return pcmd->sub_fsm_if_type;
}

char* stateType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long str_len;

	/* only create the string once */
	if (!pcmd->state_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp ,"_STATE");

			pcmd->state_type = create_string_from_file(tmp, &str_len);

			if (pcmd->sub_machine_struct_format_width < str_len + 2)
			{
				pcmd->sub_machine_struct_format_width = str_len + 2;
			}

			if (pcmd->c_machine_struct_format_width < str_len + 2)
			{
				pcmd->c_machine_struct_format_width = str_len + 2;
			}

		}
	}

	return pcmd->state_type;
}

char* stateFnType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long str_len;

	/* only create the string once */
	if (!pcmd->state_fn_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp ,"_STATE_FN");

			pcmd->state_fn_type = create_string_from_file(tmp, &str_len);

			if (pcmd->sub_machine_struct_format_width < str_len + 2)
			{
				pcmd->sub_machine_struct_format_width = str_len + 2;
			}

		}
	}

	return pcmd->state_fn_type;
}

char* subMachineFnType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long str_len;

	/* only create the string once */
	if (!pcmd->sub_machine_fn_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp ,"_SUB_MACHINE_FN");

			pcmd->sub_machine_fn_type = create_string_from_file(tmp, &str_len);

			if (pcmd->sub_fsm_if_format_width < str_len + 2)
			{
				pcmd->sub_fsm_if_format_width = str_len + 2;
			}

		}
	}

	return pcmd->sub_machine_fn_type;
}

char* subMachineEnumType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long str_len;

	/* only create the string once */
	if (!pcmd->sub_machine_enum_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp ,"_SUB_MACHINES");

			pcmd->sub_machine_enum_type = create_string_from_file(tmp, &str_len);

			if (pcmd->sub_fsm_if_format_width < str_len + 2)
			{
				pcmd->sub_fsm_if_format_width = str_len + 2;
			}

		}
	}

	return pcmd->sub_machine_enum_type;
}

char* dataTranslationFnType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long str_len;

	/* only create the string once */
	if (!pcmd->data_translation_fn_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp ,"_DATA_TRANSLATION_FN");

			pcmd->data_translation_fn_type = create_string_from_file(tmp, &str_len);

			if (pcmd->shared_event_str_format_width < str_len + 2)
			{
				pcmd->shared_event_str_format_width = str_len + 2;
			}
		}
	}

	return pcmd->data_translation_fn_type;
}

char* sharedEventStrType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long str_len;

	/* only create the string once */
	if (!pcmd->shared_event_str_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp, "_SHARED_EVENT_STR");

			pcmd->shared_event_str_type = create_string_from_file(tmp, &str_len);
			
			if (pcmd->shared_event_str_format_width < str_len + 2)
			{
				pcmd->shared_event_str_format_width = str_len + 2;
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

	if (!pcmd->fq_machine_name)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			printAncestry(pcmd->pmi, tmp, "_", alc_lower, ai_include_self);

			pcmd->fq_machine_name = create_string_from_file(tmp, NULL);

		}
	}

	return pcmd->fq_machine_name;

}

/**
 * This function returns a name suitable for use with user
 * defined functions.  If the command line argument
 * --short-user-fn-names was set <i>true</i>, then this is
 *   simply the machine name; otherwise, it is the
 *   fully-qualified machine name.
 * 
 * @author Steven Stanton (1/26/2024)
 * 
 * @param pcmd  
 * 
 * @return char* 
 */
char *ufMachineName(pCMachineData pcmd)
{
	FILE          *tmp;

	if (!pcmd->uf_machine_name)
	{
		if ((short_user_fn_names == true) && (maxDepth(pcmd->pmi) == 1))
		{
			pcmd->uf_machine_name = strdup(pcmd->pmi->name->name);
		}
		else
		{
			/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
			if (NULL != (tmp = tmpfile()))
			{
				printAncestry(pcmd->pmi, tmp, "_", alc_lower, ai_include_self);

				pcmd->uf_machine_name = create_string_from_file(tmp, NULL);

			}
		}
	}

	return pcmd->uf_machine_name;

}

/**
 * Returns the "fully qualified" machine name transformed by
 * <i>hungarianToUnderbarCaps</i>.  "Fully qualified" means that
 * the full ancestry is prepended to the name.
 * 
 * @author Steven Stanton (1/6/2024)
 * 
 * @param pcmd   
 * 
 * @return char* 
 */
char *ucfqMachineName(pCMachineData pcmd)
{
	FILE          *tmp;

	if (!pcmd->ucfq_machine_name)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			printAncestry(pcmd->pmi, tmp, "_", alc_upper, ai_include_self);

			pcmd->ucfq_machine_name = create_string_from_file(tmp, NULL);

		}
	}

	return pcmd->ucfq_machine_name;

}

/**
 * Return the hungarianToUnderbarCaps version of the local
 * machine name.
 * 
 * @author Steven Stanton (1/6/2024)
 * 
 * @param pcmd   The writer data from which to take the machine
 *  			 name.
 * 
 * @return char* The machine name, transformed by the function,
 *  	   <i>hungarianToUnderbarCaps</i>.
 */
char *ucMachineName(pCMachineData pcmd)
{
	FSMLANG_DEVELOP_PRINTF(pcmd->hFile , "/* %s */\n", __func__ );

	if (!pcmd->uc_machine_name)
	{
		pcmd->uc_machine_name = hungarianToUnderbarCaps(pcmd->pmi->name->name);
	}

	return pcmd->uc_machine_name;

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

	if (!pcmd->nf_machine_name)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			printAncestry(pcmd->pmi, tmp, "_", alc_lower, ai_include_self | ai_stop_at_parent);

			pcmd->nf_machine_name = create_string_from_file(tmp, NULL);

		}
	}

	return pcmd->nf_machine_name;

}

char *ucnfMachineName(pCMachineData pcmd)
{
	FILE          *tmp;

	if (!pcmd->uc_nf_machine_name)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			printAncestry(pcmd->pmi, tmp, "_", alc_upper, ai_include_self | ai_stop_at_parent);

			pcmd->uc_nf_machine_name = create_string_from_file(tmp, NULL);

		}
	}

	return pcmd->uc_nf_machine_name;

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

	*pcp = NULL;

	/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
	if (NULL != (tmp = tmpfile()))
	{
		printAncestry(pmi, tmp, "_", alc_lower, ai_include_self | ai_stop_at_parent);

		*pcp = create_string_from_file(tmp, NULL);

	}

	return *pcp;

}

/**
 * Returns a string composed of the fully-qualified machine
 * name.
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
char *fqMachineNamePmi(pMACHINE_INFO pmi, char **pcp)
{
	FILE          *tmp;

	*pcp = NULL;

	/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
	if (NULL != (tmp = tmpfile()))
	{
		printAncestry(pmi, tmp, "_", alc_lower, ai_include_self);

		*pcp = create_string_from_file(tmp, NULL);

	}

	return *pcp;

}

/**
 * Returns an upper-case string composed of the
 * fully-qualified machine name.
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
char *ucfqMachineNamePmi(pMACHINE_INFO pmi, char **pcp)
{
	FILE          *tmp;

	*pcp = NULL;

	/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
	if (NULL != (tmp = tmpfile()))
	{
		printAncestry(pmi, tmp, "_", alc_upper, ai_include_self);

		*pcp = create_string_from_file(tmp, NULL);

	}

	return *pcp;

}


char * instanceType(pCMachineData pcmd)
{
	FILE          *tmp;
	unsigned long str_len;

	/* only create the string once */
	if (!pcmd->instance_type)
	{
		/* use a temporary file to exploit streaming function, avoiding messy strlen calc */
		if (NULL != (tmp = tmpfile()))
		{
			streamHungarianToUnderbarCaps(tmp, pcmd->pmi->name->name);
			fprintf(tmp
					,"_INSTANCE"
					);

			pcmd->instance_type = create_string_from_file(tmp, &str_len);

		}
	}

	return pcmd->instance_type;
}

