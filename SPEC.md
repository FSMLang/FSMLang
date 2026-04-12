# FSMLang Kotlin Generator Specification (`-tk`)

This document specifies the Kotlin code generator backend for FSMLang.

## 1. Overview

Add a new code generator backend selected by the command line option:

- `-tk` : generate Kotlin output.

The Kotlin backend is intended to mirror the semantics of the **single-switch C generator** for **single-level** FSMs.

## 2. Output files

### 2.1 One file per machine
The Kotlin generator MUST emit all generated Kotlin code for a machine into a **single** `.kt` file.

The `.kt` file MUST include:
- `enum class` definitions for State and Event
- machine data type(s)
- event struct + event payload union type(s)
- the FSM class (machine instance)
- hook types for user code (actions, transitions/guards, entry/exit, event data translators)
- the generated dispatch/FSM function and helper functions

### 2.2 Package name

#### 2.2.1 Default package name
If `--kotlin-package` is NOT provided, the generator MUST emit a Kotlin `package` declaration:

- `package io.github.fsmlang.generated.<machine_name_sanitized>`

#### 2.2.2 `--kotlin-package=<pkg>`
Add a new command line option:

- `--kotlin-package=<pkg>`

If provided, the generator MUST emit:

- `package <pkg>`

and MUST NOT auto-append the machine name.

#### 2.2.3 Kotlin-safety sanitization rules
For the default package suffix `<machine_name_sanitized>`, apply:
1) lowercase
2) replace any character not in `[a-z0-9_]` with `_`
3) collapse multiple consecutive `_` into a single `_`
4) trim leading/trailing `_`
5) if empty, use `machine`
6) if starts with a digit, prefix with `m_`
7) if equals a Kotlin keyword, prefix with `m_`

The generator MAY validate `--kotlin-package`:
- it must be dot-separated identifiers
- identifiers should be Kotlin-safe; if not, the generator should either sanitize or error with a clear message (implementation choice must be consistent and documented in `--help`).

## 3. CLI options support

### 3.1 Required options
The Kotlin generator MUST support:
- `-M`
- `-Md` (write dependencies to file, consistent with existing FSMLang behavior)
- `--generate-weak-fns` (see section 6)

### 3.2 Dependency generation (`-M` / `-Md`)
Dependency output MUST include:
- the input `.fsm` file
- any included/imported files (if FSMLang supports includes), consistent with other backends

`-M` writes dependencies to stdout (or consistent with current FSMLang behavior).
`-Md` writes dependencies to the specified file (or consistent with current FSMLang behavior).

## 4. Data model mapping (C-style FSMLang data)

### 4.1 Machine data
FSMLang machine data (C-style struct) MUST be emitted as a Kotlin `data class`.

### 4.2 Event and event payload
Kotlin output MUST model events as:
- an **event enum** for the discriminator (mirrors `*_EVENT_ENUM`), and
- an **event struct** type containing:
  - `event: <EventEnum>`
  - `eventData: <EventDataUnion>`

Event payload unions MUST be modeled as a Kotlin sealed type (e.g., `sealed interface` with one variant per payload type + `None`).

### 4.3 Missing event data MUST be treated as zero-init
If an event requires payload translation but `eventData` is missing or of a different variant, the generator MUST behave as if the payload were **zero-initialized**:
- all scalar fields default to 0/false
- nested structs default-construct recursively
- arrays/strings follow the generator’s chosen representation and must default to “all zeros”

This requirement applies specifically to the call into user-defined event data translation hooks (section 5).

## 5. Event data translation (user-defined hooks)

The Kotlin generator MUST generate a `translateEventData(pfsm, eventStruct)` step that runs before the core FSM dispatch loop.

For each event that carries payload data, the generator MUST emit a user hook:

- `grab_<event>_data(pfsm, payload)`

The hook is **user-defined**. The generator must only:
- declare the hook in a generated hook type, and
- call it from `translateEventData` based on the event enum.

## 6. Weak functions option (`--generate-weak-fns`)

The Kotlin generator MUST honor `--generate-weak-fns`:

- If `--generate-weak-fns=true`:
  - Generated hook types MUST include default implementations (no-op), so the generated Kotlin compiles without user code.

- If `--generate-weak-fns=false`:
  - Generated hook types MUST require user implementations (e.g., interfaces with required methods or abstract classes with abstract methods).

Hook categories include at least:
- action functions
- transition/guard functions
- entry/exit functions
- event data translation functions

## 7. FSM runtime semantics (single-switch parity)

Generated Kotlin MUST mirror the single-switch C generator semantics for single-level machines:

