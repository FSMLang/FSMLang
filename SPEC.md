# SPEC: PyTransitions generator — FSMLang event-data translation via `prepare_event`

## Status / scope guardrails (MUST)
- Repo: `FSMLang/FSMLang`
- **Base branch (MUST):** `297-modify-pytransitions-event-data-support-to-ensure-data-are-translated-exactly-once-for-the-occurance-of-an-event`
- Generator files:
  - `src/fsm_python_transitions.c`
  - `src/fsm_python_transitions.h` (only if necessary)
- Tests:
  - Add/modify under `test/python/<new_test_dir>/**`
  - Expected outputs are `*.py.canonical` only

> If the working branch is not based on `297-modify-pytransitions-event-data-support-to-ensure-data-are-translated-exactly-once-for-the-occurance-of-an-event` STOP and ask for clarification.

### Hard non-goals
1. No drive-by cleanups/refactors/formatting-only changes.
2. No CI/workflow edits.
3. Do not commit generated artifacts (`*.py`, `fsmout`, `*.result`, binaries, build dirs).
4. Do not change unrelated Makefiles unless required by the new test directory.

---

## Problem statement
FSMLang supports *event data* associated with triggers. In PyTransitions, when multiple conditional transitions share a trigger, transition-level `prepare` callbacks may execute multiple times during a single trigger evaluation. We must instead translate event data **once per trigger invocation**, regardless of how many candidate transitions are evaluated.

Therefore: generate a model-level `prepare_event` dispatcher and wire it into the `Machine` so translation happens at event scope, not transition scope.

---

## High-level design (MUST)
### 1) Use `Machine(..., send_event=True, prepare_event=...)`
When the FSM uses FSMLang event data (i.e., `pmi->data_block_count > 0` / existing event-data gating), the generated Python must:
- set `send_event=True`, and
- set `prepare_event="_fsm_prepare_event"` (string callback name) on the Machine.

### 2) Generate a generic dispatcher implementation (MUST)
FSMLang must generate a Python method on the model:

- `_fsm_prepare_event(self, e)`

This method:
- extracts the trigger name from the event argument `e` in a **version-tolerant** way (see below),
- resolves which translator function to call for that trigger,
- **calls that translator** and returns `True`.

### 3) Translators mutate `self.data` and return `None` (MUST)
Each per-trigger translator function (whether explicitly named by the FSM or using the default name) is expected to:
- mutate `self.data` (e.g., `self.data[trigger] = dict(e.kwargs)` or a richer structure),
- and return nothing (`None`).

The dispatcher must **not** store any return value from translator calls.

### 4) Missing translator MUST raise at runtime (Option C) (MUST)
If a translator function is missing for a trigger that requires translation, the generated code must raise at runtime (Pythonic fail-fast). Concretely:
- the dispatcher must call the resolved translator using `getattr(self, name)(e)` **without** suppressing `AttributeError`,
- no `hasattr`, no try/except, no fallback translation.

---

## Detailed requirements (acceptance criteria)

### A) Trigger name extraction must be robust across `transitions` versions (MUST)
FSMLang must generate a helper method:

- `_fsm_get_trigger_name(self, e) -> Optional[str]`

It should attempt common attribute layouts using `getattr` (do not assume a pinned transitions version).

Required extraction attempts, in this general order:
1. `e.event.name` (common: `e.event` is an Event with `.name`)
2. `e.event.trigger` (alternate naming)
3. `e.trigger` / `e.name` / `e.event_name` (possible direct fields)
4. `e.transition.trigger` (last-resort)

If no trigger name can be found, return `None`.

### B) Dispatcher behavior (MUST)
`_fsm_prepare_event(self, e)` must:
1. call `trigger = self._fsm_get_trigger_name(e)`
2. if `trigger is None`, set `trigger = "__unknown_trigger__"` (stable debugging key; avoids crashing solely due to version shape)
3. resolve translator name using mapping (below)
4. call the translator with the event object: `getattr(self, translator_name)(e)`
5. return `True`

