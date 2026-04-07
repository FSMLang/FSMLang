# FSMLang – Agent Instructions (MUST)

## Output / context size limits (MUST)
- Do NOT paste or upload large artifacts (full `git diff`, full build logs, generated files, dependency dumps, etc.) into the agent prompt/context.
- Never include more than ~200 lines of logs/diff in a single message. Prefer:
  - a short summary,
  - the first error block,
  - and the exact command needed to reproduce.
- When a full diff is needed, reference changed files and key hunks only (paths + brief description), not the entire diff.
- If you hit a model/context limit error, immediately retry with reduced context and do not re-attempt sending the full content.

## Scope / discipline
- If you are asked to work on an existing pull request, you MUST push commits to that PR’s existing head branch.
- You MUST NOT create a new branch and MUST NOT open a new pull request unless explicitly instructed.
- Do not merge/rebase other branches unless explicitly instructed.

## Pre-flight (required)
Before making any code changes, restate:
- PR URL
- base branch
- head branch you will push to
- the list of files/dirs you intend to modify

If any of these are unclear, STOP and ask for clarification.

## Change hygiene
- Keep changes minimal and localized to the task.
- Do not commit generated artifacts or build outputs unless explicitly requested.
