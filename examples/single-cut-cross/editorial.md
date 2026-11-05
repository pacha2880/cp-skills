# J. The Single-Cut Cross

Problem: [Codeforces Gym 106758 J](https://codeforces.com/gym/106758/problem/J) (2026 ICPC Bolivia National Contest, Phase 2). Estimated difficulty: about 2250.

Written with the `cp-editorial` skill. The solution in `solution.cpp` was checked against a brute force on 75,000 small random cases.

## Hints

<details>
<summary>Hint 1</summary>

Fix the direction of one laser. How can you decide, with one integer operation per endpoint, whether its line cuts the interior of a cable?

</details>

<details>
<summary>Hint 2</summary>

Rotate the cross slowly. At which moments can the set of cables cut by each line change?

</details>

<details>
<summary>Hint 3</summary>

Rotating the cross by $90^\circ$ gives the same two lines.

</details>

## Solution

<details>
<summary>Solution</summary>

**Idea.** Sweep the first laser's angle $\theta$ over $(0^\circ, 90^\circ)$. Cuts change only when a laser passes over an endpoint, which gives one event per endpoint. Sort the events with exact cross products and keep the current value and the number of cables cut twice.

**Why.**

1. **One direction.** Let the first laser have direction $d$. It cuts cable $(A_i, B_i)$ exactly when $\mathrm{cross}(d, A_i)$ and $\mathrm{cross}(d, B_i)$ have opposite signs. The second laser has direction $d^\perp$ and $\mathrm{cross}(d^\perp, P) = -\mathrm{dot}(d, P)$, so it cuts the cable exactly when $\mathrm{dot}(d, A_i)$ and $\mathrm{dot}(d, B_i)$ have opposite signs. Store this in $mask_i$: bit 1 for the first laser, bit 2 for the second. An orientation is valid when no mask is 3, and its value is the sum of $w_i$ over nonzero masks.
2. **Intervals.** A point changes side of a line only when the line passes over it, so masks are constant between consecutive moments when a laser hits an endpoint. Those moments are invalid orientations, so we test one state per open interval.
3. **Quarter turn.** The cross at $\theta + 90^\circ$ has the same two lines with bits 1 and 2 swapped, which preserves validity and value. Hence $\theta \in (0^\circ, 90^\circ)$ is enough.
4. **Events.** For endpoint $P$ of cable $i$, apply $(x, y) \to (y, -x)$ until $x > 0$ and $y \ge 0$, and let $k$ be the number of rotations. If $y = 0$ ($P$ is on an axis), there is no event. Otherwise the rotated point is the event's direction: there, the first laser hits $P$ if $k$ is even ($b = 1$) and the second if $k$ is odd ($b = 2$). Only $P$ switches sides of that laser, so the event does $mask_i \oplus= b$.
5. **Initial state.** Compute the masks at $\theta = 0^+$ from the original coordinates, with $d = (1, \varepsilon)$. The sign of $\mathrm{cross}(d, P)$ is that of $P_y$, or of $-P_x$ if $P_y = 0$. The sign of $\mathrm{dot}(d, P)$ is that of $P_x$, or of $P_y$ if $P_x = 0$.
6. **Sweep.** Keep $sum$, the total weight with mask 1 or 2, and $bad$, the count with mask 3. Evaluate the initial state. Then, for each group of events with equal direction, apply the whole group and evaluate. A state counts only if $bad = 0$. The answer is the largest such $sum$, or $-1$ if there is none.

**Detail.**

- To change a mask, remove the cable's contribution, flip the bit and add it back.
- Directions $u, v$ are less than $90^\circ$ apart, so $u$ comes first when $\mathrm{cross}(u, v) > 0$ and they tie when it is $0$.
- With coordinates up to $10^9$, $|\mathrm{cross}| \le 2 \cdot 10^{18}$ fits in `long long`. Do not use `double`: distinct directions can be closer than rounding error.

**Complexity.** $O(n \log n)$ per test.

</details>
