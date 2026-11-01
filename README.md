# cp-skills

Agent skills for competitive programming. They follow the open [Agent Skills](https://agentskills.io) format (`SKILL.md`), so they work in Claude Code, Claude.ai and other tools that read it.

| Skill | What it does |
|---|---|
| [`cp-editorial`](plugins/cp-editorial/skills/cp-editorial/SKILL.md) | Writes editorials and explains solutions to Codeforces / ICPC / camp problems, with depth adjusted to the problem's difficulty. Also answers "I don't understand this step" questions about an editorial. |

## Install

**Claude Code (plugin):**

```
/plugin marketplace add pacha2880/cp-skills
/plugin install cp-editorial@cp-skills
```

**Skills CLI (Claude Code, Cursor, Codex and others):**

```
npx skills add pacha2880/cp-skills
```

**Manually:** copy `plugins/cp-editorial/skills/cp-editorial/` into your skills folder (for Claude Code, `~/.claude/skills/cp-editorial/`). On Claude.ai, upload the folder as a skill in Settings.

## `cp-editorial`

The skill is built around one test:

> A reader at the target level must be able to implement the solution from the text alone, without looking at the code.

It asks the model to:

- pick the reader from the problem's rating (green/cyan, cyan/blue, blue/purple) and adjust depth;
- solve the problem itself first, and for hard problems implement it and stress-test it against a brute force before writing;
- plan the chain of claims from statement to algorithm, each with its justification;
- write prerequisites, hints as questions, a slow-then-fast tutorial, a worked example from the samples, and code that uses the same names as the text;
- audit the draft against a checklist of the gaps that appear most often in editorials;
- when explaining to someone who is stuck, find the missing link and answer exactly what was asked, instead of rephrasing the same explanation.

It writes in the language of the request.

### Example

[`examples/single-cut-cross`](examples/single-cut-cross/editorial.md) is a full editorial for [Gym 106758 J](https://codeforces.com/gym/106758/problem/J), with a solution checked against a brute force.

The audit checklist came from real reader questions on a first draft of that editorial. One of them:

> **Draft:** "Events at angle exactly $0$ are already included in this state."
>
> **Reader:** "You say they are included, but you never say how or where."

The draft never defined the initial state of the sweep, never said how it was computed, and never said whether it used the original or the rotated points. The final editorial has a full paragraph for it. Each of those questions became an item in the checklist: every symbol defined, every value with its origin, every initial state explicit, never "already included" without saying where.

## Sources and credits

The skill combines:

- what Codeforces users ask of editorials: [Some suggestions about editorials](https://codeforces.com/blog/entry/86856), [Regarding Editorials](https://codeforces.com/blog/entry/16607), [Make proofs](https://codeforces.com/blog/entry/128945), [Improvement of Editorial for difficult problems](https://codeforces.com/blog/entry/75189);
- the structure of well-received editorials such as [Codeforces Round #715](https://codeforces.com/blog/entry/89644) and [Educational Round 95](https://codeforces.com/blog/entry/82673);
- ideas from other skills: [`math-olympiad`](https://github.com/anthropics/claude-plugins-official/tree/main/plugins/math-olympiad) (interpretation check, fresh-context review), [`competitive-programming-solver`](https://github.com/Prekzursil/agent-skills-toolchain) (stress testing), [`long-form-math`](https://github.com/dashed/claude-marketplace) (words between formulas), [`humanizer`](https://github.com/blader/humanizer) and [`avoid-ai-writing`](https://github.com/conorbronsdon/avoid-ai-writing) (prose style).

No text from those skills is copied here.

## En español

`cp-editorial` escribe editoriales y explica soluciones de programación competitiva. Responde en el idioma del pedido, así que funciona en español aunque la skill esté escrita en inglés. Se instala igual que arriba.

## License

[MIT](LICENSE)
