/**
*  fsm_kotlin.c
*
*    Creates an FSM implementation in Kotlin (single-switch style).
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
*  Generates a single-switch-style Kotlin FSM class.
*  Only flat (single-level) machines are supported.
*
*/

#include "fsm_kotlin.h"
#include "util_file_inclusion.h"
#include "action_info_list.h"
#include "y.tab.h"

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
#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  Module-level globals                                                */
/* ------------------------------------------------------------------ */

/* NULL = use default package; non-NULL = use this package string verbatim */
char *kotlin_package = NULL;

/* ------------------------------------------------------------------ */
/*  Internal data types                                                 */
/* ------------------------------------------------------------------ */

typedef struct _fsm_kotlin_output_generator_ FSMKotlinOutputGenerator, *pFSMKotlinOutputGenerator;
typedef struct _kotlin_machine_data_         KotlinMachineData,         *pKotlinMachineData;

struct _kotlin_machine_data_ {
	FILE           *file;
	char           *fileName;
	char           *baseName;
	pMACHINE_INFO   pmi;
	bool            uses_event_data;   /* true if any event has a data block */
};

struct _fsm_kotlin_output_generator_
{
	FSMOutputGenerator  fsmog;
	pKotlinMachineData  pkd;
};

/* ------------------------------------------------------------------ */
/*  Forward declarations                                                */
/* ------------------------------------------------------------------ */

static int   initKotlinWriter(pFSMOutputGenerator, char *);
static void  writeKotlinWriter(pFSMOutputGenerator, pMACHINE_INFO);
static void  closeKotlinWriter(pFSMOutputGenerator, int);

static int   initKotlinFN(pFSMOutputGenerator, char *);
static void  writeKotlinFN(pFSMOutputGenerator, pMACHINE_INFO);
static void  closeKotlinFN(pFSMOutputGenerator, int);

/* list iterators */
static bool print_state_enum_entry(pLIST_ELEMENT, void *);
static bool print_event_enum_entry(pLIST_ELEMENT, void *);
static bool print_event_data_variant(pLIST_ELEMENT, void *);
static bool scan_event_for_data(pLIST_ELEMENT, void *);
static bool print_action_hook(pLIST_ELEMENT, void *);
static bool print_transition_fn_hook(pLIST_ELEMENT, void *);
static bool print_entry_exit_hooks(pLIST_ELEMENT, void *);
static bool print_event_translator_hook(pLIST_ELEMENT, void *);
static bool print_dispatch_state_block(pLIST_ELEMENT, void *);
static bool print_entry_exit_calls(pLIST_ELEMENT, void *);
static bool print_data_field_as_kotlin(pLIST_ELEMENT, void *);

/* helpers */
static void writeKotlinMachineInternal(pKotlinMachineData pkd);
static void writePackageDeclaration(FILE *fout, const char *machine_name);
static void writeFileHeader(FILE *fout, const char *fileName);
static void writeStateEnum(FILE *fout, pMACHINE_INFO pmi);
static void writeEventEnum(FILE *fout, pMACHINE_INFO pmi);
static void writeEventDataSealedInterface(FILE *fout, pMACHINE_INFO pmi);
static void writeEventStruct(FILE *fout, pMACHINE_INFO pmi);
static void writeHooksClass(FILE *fout, pMACHINE_INFO pmi, bool uses_event_data);
static void writeMachineClass(pKotlinMachineData pkd);
static void writeTranslateEventData(FILE *fout, pMACHINE_INFO pmi);
static void writeDispatchFunction(FILE *fout, pMACHINE_INFO pmi, bool uses_event_data);
static void writeEntryExitManager(FILE *fout, pMACHINE_INFO pmi);

/* ------------------------------------------------------------------ */
/*  Static generator singleton                                          */
/* ------------------------------------------------------------------ */

static FSMKotlinOutputGenerator KotlinMachineWriter = {
	{
		initKotlinWriter
		, writeKotlinWriter
		, closeKotlinWriter
		, generateKotlinMachineWriter
	}
	, NULL
};

/* ------------------------------------------------------------------ */
/*  Public entry point                                                  */
/* ------------------------------------------------------------------ */

pFSMOutputGenerator generateKotlinMachineWriter(pFSMOutputGenerator parent)
{
	pFSMOutputGenerator pfsmog;

	if (parent)
	{
		yyerror("Kotlin generator supports only flat (single-level) machines.");
		pfsmog = NULL;
	}
	else
	{
		pfsmog = (pFSMOutputGenerator)&KotlinMachineWriter;
	}

	if (pfsmog && output_generated_file_names_only)
	{
		pfsmog->initOutput   = initKotlinFN;
		pfsmog->writeMachine = writeKotlinFN;
		pfsmog->closeOutput  = closeKotlinFN;
	}

	return pfsmog;
}

/* ------------------------------------------------------------------ */
/*  Package name sanitization                                           */
/* ------------------------------------------------------------------ */

