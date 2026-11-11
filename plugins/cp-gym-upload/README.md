# cp-gym-upload

Moves a contest that already happened to [Polygon](https://polygon.codeforces.com) and a [Codeforces Gym](https://codeforces.com/gyms), so anyone can train on it with the original scoreboard replayed as ghosts.

It has been used end to end for:

- **ICPC Bolivia 2026 national contest**, [Gym 106758](https://codeforces.com/gym/106758). 11 problems prepared in rbx, statements translated Spanish → English, ghosts from BOCA.
- **SanSi Cup 2025**, [Gym 106793](https://codeforces.com/gym/106793). 8 problems run on Juki; statements scraped from the contest pages, images recovered, tests and submissions from exports, ghosts matching the Juki scoreboard.

## What it does

```
source (rbx, Juki, BOCA, ...)  →  normalized package  →  Polygon  →  Gym  →  ghosts  →  verification
```

- **Statements** in English plus the original language. The gym only shows English statements, so a Spanish-only contest needs a translation. Includes images, editorials and PDFs.
- **Tests** normalized for Polygon, with the samples first and duplicates removed. Every solution is run locally before uploading.
- **Polygon**: problems, solutions with their expected verdicts, checker, packages, running times, and the contest with access for `codeforces`.
- **Gym**: settings, import, an HTML check of every problem, and materials.
- **Ghosts** built from the judge's export and checked team by team against the official scoreboard.
- A **final report** of everything changed or found wrong in the original.

The gym is left private for you to review. The skill never types passwords or solves CAPTCHAs: you log in yourself.

## Install

```
/plugin marketplace add pacha2880/cp-skills
/plugin install cp-gym-upload@cp-skills
```

Or copy `skills/cp-gym-upload/` into your skills folder. On Claude.ai, upload that folder as a skill in Settings.

## Requirements

- An agent that can drive your logged-in browser, such as Claude with Claude in Chrome. Codeforces has no API for gyms.
- Coach mode on your Codeforces account, to create gyms.
- Python 3 and a C++ compiler where the agent runs code, to normalize tests, run solutions and build ghosts.
- Optional: a Polygon API client, such as the `polygon` MCP server from [LLaammTTeerr/competitive-programming](https://github.com/LLaammTTeerr/competitive-programming), for the problem-level steps.

## Layout

```
skills/cp-gym-upload/
  SKILL.md                  the pipeline and the ground rules
  references/
    sources/rbx.md          one file per source judge
    sources/juki.md
    sources/boca.md
    statements.md           Polygon LaTeX, images, translations, PDFs
    polygon.md              Polygon from a logged-in tab
    gym.md                  creating and editing the gym
    ghosts.md               ghosts: format, CE policy, upload, checks
    file-transfer.md        computer ↔ sandbox ↔ browser
    checklist.md            before handing over
    scripts/normalize_tests.md  normalize, number and zip tests for Polygon
    scripts/ghosts.md           build / standings / check ghost files
  series/
    example.yml             settings shared by every edition of a series
    sansi-cup.yml
    icpc-bolivia-nacional.yml
```

## Adding a judge, a series or a fix

See [CONTRIBUTING.md](CONTRIBUTING.md). In short:

- **a new judge** is one new file in `references/sources/` that produces the normalized package;
- **a new series** is one file in `series/`;
- **a trick that stopped working** is a fix to the reference file that has it.

## Scripts

The two helpers are Python, stored as code blocks in `references/scripts/*.md` because the Claude plugin directory accepts only text files. The skill writes them to `scripts/` in its working folder before using them. To run them yourself, copy each block to a `.py` file:

```bash
python3 scripts/normalize_tests.py OUT TESTDIR [TESTDIR ...] --samples SAMPLEDIR
python3 scripts/ghosts.py build subs.csv --teams teams.csv --contest "Name" --length 300 \
    --problem A "First" --problem B "Second" --ce wa -o ghosts.dat
python3 scripts/ghosts.py check ghosts.dat official.csv
```

Both run with the standard library only. `--help` explains the formats.
