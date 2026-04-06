# FSMLang – Copilot Cloud Agent Onboarding Guide

## What This Repository Is

FSMLang is a **finite-state-machine pre-processor / compiler** written in C.  
It reads `.fsm` source files and generates implementation code in several target languages:

| Flag | Output |
|------|--------|
| `-tc` | C (array-based, default) |
| `-ts` | C (switch-based) |
| `-te` | C (event-based) |
| `-tss` | C (single-switch) |
| `-th` | HTML documentation |
| `-tp` | PlantUML diagrams |
| `-tr` | reStructuredText documentation |
| `-tpy` | Python (PyTransitions package) |

The executable produced is `fsm`. Source lives in `src/`.

---

## Repository Layout

```
src/               C source for the fsm compiler
  lexer.l          Flex lexer
  parser.y         Bison parser
  fsm_priv.h       Core data structures (MACHINE_INFO, ACTION_INFO, …)
  fsm_utils.c      Shared utilities, option parsing
  fsm_c*.c / .h    C code-generator variants
  fsm_html.c       HTML generator
  fsm_plantuml.c   PlantUML generator
  fsm_rst.c        reStructuredText generator
  fsm_python_transitions.c / .h   Python/PyTransitions generator
  usage.c          Help text and CLI flag documentation

linux/             Output directory for the Linux build
  Makefile         Drives the linux build
  system.mk        Compiler, tools (CC, DIFF, LEX, YACC)
  source.mk        Source file list (SRC=)

test/              Test suites
  Makefile         Top-level: runs all suites (lexer, parser, unit, statistics, html, full_test*)
  test.mk          Generic test rules (do_runtest, TARGET, …)
  variants.mk      C-language test variants (c, s, e, cc, sc, ss)
  weak_variants.mk Variant that generates weak-fn stubs
  full_test*/      Individual C code-generation tests (each has its own Makefile + *.canonical)
  lexer/           Lexer-level tests
  parser/          Parser-level tests
  unit/            Unit tests
  html/            HTML output tests
  statistics/      Statistics output tests
  python/          Python/PyTransitions output tests
    python.mk      Common rules for all python tests
    <name>/        Each sub-dir is one test:
                     <name>.fsm          – input
                     <name>.py.canonical – expected output (gold file)
                     Makefile            – sets FSM_FLAGS, includes python.mk

fsmrules.mk        Make rules for .fsm → .c / .h / .html / .plantuml / .rst / .py
depends.mk         Compiler/linker flag dependency rules
CMakeLists.txt     CMake build (Linux + Windows)
examples/          Real-world .fsm examples
simpleCommunicator.fsm  Canonical simple example
```

---

## Building

### Preferred: Make (Linux)

```bash
# Install prerequisite
sudo apt install -y libfl-dev

# Build the fsm binary into linux/
cd src
make Linux         # → produces linux/fsm

# Or build directly from linux/:
cd linux
CFLAGS="-DLINUX -I../src -I." make fsm
```

### CMake (Linux or Windows)

```bash
cmake -S . -B /tmp/fsmbuild
cmake --build /tmp/fsmbuild --target fsm -j4
# Binary lands at /tmp/fsmbuild/fsm; copy it if running legacy make tests:
cp /tmp/fsmbuild/fsm linux/fsm
```

Windows: replace `cmake -B ./build` with `cmake -B ./build -G "MinGW Makefiles"`, install `winflexbison3` via choco.

---

## Running Tests

> **Linux only.** Tests have only been exercised in the Linux environment; Windows/Cygwin/Mingw builds are compile-only.

All tests require `OUTPUT_DIR` to be set to the **absolute path of the architecture output directory** (e.g., the `linux/` directory). This directory must contain **both** the `fsm` binary **and** `system.mk` (the compiler/tool definitions included by every test Makefile via `include $(OUTPUT_DIR)/system.mk`). The correct value is the directory that `make Linux` targets when run from `src/`, which is `../linux` relative to `src/`, i.e., the repo-root `linux/` directory.

### All C tests (via src/Makefile)

```bash
cd src
make Linux.test        # build + run all tests
make Linux.testonly    # run tests only (no build)
```

### All tests (via test/Makefile)

```bash
cd test
make OUTPUT_DIR=$(pwd)/../linux all
```

### Python tests only

```bash
cd test/python
make OUTPUT_DIR=$(pwd)/../../linux runtest
```

Each python sub-test:
1. Runs `$(FSM) -tpy $(FSM_FLAGS) <name>.fsm` → produces `<name>.py`
2. Diffs `<name>.py` against `<name>.py.canonical`
3. Passes if identical

**Known pre-existing failure:** `ns_simple_example` may fail on some configurations; this is not caused by new changes.

### A single full_test

```bash
cd test/full_test42          # example
make OUTPUT_DIR=$(pwd)/../../linux runtest
```

---

## Key Design Conventions

### FSM Source Language Basics

```
machine MachineName
{
    state  STATE_A, STATE_B;
    event  EVENT_X, EVENT_Y;

    // action[event, state] transition NEXT_STATE
    action doSomething[EVENT_X, STATE_A] transition STATE_B;
    action doOther[EVENT_Y, (STATE_A, STATE_B)];   // cross-product syntax

    // bare transition (no action)
    transition [EVENT_Y, STATE_B] STATE_A;

    // event with data and optional explicit translator
    event distress_call data translator translate_distress_call { int severity; };

    // modifiers
    reentrant machine ...   // thread-safe re-entrance protection
}
```