/*
 * Kotlin hard keywords (used for package component safety check)
 */
static const char * const kotlin_keywords[] = {
	"as", "break", "class", "continue", "do", "else", "false",
	"for", "fun", "if", "in", "interface", "is", "null", "object",
	"package", "return", "super", "this", "throw", "true", "try",
	"typealias", "typeof", "val", "var", "when", "while",
	NULL
};

static bool isKotlinKeyword(const char *word)
{
	for (int i = 0; kotlin_keywords[i] != NULL; i++)
	{
		if (strcmp(word, kotlin_keywords[i]) == 0)
		{
			return true;
		}
	}
	return false;
}

/*
 * Sanitize a machine name into a safe Kotlin package identifier component.
 * Rules (per SPEC.md):
 *   1) lowercase
 *   2) replace any char not in [a-z0-9_] with _
 *   3) collapse multiple consecutive _ into one _
 *   4) trim leading/trailing _
 *   5) if empty, use "machine"
 *   6) if starts with a digit, prefix with "m_"
 *   7) if equals a Kotlin keyword, prefix with "m_"
 *
 * out must be at least out_size bytes.
 */
static void kotlinSanitizeName(const char *name, char *out, size_t out_size)
{
	char tmp[512];
	size_t ti = 0;

	/* Step 1+2: lowercase and replace non-[a-z0-9_] */
	for (size_t i = 0; name[i] && ti < sizeof(tmp) - 1; i++)
	{
		char c = (char)tolower((unsigned char)name[i]);
		if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_')
		{
			tmp[ti++] = c;
		}
		else
		{
			tmp[ti++] = '_';
		}
	}
	tmp[ti] = '\0';

	/* Step 3: collapse multiple _ */
	char tmp2[512];
	size_t ti2 = 0;
	for (size_t i = 0; tmp[i] && ti2 < sizeof(tmp2) - 1; i++)
	{
		if (tmp[i] == '_' && ti2 > 0 && tmp2[ti2 - 1] == '_')
		{
			continue;  /* skip duplicate _ */
		}
		tmp2[ti2++] = tmp[i];
	}
	tmp2[ti2] = '\0';

	/* Step 4: trim leading/trailing _ */
	size_t start = 0;
	while (tmp2[start] == '_') start++;
	size_t end = strlen(tmp2);
	while (end > start && tmp2[end - 1] == '_') end--;
	tmp2[end] = '\0';
	const char *trimmed = tmp2 + start;

	/* Step 5: empty -> "machine" */
	if (!trimmed[0])
	{
		trimmed = "machine";
	}

	/* Steps 6+7: starts with digit or is keyword -> prefix "m_" */
	if (trimmed[0] >= '0' && trimmed[0] <= '9')
	{
		snprintf(out, out_size, "m_%s", trimmed);
	}
	else if (isKotlinKeyword(trimmed))
	{
		snprintf(out, out_size, "m_%s", trimmed);
	}
	else
	{
		snprintf(out, out_size, "%s", trimmed);
	}
}

/* ------------------------------------------------------------------ */
/*  File-names-only mode (-M / -Md)                                    */
/* ------------------------------------------------------------------ */

static int initKotlinFN(pFSMOutputGenerator pfsmog, char *baseFileName)
{
	pFSMKotlinOutputGenerator pfsmkotog = (pFSMKotlinOutputGenerator)pfsmog;

	if (NULL != (pfsmkotog->pkd = calloc(1, sizeof(KotlinMachineData))))
	{
		if (baseFileName)
		{
			pfsmkotog->pkd->fileName = createFileName(baseFileName, ".kt");
			pfsmkotog->pkd->baseName = strdup(baseFileName);
		}
	}

	return (int)(!pfsmkotog->pkd || !pfsmkotog->pkd->fileName);
}

static void writeKotlinFN(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
	pFSMKotlinOutputGenerator pfsmkotog = (pFSMKotlinOutputGenerator)pfsmog;

	if (output_make_recipe)
	{
		printf("%s: %s.fsm "
			   , pfsmkotog->pkd->fileName
			   , pfsmkotog->pkd->baseName
			   );
		print_included_files_list();
		printf("\n");
	}
	else
	{
		printf("%s ", pfsmkotog->pkd->fileName);
	}

	(void)pmi;
}

static void closeKotlinFN(pFSMOutputGenerator pfsmog, int good)
{
	pFSMKotlinOutputGenerator pfsmkotog = (pFSMKotlinOutputGenerator)pfsmog;

	if (!pfsmkotog || !pfsmkotog->pkd)
	{
		return;
	}

	CHECK_AND_FREE(pfsmkotog->pkd->fileName);
	CHECK_AND_FREE(pfsmkotog->pkd->baseName);
	CHECK_AND_FREE(pfsmkotog->pkd);

	(void)good;
}

/* ------------------------------------------------------------------ */
/*  Normal generation mode                                              */
/* ------------------------------------------------------------------ */

