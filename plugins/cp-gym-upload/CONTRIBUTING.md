# Contributing to cp-gym-upload

The skill is meant to grow one case at a time. Each time you move a contest from a setup nobody has tried, the next person should find it already written down.

## Add a source judge (DOMjudge, vjudge, HackerRank, Kattis, ...)

Create `skills/cp-gym-upload/references/sources/<judge>.md`. It must explain how to get from that judge's material to the **normalized package** described in `SKILL.md` §1:

- where the statements, tests, solutions, submissions and scoreboard come from, and in which formats;
- how to turn the statements into Polygon LaTeX (link to `statements.md` instead of repeating it);
- the verdict mapping to `OK WA TL ML RT CE PE IL RJ`;
- how time is given (seconds, minutes, ms, clock time; which time zone);
- **how the judge treats compilation errors**, which decides `--ce`;
- anything that broke and how you got around it.

Then add the judge to the list in `SKILL.md` §1. If the conversion is mechanical, a script is better than prose: add it as a Python code block in `references/scripts/<name>.md` (no `.py` files; the plugin directory refuses them). Write it with the standard library only, and give it a `--help`.

Do not change the steps after §1 for one judge. If a judge needs something the rest of the pipeline cannot do, open an issue first.

## Add a series

Copy `series/example.yml` to `series/<series>.yml` with the settings of its existing gyms. Keep it to public information. Paths on your computer and personal preferences go in your own project instructions.

## Fix a trick that stopped working

Polygon, Codeforces and the judges change. When something in a reference file fails:

1. Fix it in that file.
2. Keep the note short: what to do, and what goes wrong otherwise.
3. Mention the month you tested it if the behaviour is likely to change again.

## Style

- English. The agent answers in the user's language anyway.
- Instructions, not stories: what to do, then why in one clause if it is not obvious.
- One place per fact. Link to a file rather than copying it.
- Never include passwords, API secrets, session ids or cookies in examples.
