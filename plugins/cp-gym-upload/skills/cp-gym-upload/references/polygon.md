# Polygon

Two ways to drive Polygon:

- **Official API** (Polygon → Settings → API keys; key + secret, signed requests). It covers problem-level work. If the user has an API client set up, such as the `polygon` MCP server in [LLaammTTeerr/competitive-programming](https://github.com/LLaammTTeerr/competitive-programming), you can use it for the problem steps below. Never ask the user to paste their API secret into the chat.
- **The website, from a logged-in tab, with `fetch`.** This is the method below, tested end to end. It also covers what the API does not (creating the contest, access lists). The endpoints are internal and may change.

Both methods: one request at a time, ~1.5 s apart.

## Session values

- `ccid` changes per browser session. Read it from `meta[name=ccid]`.
- `session` is per problem: `fetch('/edit-start?problemId=ID&ccid=' + ccid)`, and it is in the response's `r.url`.
- Send `ccid` (and `session`) both in the URL and in the body, and use `redirect: 'manual'`. An `opaqueredirect` response means success.

## Long jobs

Run them in the page in the background:

```js
window.JOB = 'running'; (async () => { ...; window.JOB = 'done'; })()
```

Poll `window.JOB` with waits of at most ~40 s per call, because browser tool calls time out at around 45 s.

## Problem steps

| Step | Request |
|---|---|
| Create | `POST /cp` {submitted, name, submit:'Create'} |
| Limits | `POST /generalInfo?action=saveProperties&session=` {inputFile:'stdin', outputFile:'stdout', timeLimit, memoryLimit, Save}. Minimum TL 250 ms. |
| Tests (zip) | multipart `POST /tests?action=add&testset=tests` {testArchive, ccid}. Zips from `normalize_tests.py`, uploaded in order; each is appended. |
| Mark sample | `POST /data/tests` {action:'setTestExample', session, testset:'tests', testIndex, example:'yes'} |
| First statement language | `POST /statements?session=` {submitted:'true', explang:'<lang>', cloneFrom:'', Save:'Create'} |
| More languages | `POST /statements?lang=select&session=` (same body) |
| Save statement | `POST /statements` {action:'saveStatementForm', session, lang, statementMode:'statement and tutorial', name, encoding:'UTF-8', showInReview:'false', showCautionsAndGrammaticalFixes:'false', legend, input, output, interaction:'', scoring:'', notes, tutorial}. The JSON reply says "has been saved". Posting the normal form does **not** save. |
| Images | multipart `POST /statements?session=` {resources_file_added}. Only after a statement exists. Shared by all languages. |
| Upload solution | multipart `POST /solutions?session=` {solutions_file_type:'cpp.gcc13-64-winlibs-g++20', submitted:'true', solutions_file_added}. New solutions get tag None. |
| Main solution | `POST /solutions?session=` {action:'tagChangeType', file, submitted:'true', chosenType:'MA'} |
| Other tags | `POST /solutions` {action:'multipleSwitchType', session, files:'a.cpp, b.cpp', newType:'OK'} (does not accept MA). Tags: OK, WA, TL, TO (TL or OK), ML, RE, RJ. Use TO for a correct alternative that gets TLE, and RJ for a wrong one that gets TL instead of WA. |
| Checker | multipart `POST /checker?session=` {submitted:'true', file_selected:'std::wcmp.cpp', 'Set checker'}. Validator in `/validation`; without one Polygon only warns. |
| Commit | `POST /edit-commit?session=` {submitted:'true', message, allContests:'true', Commit:'Commit'} |
| Package | `POST /package?session=` {action:'create', doVerification:'on', createFull:'true'}. Polygon does not build a package for a revision that already has one, so commit first. |

## Running times

1. Create an invocation from `/invocations` (form `action=judge`, every `solution-*` and `test-*` checkbox 'on').
2. Read `/invocation?id=X&ccid=&session=` in an iframe and find the `max.` row.

The main solution must stay under half the time limit (SKILL.md §0).

## Pitfalls

- **Do not `edit-start` a problem while its package is building.** It hangs all six connections to the host. If that happens, close the tab and open a new one.
- Duplicate manual tests make the package fail. Deleting them leaves gaps ("enumerated incorrectly"): point the last lines of the test script to the freed indices. Running `normalize_tests.py` first avoids this.
- Polygon stores tests with CRLF, so its sizes are slightly bigger than local ones. That is normal.

## Contest

1. Create: `POST /cc` {submitted, name, location, date, language, submit:'Create'}.
2. Add problems from the UI: "Add problems?" → search → tick them in order A, B, … → Save. Letters follow the order.
3. "Manage developers list" → add `codeforces` with READ access. Confirm in "Manage problem access" and click "Save Access".
4. The contest UID is in the sidebar; the gym needs it.