static int initKotlinWriter(pFSMOutputGenerator pfsmog, char *baseFileName)
{
	pFSMKotlinOutputGenerator pfsmkotog = (pFSMKotlinOutputGenerator)pfsmog;

	if (NULL != (pfsmkotog->pkd = calloc(1, sizeof(KotlinMachineData))))
	{
		if (!baseFileName)
		{
			pfsmkotog->pkd->file = stdout;
		}
		else
		{
			pfsmkotog->pkd->fileName = createFileName(baseFileName, ".kt");
			pfsmkotog->pkd->baseName = strdup(baseFileName);

			if (NULL == (pfsmkotog->pkd->file = openFile(pfsmkotog->pkd->fileName, "w")))
			{
				CHECK_AND_FREE(pfsmkotog->pkd->fileName);
				CHECK_AND_FREE(pfsmkotog->pkd->baseName);
			}
		}
	}

	return (int)(!pfsmkotog->pkd || !pfsmkotog->pkd->file);
}

static void writeKotlinWriter(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
	pFSMKotlinOutputGenerator pfsmkotog = (pFSMKotlinOutputGenerator)pfsmog;

	pfsmkotog->pkd->pmi = pmi;
	writeKotlinMachineInternal(pfsmkotog->pkd);
}

static void closeKotlinWriter(pFSMOutputGenerator pfsmog, int good)
{
	pFSMKotlinOutputGenerator pfsmkotog = (pFSMKotlinOutputGenerator)pfsmog;

	if (!pfsmkotog || !pfsmkotog->pkd)
	{
		return;
	}

	if (pfsmkotog->pkd->file && pfsmkotog->pkd->file != stdout)
	{
		fclose(pfsmkotog->pkd->file);
	}

	if (!good && pfsmkotog->pkd->fileName)
	{
		unlink(pfsmkotog->pkd->fileName);
	}

	CHECK_AND_FREE(pfsmkotog->pkd->fileName);
	CHECK_AND_FREE(pfsmkotog->pkd->baseName);
	CHECK_AND_FREE(pfsmkotog->pkd);
}

/* ------------------------------------------------------------------ */
/*  Main generation logic                                               */
/* ------------------------------------------------------------------ */

static void writeKotlinMachineInternal(pKotlinMachineData pkd)
{
	FILE          *fout = pkd->file;
	pMACHINE_INFO  pmi  = pkd->pmi;

	/* Detect whether any event carries a data block */
	{
		ITERATOR_HELPER scan_ih = { .fout = NULL, .found = false };
		iterate_list(pmi->event_list, scan_event_for_data, &scan_ih);
		pkd->uses_event_data = scan_ih.found;
	}

	writePackageDeclaration(fout, pmi->name->name);
	writeFileHeader(fout, pkd->fileName ? pkd->fileName : "<stdout>");
	writeStateEnum(fout, pmi);
	writeEventEnum(fout, pmi);

	/* Always emit the EventData sealed interface and EventStruct */
	writeEventDataSealedInterface(fout, pmi);
	writeEventStruct(fout, pmi);

	writeHooksClass(fout, pmi, pkd->uses_event_data);
	writeMachineClass(pkd);
}

/* ------------------------------------------------------------------ */
/*  Package declaration                                                 */
/* ------------------------------------------------------------------ */

static void writePackageDeclaration(FILE *fout, const char *machine_name)
{
	if (kotlin_package)
	{
		/* User-supplied package: emit verbatim */
		fprintf(fout, "package %s\n\n", kotlin_package);
	}
	else
	{
		/* Default package: io.github.fsmlang.generated.<sanitized_machine_name> */
		char sanitized[512];
		kotlinSanitizeName(machine_name, sanitized, sizeof(sanitized));
		fprintf(fout, "package io.github.fsmlang.generated.%s\n\n", sanitized);
	}
}

/* ------------------------------------------------------------------ */
/*  File header                                                         */
/* ------------------------------------------------------------------ */

static void writeFileHeader(FILE *fout, const char *fileName)
{
	fprintf(fout,
		"/**\n"
		" * %s\n"
		" *\n"
		" * This file generated by FSMLang\n"
		" */\n\n"
		, fileName
		);
}

/* ------------------------------------------------------------------ */
/*  State enum                                                          */
/* ------------------------------------------------------------------ */

static bool print_state_enum_entry(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate = (pID_INFO)pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER)data;

	fprintf(pih->fout, "    %s,\n", pstate->name);

	return false;
}

static void writeStateEnum(FILE *fout, pMACHINE_INFO pmi)
{
	ITERATOR_HELPER ih = { .fout = fout };

	fprintf(fout, "enum class %sState {\n", pmi->name->name);
	iterate_list(pmi->state_list, print_state_enum_entry, &ih);
	fprintf(fout, "    noState\n}\n\n");
}

