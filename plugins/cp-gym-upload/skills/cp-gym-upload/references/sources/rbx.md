# Source: rbx repository

[rbx](https://github.com/rsalesc/rbx) keeps each problem as a folder with `problem.rbx.yml`, statements in LaTeX, generators, validators and solutions. Tested with rbx 1.4 on an 11-problem contest.

## Setup

- Copy the repo to a local working folder before running anything: `cp -r <repo> ~/work/<repo>`. rbx keeps a SQLite cache, and SQLite fails on folders mounted or synced from another machine.
- Install the rbx version the repo pins (`.preset-lock.yml`) in a virtualenv. A different version may build different tests.

## LaTeX problems seen on Ubuntu 22.04

- Missing `spanish.ldf`: use `\usepackage[provide=*,spanish]{babel}`.
- Missing `logicpuzzle`: comment it out if the statements do not use it.
- `microtype` errors: `\usepackage[expansion=false]{microtype}`.

Apply the fixes to **every** `icpc.sty`, including the copies inside `.local.rbx/`.

## Building

- `rbx statements build` per problem to get the PDFs.
- `rbx package polygon` per problem. The package has the tests, the solutions with their expected verdicts, and the statement split into Polygon blocks (`blocks.sub.yml`: legend, input, output, notes). Use those blocks for `statement.<lang>.tex` in the normalized package.
- Tests: take them from the built package, not from the generators, so they are exactly the ones used in the contest.

## Translations

rbx supports several languages per problem:

- a new `statement.<lang>.rbx.tex` next to the original;
- `%- block <lang>` blocks in `samples/*.rbx.tex` for sample explanations;
- `editorial-body.<lang>.tex` for editorials.

While translating, read the original closely. This is when you notice samples that disagree with the data, wrong sums in explanations, or a constraint that contradicts the tests. Report them (SKILL.md, ground rules).

## Ghosts

rbx contests are usually run on BOCA or DOMjudge. Use the file for that judge (`boca.md`) for the submissions and scoreboard.
