# SPEC: PyTransitions generator — FSMLang action-matrix semantics + event-data translation via `prepare_event`

## Status / scope guardrails (MUST)
- Repo: `FSMLang/FSMLang`

- **Merge target (base branch):** `master`
  - This PR must target `master`.

- **Working branch (head branch to push commits to):**
  - `301-refine-pytransitions-action-handling`
  - The agent MUST push commits to this branch.
  - The agent MUST NOT create a new branch and MUST NOT open a new PR.

- Generator files:
  - `src/fsm_python_transitions.c`
  - `src/fsm_python_transitions.h` (only if necessary)

- Tests:
  - Add/modify under `test/python/<new_test_dir>/**`
  - Expected outputs are `*.py.canonical` only

### Hard non-goals
1. No drive-by cleanups/refactors/formatting-only changes.
2. No CI/workflow edits.
3. Do not commit generated artifacts (`*.py`, `fsmout`, `*.result`, binaries, build dirs).
4. Do not change unrelated Makefiles unless required by the new test directory.

---

## Problem statement
FSMLang supports an “action matrix” semantic such that, for a given `(state, trigger)` entry, an action always runs when the trigger occurs in that state, and only then is the transition (if any) considered.

Example (FSMLang conceptual form):

- `action doSomething[event1, (state1, state2)] transition transitionGuardFn;`

Semantics:
- When `event1` occurs and the machine is in `state1` or `state2`, the machine **always** invokes `doSomething` **exactly once** for that event occurrence.
- Then the machine evaluates `transitionGuardFn` to determine the next state (or no transition).

PyTransitions’ `before`/`after` callbacks are transition-centric (they only run when that transition is taken), and transition-level `prepare` may run multiple times per trigger when multiple conditional transitions are evaluated.

Therefore: generate a model-level `prepare_event` dispatcher and wire it into the `Machine` so that:
- event data translation happens exactly once per trigger occurrence, and
- action-matrix “always-run” actions happen exactly once per trigger occurrence,
independent of which transition is ultimately taken (or if no transition occurs).

---

## High-level design (MUST)

### 1) Use `Machine(..., send_event=True, prepare_event=...)` when needed
When the FSM uses FSMLang event data and/or action-matrix semantics requiring “always-run action”, the generated Python must:
- set `send_event=True`, and
- set `prepare_event="_fsm_prepare_event"` (string callback name) on the Machine.

If neither feature is used, output must remain unchanged (see Backwards compatibility).

### 2) Generate a generic `prepare_event` dispatcher (MUST)
FSMLang must generate a Python method on the model:

- `_fsm_prepare_event(self, e)`

This method:
- extracts the trigger name from the event argument `e` in a **version-tolerant** way,
- performs event-data translation **only for triggers that have a data block**,
- performs “always-run” action dispatch for applicable `(state, trigger)` pairs,
- returns `True`.

### 3) Event-data translators mutate `self.data` and return `None` (MUST)
Each per-trigger translator function is expected to:
- mutate `self.data`,
- and return nothing (`None`).

The dispatcher must **not** store any return value from translator calls.

### 4) Action functions run via `prepare_event`, not transition `before` (MUST)
For FSMLang action-matrix semantics, generated transitions MUST NOT attach the action function as a transition-level `before` callback (because it would not run when no transition occurs, and it would become transition-centric).

Instead, action functions MUST be invoked from `_fsm_prepare_event` based on `(current_state, trigger)`.

---

## Detailed requirements (acceptance criteria)

### A) Trigger name extraction must be robust across `transitions` versions (MUST)
FSMLang must generate a helper method:

- `_fsm_get_trigger_name(self, e) -> Optional[str]`

It must attempt common attribute layouts using `getattr` (do not assume a pinned transitions version).

Required extraction attempts, in this general order:
1. `e.event.name`
2. `e.event.trigger`
3. `e.trigger` / `e.name` / `e.event_name`
4. `e.transition.trigger`

If no trigger name can be found, return `None`.

### B) Dispatcher behavior (MUST)
`_fsm_prepare_event(self, e)` must:
1. call `trigger = self._fsm_get_trigger_name(e)`
2. if `trigger is None`, set `trigger = "__unknown_trigger__"` (stable debugging key; avoids crashing solely due to version shape)

Then perform the following in order:

3. **Event-data translation** (ONLY if this trigger has a data block):
   - `translator_name = self._fsm_translators.get(trigger)`
   - if `translator_name` is not `None`, call `getattr(self, translator_name)(e)`

4. **Action-matrix action dispatch**:
   - `action_name = self._fsm_actions.get((self.state, trigger))`
   - if `action_name` is not `None`, call `getattr(self, action_name)(e)`

5. return `True`

Notes:
- The only guards permitted are:
  - guarding on whether a trigger is present in `_fsm_translators` (meaning it has a data block)
  - guarding on whether a `(state, trigger)` key exists in `_fsm_actions`
- The dispatcher MUST NOT guard against missing method attributes once a name has been selected:
  - do not use `hasattr` for existence
  - do not suppress `AttributeError` with try/except
  - do not provide fallback translation/action behavior

### C) Event-data translator name resolution (MUST)
FSMLang events with a data block may specify an explicit translator name; otherwise a default name is used.

Generate a mapping dict on the model:

- `_fsm_translators: Dict[str, str]`

Keys:
- trigger strings for events that have a data block (e.g., `"event1"`)

Values:
- the method name to call (string), either:
  - explicit translator name from the FSM, OR
  - default translator name `translate_<trigger>`

