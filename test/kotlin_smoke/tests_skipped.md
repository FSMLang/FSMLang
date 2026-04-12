# Kotlin Smoke Tests — Skipped / Excluded Directories

This document explains which `full_test*` directories are absent from the
Kotlin smoke-test suite and why.

---

## 1. Hard exclusions — defective fixtures

These directories contain `.fsm` files that are intentionally malformed (they
are parser-error or semantic-error regression tests).  The `fsm -tk` generator
fails on them by design, so no `.kt` is produced.

| Directory | Reason |
|-----------|--------|
| `full_test12` | Intentionally malformed parser-test fixture |
| `full_test39` | Intentionally malformed / error-only fixture |
| `full_test40` | Intentionally malformed / error-only fixture |
| `full_test41` | Intentionally malformed / error-only fixture |
| `full_test42` | Intentionally malformed / error-only fixture |
| `full_test43` | Intentionally malformed / error-only fixture |

---

## 2. Does not exist

These directory numbers are absent from the repository entirely.

| Directory | Note |
|-----------|------|
| `full_test4` | Does not exist in repo |
| `full_test80` | Does not exist in repo |
| `full_test90` | Does not exist in repo |

---

## 3. Kotlin generator limitation — sub-machine event data

| Directory | Reason |
|-----------|--------|
| `full_test44` | `fsm -tk` fails with "event user data not allowed in sub-machines" |

---

## 4. No C main / library-only fixtures (out of scope)

These directories produce only library code (no `main`/driver), so there is
no C-level execution model to mirror in JUnit smoke tests.

| Directories |
|-------------|
| `full_test50`, `full_test51`, `full_test52`, `full_test53`, `full_test54` |
| `full_test56`, `full_test57`, `full_test58`, `full_test59`, `full_test60` |
| `full_test61`, `full_test62`, `full_test63`, `full_test64`, `full_test65` |
| `full_test66`, `full_test67`, `full_test68`, `full_test71` |
| `full_test104`, `full_test105`, `full_test106`, `full_test107`, `full_test108` |
| `full_test132`, `full_test133`, `full_test134`, `full_test148` |

---

## 5. Weak-functions variant (out of scope)

These directories exercise the `--generate-weak-fns` / `WEAK_FNS=FALSE` path
which produces a separate weak-stub file.  Wiring a second source file into
the Gradle project is out of scope for the current smoke-test infrastructure.

| Directory | Reason |
|-----------|--------|
| `full_test140` | Weak-functions variant |
| `full_test141` | Weak-functions variant |

---

## 6. Duplicate FSM content (covered by first in group)

These directories contain `.fsm` files whose generated Kotlin is identical in
structure to another directory already tested.  The Makefile still generates
the `.kt` for completeness; only the JUnit test is omitted to avoid redundancy.

| Skipped | Covered by |
|---------|-----------|
| `full_test7` | `full_test6` |
| `full_test11` | `full_test10` |
| `full_test15` | `full_test14` |
| `full_test20` | `full_test19` |
| `full_test22`, `full_test23`, `full_test24`, `full_test25`, `full_test86` | `full_test21` |
| `full_test33` | `full_test31` |
| `full_test36` | `full_test35` |
| `full_test49`, `full_test55` | `full_test48` |
| `full_test72`, `full_test87` | `full_test70` |
| `full_test84`, `full_test85` | `full_test69` |
| `full_test102`, `full_test103` | `full_test29` |
| `full_test110` | `full_test76` |
| `full_test124` | `full_test119` |
| `full_test145` | `full_test121` |
| `full_test146` | `full_test122` |
| `full_test147` | `full_test123` |

---

## 7. full_test144 — full behavioral parity (separate test class)

`full_test144` is the primary Kotlin regression target.  It has full
behavioral assertions (mirroring the C driver in `test.c` and action stubs in
`test_fsm-actions.c`) implemented in `TestFsmSmokeTest.kt`.  It is therefore
excluded from `AllFsmSmokeTests.kt` to avoid duplication.

---

## 8. Remaining smoke tests — compile + dispatch only

All tests in `AllFsmSmokeTests.kt` verify:

1. The generated `.kt` compiles successfully under Kotlin/JVM.
2. Constructing the FSM with default (no-op) hooks does not throw.
3. Dispatching one event does not throw.

Full behavioral parity with the corresponding C tests (state transitions,
action call counts, guard logic) is **not yet implemented** for these
directories.  Contributions welcome.
