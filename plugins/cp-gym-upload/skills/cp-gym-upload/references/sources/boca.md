# Source: BOCA

[BOCA](https://github.com/cassiopc/boca) is common in Latin American ICPC regionals. Tested for ghosts on an ICPC national contest whose problems came from an rbx repo (`rbx.md`).

## Problems

BOCA problem packages are zips with `description/`, `input/`, `output/`, `limits/` and `compare/`. If the contest also has an rbx or Polygon repo, prefer that for statements and solutions. Otherwise take:

- tests from `input/` and `output/`;
- limits from `limits/`;
- statement PDFs from `description/`.

PDF-only statements have to be rewritten as LaTeX (`../statements.md`).

## Ghosts

Material:

- the admin's **Runs** listing: run, team, problem, time **in minutes**, answer;
- the scoreboard export (CSV or HTML): team id, team name, and per problem `attempts/time`.

Steps:

1. Times are minutes since the start. Use `seconds = minutes * 60`. If the listing uses the clock time instead, find the start time by comparing accepted runs with the scoreboard's times.
2. Map the answers: YES→OK, wrong answer→WA, time limit→TL, runtime error→RT, compilation error→CE. Map other codes (contact staff, presentation…) after asking the user.
3. BOCA counts CE as an attempt **without penalty**, so use `--ce keep`.
4. Drop judge, staff and guest teams that are not on the public scoreboard. Simply leave them out of `teams.csv`.
5. Because times are whole minutes, `scripts/ghosts.py check` can be off by up to one minute per solved problem. That is expected.