/* ------------------------------------------------------------------ */
/*  Event enum                                                          */
/* ------------------------------------------------------------------ */

static bool print_event_enum_entry(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pevent = (pID_INFO)pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER)data;

	fprintf(pih->fout, "    %s,\n", pevent->name);

	return false;
}

static void writeEventEnum(FILE *fout, pMACHINE_INFO pmi)
{
	ITERATOR_HELPER ih = { .fout = fout };

	fprintf(fout, "enum class %sEvent {\n", pmi->name->name);
	iterate_list(pmi->event_list, print_event_enum_entry, &ih);
	fprintf(fout, "    noEvent\n}\n\n");
}

/* ------------------------------------------------------------------ */
/*  Event data sealed interface and EventStruct                         */
/* ------------------------------------------------------------------ */

static bool scan_event_for_data(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pevent = (pID_INFO)pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER)data;

	if (pevent->type_data.event_data.puser_event_data)
	{
		pih->found = true;
	}

	return false;
}

/*
 * Emit one Kotlin data class field from a C-style DATA_FIELD.
 */
static bool print_data_field_as_kotlin(pLIST_ELEMENT pelem, void *data)
{
	pDATA_FIELD      pdf = (pDATA_FIELD)pelem->mbr;
	pITERATOR_HELPER pih = (pITERATOR_HELPER)data;
	const char      *field_name = pdf->data_field_name->name;
	const char      *kotlin_type;

	switch (pdf->pdts->dtt)
	{
	case dtt_simple:
	{
		const char *c_type = pdf->pdts->dtu.name->name;
		/* Map common C primitive types to Kotlin equivalents */
		if (strcmp(c_type, "int") == 0 || strcmp(c_type, "short") == 0)
		{
			kotlin_type = "Int";
		}
		else if (strcmp(c_type, "long") == 0)
		{
			kotlin_type = "Long";
		}
		else if (strcmp(c_type, "char") == 0)
		{
			kotlin_type = "Char";
		}
		else if (strcmp(c_type, "float") == 0)
		{
			kotlin_type = "Float";
		}
		else if (strcmp(c_type, "double") == 0)
		{
			kotlin_type = "Double";
		}
		else if (strcmp(c_type, "bool") == 0 || strcmp(c_type, "_Bool") == 0)
		{
			kotlin_type = "Boolean";
		}
		else if (strcmp(c_type, "unsigned") == 0
				 || strcmp(c_type, "unsigned int") == 0
				 || strcmp(c_type, "uint32_t") == 0)
		{
			kotlin_type = "UInt";
		}
		else
		{
			/* Unknown type: use Any? as a safe fallback */
			kotlin_type = "Any?";
		}

		if (pdf->pdts->is_array)
		{
			fprintf(pih->fout
					, "        val %s: Array<%s> = emptyArray()"
					, field_name
					, kotlin_type
					);
		}
		else
		{
			const char *default_val;
			if (strcmp(kotlin_type, "Int") == 0)          default_val = "0";
			else if (strcmp(kotlin_type, "Long") == 0)    default_val = "0L";
			else if (strcmp(kotlin_type, "Char") == 0)    default_val = "'\\u0000'";
			else if (strcmp(kotlin_type, "Float") == 0)   default_val = "0.0f";
			else if (strcmp(kotlin_type, "Double") == 0)  default_val = "0.0";
			else if (strcmp(kotlin_type, "Boolean") == 0) default_val = "false";
			else if (strcmp(kotlin_type, "UInt") == 0)    default_val = "0u";
			else                                           default_val = "null";

			fprintf(pih->fout
					, "        val %s: %s = %s"
					, field_name
					, kotlin_type
					, default_val
					);
		}
		break;
	}
	case dtt_struct:
	case dtt_union:
		/* Nested structs/unions: emit a comment and use Any? */
		fprintf(pih->fout
				, "        /* nested struct/union */ val %s: Any? = null"
				, field_name
				);
		break;
	}

	return false;
}

/*
 * Emit one variant inside the sealed EventData interface for a data-bearing event.
 * The variant is a data class with all fields having Kotlin-zero defaults.
 */
static bool print_event_data_variant(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pevent = (pID_INFO)pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER)data;
	pUSER_EVENT_DATA pued   = pevent->type_data.event_data.puser_event_data;

	if (!pued || !pued->data_fields)
	{
		return false;
	}

	/* Capitalise first letter for the class name */
	char cap_name[256];
	snprintf(cap_name, sizeof(cap_name), "%s", pevent->name);
	if (cap_name[0] != '\0')
	{
		cap_name[0] = (char)toupper((unsigned char)cap_name[0]);
	}

	fprintf(pih->fout
			, "    data class %sPayload(\n"
			, cap_name
			);

	/* Emit fields */
	pLIST_ELEMENT pe = pued->data_fields->head;
	while (pe)
	{
		ITERATOR_HELPER field_ih = { .fout = pih->fout };
		print_data_field_as_kotlin(pe, &field_ih);
		pe = pe->next;
		if (pe)
		{
			fprintf(pih->fout, ",\n");
		}
		else
		{
			fprintf(pih->fout, "\n");
		}
	}

	fprintf(pih->fout
			, "    ) : %sEventData\n"
			, pih->pmi->name->name
			);

	return false;
}