### C) Translator name resolution (MUST)
FSMLang events may specify an explicit translator name; otherwise a default name is used.

Generate a mapping dict on the model:

- `_fsm_translators: Dict[str, str]`

Keys:
- exact trigger strings as used by the machine (e.g., `"distress_call"`)

Values:
- the method name to call (string), either:
  - explicit translator name from the FSM (e.g., `"handle_distress"`), OR
  - default translator name `"translate_<trigger>"`

Then dispatcher resolves:
- `translator_name = self._fsm_translators.get(trigger, "translate_" + trigger)`

Important:
- Even though a fallback is present, **missing translator must still raise**; the fallback only selects a name, it does not guard the call.

### D) `self.data` container (MUST)
When event data is enabled, the generated model must have:

- `self.data = {}` created in `__init__`

Translators mutate this dict.

### E) Backwards compatibility (MUST)
If the FSM has **no event data** (`pmi->data_block_count == 0` or equivalent existing gate), then:
- do not emit `_fsm_prepare_event`, `_fsm_get_trigger_name`, `_fsm_translators`, or `self.data`,
- do not set `send_event=True`,
- do not set `prepare_event=...`.

Output should remain unchanged (byte-for-byte) for machines without event data.

---

## Implementation constraints (to prevent noisy PRs)
- Only change generator code and the new/updated python canonicals for the targeted tests.
- Do not touch unrelated tests, make infrastructure, or tracked build artifacts.
- Keep changes minimal and localized.

---

## Test requirements
Add at least one new test directory: `test/python/<new_test_dir>/`

The test FSM must:
- define **multiple transitions with the same trigger** and **different conditions**, so that PyTransitions may evaluate multiple candidates,
- declare FSMLang event data for that trigger,
- ensure generated Python uses `prepare_event="_fsm_prepare_event"` and contains the dispatcher/helper/mapping.

Golden expectations:
- `<new_test_dir>/<name>.py.canonical` must contain:
  - `send_event=True`
  - `prepare_event="_fsm_prepare_event"`
  - definitions of `_fsm_get_trigger_name`, `_fsm_prepare_event`, and `_fsm_translators`
  - a per-trigger translator method (default and/or explicit) whose body mutates `self.data` and returns nothing

---

## Generated Python sketch (normative shape, names MUST match)
The generated output must be equivalent in behavior to:

```python
def _fsm_get_trigger_name(self, e):
    ev = getattr(e, "event", None)
    if ev is not None:
        name = getattr(ev, "name", None)
        if name:
            return name
        name = getattr(ev, "trigger", None)
        if name:
            return name

    for attr in ("trigger", "name", "event_name"):
        name = getattr(e, attr, None)
        if name:
            return name

    tr = getattr(e, "transition", None)
    if tr is not None:
        name = getattr(tr, "trigger", None)
        if name:
            return name

    return None


_fsm_translators = {
    # trigger -> translator method name
    "distress_call": "translate_distress_call",
    "complete_mission": "handle_mission",  # example explicit translator name
}


def _fsm_prepare_event(self, e):
    trigger = self._fsm_get_trigger_name(e)
    if trigger is None:
        trigger = "__unknown_trigger__"

    translator_name = self._fsm_translators.get(trigger, "translate_" + trigger)
    # MUST raise if translator is missing:
    getattr(self, translator_name)(e)

    return True
```

Per-trigger translator methods (whether generated stubs or user-provided) must mutate `self.data` and return nothing.

---

## Pre-flight checklist (MUST be restated by agent before coding)
1. Confirm PR number + URL.
2. Confirm base branch is `297-modify-pytransitions-event-data-support-to-ensure-data-are-translated-exactly-once-for-the-occurance-of-an-event`.
3. Confirm head branch name you will push commits to.
4. Confirm you will NOT create a new branch or new PR.
5. Confirm only allowed files/dirs will change.

If any item is not true: STOP and ask for clarification.
