# ghosts.py

Write the code block below, unchanged, to `scripts/ghosts.py` in the work folder (see `SKILL.md`, "Helper scripts"), then run it with `python3 scripts/ghosts.py --help`. Standard library only.

```python
"""Build and check Codeforces Gym ghost files (.dat) from a normalized CSV.

Every source judge (BOCA, Juki, DOMjudge, ...) is first converted to one
normalized submissions CSV. This script does the rest, so the rules for
attempts, cut-offs and penalties live in one place.

Normalized submissions CSV (header required):
    team,problem,seconds,verdict
  team     team key (name or id; mapped to names with --teams)
  problem  letter A, B, ...
  seconds  seconds from contest start (integer)
  verdict  OK WA TL ML RT CE PE IL RJ  (map the judge's verdicts to these)

Optional teams CSV (header required):  team,name
  Lists every team that should appear (also teams with no submissions), in
  the order they should be numbered. Teams missing from it are dropped,
  which is how judges and guests are excluded.

Subcommands
  build      write the .dat
  standings  recompute solved/penalty from a .dat and print a CSV
  check      compare a .dat with the official scoreboard (team,solved,penalty)

Examples
  ghosts.py build subs.csv --teams teams.csv --contest "SanSi Cup 2025" \
      --length 300 --problem A "Easy one" --problem B "Harder, one" --ce wa -o ghosts.dat
  ghosts.py check ghosts.dat official.csv
"""
import argparse
import csv
import re
import sys
from collections import defaultdict

VERDICTS = {"OK", "WA", "TL", "ML", "RT", "CE", "PE", "IL", "RJ"}


def clean_name(name: str) -> str:
    out, opening = [], True
    for ch in name.strip():
        if ch == '"':
            out.append("“" if opening else "”")
            opening = not opening
        else:
            out.append(ch)
    return "".join(out)


def cmd_build(a):
    probs = [(l.strip().upper(), n.strip()) for l, n in a.problem]
    letters = [p for p, _ in probs]
    with open(a.submissions, newline="", encoding="utf-8-sig") as f:
        subs = list(csv.DictReader(f))
    for need in ("team", "problem", "seconds", "verdict"):
        if subs and need not in subs[0]:
            sys.exit(f"submissions CSV lacks column '{need}'")

    if a.teams:
        with open(a.teams, newline="", encoding="utf-8-sig") as f:
            teams = [(r["team"].strip(), r.get("name") or r["team"]) for r in csv.DictReader(f)]
    else:
        keys = sorted({s["team"].strip() for s in subs})
        teams = [(k, k) for k in keys]
    tid = {k: i for i, (k, _) in enumerate(teams, 1)}
    exclude = set(x.strip() for x in (a.exclude or "").split(",") if x.strip())

    cutoff = a.cutoff if a.cutoff is not None else a.length * 60
    rows, dropped = [], defaultdict(int)
    for s in subs:
        team, prob = s["team"].strip(), s["problem"].strip().upper()
        v = s["verdict"].strip().upper()
        sec = int(float(s["seconds"]))
        if team in exclude or team not in tid:
            dropped["team not in teams list / excluded"] += 1
            continue
        if prob not in letters:
            dropped[f"unknown problem {prob}"] += 1
            continue
        if v not in VERDICTS:
            sys.exit(f"unknown verdict '{v}' (map it to one of {sorted(VERDICTS)})")
        if sec < 0 or sec > cutoff:
            dropped["outside contest window"] += 1
            continue
        if v == "CE":
            if a.ce == "drop":
                dropped["CE dropped"] += 1
                continue
            if a.ce == "wa":
                v = "WA"
        rows.append((sec, tid[team], prob, v))
    rows.sort()

    attempt = defaultdict(int)
    solved = set()
    out_subs = []
    for sec, t, p, v in rows:
        if (t, p) in solved:
            dropped["after first OK"] += 1
            continue
        attempt[(t, p)] += 1
        out_subs.append((t, p, attempt[(t, p)], sec, v))
        if v == "OK":
            solved.add((t, p))

    lines = [f'@contest "{clean_name(a.contest)}"', f"@contlen {a.length}",
             f"@problems {len(probs)}", f"@teams {len(teams)}",
             f"@submissions {len(out_subs)}"]
    lines += [f"@p {l},{clean_name(n).replace(',', '')},{a.penalty},0" for l, n in probs]
    lines += [f'@t {tid[k]},0,1,"{clean_name(n)}"' for k, n in teams]
    lines += [f"@s {t},{p},{k},{sec},{v}" for t, p, k, sec, v in out_subs]
    with open(a.output, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(lines) + "\n")
    print(f"{a.output}: {len(teams)} teams, {len(out_subs)} submissions")
    for why, n in sorted(dropped.items()):
        print(f"  dropped {n}: {why}")


def read_dat(path):
    probs, teams, subs, penalty = [], {}, [], 20
    with open(path, encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if line.startswith("@p "):
                parts = line[3:].split(",")
                probs.append(parts[0])
                penalty = int(parts[-2])
            elif line.startswith("@t "):
                m = re.match(r'@t (\d+),[^,]*,[^,]*,"(.*)"$', line)
                teams[int(m.group(1))] = m.group(2)
            elif line.startswith("@s "):
                t, p, k, sec, v = line[3:].split(",")
                subs.append((int(t), p, int(k), int(sec), v))
    return probs, teams, subs, penalty


def standings(path, ce_penalty=False):
    probs, teams, subs, pen = read_dat(path)
    res = {t: {"solved": 0, "penalty": 0, "cells": {}} for t in teams}
    tries = defaultdict(int)
    for t, p, k, sec, v in sorted(subs, key=lambda s: s[3]):
        r = res[t]
        if p in r["cells"] and r["cells"][p].startswith("+"):
            continue
        if v == "OK":
            r["solved"] += 1
            r["penalty"] += sec // 60 + pen * tries[(t, p)]
            r["cells"][p] = f"+{tries[(t, p)] or ''}:{sec // 60}"
        else:
            if v != "CE" or ce_penalty:
                tries[(t, p)] += 1
            r["cells"][p] = f"-{tries[(t, p)]}"
    order = sorted(teams, key=lambda t: (-res[t]["solved"], res[t]["penalty"], teams[t]))
    return probs, teams, res, order


def cmd_standings(a):
    probs, teams, res, order = standings(a.dat, a.ce_penalty)
    w = csv.writer(sys.stdout)
    w.writerow(["rank", "team", "solved", "penalty"] + probs)
    for i, t in enumerate(order, 1):
        r = res[t]
        w.writerow([i, teams[t], r["solved"], r["penalty"]] + [r["cells"].get(p, "") for p in probs])


def cmd_check(a):
    _, teams, res, _ = standings(a.dat, a.ce_penalty)
    by_name = {teams[t]: res[t] for t in teams}
    with open(a.official, newline="", encoding="utf-8-sig") as f:
        official = list(csv.DictReader(f))
    bad = 0
    for row in official:
        name = clean_name(row["team"])
        if name not in by_name:
            print(f"MISSING in .dat: {name}")
            bad += 1
            continue
        r = by_name.pop(name)
        s, pz = int(row["solved"]), int(row["penalty"])
        if r["solved"] != s or abs(r["penalty"] - pz) > a.tol * max(s, 1):
            print(f"DIFF {name}: official {s}/{pz}, dat {r['solved']}/{r['penalty']}")
            bad += 1
    for name in by_name:
        print(f"EXTRA in .dat (not in official scoreboard): {name}")
        bad += 1
    print("OK: all teams match" if not bad else f"{bad} problem(s)")
    sys.exit(1 if bad else 0)


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    sp = ap.add_subparsers(dest="cmd", required=True)
    b = sp.add_parser("build")
    b.add_argument("submissions")
    b.add_argument("--teams")
    b.add_argument("--contest", required=True)
    b.add_argument("--length", type=int, required=True, help="minutes")
    b.add_argument("--problem", nargs=2, action="append", required=True,
                   metavar=("LETTER", "NAME"), help="repeat once per problem, in order")
    b.add_argument("--ce", choices=["keep", "wa", "drop"], required=True,
                   help="how to treat compilation errors; ask the user (it depends on "
                        "how the original judge penalized them)")
    b.add_argument("--penalty", type=int, default=20)
    b.add_argument("--cutoff", type=int, help="seconds; default = length")
    b.add_argument("--exclude", help="comma-separated team keys")
    b.add_argument("-o", "--output", default="ghosts.dat")
    b.set_defaults(func=cmd_build)
    for name, fn in (("standings", cmd_standings), ("check", cmd_check)):
        s = sp.add_parser(name)
        s.add_argument("dat")
        if name == "check":
            s.add_argument("official", help="CSV with columns team,solved,penalty")
            s.add_argument("--tol", type=int, default=1,
                           help="allowed penalty difference per solved problem (rounding)")
        s.add_argument("--ce-penalty", action="store_true",
                       help="count CE as a penalized attempt when recomputing")
        s.set_defaults(func=fn)
    a = ap.parse_args()
    a.func(a)


if __name__ == "__main__":
    main()
```
