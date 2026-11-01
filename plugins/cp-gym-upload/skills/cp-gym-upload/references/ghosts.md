# Ghosts

Ghosts replay the original scoreboard in the gym. Codeforces takes them as a `.dat` file:

```
@contest "Contest name"
@contlen 300
@problems 11
@teams N
@submissions M
@p A,Problem name,20,0
@t 1,0,1,"Team name"
@s team,problem,attempt,seconds,verdict
```

`scripts/ghosts.py build` writes this file. Do not write it by hand. The script:

- numbers the teams;
- drops everything after a team's first OK on a problem;
- counts attempts;
- cuts at the contest length;
- applies the CE policy;
- replaces `"` in names with “ ”.

## From the source to `submissions.csv`

Each source file (`sources/*.md`) explains how its export maps to:

```
team,problem,seconds,verdict
```

The general rules:

- `seconds` are from the **real** start of the contest. If the judge gives clock times, subtract the start. If you are unsure of the start, compare a few accepted submissions with the scoreboard's times.
- Keep only the contest window. Exports often include upsolving after the end.
- Verdicts must be one of OK WA TL ML RT CE PE IL RJ. Map anything else after asking.
- `teams.csv` lists exactly the teams on the official scoreboard. Judges, testers and guests that were not on it are left out.
- `official.csv` (team,solved,penalty) is copied from the official scoreboard. The names must match `teams.csv`.

## CE policy

| Original judge | `--ce` |
|---|---|
| CE is a penalized attempt (Juki) | `wa` |
| CE is an attempt without penalty (BOCA) | `keep` |
| CE does not count at all | `drop` |

When unsure, ask the user, then run `check`: a wrong CE policy shows up as penalty differences of 20 minutes per CE.

## Check, upload, check again

1. `python3 scripts/ghosts.py check ghosts.dat official.csv` must pass. Up to one minute of difference per solved problem is rounding. `check` treats a `CE` left in the file as an attempt without penalty (the usual ICPC rule). `python3 scripts/ghosts.py standings ghosts.dat` prints the recomputed scoreboard, if you want to look at it.
2. Upload from the gym's ADM. page: put the file's text in `#setup-ghosts textarea[name=contestDat]` and send `fetch(location.href, {method: 'POST', body: new FormData(form)})`. **Do not** click "Set »": it opens a native `confirm()` and freezes the tab.
3. Check that the page shows "Ghosts: (N)".
4. Parse the gym's `table.standings` and compare it with `official.csv` team by team (upload the CSV into the page with the file-upload tool, as in `file-transfer.md`).

Re-upload the ghosts after any "Update contest" (`gym.md`).
