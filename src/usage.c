/**
*  usage.c
*
*    Contains the help/usage function.
*
*    FSMLang (fsm) - A Finite State Machine description language.
*    Copyright (C) 2025  Steven Stanton
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

#include "usage.h"

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

#include "fsm_priv.h"

char help_fmt = '\0';

void usage(void)
{
	char *list_start;
	char *list_end;
	char *item_start;
	char *expl_start;
	char *inner_item_start;
	char *item_end;
	char *list_item_end;
	char *lt;
	char *gt;
	char *dflt;

  switch (help_fmt)
  {
    default:
      list_start       = "\n";
      list_end         = "";
      item_start       = "\t";
      expl_start       = " ";
      inner_item_start = "\t\t";
      item_end         = "\n";
      list_item_end    = "\n";
      lt               = "<";
      gt               = ">";
      dflt             = "*";
      break;

    case 'h':
      list_start       = "\n<ul class=\"syntax\">\n";
      list_end         = "</ul>\n";
      item_start       = "<li>";
      expl_start       = " ";
      inner_item_start = "<li>";
      item_end         = "</li>\n";
      list_item_end    = "</li>\n\t</ul>\n</li>\n";
      lt               = "&lt;";
      gt               = "&gt;";
      dflt             = "*";
      break;

    case 'r':
      list_start       = "";
      list_end         = "\n\n";
      item_start       = "\n\n.. option:: ";
      expl_start       = "\n\n\t";
      inner_item_start = "\t";
      item_end         = "";
      list_item_end    = "\n\n";
      lt               = "<";
      gt               = ">";
      dflt             = "*";
      break;

  }

	fprintf(stdout
			, "%s%sUsage : %s [-tc|s|e|h|p|r] [-o outfile] %sfilename%s.fsm%s"
			, list_start
			, item_start
			, me
      , lt
      , gt
			, list_start
			);

	fprintf(stdout
			, "%s'c'%sgets you c code output based on an event/state table,%s"
			, help_fmt == 'r' ? item_start : inner_item_start
      , expl_start
			, help_fmt == 'r' ? list_item_end : item_end
			);
	fprintf(stdout
			,"%s's'%sgets you c code output with individual state functions using switch constructions,%s"
			, help_fmt == 'r' ? item_start : inner_item_start
      , expl_start
			, help_fmt == 'r' ? list_item_end : item_end
			);
	fprintf(stdout
			,"%s'e'%sgets you c code output with a table of functions for each event using switch constructions,%s"
			, help_fmt == 'r' ? item_start : inner_item_start
      , expl_start
			, help_fmt == 'r' ? list_item_end : item_end
			);
	fprintf(stdout
			,"%s'h'%sgets you html output%s"
			, help_fmt == 'r' ? item_start : inner_item_start
      , expl_start
			, help_fmt == 'r' ? list_item_end : item_end
			);
	fprintf(stdout
			,"%s'p'%sgets you PlantUML output%s"
			, help_fmt == 'r' ? item_start : inner_item_start
      , expl_start
			, help_fmt == 'r' ? list_item_end : item_end
			);

	fprintf(stdout
			,"%s'r'%sgets you reStructuredText output%s"
			, help_fmt == 'r' ? item_start : inner_item_start
      , expl_start
			, list_item_end
			);

	fprintf(stdout
			,"%s-i0%sinhibits the creation of a machine instance%s"
			, item_start
      , expl_start
			, list_start
			);
	fprintf(stdout
			,"%sany other argument to 'i' allows the creation of an instance;%s"
			, inner_item_start
			, item_end
			);
	fprintf(stdout
			,"%sthis is the default%s"
			, inner_item_start
			, list_item_end
			);
	fprintf(stdout
			,"%s-c%swill create a more compact event/state table when -tc is used%s"
			, item_start
      , expl_start
			, list_start
			);
	fprintf(stdout
			,"%swith machines having actions which return states%s"
			, inner_item_start
			, list_item_end
			);
	fprintf(stdout
			,"%s-s%sprints some useful statistics and exits%s"
			, item_start
      , expl_start
			, item_end
			);
	fprintf(stdout
			,"%s-o %soutfile%s%swill use %soutfile%s as the filename for the top-level machine output.%s"
			, item_start
      , lt
      , gt
      , expl_start
      , lt
      , gt
			, list_start
			);
	fprintf(stdout
			,"%sAny sub-machines will be put into files based on the sub-machine names.%s"
			, inner_item_start
			, list_item_end
			);
	fprintf(stdout
			,"%s--generate-weak-fns=false%ssuppresses the generation of weak function stubs.%s"
			, item_start
      , expl_start
			, item_end
			);
	fprintf(stdout
			,"%s--short-user-fn-names=true%scauses user functions (such as action functions to use only the%s"
			, item_start
      , expl_start
			, item_end
			);

	fprintf(stdout
     , "%smachine name when the sub-machine depth is 1).%s"
		 , inner_item_start
		 , item_end
		 );

	fprintf(stdout
			,"%s--force-generation-of-event-passing-actions%sforces the generation of actions which pass events%s"
			, item_start
      , expl_start
			, list_start
			);
 fprintf(stdout
		 ,"%swhen weak function generation is disabled.%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sThe generated functions are not weak.%s"
		 , inner_item_start
		 , list_item_end
		 );
	fprintf(stdout
			,"%s--core-logging-only=true%ssuppresses the generation of debug log messages in all but the core FSM function.%s"
			, item_start
      , expl_start
			, item_end
			);
 fprintf(stdout
		 ,"%s--generate-run-function%s=true|false%s%sthis option is deprecated.  The run function is always generated;%s"
		 , item_start
     , lt
     , gt
      , expl_start
		 , list_start
		 );
	fprintf(stdout
			,"%sno RUN_STATE_MACHINE macro is provided.%s"
			, inner_item_start
			, list_item_end
			);
	fprintf(stdout
			,"%s--include-svg-img%s=%strue|false%s%sadds %simg/%s tag referencing %sfilename%s.svg to include an image at%s"
			, item_start
      , lt
      , dflt
      , gt
      , expl_start
      , lt
      , gt
      , lt
      , gt
			, list_start
			);
  fprintf(stdout
		  ,"%sthe top of the web page.%s"
		  , inner_item_start
		  , list_item_end
		  );
	fprintf(stdout
			,"%s--css-content-internal=true%sputs the CSS directly into the html.%s"
			, item_start
      , expl_start
			, item_end
			);
	fprintf(stdout
			,"%s--css-content-filename=%sfilename%s%suses the named file for the css citation, or%s"
			, item_start
      , lt
      , gt
      , expl_start
			, list_start
			);
	fprintf(stdout
			,"%sfor the content copy.%s"
			, inner_item_start
			, list_item_end
			);
 fprintf(stdout
		 ,"%s--add-plantuml-title=%s%strue|false%s%sadds the machine name as a title to the plantuml.%s"
		 , item_start
      , lt
      , dflt
      , gt
      , expl_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%s--add-plantuml-legend=%s%scenter|left|right|top|%sbottm%s%sadds a legend to the plantuml.%s"
		 , item_start
      , lt
      , dflt
      , dflt
      , gt
      , expl_start
		 , list_start
		 );
 fprintf(stdout
		 ,"%sCenter, bottom are the defaults.  Horizontal and vertial parameters can be added in a quoted string.%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sCenter is a horizontal parameter.%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sBy default, event, state, and action lists are%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sincluded in the legend, and event descriptions are removed%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sfrom the body of the diagram.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--exclude-states-from-plantuml-legend=%s%strue|false%s%sexcludes state information from the plantuml legend.%s"
		 , item_start
      , lt
      , dflt
      , gt
      , expl_start
		 , list_start
		 );
 fprintf(stdout
		 ,"%sWhen excluded from legend, state comments are included in the diagram body.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--exclude-events-from-plantuml-legend=%s%strue|false%s%sexcludes event information from the plantuml legend.%s"
		 , item_start
      , lt
      , dflt
      , gt
      , expl_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%s--exclude-actions-from-plantuml-legend=%s%strue|false%s%sexcludes action information from the plantuml legend.%s"
		 , item_start
      , lt
      , dflt
      , gt
      , expl_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%s--convenience-macros-in-public-header[=%s%strue|false%s]%sincludes convenience macros%s"
		 , item_start
      , lt
      , dflt
      , gt
      , expl_start
		 , list_start
		 );
 fprintf(stdout
		 ,"%s(THIS, UFMN, e.g.) in the public header of the top-level machine;%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sotherwise, they are placed in the private header.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--add-machine-name%sadds the machine name when using the --short-debug-names option%s"
		 , item_start
      , expl_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%s--add-event-cross-reference%s=true|%sfalse%s%sadds a cross-reference list as a comment block%s"
		 , item_start
      , lt
      , dflt
      , gt
      , expl_start
		 , list_start
		 );
 fprintf(stdout
		 ,"%sin front of the machine event enumeration. Omitting the optional argument is equivalent%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sto specifying \"true\"%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--event-cross-ref-only%s=%strue|false%s%screates a cross-reference list as a separate file.%s"
		 , item_start
      , lt
      , dflt
      , gt
      , expl_start
		 , list_start
		 );
 fprintf(stdout
		 ,"%sWhen the format is not specified by --event-cross-ref-format, json is provided.%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sThe file created is %sfilename%s.[json|csv|tab|xml]%s"
		 , inner_item_start
      , lt
      , gt
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--event-cross-ref-format=[json|csv|tab|xml]%sspecifies the output format for --event-cross-ref-only.%s"
		 , item_start
      , expl_start
		 , list_start
		 );
 fprintf(stdout
		 ,"%sSpecifying this option obviates --event-cross-ref-only.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--add-plantuml-prefix-string=%stext%s%swill add the specified text to the plantuml output before%s"
		 , item_start
      , lt
      , gt
      , expl_start
		 , list_start
		 );
 fprintf(stdout
		 ,"%sany generated output.  This option can be specified multiple times; all text will be%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sadded in the order given%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sfor the content copy.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--add-plantuml-prefix-file=%stext%s%swill add the text in the specified file%s"
		 , item_start
      , lt
      , gt
      , expl_start
		 , list_start
		 );
 fprintf(stdout
		 ,"%sto the plantuml output before any generated output.%s"
		 , inner_item_start
		 , item_end
		 );
	fprintf(stdout
			,"%sThis option can be specified multiple times; all text will be%s"
			, inner_item_start
			, item_end
			);
 fprintf(stdout
		 ,"%sadded in the order given%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sfor the content copy.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s-M%sprints the file name(s) of the source files that would have been created to stdout.%s"
		 , item_start
      , expl_start
		 , list_start
		 );
 fprintf(stdout
		 ,"%sThis is useful in Makefiles for getting the list of files%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sthat will be generated %s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%s(e.g. GENERATED_SRC=$(shell $(FSM) -M -tc $(FSM_SRC))).%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sThis option must preceed the -t option.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s-Mh%sprints the file name(s) of the headers that would have been created to stdout.%s"
		 , item_start
      , expl_start
		 , list_start
		 );
 fprintf(stdout
		 ,"%sThis is useful in Makefiles for getting the list of files%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sthat will be generated %s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%s(e.g. GENERATED_HDRS=$(shell $(FSM) -M -tc $(FSM_SRC))).%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sThis option must preceed the -t option.  And, only tc or ts are applicable.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s-Md%sprint a line suitable for inclusion in a Makefile giving the recipe for%s"
		 , item_start
      , expl_start
		 , list_start
		 );
 fprintf(stdout
		 ,"%screating dependent files.%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sThis option must preceed the -t option.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--add-profiling-macros%s=true|%sfalse%s%sadds profiling macros at the beginning%s"
		 , item_start
      , lt
      , dflt
      , gt
      , expl_start
		 , list_start
		 );
 fprintf(stdout
		 ,"%sand end of the FSM function, and before and after invocation of action functions.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--profile-sub-fsms%s=true|%sfalse%s%sadds profiling macros at the beginning%s"
		 , item_start
      , lt
      , dflt
      , gt
      , expl_start
		 , list_start
		 );
 fprintf(stdout
		 ,"%sand end of the FSM function in sub-machines.  Profiling macros%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%smust also be enabled.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--empty-cell-fn=%sname%s%sdesignates a function to be called when%s"
		 , item_start
      , lt
      , gt
      , expl_start
		 , list_start
		 );
 fprintf(stdout
		 ,"%san event/state cell is empty.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s--inhibiting-states-share-events%s=%strue|false%s%sWhen true, events are shared to%s"
		 , item_start
		 , lt
		 , dflt
		 , gt
		 , expl_start
		 , list_start
		 );
 fprintf(stdout
		 ,"%ssub-machines even in states which inhibit them.  The default is to not share.%s"
		 , inner_item_start
		 , item_end
		 );
 fprintf(stdout
		 ,"%sThis option allows sharing behavior of version 1.45.1 and before to be preserved.%s"
		 , inner_item_start
		 , list_item_end
		 );
 fprintf(stdout
		 ,"%s-v%sprints the version and exits%s%s"
		 , item_start
      , expl_start
		 , item_end
		 , list_end
		 );
	
}
