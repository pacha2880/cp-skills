# normalize_tests.py

Write the code block below, unchanged, to `scripts/normalize_tests.py` in the work folder (see `SKILL.md`, "Helper scripts"), then run it with `python3 scripts/normalize_tests.py --help`. Standard library only.

```python
"""Normalize a problem's tests and pack them for Polygon.

Takes one or more folders with input/answer pairs in any common naming
(01.in/01.out, test1.in/test1.ans, 1/1.a, ...), plus optional sample files, and
writes:

  OUT/tests/001, 002, ...        inputs, in order (samples first)
  OUT/answers/001.a, ...          matching answers, for local checking
  OUT/polygon-tests-1.zip, ...    inputs only, each zip <= --max-zip-mb
  OUT/manifest.json               index -> original file, sample flag, sizes

Normalization (Polygon's "tests well-formed" check rejects anything else):
CRLF/CR -> LF, no trailing spaces on any line, no blank lines at the end,
exactly one final newline (empty files stay empty).

Samples are prepended as tests 1..k unless an identical input already exists,
in which case that test is moved to the front instead of duplicated.

Usage:
  normalize_tests.py OUT TESTDIR [TESTDIR ...] [--samples SAMPLEDIR]
"""
import argparse
import json
import re
import sys
import zipfile
from pathlib import Path

IN_EXT = {".in", ".inp", ".input"}
ANS_EXT = [".out", ".ans", ".a", ".ok", ".sol", ".output", ".res"]


def natural_key(s: str):
    return [int(t) if t.isdigit() else t.lower() for t in re.split(r"(\d+)", s)]


def normalize(data: bytes) -> bytes:
    text = data.decode("utf-8", errors="surrogateescape")
    text = text.replace("\r\n", "\n").replace("\r", "\n")
    lines = [ln.rstrip(" \t") for ln in text.split("\n")]
    while lines and lines[-1] == "":
        lines.pop()
    if not lines:
        return b""
    return ("\n".join(lines) + "\n").encode("utf-8", errors="surrogateescape")


def find_pairs(folder: Path):
    """Return [(input_path, answer_path_or_None)] in natural order."""
    files = sorted((p for p in folder.rglob("*") if p.is_file()),
                   key=lambda p: natural_key(str(p.relative_to(folder))))
    by_stem = {}
    for p in files:
        by_stem.setdefault((p.parent, p.stem), []).append(p)
    pairs = []
    used = set()
    for p in files:
        if p in used:
            continue
        stem_files = by_stem[(p.parent, p.stem)]
        if p.suffix.lower() in IN_EXT:
            ans = next((q for ext in ANS_EXT for q in stem_files
                        if q.suffix.lower() == ext), None)
            pairs.append((p, ans))
            used.add(p)
            if ans:
                used.add(ans)
        elif p.suffix == "" and re.fullmatch(r"\d+", p.name):
            # Polygon/Codeforces style: "1" with "1.a"
            ans = p.with_name(p.name + ".a")
            pairs.append((p, ans if ans.exists() else None))
            used.add(p)
            used.add(ans)
    return pairs


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("out")
    ap.add_argument("testdirs", nargs="+")
    ap.add_argument("--samples", help="folder with the statement's samples")
    ap.add_argument("--max-zip-mb", type=float, default=9.0)
    args = ap.parse_args()

    out = Path(args.out)
    (out / "tests").mkdir(parents=True, exist_ok=True)
    (out / "answers").mkdir(parents=True, exist_ok=True)

    regular = []
    for d in args.testdirs:
        regular += find_pairs(Path(d))
    samples = find_pairs(Path(args.samples)) if args.samples else []
    if not regular and not samples:
        sys.exit("no test pairs found")

    entries = []  # dicts with input bytes, answer bytes, source, sample
    seen = {}
    for src, sample in [(s, True) for s in samples] + [(r, False) for r in regular]:
        inp, ans = src
        data = normalize(inp.read_bytes())
        a = normalize(ans.read_bytes()) if ans else None
        if data in seen:
            prev = entries[seen[data]]
            if sample:
                prev["sample"] = True
            else:
                prev["duplicates"].append(str(inp))
                if prev["answer"] is not None and a is not None and prev["answer"] != a:
                    print(f"WARNING: {inp} duplicates {prev['source']} with a different answer",
                          file=sys.stderr)
            continue
        seen[data] = len(entries)
        entries.append({"input": data, "answer": a, "source": str(inp),
                        "sample": sample, "duplicates": []})

    manifest = []
    for i, e in enumerate(entries, 1):
        name = f"{i:03d}"
        (out / "tests" / name).write_bytes(e["input"])
        if e["answer"] is not None:
            (out / "answers" / f"{name}.a").write_bytes(e["answer"])
        manifest.append({"index": i, "file": name, "source": e["source"],
                         "sample": e["sample"], "bytes": len(e["input"]),
                         "has_answer": e["answer"] is not None,
                         "duplicates_dropped": e["duplicates"]})

    limit = int(args.max_zip_mb * 1024 * 1024)
    zips, cur, cur_size = [], [], 0
    for m in manifest:
        if cur and cur_size + m["bytes"] > limit:
            zips.append(cur)
            cur, cur_size = [], 0
        cur.append(m["file"])
        cur_size += m["bytes"]
    if cur:
        zips.append(cur)
    zip_names = []
    for k, group in enumerate(zips, 1):
        zp = out / f"polygon-tests-{k}.zip"
        with zipfile.ZipFile(zp, "w", zipfile.ZIP_DEFLATED, compresslevel=9) as z:
            for f in group:
                z.write(out / "tests" / f, arcname=f)
        zip_names.append({"zip": zp.name, "first": group[0], "last": group[-1],
                          "bytes": zp.stat().st_size})
        if zp.stat().st_size > limit:
            print(f"WARNING: {zp.name} is {zp.stat().st_size} bytes (one test is too big?)",
                  file=sys.stderr)

    (out / "manifest.json").write_text(json.dumps(
        {"tests": manifest, "zips": zip_names}, indent=2))
    n_s = sum(m["sample"] for m in manifest)
    n_d = sum(len(m["duplicates_dropped"]) for m in manifest)
    print(f"{len(manifest)} tests ({n_s} samples), {n_d} duplicates dropped, "
          f"{len(zip_names)} zip(s) in {out}")


if __name__ == "__main__":
    main()
```
