# SPEC: PyTransitions generator — Machine Data support

## Status
- Owner: @9Fingered
- Implementation agent: Copilot
- Repo: FSMLang/FSMLang
- **Base branch (MUST):** `282-support-for-generating-python-based-fsm-code`
- Target files (expected):
  - `src/fsm_python_transitions.c`
  - `src/fsm_python_transitions.h` (only if needed)
  - Tests under `test/python/<new_test_name>/...` (Pattern B canonicals only)

> If the working branch is not based on the base branch above, STOP and fix the branch before making changes.

---

## Problem statement
FSMLang supports **machine data** via `pmi->data`. The PyTransitions generator must incorporate machine data into the generated Python model.

---

## Required behavior (acceptance criteria)

### Gating / backwards compatibility
- If `pmi->data == NULL`, generated Python output must remain unchanged.

### Machine-data initialization hook (MUST)
- If `pmi->data != NULL`, generated Python code MUST call the user-supplied initializer inside `__init__`:

```python
def __init__(self):
    ...
    self.data_init(self)
    ...
```

Notes:
- `data_init` is user-supplied Python, provided via FSMLang **native impl prologue**.
- The generator MUST NOT emit a stub for `data_init` (even if `generate_weak_fns=true`).
- If the user does not provide `data_init`, the resulting runtime exception is expected and acceptable (preferred “Pythonic” behavior).

### Native code placement expectation
- The docs/tests should assume the user defines something like:

```python
def data_init(self):
    ...
```

(or another callable named `data_init` accessible as an attribute on `self`), in the native impl prologue section.

---

## Non-goals (hard constraints)
1. No drive-by cleanups; do not modify unrelated files.
2. No CI/workflow changes.
3. No test artifacts committed (only `*.py.canonical`, `*.fsm`, Makefile).

---

## Tests
Add a focused test under `test/python/<new_test_name>/`:
- `<new_test_name>.fsm` includes a `data { ... }` block so that `pmi->data != NULL`.
- The `.py.canonical` must show `self.data_init(self)` inside `__init__`.
- Include a native prologue in the `.fsm` that defines `data_init` so the generated file is runnable.

