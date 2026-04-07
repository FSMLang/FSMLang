# SPEC: Remove `condition`, enforce single matrix cell entry, and add `when/otherwise` return-choice metadata for guard transitions (PyTransitions)

## Status / scope guardrails (MUST)
- Repo: `FSMLang/FSMLang`
- PR: #304 https://github.com/FSMLang/FSMLang/pull/304

- **Merge target (base branch):** `master`
- **Working branch (head branch to push commits to):**
  - `303-make-pytransitions-support-look-more-like-fsmlang`
  - The agent MUST push commits to this branch.
  - The agent MUST NOT create a new branch and MUST NOT open a new PR.

### Allowed files / directories to modify (MUST)
- Lexer / parser / core FSM structures:
  - `src/lexer.l`
  - `src/parser.y`
  - `src/fsm_priv.h` (only if necessary)
  - `src/fsm_utils.c` (only if necessary)
- Action matrix insertion logic:
  - wherever `add_to_action_array` is implemented/declared/used (expected under `src/`)
- PyTransitions generator:
  - `src/fsm_python_transitions.c`
  - `src/fsm_python_transitions.h` (only if necessary)
- Tests / canonicals:
  - update any impacted canonicals to match the new language (since PyTransitions support is not officially released yet)
  - python tests under `test/python/**`
  - parser/lexer tests under `test/parser/**` and/or `test/lexer/**` only if existing coverage breaks and needs minimal updates

### Hard non-goals
1. No drive-by refactors or formatting-only changes.
2. No CI/workflow edits.
3. Do not commit generated artifacts (`*.py`, `fsmout`, `*.result`, binaries, build dirs).
4. Do not introduce new features beyond what is described here.

---

## Problem statement
Early PyTransitions support introduced:
1) a new FSMLang keyword `condition`, and
2) support for multiple action/transition entries in a single `(event,state)` matrix cell.

These will be undone:
- The keyword `condition` must be removed.
- The core event/state matrix must return to allowing **only one entry** per `(event,state)` pair.

To retain PyTransitions conditional branching functionality, the “transition via function (guard)” feature is expanded to support **conditional return-choice metadata** using new keywords:
- `when <conditionFn>`: attaches a named condition function to a specific return choice
- `otherwise`: a required fallback choice when conditional metadata is used

This metadata is primarily for PyTransitions generation and is not strictly enforced beyond grammar/structure rules (consistent with FSMLang’s approach of not requiring return metadata).

---

## Glossary
- **Trigger**: the event name used to trigger a transition (PyTransitions terminology).
- **Cell**: an `(event,state)` entry in the action/transition matrix.
- **Guard transition**: `transition [event, state] guardFn;` where `guardFn` decides what to do next.
- **Return-choice metadata**: `guardFn returns ...;` declarations that list possible return values (states + `noTransition`) and, optionally, conditions.

---

## Language changes (MUST)

### A) Remove `condition` keyword from the language (MUST)
1. Remove the token/keyword `condition` from the lexer (`src/lexer.l`).
2. Remove all grammar productions / parser reductions deriving from `condition` (`src/parser.y`).
3. Remove any AST/data-structure fields that were exclusively supporting the `condition` keyword.
4. Error behavior:
   - It is acceptable for `condition` to become a generic parse error (“syntax error”) without custom messaging.

### B) Revert matrix cell cardinality: only one entry per (event,state) (MUST)
1. Revert `add_to_action_array` behavior to enforce:
   - only a single entry is allowed per `(event,state)` cell
2. If a second entry is added for the same cell, generation must fail (existing FSMLang error patterns apply).
3. This applies to both:
   - `action ... [event,state] ...;`
   - `transition [event,state] ...;`
   regardless of backend.

---

## New/expanded guard return metadata syntax (MUST)

### C) Placement: returns statements occur at end of machine block (MUST)
All return metadata declarations MUST appear:
- **after all other statements**
- **just before** the machine’s closing `}`

This is a grammar constraint. Example placement:

```fsm
machine M {
    state a, b;
    event e;

    transition [e, a] guard;

    guard returns b, noTransition;
}
```

### D) Return metadata statement forms (MUST)
A return metadata statement has the form:

```fsm
<guardFnName> returns <returnChoiceList> ;
```

Where `<returnChoiceList>` can be:

#### Mode A (legacy/unconditional list)
A comma-separated list of return values with **no** `when` or `otherwise` keywords:

```fsm
guardFn returns asleep, hanging_out, noTransition;
```

Semantics: This is a set/list of possible return values (no conditional selection metadata).

#### Mode B (conditional list with `when/otherwise`)
A comma-separated list where each choice is annotated with either:
- `when <conditionFn>` OR
- `otherwise`

Example:

```fsm
guardFn returns
      asleep when is_exhausted
    , hanging_out otherwise
    ;
```