/*
 * Emit the sealed interface <Machine>EventData with one variant per
 * data-bearing event plus an object None.
 */
static void writeEventDataSealedInterface(FILE *fout, pMACHINE_INFO pmi)
{
	ITERATOR_HELPER ih = { .fout = fout, .pmi = pmi };
	const char     *machine_name = pmi->name->name;

	fprintf(fout, "sealed interface %sEventData {\n", machine_name);
	fprintf(fout, "    object None : %sEventData\n", machine_name);
	iterate_list(pmi->event_list, print_event_data_variant, &ih);
	fprintf(fout, "}\n\n");
}

/*
 * Emit the event-struct data class.
 */
static void writeEventStruct(FILE *fout, pMACHINE_INFO pmi)
{
	const char *machine_name = pmi->name->name;

	fprintf(fout,
		"data class %sEventStruct(\n"
		"    val event: %sEvent,\n"
		"    val eventData: %sEventData = %sEventData.None\n"
		")\n\n"
		, machine_name
		, machine_name
		, machine_name
		, machine_name
		);
}

/* ------------------------------------------------------------------ */
/*  Hooks class                                                         */
/* ------------------------------------------------------------------ */

/*
 * Returns the Kotlin return type string for an action.
 * The caller must not free the returned string.
 */
static const char *actionReturnType(pMACHINE_INFO pmi, const char *machineName)
{
	static char buf[256];

	switch (pmi->modFlags & ACTIONS_RETURN_FLAGS)
	{
	case mfActionsReturnVoid:
		return "Unit";
	case mfActionsReturnStates:
		snprintf(buf, sizeof(buf), "%sState", machineName);
		return buf;
	default: /* returns events */
		snprintf(buf, sizeof(buf), "%sEvent", machineName);
		return buf;
	}
}

/* Emit one action hook */
static bool print_action_hook(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         paction = (pID_INFO)pelem->mbr;
	pITERATOR_HELPER pih     = (pITERATOR_HELPER)data;

	if (!paction->name || !strlen(paction->name))
	{
		return false;
	}

	const char *machine_name = pih->pmi->name->name;
	const char *ret_type     = actionReturnType(pih->pmi, machine_name);
	bool        is_abstract  = !generate_weak_fns;

	if (is_abstract)
	{
		if ((pih->pmi->modFlags & ACTIONS_RETURN_FLAGS) == mfActionsReturnVoid)
		{
			fprintf(pih->fout
					, "    abstract fun %s(fsm: %s)\n"
					, paction->name
					, machine_name
					);
		}
		else
		{
			fprintf(pih->fout
					, "    abstract fun %s(fsm: %s): %s\n"
					, paction->name
					, machine_name
					, ret_type
					);
		}
	}
	else
	{
		if ((pih->pmi->modFlags & ACTIONS_RETURN_FLAGS) == mfActionsReturnVoid)
		{
			fprintf(pih->fout
					, "    open fun %s(fsm: %s) { println(\"%s_%s\") }\n"
					, paction->name
					, machine_name
					, machine_name
					, paction->name
					);
		}
		else if ((pih->pmi->modFlags & ACTIONS_RETURN_FLAGS) == mfActionsReturnStates)
		{
			fprintf(pih->fout
					, "    open fun %s(fsm: %s): %s {\n"
					  "        println(\"%s_%s\")\n"
					  "        return %sState.noState\n"
					  "    }\n"
					, paction->name
					, machine_name
					, ret_type
					, machine_name
					, paction->name
					, machine_name
					);
		}
		else
		{
			/* default: actions return events */
			fprintf(pih->fout
					, "    open fun %s(fsm: %s): %s {\n"
					  "        println(\"%s_%s\")\n"
					  "        return %sEvent.noEvent\n"
					  "    }\n"
					, paction->name
					, machine_name
					, ret_type
					, machine_name
					, paction->name
					, machine_name
					);
		}
	}

	return false;
}

/* Emit one transition-function hook */
static bool print_transition_fn_hook(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         ptfn = (pID_INFO)pelem->mbr;
	pITERATOR_HELPER pih  = (pITERATOR_HELPER)data;
	const char      *machine_name = pih->pmi->name->name;
	bool             is_abstract  = !generate_weak_fns;

	if (is_abstract)
	{
		fprintf(pih->fout
				, "    abstract fun %s(fsm: %s): %sState\n"
				, ptfn->name
				, machine_name
				, machine_name
				);
	}
	else
	{
		fprintf(pih->fout
				, "    open fun %s(fsm: %s): %sState {\n"
				  "        println(\"%s_%s\")\n"
				  "        return %sState.noState\n"
				  "    }\n"
				, ptfn->name
				, machine_name
				, machine_name
				, machine_name
				, ptfn->name
				, machine_name
				);
	}

	return false;
}

