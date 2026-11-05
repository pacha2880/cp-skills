# cp-skills

Agent skills for competitive programming. They follow the open [Agent Skills](https://agentskills.io) format (`SKILL.md`), so they work in Claude Code, Claude.ai and other tools that read it.

| Skill | What it does |
|---|---|
| [`cp-editorial`](plugins/cp-editorial/skills/cp-editorial/SKILL.md) | Writes editorials and explains solutions to Codeforces / ICPC / camp problems, with depth adjusted to the problem's difficulty. Also answers "I don't understand this step" questions about an editorial. |
| [`cp-gym-upload`](plugins/cp-gym-upload/README.md) | Moves a contest that already happened (rbx, Juki, BOCA, ...) to Polygon and a Codeforces Gym: statements in English plus the original language, tests, solutions, materials and ghosts from the real scoreboard. |

## Install

**Claude Code (plugin):**

```
/plugin marketplace add pacha2880/cp-skills
/plugin install cp-editorial@cp-skills
/plugin install cp-gym-upload@cp-skills
```

**Skills CLI (Claude Code, Cursor, Codex and others):**

```
npx skills add pacha2880/cp-skills
```

**Manually:** copy `plugins/<plugin>/skills/<skill>/` into your skills folder (for Claude Code, `~/.claude/skills/<skill>/`). On Claude.ai, upload the folder as a skill in Settings.

## `cp-editorial`

The skill is built around one rule and one test:

> A known tool gets named. A claim about this problem gets justified.
>
> A reader at the problem's rating must be able to implement the solution from the text alone, without looking at the code.

It asks the model to:

- set the focus and a target length from the problem's rating, using ranges measured on 55 Codeforces editorials from 2022 to 2026;
- solve the problem itself first, and for hard problems implement it and stress-test it against a brute force before writing;
- plan the chain of claims from statement to algorithm, each with its justification;
- write hints, then the complete idea, numbered claims, non-standard implementation details and the complexity, with long proofs in spoilers;
- audit the draft against a checklist of the gaps that appear most often in editorials, then delete what the reader does not need;
- when explaining to someone who is stuck, find the missing link and answer exactly what was asked, instead of rephrasing the same explanation.

It writes in the language of the request.

### Example

[`examples/single-cut-cross`](examples/single-cut-cross/editorial.md) is a full editorial for [Gym 106758 J](https://codeforces.com/gym/106758/problem/J), with a solution checked against a brute force.

The audit checklist came from real reader questions on a first draft of that editorial. One of them:

> **Draft:** "Events at angle exactly $0$ are already included in this state."
>
> **Reader:** "You say they are included, but you never say how or where."

The draft never defined the initial state of the sweep, never said how it was computed, and never said whether it used the original or the rotated points. The final editorial has a numbered step for it. Each of those questions became an item in the checklist: every symbol defined, every value with its origin, every initial state explicit, never "already included" without saying where.

## `cp-gym-upload`

Rebuilds a finished contest on Codeforces so anyone can train on it, with the original scoreboard replayed as ghosts. It has been used for the [ICPC Bolivia 2026 national contest](https://codeforces.com/gym/106758) (rbx + BOCA) and the [SanSi Cup 2025](https://codeforces.com/gym/106793) (Juki).

The pipeline is the same for every contest: source → normalized package → Polygon → Gym → ghosts → verification. Only the first step depends on the judge, so supporting a new one means adding one file. The gym is left private for you to review, and you log in yourself.

It needs an agent that can drive your logged-in browser (for example Claude in Chrome), because Codeforces has no API for gyms. See its [README](plugins/cp-gym-upload/README.md) for details and [CONTRIBUTING](plugins/cp-gym-upload/CONTRIBUTING.md) to add a judge or a series.

## Sources and credits

The skill combines:

- what Codeforces users ask of editorials: [Some suggestions about editorials](https://codeforces.com/blog/entry/86856), [Regarding Editorials](https://codeforces.com/blog/entry/16607), [Make proofs](https://codeforces.com/blog/entry/128945), [Improvement of Editorial for difficult problems](https://codeforces.com/blog/entry/75189);
- the structure of well-received editorials such as [Codeforces Round #715](https://codeforces.com/blog/entry/89644) and [Educational Round 95](https://codeforces.com/blog/entry/82673);
- ideas from other skills: [`math-olympiad`](https://github.com/anthropics/claude-plugins-official/tree/main/plugins/math-olympiad) (interpretation check, fresh-context review), [`competitive-programming-solver`](https://github.com/Prekzursil/agent-skills-toolchain) (stress testing), [`long-form-math`](https://github.com/dashed/claude-marketplace) (words between formulas), [`humanizer`](https://github.com/blader/humanizer) and [`avoid-ai-writing`](https://github.com/conorbronsdon/avoid-ai-writing) (prose style).

No text from those skills is copied here.

`cp-gym-upload` complements [`uploading-to-polygon`](https://github.com/LLaammTTeerr/competitive-programming) from LLaammTTeerr/competitive-programming. That skill uploads new problems prepared from scratch through the official Polygon API; this one moves whole contests that already happened, including the gym and its ghosts.

## En español

- `cp-editorial` escribe editoriales y explica soluciones de programación competitiva.
- `cp-gym-upload` sube un contest ya jugado (rbx, Juki, BOCA…) a Polygon y a un Gym de Codeforces. Traduce los enunciados al inglés y conserva el original. Arma los fantasmas desde el scoreboard real y los verifica equipo por equipo.

Las dos responden en el idioma del pedido, así que funcionan en español aunque estén escritas en inglés. Se instalan igual que arriba.

## License

[MIT](LICENSE)