#### Mode selection and consistency (MUST)
- If **any** return choice uses `when` or `otherwise`, then the list is in **Mode B**, and:
  1) **Every** return choice MUST use exactly one of:
     - `when <conditionFn>`
     - `otherwise`
  2) `otherwise` MUST appear **exactly once**
  3) `otherwise` MUST be the **last** return choice
  4) `otherwise` takes **no** condition function

If no choice uses `when/otherwise`, the list is **Mode A** and remains backward-compatible.

### E) Return values allowed (MUST)
Each return choice value MUST be one of:
- a declared machine state name, OR
- the literal `noTransition`

(Consistent with prior guard-return support for PyTransitions.)

### F) Evaluation semantics for Mode B (MUST)
Mode B defines an ordered selection model:
1. Evaluate `when` clauses **top-to-bottom**.
2. The **first** `when` condition that returns true selects its associated return value.
3. If all `when` conditions are false, select the `otherwise` return value.
4. If the selected return value is `noTransition`, then no transition occurs.

Important: FSMLang does not necessarily execute these conditions at runtime for all backends; this is **metadata used for PyTransitions generation**. (See enforcement policy.)

### G) Enforcement policy / compatibility (MUST)
FSMLang has historically not enforced the presence of return metadata for guard transitions. This must continue:
- A guard transition may exist without any `returns` statement.
- A `returns` statement may exist without a corresponding guard transition (not recommended; allowed unless it breaks existing design).
- The parser must accept both Mode A and Mode B declarations as defined above.

Documentation/narrative in code comments should indicate that PyTransitions output relies on the presence of sufficient return metadata.

---

## PyTransitions generator requirements (MUST)

### H) No usage of removed `condition` feature (MUST)
The PyTransitions generator must not depend on the deleted `condition` token/AST path.
All conditional branching must be derived from guard return metadata (Mode B) for guard transitions.

### I) Mapping Mode B to PyTransitions transitions (MUST)
For each guard transition cell:

```fsm
transition [EVENT, STATE] guardFn;
```

When `guardFn returns ...` exists in Mode B:
- Generate one PyTransitions transition entry per return choice value which is a state (exclude `noTransition` from actual destination transitions).
- Each generated transition must:
  - use trigger `EVENT`
  - use source `STATE`
  - use destination of the return choice state
  - use a `conditions=` callback that corresponds to the choice’s condition
    - for `when <condFn>`: conditions must invoke the named condition function
    - for `otherwise`: conditions must be the negation of all prior `when` conditions (or an equivalent final condition function)

Implementation note: the generator may synthesize per-choice condition wrapper functions (e.g., `choose_<state>` or `choose_<retVal>`) as it does today, but these wrappers must reflect the `when/otherwise` metadata.

When the selected return is `noTransition`:
- There is no PyTransitions destination transition for it.

### J) Mapping Mode A / missing returns to PyTransitions (MUST)
Existing behavior for guard transitions where return metadata is missing or unconditional (Mode A) is allowed to remain as-is, except:
- it must not require declarations solely to parse (it may still be required to generate PyTransitions if that is current behavior)
- do not reintroduce `condition`

If the current generator requires explicit return declarations to generate PyTransitions for guard transitions, that may remain the default; however Mode B must be supported when present.

### K) Action semantics and `prepare_event` (MUST)
Preserve the previously-established action semantics for PyTransitions output:
- action-matrix “always-run actions” must be invoked exactly once per trigger occurrence via `prepare_event`
- transition entries must not attach action functions as `before` callbacks solely to guarantee invocation

If the repo currently already contains the `prepare_event` dispatcher logic, do not regress it.

---

## Tests / canonicals (MUST)

### L) Update canonical tests that used `condition` (MUST)
Any existing tests/canonicals that relied on `condition` or multiple entries per cell must be updated to the new syntax.

### M) Add a Python test demonstrating Mode B (MUST)
Add a new python test (or update an existing one) that demonstrates replacing:

```fsm
transition [clean_up, sweaty] asleep condition is_exhausted;
transition [clean_up, sweaty] hanging_out;
```

with:

```fsm
transition [clean_up, sweaty] what_should_i_do;

what_should_i_do returns
      asleep when is_exhausted
    , hanging_out otherwise
    ;
```

The canonical `.py.canonical` must show that:
- there is exactly one transition statement in the FSM for `[clean_up, sweaty]`
- PyTransitions output contains conditional transitions derived from `when/otherwise` metadata
- no `condition` keyword is present anywhere
- cell multiplicity is not used

---

## Pre-flight checklist (MUST be restated by agent before coding)
1. Confirm PR #304 URL.
2. Confirm base branch is `master`.
3. Confirm head branch name you will push commits to is `303-make-pytransitions-support-look-more-like-fsmlang`.
4. Confirm you will NOT create a new branch and will NOT open a new PR.
5. Confirm allowed file scope as defined above.

If any item is not true: STOP and ask for clarification.
