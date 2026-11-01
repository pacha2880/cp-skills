# Source: Juki (juki.app)

Tested on an 8-problem individual contest, late 2026.

## Typical material

- statements on the contest's web pages (Markdown);
- tests as zips or folders of `.in/.out`, with mixed naming (`01.in`, `1.in`, `test1.in`, `test_all.in`); `scripts/normalize_tests.py` handles them;
- `submissions.zip`: a CSV plus the code of each submission as `<ID>.cpp`;
- a scoreboard CSV;
- sometimes the authors' own solutions.

## Statements

The pages are Next.js and there is no public API. From a tab on the problem page, with the browser's JavaScript tool:

1. `fetch(location.href)` and collect every `self.__next_f.push([1,"..."])` string; `JSON.parse` each one.
2. Search them for `"statement":{...}` objects (description, input, output, note, sampleCases), one per language.
3. Long texts are references like `$1d` to chunks written as `1d:T<len>,...`. **`len` is in UTF-8 bytes**, so if you cut by characters you get garbage at the end; trim it.

Browser tool output is truncated (about 1 KB with Claude in Chrome). To read a long text, put the JSON in `document.body` inside a `<pre>` and read it with the page-text tool.

The same page has the start and end time, penalty, limits and **clarifications**. Clarifications may correct statements during the contest; apply them.

Convert Markdown to Polygon LaTeX following `../statements.md`. Look for mistakes in the original while you do (e.g. "hard version" written in the easy version) and report them.

## Images

`images.juki.pub` does not allow CORS, so `fetch` from the page fails. These work:

- The same-origin proxy: `/_next/image?url=<urlencoded>&w=3840&q=75` (q=100 fails).
- Browser downloads (blob + `a.download`) may not reach any folder you can read.

The method that worked every time:

1. Draw the image in the page at physical 1:1, i.e. CSS width = `naturalWidth / devicePixelRatio`.
2. Capture it with the browser's zoom/screenshot tool saved to disk, without scaling.
3. Do this in tiles of at most ~450 px high, moving the image's `top`, and stitch the tiles with PIL.

The zoom region is in CSS units: with DPR 0.75, an 800×600 region gives 600×450 px.

Images that point to external sites that no longer work (for example chatgpt.com links) are removed; say so in the report.

## Tests

From the exports. Check that the count per problem matches what Juki shows.

## Solutions

The submissions CSV includes upsolving after the contest. Keep only submissions between the real start and the last valid submission of the contest. Candidates for main and alternative solutions are the accepted ones in that window, plus the authors' solutions. Compile and run them all locally on all tests before choosing.

## Ghosts

The submissions CSV has:

- a timestamp in **milliseconds, UTC**: seconds from start = (timestamp − start) / 1000;
- verdicts AC, WA, TLE, RE, CE, PE, MLE. Map AC→OK, WA→WA, TLE→TL, RE→RT, CE→CE, PE→WA, MLE→ML.

The scoreboard CSV has cells like `1 point a/pen` and `a/a*20`. Use it for `teams.csv` and `official.csv`.

**Juki penalizes compilation errors.** Ask the user. To reproduce the Juki scoreboard exactly, use `--ce wa`. That was the choice for the contest this was tested on.