/* Emit entry and exit hooks for a state (if any) */
static bool print_entry_exit_hooks(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate = (pID_INFO)pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER)data;
	pSTATE_DATA      psd    = &(pstate->type_data.state_data);
	const char      *machine_name = pih->pmi->name->name;
	bool             is_abstract  = !generate_weak_fns;

	if (psd->entry_fn)
	{
		if (is_abstract)
		{
			fprintf(pih->fout
					, "    abstract fun %s(fsm: %s)\n"
					, psd->entry_fn->name
					, machine_name
					);
		}
		else
		{
			fprintf(pih->fout
					, "    open fun %s(fsm: %s) { println(\"%s_%s\") }\n"
					, psd->entry_fn->name
					, machine_name
					, machine_name
					, psd->entry_fn->name
					);
		}
	}

	if (psd->exit_fn)
	{
		if (is_abstract)
		{
			fprintf(pih->fout
					, "    abstract fun %s(fsm: %s)\n"
					, psd->exit_fn->name
					, machine_name
					);
		}
		else
		{
			fprintf(pih->fout
					, "    open fun %s(fsm: %s) { println(\"%s_%s\") }\n"
					, psd->exit_fn->name
					, machine_name
					, machine_name
					, psd->exit_fn->name
					);
		}
	}

	return false;
}

/*
 * Compute the translator hook name for an event.
 * If an explicit translator was declared in the FSM, use that name.
 * Otherwise use grab_<event>_data (per SPEC.md section 5).
 */
static void translatorName(const char *event_name, pUSER_EVENT_DATA pued, char *out, size_t out_size)
{
	if (pued && pued->translator && pued->translator->name && strlen(pued->translator->name))
	{
		snprintf(out, out_size, "%s", pued->translator->name);
	}
	else
	{
		snprintf(out, out_size, "grab_%s_data", event_name);
	}
}

/*
 * Emit one event-data translator hook.
 * The payload type is <Machine>EventData.<EventName>Payload.
 */
static bool print_event_translator_hook(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pevent = (pID_INFO)pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER)data;
	pUSER_EVENT_DATA pued   = pevent->type_data.event_data.puser_event_data;
	const char      *machine_name = pih->pmi->name->name;
	bool             is_abstract  = !generate_weak_fns;

	if (!pued || !pued->data_fields)
	{
		return false;
	}

	char tname[256];
	translatorName(pevent->name, pued, tname, sizeof(tname));

	/* Build payload type: <Machine>EventData.<EventName>Payload */
	char cap_event[256];
	snprintf(cap_event, sizeof(cap_event), "%s", pevent->name);
	if (cap_event[0] != '\0')
	{
		cap_event[0] = (char)toupper((unsigned char)cap_event[0]);
	}

	char payload_type[512];
	snprintf(payload_type, sizeof(payload_type), "%sEventData.%sPayload", machine_name, cap_event);

	if (is_abstract)
	{
		fprintf(pih->fout
				, "    abstract fun %s(fsm: %s, payload: %s): %s\n"
				, tname
				, machine_name
				, payload_type
				, payload_type
				);
	}
	else
	{
		fprintf(pih->fout
				, "    open fun %s(fsm: %s, payload: %s): %s {\n"
				  "        println(\"%s_%s\")\n"
				  "        return payload\n"
				  "    }\n"
				, tname
				, machine_name
				, payload_type
				, payload_type
				, machine_name
				, tname
				);
	}

	return false;
}

static void writeHooksClass(FILE *fout, pMACHINE_INFO pmi, bool uses_event_data)
{
	ITERATOR_HELPER ih = { .fout = fout, .pmi = pmi };
	const char     *machine_name = pmi->name->name;
	bool            is_abstract  = !generate_weak_fns;
	const char     *class_prefix = is_abstract ? "abstract" : "open";

	fprintf(fout, "%s class %sUserHooks {\n", class_prefix, machine_name);

	/* Machine-level transition function */
	if (pmi->machineTransition)
	{
		if (is_abstract)
		{
			fprintf(fout
					, "    abstract fun %s(fsm: %s)\n"
					, pmi->machineTransition->name
					, machine_name
					);
		}
		else
		{
			fprintf(fout
					, "    open fun %s(fsm: %s) { println(\"%s_%s\") }\n"
					, pmi->machineTransition->name
					, machine_name
					, machine_name
					, pmi->machineTransition->name
					);
		}
	}

	/* Action hooks */
	iterate_list(pmi->action_list, print_action_hook, &ih);

	/* Transition-function hooks */
	iterate_list(pmi->transition_fn_list, print_transition_fn_hook, &ih);

	/* Entry/exit hooks */
	if (pmi->states_with_entry_fns_count || pmi->states_with_exit_fns_count)
	{
		iterate_list(pmi->state_list, print_entry_exit_hooks, &ih);
	}

	/* Event-data translator hooks */
	if (uses_event_data)
	{
		iterate_list(pmi->event_list, print_event_translator_hook, &ih);
	}

	fprintf(fout, "}\n\n");
}