Important:
- `_fsm_translators` MUST include **only** triggers that have a data block.
- When an event has **no data block**, `_fsm_prepare_event` MUST NOT call any translator for that trigger.

### D) Missing translator failure modes (Option C, conditional) (MUST)

#### When `generate_weak_fns` is OFF
If a translator function is required (trigger is in `_fsm_translators`) but is missing from the model, the generated code must raise at runtime (Pythonic fail-fast). Concretely:
- `_fsm_prepare_event` must call the translator using `getattr(self, translator_name)(e)` with no suppression of `AttributeError`.

#### When `generate_weak_fns` is ON
FSMLang MUST generate stubs for translator methods referenced by `_fsm_translators` (see “Weak-function stubs”).

### E) Weak-function stubs (MUST when `generate_weak_fns` is ON)

When `generate_weak_fns` is enabled (configured by the python test Makefile):

#### E1) Translator stubs
- FSMLang MUST generate stub methods for **all** translator method names referenced by `_fsm_translators`.
- Each translator stub MUST:
  1) print its own function name to stdout, and
  2) do nothing else (no `self.data` mutation; implicit `None` return).

Required stub shape:

```python
def translate_event1(self, e):
    print("translate_event1")
    # intentionally empty
```

#### E2) Action stubs (if actions are weak-generated elsewhere)
If the Python generator’s weak-function mode also applies to action callbacks (implementation-specific), any generated weak stubs for action functions must likewise:
- print their function name to stdout, and
- do nothing else.

(If actions are not weak-generated by the existing generator, do not introduce new weak-generation behavior beyond translator stubs unless required by existing conventions.)

### F) Action-matrix mapping for “always-run” actions (MUST)
Generate a mapping on the model:

- `_fsm_actions: Dict[Tuple[str, str], str]`

Keys:
- `(state_name, trigger_name)` tuples

Values:
- the method name to call for the action (string)

This mapping must include an entry for every `(state, trigger)` pair for which FSMLang semantics require an always-run action.

### G) `self.data` container (MUST)
When event data exists (i.e., at least one trigger has a data block), the generated model must have:

- `self.data = {}` created in `__init__`

Translators mutate this dict.

If there is no event data in the FSM, do not generate `self.data`.

### H) Backwards compatibility (MUST)
If the FSM uses neither:
- event data, nor
- action-matrix always-run actions requiring `prepare_event`,
then output must remain unchanged (byte-for-byte):
- do not set `send_event=True`
- do not set `prepare_event=...`
- do not emit `_fsm_prepare_event`, `_fsm_get_trigger_name`, `_fsm_translators`, `_fsm_actions`, or `self.data`

---

## Transition table generation requirements (MUST)

### 1) Guard/choice-based transitions
For FSMLang guard functions which return a discrete value (e.g., enum-like), the generator may continue to emit:
- one PyTransitions transition entry per possible return value, with `conditions=choose_<retVal>` (or equivalent).

### 2) Actions MUST NOT be transition `before` callbacks
For these transitions, the generator MUST NOT attach FSMLang action-matrix action functions as transition-level `before` callbacks.
Actions must be invoked only via `_fsm_prepare_event` and `_fsm_actions`.

---

## Implementation constraints (to prevent noisy PRs)
- Only change generator code and the new/updated python canonicals for the targeted tests.
- Do not touch unrelated tests, make infrastructure, or tracked build artifacts.
- Keep changes minimal and localized.

---

## Test requirements

Add at least one new test directory: `test/python/<new_test_dir>/`

The test FSM must include:
1) An action-matrix example where an action is specified for `(state, trigger)` and must run even if no transition is taken.
2) Multiple conditional transitions for a single trigger (to demonstrate that the action is not multiplied by multiple evaluated transitions).
3) At least one event with a data block, and at least one event without a data block.

Golden expectations:
- `<new_test_dir>/<name>.py.canonical` must contain:
  - `send_event=True`
  - `prepare_event="_fsm_prepare_event"`
  - definitions of `_fsm_get_trigger_name` and `_fsm_prepare_event`
  - `_fsm_actions` mapping with `(state, trigger)` tuple keys
  - `_fsm_translators` mapping containing ONLY triggers with data blocks
  - `self.data = {}` in `__init__` if and only if event data exists
  - transition table entries for guard return values that do NOT include the action as `before`

Additionally:
- if `generate_weak_fns` is ON in the test Makefile for that directory, the canonical must include translator stubs that print their name and do nothing else.

---

## Generated Python sketch (normative shape, names MUST match)

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


# Only data-bearing triggers appear here:
_fsm_translators = {
    "event1": "translate_event1",
    "event2": "handle_event2",  # example explicit translator
}


# Action-matrix mapping: (state, trigger) -> action method name
_fsm_actions = {
    ("state1", "event1"): "doSomething",
    ("state2", "event1"): "doSomething",
}


def _fsm_prepare_event(self, e):
    trigger = self._fsm_get_trigger_name(e)
    if trigger is None:
        trigger = "__unknown_trigger__"

    translator_name = self._fsm_translators.get(trigger)
    if translator_name is not None:
        getattr(self, translator_name)(e)

    action_name = self._fsm_actions.get((self.state, trigger))
    if action_name is not None:
        getattr(self, action_name)(e)

    return True
```

---

## Pre-flight checklist (MUST be restated by agent before coding)
1. Confirm PR number + URL.
2. Confirm base branch is `master`.
3. Confirm head branch name you will push commits to is `301-refine-pytransitions-action-handling`.
4. Confirm you will NOT create a new branch and will NOT open a new PR.
5. Confirm only allowed files/dirs will change.

If any item is not true: STOP and ask for clarification.
