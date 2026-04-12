# Baseline Kotlin full_test parity manifest

This file defines the **baseline** set of `test/full_test*` `.fsm` inputs that must be covered by the Kotlin (`-tk`) behavior smoke tests.

Why this exists:
- Automated eligibility detection previously drifted and pulled in unintended full_tests.
- Machine/file names are not unique across full_tests; Kotlin smoke generation must use `--kotlin-package` (typically derived from the full_test directory name) so all generated sources can coexist in a single Gradle/JUnit project.
- This manifest provides a stable “minimum coverage” floor while still allowing automated discovery to add *new* eligible tests over time (per the rules in `SPEC.md`).

How it is used:
- The Kotlin smoke Makefile logic must **always include** the `.fsm` paths listed below.
- Kotlin smoke must **not** keep lingering test code for full_tests outside this baseline unless/ until intentionally promoted (and/or newly eligible via automated detection, per `SPEC.md`).
- For any full_test directory containing multiple `.fsm` files, only the `.fsm` files listed here are required for the baseline.

## Baseline `.fsm` inputs (one per line)

test/full_test1/parser-test2.fsm
test/full_test2/parser-test4.fsm
test/full_test3/parser-test5.fsm
test/full_test5/parser-test9.fsm
test/full_test6/parser-test11.fsm
test/full_test7/parser-test11.fsm
test/full_test8/parser-test12.fsm
test/full_test9/parser-test12.fsm
test/full_test10/parser-test12.fsm
test/full_test11/parser-test12.fsm
test/full_test13/parser-test5.fsm
test/full_test14/parser-test5.fsm
test/full_test15/parser-test5.fsm
test/full_test16/parser-test12.fsm
test/full_test17/flashdriver.fsm
test/full_test74/test_fsm.fsm
test/full_test75/test_fsm.fsm
test/full_test81/parser-test2.fsm
test/full_test82/parser-test2.fsm
test/full_test83/parser-test2.fsm
test/full_test88/test_fsm.fsm
test/full_test99/test_empty_cells.fsm
test/full_test104/test_fsm.fsm
test/full_test109/test_fsm.fsm
test/full_test111/test_fsm.fsm
test/full_test112/test_fsm.fsm
test/full_test113/test_fsm.fsm
test/full_test135/test_fsm.fsm
test/full_test136/sanitized.fsm
test/full_test137/test_fsm.fsm
test/full_test138/test_fsm.fsm
test/full_test139/issue.fsm
test/full_test142/test_fsm.fsm
test/full_test143/test_fsm.fsm
test/full_test144/test_fsm.fsm
