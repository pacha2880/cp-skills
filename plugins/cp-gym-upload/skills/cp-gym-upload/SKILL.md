---
name: cp-gym-upload
description: Move a contest that already happened (an rbx/Polygon package, or a contest on an online judge such as Juki or BOCA with exported tests, submissions and scoreboard) to Polygon and a Codeforces Gym — statements in English plus the original language, tests, solutions, ghosts from the real scoreboard, and materials. Use when asked to upload, mirror or archive a contest on Codeforces Gym.
---

# Move a finished contest to Polygon and a Codeforces Gym

The job: take a contest that was already held somewhere else and rebuild it on Codeforces so anyone can train on it, with the original scoreboard replayed as ghosts. It has been used end to end for an 11-problem ICPC regional (rbx repo) and an 8-problem university contest (Juki export).

The whole skill is one pipeline:

```
source (rbx, Juki, BOCA, ...)  →  normalized package  →  Polygon problems + contest  →  Gym  →  ghosts  →  verification
```

Only the first arrow depends on the source. Everything after it is the same for every contest. To support a new judge, add a file under `references/sources/` that produces the normalized package; do not change the rest.

## Ground rules

- **The user signs in.** Never type passwords, never solve CAPTCHAs, never create accounts. If a page needs a login, ask the user to log in and wait.
- **Gym stays Private** until the user has reviewed it.
- **Ask before anything destructive**: deleting problems or tests, "Update contest" on a gym that already has ghosts (it wipes them), overwriting materials.
- **Keep the original.** Do not change time limits, statements or tests silently. When something must change (see §1), do it, and list it in the final report.
- **Report errors in the original, do not fix them** unless the user asks: samples that disagree with the data, tests that break the constraints, typos that change meaning.
- **Go slowly with Codeforces and Polygon.** Requests one at a time, about 1.5 s apart. Bursts trigger Cloudflare and log the user out.
- Write the final report in the user's language.

## 0. Decide before starting

Ask about anything the user has not already answered (a series config in `series/` may answer several):

1. **Source and material.** Where are the statements, tests, solutions, submissions export and scoreboard? What is missing?
2. **Languages.** The Gym only shows statements in English (a problem with only, say, a Spanish statement shows "There is no problem statement for this language"). If the original is not in English, translate to English and keep the original language too, in Polygon and in the PDFs. Ask whether there are editorials to include.
3. **Time limits.** Keep the original. Raise one only if the main solution does not run in under half the limit on Polygon, and report it.
4. **Main solution.** Fastest accepted, first accepted, or the author's? Ask.
5. **Compilation errors in ghosts.** Did the original judge penalize CE? (BOCA does not; Juki does.) This decides `--ce` in `scripts/ghosts.py` and is the most common reason a ghost scoreboard does not match.
6. **Gym settings.** Name, season, type, location, difficulty, website, description, logo. For a series, copy the previous edition (see `series/`).

## Helper scripts

Two helpers ship as Markdown, because the plugin directory only accepts text files: `references/scripts/normalize_tests.md` and `references/scripts/ghosts.md`. Before step 1, write the Python block of each one, unchanged, to `scripts/normalize_tests.py` and `scripts/ghosts.py` in the working folder. Every `scripts/*.py` command below refers to those files.

## 1. Build the normalized package

Read the file for the source and follow it:

- `references/sources/rbx.md`: an rbx repository (statements in LaTeX, generators, `rbx package polygon`).
- `references/sources/juki.md`: Juki (statements scraped from the contest page, tests and submissions from exports).
- `references/sources/boca.md`: BOCA (runs listing and scoreboard; tests usually come from the problem packages).

Each one must end with this layout in a working folder (outside any folder synced with the user's computer if a tool uses SQLite or many small files):

```
work/
  contest.yml                 name, start time, length (min), penalty (min), problems in order
  problems/<letter>/
    meta.yml                  name, time limit (ms), memory limit (MB), checker (e.g. std::wcmp.cpp)
    statement.<lang>.tex      legend / input / output / notes, Polygon markup (references/statements.md)
    tutorial.<lang>.tex       optional
    img/                      images used by the statement
    tests/                    raw tests from the source, any naming
    samples/                  the statement's samples as .in/.out
    solutions/                main + alternatives, with the verdict each must get
  ghosts/
    submissions.csv           team,problem,seconds,verdict  (normalized, see scripts/ghosts.py)
    teams.csv                 team,name  (only teams on the official scoreboard)
    official.csv              team,solved,penalty  (from the official scoreboard)
```

Then, per problem:

```bash
python3 scripts/normalize_tests.py work/problems/A/polygon work/problems/A/tests --samples work/problems/A/samples
```

It normalizes line endings and trailing spaces (Polygon rejects tests that are not well formed), puts the samples first without duplicating them, numbers the tests `001…`, and packs zips of at most 9 MB.

**Before uploading anything**, compile every solution and run it on every test locally. Check that the main solution matches the answers and that each alternative gets its intended verdict. Also run all solutions on the samples and compare with the statement. Write a small validator that checks every test against the constraints in the statement, and report violations.

## 2. Polygon

Follow `references/polygon.md`: create each problem, set limits, upload the test zips, mark samples, statements in every language plus images, solutions with their tags, checker, commit, build the package and measure the solutions' running times. Then create a Polygon contest, add the problems in order and give the `codeforces` user read access.

Name problems `<series><year>-<letter>-<short-name>` so they are easy to find later.

## 3. Gym

Follow `references/gym.md`: create the gym (needs coach mode), import the problems from the Polygon contest, check that every problem renders as HTML in English, and upload materials (statements PDF per language, editorials).

## 4. Ghosts

Follow `references/ghosts.md`. In short: convert the source's export to `ghosts/submissions.csv`, then

```bash
python3 scripts/ghosts.py build ghosts/submissions.csv --teams ghosts/teams.csv \
    --contest "<name>" --length <min> --problem A "<name>" --problem B "<name>" ... \
    --ce <keep|wa|drop> -o ghosts/ghosts.dat
python3 scripts/ghosts.py check ghosts/ghosts.dat ghosts/official.csv
```

Do not upload until `check` passes (a difference of up to one minute per solved problem is rounding). After uploading, compare the gym's standings with `official.csv` team by team.

## 5. Verify and hand over

Go through `references/checklist.md` and fix or report every item. Then hand the user, in their folder: `img/`, statements `.tex` and PDFs per language (keep originals as `*-official.pdf`), `solutions/`, `polygon/` (normalized tests), `ghosts.dat`, logo, and a short report:

- time limits changed and why;
- solutions re-tagged;
- tests added (samples) or dropped (duplicates);
- errors found in the original, and whether they were fixed;
- scoreboard differences;
- confirmation that the gym is still private.

## Reference files

| File | Read it when |
|---|---|
| `references/scripts/*.md` | before step 1: write the helper scripts to the working folder |
| `references/sources/*.md` | building the package from that source |
| `references/statements.md` | writing or converting statements (Polygon LaTeX, images, translations, PDFs) |
| `references/file-transfer.md` | moving files between the user's computer, the sandbox and the browser |
| `references/polygon.md` | doing anything on Polygon |
| `references/gym.md` | creating or editing the gym |
| `references/ghosts.md` | building or uploading ghosts |
| `references/checklist.md` | before handing over |
| `series/*.yml` | the contest is part of a series that already has gyms |

The site-specific tricks in these files were tested in late 2026. Codeforces and the judges change; when something no longer works, find the new way, finish the job, and tell the user which file needs updating.