### Output Generator Structure

Each output generator (`fsm_c.c`, `fsm_python_transitions.c`, …) exports a concrete `generate*Writer` function. The signature follows this pattern:

```c
pFSMOutputGenerator generate<Backend>Writer(pFSMOutputGenerator);

// Examples:
// - `generateCMachineWriter(...)` in `src/fsm_c.c`
// - `generateCSwitchMachineWriter(...)` in `src/fsm_cswitch.c`
// - `generatePyTransitionsWriter(...)` in `src/fsm_python_transitions.c`
// - `generateHTMLMachineWriter(...)` in `src/fsm_html.c`
```

The generator receives a `pMACHINE_INFO pmi` (parsed machine) and writes files via `FILE*` handles.

### Python Generator (`src/fsm_python_transitions.c`)

The Python generator uses a local `PYTD` (PyTransitions Data) struct that carries per-machine state during generation.  Key fields:

- `ptd->uses_event_data` – true when the machine has events with `data { … }` blocks
- `pmi->data` – non-NULL when the machine has a top-level `data { … }` block (emits `self.data_init()` in `__init__`)

When `uses_event_data` is true the generator emits:
- `self.data = {}` in `__init__`
- `send_event=True` and `prepare_event="_fsm_prepare_event"` on `Machine()`
- `_fsm_translators` dict mapping trigger names → translator method names
- `_fsm_get_trigger_name(self, e)` helper (version-tolerant trigger extraction)
- `_fsm_prepare_event(self, e)` dispatcher

When `--generate-weak-fns=false` is **not** set (default), weak-function stubs are generated for all translator names in `_fsm_translators`. Each stub prints its own name and returns `None`.

### Canonical / Gold Files

- C tests: `test/<suite>/test.canonical`
- Python tests: `test/python/<name>/<name>.py.canonical`
- **Never commit** generated artifacts (`.py`, `fsmout`, `*.result`, object files, build dirs).

---

## Important Make Variables

| Variable | Meaning |
|----------|---------|
| `OUTPUT_DIR` | Absolute path to directory containing the `fsm` binary (required by most tests) |
| `FSM_FLAGS` | Extra flags passed to `fsm` (e.g., `--generate-weak-fns=false`) |
| `WEAK_FNS=FALSE` | Passed by some architectures (Cygwin, Mingwsa) to suppress weak stubs |
| `DIFF` | Diff command (default: `diff --strip-trailing-cr --ignore-space-change`) |
| `DIFF_FLAGS` | Extra args forwarded to `DIFF` |

---

## CLI Flags (Selected)

```
fsm [-t[c|s|e|ss|h|p|r|py]] [-o outfile] filename.fsm

-tc          C array-based output (default)
-ts          C switch-based output
-te          C event-based output
-tss         C single-switch output
-th          HTML output
-tp          PlantUML output
-tr          reStructuredText output
-tpy         Python (PyTransitions) output
-c           Compact event/state table (with -tc)
-M           Print list of generated source files (for Makefile GENERATED_SRC)
-Mh          Print list of generated header files
-Md          Print make dependency fragment
-o outfile   Redirect output

--generate-weak-fns=false           Suppress weak-function stubs
--force-generation-of-event-passing-actions
--empty-cell-fn=<name>              Function called for empty action cells
--weak-fns-separate-file=true       Emit weak fns in a separate file
```

---

## Adding a New Python Test

1. Create `test/python/<new_dir>/`
2. Write `<name>.fsm` (the input machine)
3. Build `fsm`, generate `<name>.py`:
   ```bash
   cd test/python/<new_dir>
   ../../../linux/fsm -tpy [flags] <name>.fsm
   ```
4. Inspect the generated `<name>.py`, verify it is correct.
5. Copy it as the canonical: `cp <name>.py <name>.py.canonical`
6. Write `Makefile` (at minimum `include ../python.mk`; add `FSM_FLAGS=...` if needed).
7. Run `make OUTPUT_DIR=<abs_linux_dir> runtest` from `test/python/<new_dir>/` to verify.

---

## Adding a New Full (C) Test

1. Create `test/full_test<N>/` with `.fsm`, `test.c`, `test.canonical`.
2. Makefile typically includes `../variants.mk` or `../weak_variants.mk`.
3. Generate a first canonical by running the test and capturing output:
   ```bash
   make OUTPUT_DIR=<abs_linux_dir> do_runtest   # will fail; output is in test.out
   cp test.out test.canonical
   ```
4. Run `make OUTPUT_DIR=<abs_linux_dir> runtest` to confirm the test passes.

---

## Errors and Workarounds Encountered

- **`libfl-dev` missing**: `make` fails with `cannot find -lfl`. Fix: `sudo apt install -y libfl-dev`.
- **`OUTPUT_DIR` not set**: tests abort with `OUTPUT_DIR must be set to the location of the built executable`. Always pass it as an absolute path.
- **`ns_simple_example` python test failure**: This is a pre-existing known failure unrelated to generator changes; do not attempt to fix it unless specifically tasked.
- **Generated `.py` files are gitignored**: `test/python/*/*.py` (excluding `*.py.canonical`) are excluded via `.gitignore`. Do not attempt to force-add them.
- **CMake build artefacts**: The `build/` directory is gitignored. Use `/tmp/fsmbuild` or similar when building with CMake to avoid cluttering the repo root.
- **Merge target is `master`**: All PRs must target the `master` branch; there is no `main` branch.