/* ------------------------------------------------------------------ */
/*  Machine class and dispatch loop                                     */
/* ------------------------------------------------------------------ */

/*
 * Write the entry/exit manager calls after a state transition.
 */
static bool print_entry_exit_calls(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate = (pID_INFO)pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER)data;
	pSTATE_DATA      psd    = &(pstate->type_data.state_data);
	const char      *machine_name = pih->pmi->name->name;

	if (!psd->entry_fn && !psd->exit_fn)
	{
		return false;
	}

	/* We encode which list (exit vs entry) in pih->error */
	if (pih->error)
	{
		/* exit manager */
		if (psd->exit_fn)
		{
			fprintf(pih->fout
					, "            %sState.%s -> hooks.%s(this)\n"
					, machine_name
					, pstate->name
					, psd->exit_fn->name
					);
		}
	}
	else
	{
		/* entry manager */
		if (psd->entry_fn)
		{
			fprintf(pih->fout
					, "            %sState.%s -> hooks.%s(this)\n"
					, machine_name
					, pstate->name
					, psd->entry_fn->name
					);
		}
	}

	return false;
}

static void writeEntryExitManager(FILE *fout, pMACHINE_INFO pmi)
{
	ITERATOR_HELPER ih = { .fout = fout, .pmi = pmi };

	if (pmi->states_with_exit_fns_count)
	{
		fprintf(fout,
			"        if (state != prevState) {\n"
			"            when (prevState) {\n"
			);
		ih.error = true;  /* signal: emit exit fns */
		iterate_list(pmi->state_list, print_entry_exit_calls, &ih);
		fprintf(fout,
			"            else -> {}\n"
			"            }\n"
			"        }\n"
			);
	}

	if (pmi->states_with_entry_fns_count)
	{
		fprintf(fout,
			"        if (state != prevState) {\n"
			"            when (state) {\n"
			);
		ih.error = false;  /* signal: emit entry fns */
		iterate_list(pmi->state_list, print_entry_exit_calls, &ih);
		fprintf(fout,
			"            else -> {}\n"
			"            }\n"
			"        }\n"
			);
	}
}

/*
 * Emit a single state block inside the outer when (state).
 */
static bool print_dispatch_state_block(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pstate = (pID_INFO)pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER)data;
	pMACHINE_INFO    pmi    = pih->pmi;
	const char      *machine_name = pmi->name->name;
	bool             acts_return_void   = (pmi->modFlags & mfActionsReturnVoid) != 0;
	bool             acts_return_states = (pmi->modFlags & mfActionsReturnStates) != 0;

	fprintf(pih->fout
			, "                %sState.%s -> when (currentEvent) {\n"
			, machine_name
			, pstate->name
			);

	/* Iterate over events for this state */
	unsigned num_events = pmi->event_list->count;
	for (unsigned ev = 0; ev < num_events; ev++)
	{
		pACTION_INFO pai = pmi->actionArray[ev][pstate->order];
		if (!pai)
		{
			continue;
		}

		pID_INFO pevent = eventPidByIndex(pmi, (int)ev);
		if (!pevent)
		{
			continue;
		}

		bool has_action          = pai->action && strlen(pai->action->name);
		bool has_transition      = (pai->transition != NULL);
		bool transition_is_fn    = has_transition && (pai->transition->type == TRANSITION_FN);
		bool transition_is_state = has_transition && (pai->transition->type == STATE);

		fprintf(pih->fout
				, "                    %sEvent.%s -> {\n"
				, machine_name
				, pevent->name
				);

		if (has_action)
		{
			if (acts_return_void)
			{
				fprintf(pih->fout
						, "                        hooks.%s(this)\n"
						  "                        currentEvent = %sEvent.noEvent\n"
						, pai->action->name
						, machine_name
						);
			}
			else if (acts_return_states)
			{
				fprintf(pih->fout
						, "                        state = hooks.%s(this)\n"
						  "                        currentEvent = %sEvent.noEvent\n"
						, pai->action->name
						, machine_name
						);
			}
			else
			{
				/* actions return events */
				fprintf(pih->fout
						, "                        currentEvent = hooks.%s(this)\n"
						, pai->action->name
						);
			}
		}
		else
		{
			/* No action */
			fprintf(pih->fout
					, "                        currentEvent = %sEvent.noEvent\n"
					, machine_name
					);
		}

		/* Transition (only if actions do not return states) */
		if (!acts_return_states && has_transition)
		{
			if (transition_is_state)
			{
				fprintf(pih->fout
						, "                        state = %sState.%s\n"
						, machine_name
						, pai->transition->name
						);
			}
			else if (transition_is_fn)
			{
				fprintf(pih->fout
						, "                        state = hooks.%s(this)\n"
						, pai->transition->name
						);
			}
		}

		fprintf(pih->fout, "                    }\n");
	}

	/* Else branch - no cell defined for this (state, event) combination */
	fprintf(pih->fout
			, "                    else -> currentEvent = %sEvent.noEvent\n"
			  "                }\n"
			, machine_name
			);

	return false;
}

