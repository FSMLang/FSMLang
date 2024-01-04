#!/usr/bin/bash

# Create test.c by moving main to it.
# Don't forget to pull all functions which must not
#   molested by the subsequent operations.
vi `grep -l main *.c`

# add the new file to the repo
git add test.c

# the RUN_STATE_MACHINE macro is no longer with us
for f in `grep -l RUN_STATE_MACHINE *.c`; do
	sed -f ../alter_RUN_macro.sedscr < $f > $f.sedout;
	mv $f.sedout $f;
done

# we use THIS to avoid long strings
for f in `ls *actions.c`; do
	sed -f ../alter_action_files.sedscr < $f > $f.sedout;
	mv $f.sedout $f;
done

# also use THIS here
for f in `grep -l top_level *.c`; do
	sed -f ../add_tl_THIS.sedscr < $f > $f.sedout;
	mv $f.sedout $f;
done

# fix up the translate_e7_data function
if [ -f "sm1-actions.c" ]; then

	awk -f ../fix_translate_e7_data_function.awkscr < sm1-actions.c > sm1-actions.c.awkout;
	mv sm1-actions.c.awkout sm1-actions.c;

fi

# the machineTransitionFn function has a DBG_PRINTF which has now been munged by the foregoing
for f in `grep -l machineTransitionFn *actions.c`; do
	awk -f ../fix_machineTransitionFn.awkscr < $f > $f.awkout;
	mv $f.awkout $f
done

# the print_machine_data function also has a DBG_PRINTF which has been munged
for f in `grep -l print_machine_data *actions.c`; do
	awk -f ../fix_print_machine_data.awkscr < $f > $f.awkout;
	mv $f.awkout $f
done

# the eShared_dt function also has a DBG_PRINTF which has been munged
for f in `grep -l eShared_dt *actions.c`; do
	awk -f ../fix_eShared_dt.awkscr < $f > $f.awkout;
	mv $f.awkout $f
done

# the newMachine_baz function also has a DBG_PRINTF which has been munged
for f in `grep -l newMachine_baz *actions.c`; do
	awk -f ../fix_newMachine_baz.awkscr < $f > $f.awkout;
	mv $f.awkout $f
done

# if external event values are used, cast them to the machine's event enum
if grep ext[1-4] test.c; then
	sed -e 's/\(ext[1-4]\)/ (NEW_MACHINE_EVENT) \1/' < test.c > test.c.sedout;
	mv test.c.sedout test.c;
fi

# add test.c to the SRC macro
awk -f ../add_test_c_to_Makefile.awkscr < Makefile > Makefile.awkout;
mv Makefile.awkout Makefile