- The FSM core function accepts an event struct (event enum + payload union).
- It performs `translateEventData(pfsm, eventStruct)` before processing events.
- It uses an internal loop:
  - continues while the current event enum is not `noEvent`
- It maintains a read-only “current event enum” field on the machine instance for action error-reporting parity.
- It dispatches based on the pair (event, state) using a combined switch structure (nested `when`, or equivalent).
- Actions return the next event enum (or `noEvent`).
- Transition/guard functions:
  - receive only `(pfsm, eventEnum)` and return next state
- State changes trigger:
  - exit function for previous state
  - entry function for next state
## 8. Testing requirements

Add/update tests so CI and local `make` flows exercise `-tk`, including:
- basic generation smoke/golden output
- `--generate-weak-fns` true vs false (at least compilation coverage)
- `-M` and `-Md` dependency output behavior
- event payload translation hook invocation with zero-init fallback

### 8.1 Kotlin behavior smoke test (full_test144)
In addition to golden/output-consistency tests, the repository MUST include at least one Kotlin **behavior** smoke test based on `test/full_test144` that:

- generates Kotlin using `-tk` from the `test/full_test144/*.fsm` input(s)
- compiles the generated Kotlin on Linux
- runs a JUnit-based test that drives the FSM with the same event sequence as `test/full_test144/test.c` (single instance is sufficient for now)
- asserts at least one observable outcome (e.g., `e1_count == 4` after the sequence)
- includes a test case that dispatches `e1` without payload and verifies the event-data translator receives a zero-initialized payload (and that machine data reflects that)

### 8.2 Integration into existing Make-based test flow
The Kotlin smoke test MUST run as part of the existing test sequence driven by:

- `make Linux.test` from `src/Makefile` (which recursively invokes `make` under `test/`)

I.e., `make Linux.test` MUST cause:
1) Kotlin `.kt` files to be generated (next to the `.fsm`, consistent with generator behavior),
2) the generated `.kt` files to be copied into a Gradle/JUnit harness located under `test/`,
3) Gradle tests to be executed, failing the overall `make Linux.test` target on Kotlin test failure.

Multi-instance parity testing is out of scope for the initial Kotlin backend and may be added later.

### 8.3 Kotlin full_test parity coverage (incremental)

Kotlin behavior smoke tests SHOULD be added incrementally to cover all applicable `test/full_test*` directories, subject to the inclusion/exclusion rules below.

#### Inclusion rules (a full_test directory is eligible only if all are true)
A `test/full_test<N>/` directory is eligible for Kotlin parity smoke testing only if:

1) It contains at least one `.fsm` file that is a **single-level FSM**, as determined by:
   - `fsm -s -M --find-on-sub-machine-depth=0 <name>.fsm`
   - The file is single-level if the command outputs the `.fsm` filename.

2) The directory uses a standalone C driver file containing `int main` (typically `test.c` but not required to be named `test.c`).
   - Directories where `main` is defined in the `.fsm` itself are not eligible.

3) The test is not “documentation-only” output:
   - Directories whose Makefile specifies only `-th` (HTML) and/or `-tp` (PlantUML) and/or `-tr` (reStructuredText) outputs are not eligible.

4) The directory does not use `-i0` in `FSM_FLAGS` (these tests do not create a real machine instance and require different semantics).

5) The directory does not expect weak functions to be generated (tests requiring weak-fn generation are currently out of scope for Kotlin behavior parity).

6) Kotlin tests are single-instance only for now (Kotlin multi-instance parity may be added later).

#### Exclusion rules / known-bad fixtures
The following directories contain intentionally defective `.fsm` inputs and MUST be skipped:
- `test/full_test12`
- `test/full_test39` through `test/full_test43` (inclusive)

#### Package uniqueness requirement for Kotlin parity runs
Because machine and/or output filenames are not globally unique across `full_test*` directories, Kotlin parity smoke tests MUST ensure uniqueness by using `--kotlin-package` when generating Kotlin sources. The package should incorporate the full_test directory name (e.g., `io.github.fsmlang.generated.full_test144`) so that multiple generated machines can coexist in a single Gradle test project.

#### Coverage tracking (tests_skipped.md)
The Kotlin smoke harness MUST maintain a coverage report at:
- `test/kotlin_smoke/tests_skipped.md`

This file MUST list:
- all `test/full_test*` directories that are outside the hard exclusions above, but were skipped by the Kotlin smoke selection rules
- the reason each directory was skipped

It SHOULD separate:
- hard exclusions (intentionally defective fixtures)
- not applicable (documentation-only output)
- out of scope (multi-instance, `-i0`, weak-fns expectation, non-single-level)
- currently unsupported (parity not attainable yet)
