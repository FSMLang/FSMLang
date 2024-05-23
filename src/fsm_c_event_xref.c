/**
*  fsm_c_event_xref.c
*
*    Create stand-alone event cross-reference
*
*    FSMLang (fsm) - A Finite State Machine description language.
*    Copyright (C) 2024  Steven Stanton
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

#include "fsm_c_event_xref.h"

#include "fsm_priv.h"
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

typedef void (*fpInternalWriter)(pMACHINE_INFO);
typedef void (*fpXrefEntryWriter)(char*,pITERATOR_HELPER);

typedef struct _xref_writer_struct_ XREF_WRITER, *pXREF_WRITER;

struct _xref_writer_struct_
{
	fpInternalWriter  writer;
	fpXrefEntryWriter entry_writer;
};

static int  initCEventXRefWriter(pFSMOutputGenerator,char*);
static void writeCEventXRef(pFSMOutputGenerator,pMACHINE_INFO);
static void closeCEventXRefWriter(pFSMOutputGenerator,int);
static void writeCEventXRefJSON(pMACHINE_INFO);
static void writeCEventXRefCSV(pMACHINE_INFO);
static void writeCEventXRefTab(pMACHINE_INFO);
static void writeCEventXRefXML(pMACHINE_INFO);

static void complete_top_level(pITERATOR_HELPER);
static void process_any_sub_machines(pITERATOR_HELPER);

static bool iterate_sub_machines(pLIST_ELEMENT,void*);
static bool print_event_xref_json(pLIST_ELEMENT,void*);
static bool print_event_xref_csv(pLIST_ELEMENT,void*);
static bool print_event_xref_tab(pLIST_ELEMENT,void*);
static bool print_event_xref_xml(pLIST_ELEMENT,void*);
static void print_xref_entry_json(char *,pITERATOR_HELPER);
static void print_xref_entry_csv(char *,pITERATOR_HELPER);
static void print_xref_entry_tab(char *,pITERATOR_HELPER);
static void print_xref_entry_xml(char *,pITERATOR_HELPER);

static FSMOutputGenerator ceventxrefwriter = {
	.initOutput = initCEventXRefWriter
	, .writeMachine = writeCEventXRef
	, .closeOutput  = closeCEventXRefWriter
};

static const XREF_WRITER json_writer = {
	.writer = writeCEventXRefJSON
	, .entry_writer = print_xref_entry_json
};

static const XREF_WRITER csv_writer = {
	.writer = writeCEventXRefCSV
	, .entry_writer = print_xref_entry_csv
};

static const XREF_WRITER tab_writer = {
	.writer = writeCEventXRefTab
	, .entry_writer = print_xref_entry_tab
};

static const XREF_WRITER xml_writer = {
	.writer = writeCEventXRefXML
	, .entry_writer = print_xref_entry_xml
};

static const XREF_WRITER  * const xref_writers[] = {
	&json_writer
	, &csv_writer
	, &tab_writer
	, &xml_writer
	, NULL
};

static const char * const supported_formats[] = {
	".json"
	, ".csv"
	, ".tab"
	, ".xml"
	, NULL
};

static const XREF_WRITER * const * pxref_writer = xref_writers;
static const char * const     * psf        = supported_formats;
static char *fname                    = NULL;
static FILE *fout                     = NULL;

pFSMOutputGenerator generateCEventXRefWriter(pFSMOutputGenerator pfsmog)
{
	(void) pfsmog;
	return &ceventxrefwriter;
}

bool check_requested_xref_format(char *fmt)
{
	for (psf = supported_formats, pxref_writer = xref_writers; 
		 *psf;
		 psf++, pxref_writer++
		)
	{
		if (!strcmp(fmt,*psf+1))
		{
			return true;
		}
	}
	return false;
}

static int  initCEventXRefWriter(pFSMOutputGenerator pfsmog, char *name)
{
	(void) pfsmog;

	fname = createFileName(name, (char *)*psf);

	if ((fout = openFile(fname, "w")) == NULL)
	{
		return 1;
	}

	return 0;
}

static void writeCEventXRef(pFSMOutputGenerator pfsmog, pMACHINE_INFO pmi)
{
	(void) pfsmog;

	(*pxref_writer)->writer(pmi);
}

static void closeCEventXRefWriter(pFSMOutputGenerator pfsmog, int good)
{
	(void) pfsmog;

	if (fout)
	{
		fclose(fout);
	}

	if (!good)
	{
		unlink(fname);
	}

	CHECK_AND_FREE(fname);

}

static void writeCEventXRefJSON(pMACHINE_INFO pmi)
{
	unsigned event_count = 0;
	ITERATOR_HELPER ih = {
		.pmi        = pmi
		, .first    = true
		, .counter0 = &event_count
		, .pfn_sub_iterator = print_event_xref_json
	};

	fprintf(fout, "{\n\"event_xrefs\":[\n");

	iterate_list(pmi->event_list
				 , print_event_xref_json
				 , &ih
				 );

	complete_top_level(&ih);
	process_any_sub_machines(&ih);

	fprintf(fout, "]\n}\n");
}

static void writeCEventXRefCSV(pMACHINE_INFO pmi)
{
	unsigned event_count = 0;
	ITERATOR_HELPER ih = {
		.pmi = pmi
		, .counter0 = &event_count
		, .pfn_sub_iterator = print_event_xref_csv
	};

	iterate_list(pmi->event_list
				 , print_event_xref_csv
				 , &ih
				 );

	complete_top_level(&ih);
	process_any_sub_machines(&ih);

	fprintf(fout, "\n");

}

static void writeCEventXRefTab(pMACHINE_INFO pmi)
{
	unsigned event_count = 0;
	ITERATOR_HELPER ih = {
		.pmi = pmi
		, .counter0 = &event_count
		, .pfn_sub_iterator = print_event_xref_tab
	};

	iterate_list(pmi->event_list
				 , print_event_xref_tab
				 , &ih
				 );

	complete_top_level(&ih);
	process_any_sub_machines(&ih);

	fprintf(fout, "\n");

}

static void writeCEventXRefXML(pMACHINE_INFO pmi)
{
	unsigned event_count = 0;
	ITERATOR_HELPER ih = {
		.pmi = pmi
		, .counter0 = &event_count
		, .pfn_sub_iterator = print_event_xref_xml
	};

	fprintf(fout
			, "<event_xrefs machine_name='%s'>\n"
			, pmi->name->name
			);

	iterate_list(pmi->event_list
				 , print_event_xref_xml
				 , &ih
				 );


	complete_top_level(&ih);
	process_any_sub_machines(&ih);

	fprintf(fout, "</event_xrefs>\n");
}

static bool iterate_sub_machines(pLIST_ELEMENT pelem, void *data)
{
	pMACHINE_INFO    pmi = (pMACHINE_INFO)    pelem->mbr;
	pITERATOR_HELPER pih = (pITERATOR_HELPER) data;

	//who called us?
	pMACHINE_INFO    pparent = pih->pmi;

	//set ourself as the current machine
	pih->pmi = pmi;
	iterate_list(pmi->event_list
				 , pih->pfn_sub_iterator
				 , pih
				 );

	if (pmi->machine_list)
	{
		iterate_list(pmi->machine_list
					 , iterate_sub_machines
					 , pih
					 );
	}

	//reset our parent as the current machine
	pih->pmi = pparent;

	return false;
}

static bool print_event_xref_json(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pevent = (pID_INFO) pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;

	print_xref_entry_json(pevent->name, pih);

	return false;
}

static bool print_event_xref_csv(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pevent = (pID_INFO) pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;

	print_xref_entry_csv(pevent->name, pih);

	return false;
}

static bool print_event_xref_tab(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pevent = (pID_INFO) pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;

	print_xref_entry_tab(pevent->name, pih);

	return false;
}

static bool print_event_xref_xml(pLIST_ELEMENT pelem, void *data)
{
	pID_INFO         pevent = (pID_INFO) pelem->mbr;
	pITERATOR_HELPER pih    = (pITERATOR_HELPER) data;

	print_xref_entry_xml(pevent->name, pih);

	return false;
}

static void print_xref_entry_json(char *name, pITERATOR_HELPER pih)
{
	fprintf(fout
			, "%s{ \"index\": \"%u\", \"name\": \""
			, pih->first ? (pih->first = false, "  ") : ", "
			, (*pih->counter0)++
			);

	printNameWithAncestry(name, pih->pmi, fout, "_", alc_lower, ai_include_self);

	fprintf(fout, "\" }\n");
}

static void print_xref_entry_csv(char *name, pITERATOR_HELPER pih)
{
	fprintf(fout
			, "%u,"
			, (*pih->counter0)++
			);

	printNameWithAncestry(name, pih->pmi, fout, "_", alc_lower, ai_include_self);

	fprintf(fout, "\n");
}

static void print_xref_entry_tab(char *name, pITERATOR_HELPER pih)
{
	fprintf(fout
			, "%u\t"
			, (*pih->counter0)++
			);

	printNameWithAncestry(name, pih->pmi, fout, "_", alc_lower, ai_include_self);

	fprintf(fout, "\n");
}

static void print_xref_entry_xml(char *name, pITERATOR_HELPER pih)
{
	fprintf(fout
			, "<event_xref index='%u' name='"
			, (*pih->counter0)++
			);

	printNameWithAncestry(name, pih->pmi, fout, "_", alc_lower, ai_include_self);

	fprintf(fout, "'/>\n");
}

static void complete_top_level(pITERATOR_HELPER pih)
{
	if (
		!(pih->pmi->modFlags & mfActionsReturnStates)
		&& !(pih->pmi->modFlags & mfActionsReturnVoid)
	   )
	{
		(*pxref_writer)->entry_writer("noEvent", pih);
	}

	(*pxref_writer)->entry_writer("numEvents", pih);

}

static void process_any_sub_machines(pITERATOR_HELPER pih)
{

	if (pih->pmi->machine_list)
	{
		iterate_list(pih->pmi->machine_list
					 , iterate_sub_machines
					 , pih
					 );

		(*pxref_writer)->entry_writer("numAllEvents", pih);
	}

}