/*
 * Emit translateEventData: dispatches based on eventStruct.event to call
 * the appropriate user translator hook, with zero-init fallback for
 * missing/wrong payload variant.
 */
static void writeTranslateEventData(FILE *fout, pMACHINE_INFO pmi)
{
	const char *machine_name = pmi->name->name;

	fprintf(fout,
		"    private fun translateEventData(eventStruct: %sEventStruct) {\n"
		"        when (eventStruct.event) {\n"
		, machine_name
		);

	pLIST_ELEMENT pe = pmi->event_list->head;
	while (pe)
	{
		pID_INFO         pevent = (pID_INFO)pe->mbr;
		pUSER_EVENT_DATA pued   = pevent->type_data.event_data.puser_event_data;

		if (pued && pued->data_fields)
		{
			char cap_event[256];
			snprintf(cap_event, sizeof(cap_event), "%s", pevent->name);
			if (cap_event[0] != '\0')
			{
				cap_event[0] = (char)toupper((unsigned char)cap_event[0]);
			}

			char tname[256];
			translatorName(pevent->name, pued, tname, sizeof(tname));

			fprintf(fout,
				"            %sEvent.%s -> {\n"
				"                val payload = eventStruct.eventData as? %sEventData.%sPayload\n"
				"                    ?: %sEventData.%sPayload()\n"
				"                hooks.%s(this, payload)\n"
				"            }\n"
				, machine_name, pevent->name
				, machine_name, cap_event
				, machine_name, cap_event
				, tname
				);
		}

		pe = pe->next;
	}

	fprintf(fout,
		"            else -> {}\n"
		"        }\n"
		"    }\n\n"
		);
}

/*
 * Write the full dispatch function (single-switch style).
 * Always accepts a <Machine>EventStruct (event enum + payload union).
 */
static void writeDispatchFunction(FILE *fout, pMACHINE_INFO pmi, bool uses_event_data)
{
	ITERATOR_HELPER  ih           = { .fout = fout, .pmi = pmi };
	const char      *machine_name = pmi->name->name;

	fprintf(fout,
		"    fun dispatch(eventStruct: %sEventStruct) {\n"
		"        translateEventData(eventStruct)\n"
		"        var currentEvent = eventStruct.event\n"
		"        this.event = currentEvent\n"
		"        while (currentEvent != %sEvent.noEvent) {\n"
		"            val prevState = state\n"
		"            when (state) {\n"
		, machine_name
		, machine_name
		);

	/* Emit one block per state */
	iterate_list(pmi->state_list, print_dispatch_state_block, &ih);

	fprintf(fout,
		"                else -> currentEvent = %sEvent.noEvent\n"
		"            }\n"
		"            this.event = currentEvent\n"
		, machine_name
		);

	/* Entry/exit manager */
	writeEntryExitManager(fout, pmi);

	/* Machine-level transition callback */
	if (pmi->machineTransition)
	{
		fprintf(fout,
			"            if (state != prevState) {\n"
			"                hooks.%s(this)\n"
			"            }\n"
			, pmi->machineTransition->name
			);
	}

	fprintf(fout,
		"        }\n"
		"    }\n"
		);

	(void)uses_event_data;
}

static void writeMachineClass(pKotlinMachineData pkd)
{
	FILE          *fout         = pkd->file;
	pMACHINE_INFO  pmi          = pkd->pmi;
	const char    *machine_name = pmi->name->name;
	bool           uses_event_data = pkd->uses_event_data;

	if (generate_weak_fns)
	{
		fprintf(fout,
			"class %s(val hooks: %sUserHooks = %sUserHooks()) {\n"
			, machine_name
			, machine_name
			, machine_name
			);
	}
	else
	{
		fprintf(fout,
			"class %s(val hooks: %sUserHooks) {\n"
			, machine_name
			, machine_name
			);
	}

	/* State and event fields */
	fprintf(fout,
		"    var state: %sState = %sState.%s\n"
		"    var event: %sEvent = %sEvent.noEvent\n\n"
		, machine_name
		, machine_name
		, stateNameByIndex(pmi, 0)
		, machine_name
		, machine_name
		);

	/* translateEventData helper (always emit) */
	writeTranslateEventData(fout, pmi);

	/* Dispatch function */
	writeDispatchFunction(fout, pmi, uses_event_data);

	fprintf(fout, "}\n");
}
