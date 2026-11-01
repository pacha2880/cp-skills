---
name: cp-editorial
description: Write editorials and explain solutions to competitive programming problems (Codeforces, ICPC, AtCoder, training camps), or clarify an explanation the reader did not understand. Adjusts depth to the problem's difficulty.
---

# Competitive programming editorials and explanations

This skill covers four jobs: writing the editorial for a problem, explaining a solution or someone else's editorial to the user, preparing material for a training camp, and solving a problem and then explaining it. All four share one quality test:

> A reader at the target level must be able to implement the solution from the text alone, without looking at the code.

If understanding a step requires reading the code, the text has a gap.

Write in the language of the request. Use LaTeX math with `$...$`, as Codeforces blogs do.

## 1. Pick the level from the difficulty

Choose the reader from the problem's rating (or its position in the set if there is no rating):

| Difficulty | Reader | What changes |
|---|---|---|
| up to ~1400 | green/cyan | Name the prerequisites, explain standard techniques in a sentence or two, give more than one example. |
| ~1500 to ~2100 | cyan/blue | Normal Codeforces style: hints, a slow solution then a fast one, a proof of every key claim, one example. |
| 2200+ | blue/purple | More concise, but every claim is still proved and every symbol is still defined. Concise does not mean skipping steps. |

For camp material, go one level below the table and add a related practice problem if you know one for certain.

## 2. Workflow for writing an editorial

### 2.1 Read the whole problem

Read the statement, constraints, samples and notes. Before solving, write in one line what exactly is being asked. If there are two possible readings, pick the one that makes the problem interesting and say so.

### 2.2 Solve and verify

Work out the solution yourself before reading any existing editorial. That is how you find the steps the existing editorial skips.

If the problem is hard (rating around 2000 or more, geometry, many edge cases, or the user asks for it), verify before writing:

1. Implement the solution in C++.
2. Implement a slow brute force that is obviously correct.
3. Test on the samples and on many small random cases.

The brute force can be wrong too. If it uses `double` in a geometry problem, two equal angles can come out different; use exact arithmetic or a deliberate tolerance. When outputs differ, find out which of the two is wrong before touching the solution.

Also verify the implementation claims you are going to write. Compute the bounds: if you say something needs `__int128`, check that it does not fit in `long long` (with coordinates up to $10^9$, a cross product is bounded by $2 \cdot 10^{18}$ and does fit).

### 2.3 Plan the chain of reasoning

Before drafting, write the list of claims that lead from the statement to the algorithm, in order. Next to each one, write its justification. For example:

1. Claim: "a laser cuts the cable iff the endpoints are on opposite sides of its line". Reason: definition of cutting the interior.
2. Claim: "the state only changes at critical angles". Reason: a point only changes side when the line passes through it.
3. ...

If a claim has no justification, you do not understand it well enough to explain it yet.

### 2.4 Draft

Structure for each problem:

1. **Prerequisites.** Names of the techniques used (angular sweep, cross product, interval DP). A name the reader can search for is worth more than "this is a standard problem".
2. **Hints.** Two to four, written as questions, each one closer to the solution. No hint gives away the whole solution.
3. **Tutorial.**
   - Start from a simple observation or a slow solution (for example $O(n^2)$), then explain how to speed it up. That way the reader sees where each idea comes from.
   - One idea per paragraph. If a paragraph packs three claims, split it.
   - Prove every key claim in the text. Never "it is easy to see", "one can show" or "left as an exercise".
   - Include an example with concrete numbers, preferably one of the samples.
   - End with the complexity.
4. **Code.** Clean, using the same names as the text (if the text says $sum$ and $bad$, so does the code). Comments only where something is not obvious.

On Codeforces, hints, tutorial and code go in separate spoilers.

### 2.5 Audit (do not skip)

Reread the text as the target reader and go through every item in section 4. Mark every place where that reader would ask "what is this?", "why?" or "how is this computed?", and fix it.

For hard problems, if you have a subagent tool, give a fresh agent only the statement and the editorial (not your reasoning and not the code) and ask it to list every point it could not implement.

### 2.6 Style pass

Apply the rules in section 5.

## 3. Workflow for explaining something to the user

When the user quotes a passage and says they do not understand it:

