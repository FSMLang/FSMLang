How To Use FSMLang

Philosophy

FSMLang is designed to allow design work in the problem space of finite 
state machines without the encumbrances of any particular implementation 
language. Thus, FSMLang is implemented as a "pre-processor," generating 
code in any desired general programming language to implement the described 
finite state machine. FSMLang allows effort to be focused on the definition 
of events, states, and transitions; indeed, though the action to be taken 
in any particular event/state intersection is declarable (of course), the 
actual definition of that action is treated as a detail which falls outside 
the scope of FSMLang.  Moreover, the mechanisms for collecting or 
determining events are also outside the language scope.  FSMLang creates an 
object or objects in a target programming language which, when inserted 
into the larger program structure will invoke the correct actions and make 
the correct transitions for the events handed it.

(Though I say "any desired general programming language," implementation of 
FSMLang output in languages other than C is an exercise currently left to 
the reader.)

As an example, consider a simple communications protocol which specifies 
that an acknowledgement must be received for each message before another 
may be sent.  The sender of messages in such a protocol must have atleast 
two states: In the first state, which will be called IDLE, the sender has 
sent no message.  In the second, which will be called AWAITING_ACK, the 
sender has sent a message and is awaiting an acknowledgement. The events 
which this automaton will see are SEND_MESSAGE, the request by a client 
entity for a message to be sent, and ACK, the receipt of the acknowledgment 
from the peer automaton.

The valid actions, then, are to send a message if one is requested, and the 
automaton is in the IDLE state,and to simply return to the IDLE state if an 
ACK is received while in the AWAITING_ACK state.  Sending a message 
requires a transition to the AWAITING_ACK state. The receipt of an 
acknowledgement while in the IDLE state represents a protocol error which 
may be safely ignored.  A request to send a message while in the AWAITING_
ACK state, however, should be met with some sort of refusal indication to 
the client entity.

Using FSMLang, this machine can be described this way:

machine simpleCommunicator {

	state	IDLE,
				AWAITING_ACK;

	event	SEND_MESSAGE,
				ACK;

	action	sendMessage[SEND_MESSAGE,IDLE] transition AWAITING_ACK;
	action	refuseMessage[SEND_MESSAGE,AWAITING_ACK];
	action	idle[ACK,(IDLE,AWAITING_ACK)] transition IDLE;

}

(When no transition is specified, the machine remains in the state it was 
in when the event occured.  And, a comma seperated list of events or states 
enclosed within parentheses may be used in place of any single event or 
state designation; in which case, the action specifed will be taken in the 
"cross product" of the two (event and/or state) vectors so described.)

Making the fsm Executable

	The make target is "fsm."  The other targets were either used during
development, or invoke certain regression tests.  You are welcome to
experiment, but the tests expect the bash shell, and some are now out of
date.

  The code compiles "as is" on RH Linux 7.0 and 7.2.  It compiles with
minor tweaks to some libraries and header files on Solaris 8.0 (you'll
need to make the tweaks).  The particular code in this directory has been
adjusted in order to compile under Cygwin's Linux on Win2000 corpus.  No
other systems have been tested.

Command Syntax

	fsm [-tc|h] <filename>

	Where <filename> is required to end in ".fsm"

	The -tc flag is the optional default, and will produce C language code to 
implement the machine(s) described in the input file.  The created files 
will be named <basename>.c and <basename>.h where <basename> is the input 
file name stripped of the .fsm extension.

	If the -th flag is used (only one may be specified, "c" or "h"), then an 
HTML table will be created describing each machine in the input file.  The 
output file will be named <basename>.html where, again, <basename> is the 
input file name stripped of the .fsm extension.  The HTML code allows for a 
cascading style sheet to specify different presentations for different 
elements within the table.  The stylesheet must be named "fsmlang.css" and 
must reside within the same directory as the .html file.  A seperate HTML 
table is made for each machine in the inpute file.

  A known limitation is that paths are not handled.  The program must be
executed from the same directory as the input file is located, and the
output will be placed in that same directory.  This will need to be fixed
before fsm can fit into an industrial strength build system.

As an exercise, and to illustrate concretely all that has been said thus 
far, the reader should examine the output of running fsm on the example 
code given above.  One HTML feature not seen in this example is that blanks 
in the event/state table are given a class name of "nullAction."  These 
cells can then be given different attributes to make them easier to spot in 
a code review.  The example machine above can be made to illustrate this 
feature by substituting "AWAITING_ACK" for the "(IDLE,AWAITING_ACK)" vector 
in the idle action.  A sample style sheet is included in the distribution.

Language Syntax

The example machine above illustrates the major language features and 
syntax.  There are only five keywords: machine, state, event, action, 
transition.  Identifiers follow the C rules, as do declarations, which may 
be comma seperated, as in the state and event declarations above, or set 
seperately delimited by semi-colons as are all of the action declarations.  
State and event declarations may be interspersed, but all must come before 
the first action declaration.  Naming scope is within a given machine; that 
is, one the one hand, all states and events must be declared within a 
machine in order to be used by the machine, and on the other, different 
machines may use the same names for states and events because they will 
apply only within that machine definition.

The action declaration is :

	action action_identifier[event_vector,state_vector];

Or,

	action action_identifier[event_vector,state_vector] transition state_identifier;

Where, event_vector is 

	event_identifier

or,

	(event_identifier_list)

with event_identifier_list being

	event_identifier

or

	event_identifier, event_identifier_list.

The analogous definition holds for state_vectors.

More examples

There are several files with the .fsm extension included in the 
distribution.  All illustrate valid constructions.  Moreover, the make 
target "full_test" will build an executable image from the input files 
parser-test2.fsm and pt2-actions.c.  Finally, the Makefile shows how to add
.fsm targets to a makefile.
