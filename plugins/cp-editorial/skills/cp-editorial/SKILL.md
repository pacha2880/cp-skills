---
name: cp-editorial
description: Write editorials and explain solutions to competitive programming problems (Codeforces, ICPC, AtCoder, training camps), or clarify an explanation the reader did not understand. Sets length and focus from the problem's difficulty.
---

# Competitive programming editorials and explanations

> **A known tool gets named. A claim about this problem gets justified.**

Test: a reader at the problem's rating can implement the solution from the text alone, without the code. If a step requires reading the code, the text has a gap.

Write in the language of the request. Use LaTeX math with `$...$`.

## 1. Set focus and length from the rating

Use the rating, or the problem's position in the set if there is none.

| Rating | Words | Spend them on |
|---|---|---|
| up to 1399 | 100–215, about 155 | The key observation, why it is enough, the cases. |
| 1400–1899 | 155–400, about 305 | The whole chain of reasoning, with a short reason at each step. |
| 1900–2399 | 210–480, about 355 | The reduction and the key claim with its justification. |
| 2400–2899 | 220–340, about 275 | The one non-obvious insight and the model or stored state. Supporting steps stay brief. |
| 2900+ | 345–915, about 565 | The stages of the solution and their lemmas. |

Words count the visible solution text: no hints, code or folded proofs. Ranges are the middle half of 55 Codeforces editorials from 2022 to 2026, with the mean as the target. Stay in the range and lean toward the mean. A problem with more distinct claims may need the upper end or a little more. Cut anything else first, and never drop a needed claim to fit.

For reference, these tools usually appear named and unexplained from about these ratings. Use judgment, not a rule:

| From about | Named without explanation |
|---|---|
| 1700 | Binary search, prefix sums |
| 2000 | LCA, hashing and Z-function, harmonic sum, DSU, monotonic stack |
| 2400 | Bitmask DP |
| 2700 | Segment tree, bitset |
| 3000 | NTT, min cut, heavy-light decomposition, inclusion-exclusion |

Naming a tool does not cover how this problem uses it. Say what the structure stores, what the graph's nodes and edges are, and what the DP state means.

## 2. Write an editorial

1. **Restate.** Write in one line what is being asked. If there are two readings, pick the one that makes the problem interesting and say so.
2. **Solve first.** Work it out before reading any existing editorial; that shows which steps the existing one skips.
3. **Verify hard problems** (around 2000+, geometry, many edge cases, or on request). Write the C++ solution and an obviously correct brute force, then test on samples and small random cases. When they differ, find which one is wrong before changing the solution. Compute the bounds behind any implementation claim.
4. **List the claims.** Write the claims from statement to algorithm in order, each with its reason. A claim without a reason means you do not understand it yet.
5. **Draft** with the template below.
6. **Audit and delete** with sections 4 and 5. For hard problems, if a subagent tool is available, give a fresh agent only the statement and the text, and ask for every point it could not implement.

### Template

```
Hints: 2 or 3, always; on Codeforces, in spoilers
Idea: the complete solution in 1 or 2 sentences
Why: numbered claims, one idea per step, each with its reason
Detail: only the non-standard implementation points
Complexity
Code: always; same names as the text
```

- **Hints** build toward the solution. They can be questions or statements, and none gives away everything.
- **Idea** comes first in the solution. A reader who stops there should know the algorithm.
- **Slow solution** only when the speed-up is the point of the problem. Otherwise open with the claim.
- **Examples** only when one breaks the naive idea or settles an ambiguity. Use concrete numbers, preferably from a sample.
- **Proofs** follow Codeforces style by default: state the claim with a one-line reason. When the proof is long and the claim is not obvious, put the full proof in a spoiler right after it.

Bad: "Let's think about how the angle affects the masks. First we should consider what happens as the cross rotates."
Good: "Masks change only when a laser passes over an endpoint, so one angle per interval between those moments is enough."

## 3. Explain something to the user

When the user quotes a passage they do not understand:

1. Find the missing link: an undefined symbol, a value without its origin, a claim without its reason, or a detail that only appears in the code. Do not repeat the same explanation in other words.
2. Answer the literal question in the first sentence. If they ask "what is $e_1$?", the first sentence says what $e_1$ is.
3. If the answer is a chain, number it. Add a concrete example if it helps.
4. If the gap is a defect in the editorial, say so and give the corrected paragraph, ready to paste.
5. Stop. Do not add hints, re-explain what they understood or close with a summary.

To explain a whole solution, use the template from section 2.

## 4. Audit checklist

- **Symbols.** Each is defined before use, with the object it belongs to, and keeps one meaning. Say whether a step uses original or transformed data.
- **Values.** Each constant or label has its value and the rule that assigns it. Say when a computed value can be zero, negative or infinite if the implementation must handle it.
- **State.** Each sweep, DP or simulation has an explicit initial state and how to compute it.
- **Edge cases.** Ties, $n = 1$ and answer $-1$ are in the text when they change the algorithm.
- **Claims.** "It is enough to check X" gives the chain: what determines the result, what can change it, when, conclusion. "At most $4n$" gives its derivation. A symmetry says what it preserves and what it swaps.
- **Warnings.** Precision, overflow and tied events appear once, with the number that justifies them.

## 5. Delete before delivering

Delete:

1. Sentences that announce instead of saying ("let's now look at", "here is the key").
2. Explanations of tools that this rating takes as known.
3. Examples that break nothing and settle nothing.
4. Closing lines that repeat what came before, and punchlines.
5. "Clearly", "obviously", "it is easy to see": give the reason or drop the adverb. If the claim is specific to this problem and you cannot give a reason, it is a gap.
6. Em dashes, invented "not X but Y" contrasts, and bold outside section names and real warnings.

Then check the math: words between formulas, no sentence starting with a symbol, short formulas inline, and the formula a step produces on its own line. Use first person plural.

Then count the words against section 1. If over the range, cut supporting detail first.

## 6. When to break these rules

- The user asks for more depth or a full proof: give it, and ignore the word range.
- The problem is unusually rich for its rating: go over the range and keep every claim.
- You have no justified solution: say so and deliver what is proved. A confident wrong answer is worse than an honest partial one.