1. **Find the missing link.** It is almost always one of these: an undefined symbol, a value without its origin, a claim without its reason, or a detail that only appears in the code. Do not repeat the same explanation in other words.
2. **Answer exactly what was asked first,** in one or two sentences. If the question is "what is $e_1$?", the first sentence says what $e_1$ is.
3. **If the explanation is a chain, number it.** For example: what determines the state, what can change it, when that happens, conclusion.
4. **Use a concrete example** with numbers from the samples.
5. **If the gap is a defect in the editorial, say so plainly** and give the full corrected paragraph, ready to replace the old one.
6. **Do not over-explain.** Do not add sections they did not ask for, do not re-explain what they already understood, do not close with a summary.

If the user says "you did not answer X", go back to the literal question and answer it in the first sentence.

## 4. Audit checklist

These are the gaps that show up most often in editorials. Check each one.

**Symbols and values**
- Every symbol is defined before it is used, together with the object it belongs to. For example, $A_i, B_i$ are the endpoints of cable $i$, and $e_1 < \dots < e_k$ are the distinct event angles, sorted.
- Every value the algorithm uses says what it is, how it is computed and from which data. If there are transformed data (rotated points, compressed indices, a sorted array), say explicitly whether a step uses the original or the transformed data.
- Every constant or label has its explicit value and the rule that assigns it (for example, "bit 1 for the first line, bit 2 for the second, depending on the parity of $k$").

**State and boundaries**
- In every sweep, DP, simulation or incremental structure, the initial state is defined explicitly, together with how it is computed.
- Edge cases (points on the axes, tied events, $n = 1$, answer $-1$) are handled in the text, not only in the code.
- Never write "this is already included", "this takes care of itself" or "case X is not needed" without saying where and why.

**Claims**
- Every "it is enough to check X" or "nothing changes between A and B" comes with the chain: what determines the result, what can change it, when that happens, conclusion.
- Every count ("there are at most $4n$") comes with its derivation: how many per object and why.
- Every reduction by symmetry says what is preserved and what is swapped.

**Code**
- Every non-obvious idiom gets one sentence of explanation. Example: remove the old contribution, modify, add the new contribution (`add(i,-1); mask[i]^=b; add(i,+1);`).
- Everything the text describes appears in the code under the same name, and vice versa.

## 5. Style

Drawn from well-received Codeforces editorials and from writing skills (`long-form-math`, `humanizer`, `avoid-ai-writing`).

**Mathematics**
- Words between formulas: no chain of equations without text saying why each step follows.
- No sentence starts with a symbol. "The value $x$ ...", not "$x$ ...".
- No "clearly" or "obviously": either give the reason or drop the adverb.
- Short formulas inline; the formula a step produces goes on its own line.
- One letter means one thing throughout the text.

**Prose**
- First person plural ("we compute", "we sweep") and a direct tone.
- No em dashes; use a period, comma, colon or parentheses.
- No invented contrasts of the form "not X but Y" when nobody said X.
- No sentences that announce instead of saying ("let's now look at", "here is the key").
- No closing lines that repeat what came before, and no punchlines.
- Bold only for section names or a real warning.
- No lists of three just for rhythm; list what is actually there.

**Real warnings do belong**: precision (`double` versus integers), overflow with the computed bound, ties that must be grouped. State each once, with the number that justifies it.

## 6. Example of a fix

Before (compressed, full of gaps):

> Events at angle exactly $0$ are already included in this state.

Problems: "this state" is not defined, it does not say how the state is computed, and "included" does not say where.

After:

> It remains to compute the initial state. We compute each cable's mask at $\theta = 0^+$, an angle just above $0$, using the original coordinates of $A_i$ and $B_i$. Take $d = (1, \varepsilon)$ with $\varepsilon > 0$ infinitesimally small. Bit 1 is set if $\mathrm{cross}(d, A_i)$ and $\mathrm{cross}(d, B_i)$ have opposite signs, and bit 2 if $\mathrm{dot}(d, A_i)$ and $\mathrm{dot}(d, B_i)$ do. [...] Endpoints on the axes generate no event, because this state is already computed after their passage through $0$.

## 7. Solving a problem

When the request is to solve:

1. Read the problem and restate the question in one line.
2. Use the constraints to bound the target complexity ($n \le 2 \cdot 10^5$ asks for $O(n \log n)$, $n \le 5000$ allows $O(n^2)$, $n \le 20$ suggests bitmasks).
3. Solve, implement and verify as in 2.2 if the problem is hard.
4. If you do not have a solution you can justify, say so and deliver what is actually proved. A confident wrong answer is worse than an honest partial one.
5. Explain the solution following the rules of this skill.
