# J. The Single-Cut Cross

Problem: [Codeforces Gym 106758 J](https://codeforces.com/gym/106758/problem/J) (2026 ICPC Bolivia National Contest, Phase 2). Estimated difficulty: about 2250.

This editorial was written with the `cp-editorial` skill and then revised through the questions a reader asked about a first draft. The solution in `solution.cpp` was checked against a brute force on 75,000 small random cases.

## Prerequisites

Cross and dot product, angular sweep, exact (integer) angle comparison.

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

## Tutorial

<details>
<summary>Tutorial</summary>

**One fixed orientation.** Let $d = (d_x, d_y)$ be the direction of the first laser; the second one has direction $d^\perp = (-d_y, d_x)$. Cable $i$ goes from $A_i$ to $B_i$. The sign of $\mathrm{cross}(d, P)$ tells on which side of the first line the point $P$ lies, so the first line cuts the interior of cable $i$ exactly when $\mathrm{cross}(d, A_i)$ and $\mathrm{cross}(d, B_i)$ have opposite signs. Neither is zero, because in a valid orientation no line passes through an endpoint. For the second line we do not need $d^\perp$: since $\mathrm{cross}(d^\perp, P) = -\mathrm{dot}(d, P)$, it is enough to compare the signs of $\mathrm{dot}(d, A_i)$ and $\mathrm{dot}(d, B_i)$.

We give each cable a two-bit mask: bit 1 is set if the first line cuts it, and bit 2 if the second line cuts it. The orientation is valid if no cable has mask 3, and its value is the sum of $w_i$ over cables with mask 1 or 2. Evaluating one direction takes $O(n)$.

**Which directions to try.** A line through the origin with direction $\theta$ is the same line as the one with direction $\theta + 180^\circ$, so it is enough to look at $\theta \in [0^\circ, 180^\circ)$.

Call an angle *critical* if one of the two lasers passes through some endpoint. Take an endpoint $P$. Exactly one line passes through the origin and $P$, so in $[0^\circ, 180^\circ)$ the first laser passes through $P$ at exactly one angle (when it points at $P$), and the second laser at exactly one other angle (when the first laser is perpendicular to $OP$). Each of the $2n$ endpoints gives two critical angles, so there are at most $4n$ of them. Some may coincide.

Now take two consecutive critical angles $c_1 < c_2$. We claim that every $\theta$ strictly between them gives the same masks:

1. Whether a laser cuts cable $i$ depends only on which side of its line each of $A_i$ and $B_i$ lies.
2. A fixed point can only switch sides of a rotating line at the moment the line passes over it.
3. The moments when a laser passes over an endpoint are, by definition, the critical angles.
4. There is no critical angle strictly between $c_1$ and $c_2$, so no endpoint switches sides, so no mask changes.

Hence one angle per interval is enough, which already gives an $O(n^2)$ solution.

*Example.* In the first sample the critical angles are $14.0^\circ, 18.4^\circ, 26.6^\circ, 63.4^\circ, 71.6^\circ, 76.0^\circ, 104.0^\circ, 108.4^\circ, 116.6^\circ, 153.4^\circ, 161.6^\circ, 166.0^\circ$. Every $\theta$ between $63.4^\circ$ and $71.6^\circ$ cuts cables 2 and 3 with the first laser and nothing with the second, for a total of $11$. The orientation in the statement's note, $y = \tfrac{5}{2}x$, has $\theta \approx 68.2^\circ$ and lies in that interval.

**Folding to a quarter turn.** The cross at angle $\theta + 90^\circ$ consists of the same two lines as the cross at $\theta$, with roles swapped. Masks 1 and 2 swap, and neither validity nor value changes, so we only need $\theta \in [0^\circ, 90^\circ)$. Modulo $90^\circ$, both critical angles of an endpoint $P$ become the same one, $\theta \equiv \arg P \pmod{90^\circ}$, so each endpoint produces exactly one *event* in the quarter turn.

We compute events without angles. Rotate $P$ by $-90^\circ$, that is $(x, y) \to (y, -x)$, until it lands in the region $x > 0,\ y \ge 0$, and count the rotations $k$. The event's direction is the rotated point. Its bit $b$ tells which laser passes through $P$ at that angle: at $\theta = \arg P - 90^\circ k$, the line through $P$ has direction $\theta + 90^\circ k$, which is the first laser if $k$ is even ($b = 1$) and the second if $k$ is odd ($b = 2$). The rotation is only used to build events; everything else uses the original coordinates.

**What an event does.** An event comes from one endpoint of some cable $i$; say it is $A_i$, with bit $b$. The laser of bit $b$ passes through $A_i$ and not through $B_i$, so $A_i$ switches sides of that line and $B_i$ does not. Hence "laser $b$ cuts cable $i$" flips, and we do `mask[i] ^= b`; the other bit is unchanged. If $B_i$ has an event at the same direction with the same $b$ (the cable lies on a line through the origin), the two XORs cancel, which is correct: that laser can never cut that cable.

**Initial state.** We compute each cable's mask at $\theta = 0^+$, an angle just above $0$, using the original coordinates of $A_i$ and $B_i$. We do not use $\theta = 0$ because there may be endpoints on the axes, and a laser would pass through them. Take $d = (1, \varepsilon)$ with $\varepsilon > 0$ infinitesimally small. Bit 1 is set if $\mathrm{cross}(d, A_i)$ and $\mathrm{cross}(d, B_i)$ have opposite signs, and bit 2 if $\mathrm{dot}(d, A_i)$ and $\mathrm{dot}(d, B_i)$ do. For an endpoint $P$:

- $\mathrm{cross}(d, P) = P_y - \varepsilon P_x$ has the sign of $P_y$, or of $-P_x$ if $P_y = 0$;
- $\mathrm{dot}(d, P) = P_x + \varepsilon P_y$ has the sign of $P_x$, or of $P_y$ if $P_x = 0$.

Neither is zero, because no endpoint is the origin. In other words, bit 1 asks whether the cable crosses the $x$-axis and bit 2 whether it crosses the $y$-axis, with the tie rule above for endpoints lying on an axis. Endpoints on the axes generate no event (rotated point with $y = 0$), because this state is already computed after their passage through $0$.

For example, cable $(3, 0)$ to $(-1, 2)$: for bit 1 the signs are those of $-3$ and $2$, opposite; for bit 2 they are those of $3$ and $-1$, opposite. Its initial mask is 3.

**The sweep.** Sort the events by direction and group those with exactly the same direction; let $e_1 < e_2 < \dots < e_k$ be the distinct directions. We maintain two numbers: $sum$, the total weight of cables with mask 1 or 2, and $bad$, the number of cables with mask 3, both initialized from the initial masks. We process groups in order, applying every event of a group before evaluating, because they happen at the same angle. After group $e_j$, the state describes the interval $(e_j, e_{j+1})$, or $(e_k, 90^\circ)$ for the last group, and if $bad = 0$ we update the answer with $sum$. The initial state describes $(0^\circ, e_1)$, or all of $(0^\circ, 90^\circ)$ if there are no events, so every interval is evaluated. If none is valid, the answer is $-1$.

To update $sum$ and $bad$ when a mask changes, we remove the cable's old contribution, change the mask, and add the new contribution. This avoids handling each transition (0 to 1, 1 to 3, 3 to 2, ...) separately.

**Exact comparison.** Two points $u, v$ in the region $x > 0,\ y \ge 0$ are less than $90^\circ$ apart, so $u$ comes before $v$ exactly when $\mathrm{cross}(u, v) > 0$, and they have the same direction when it is $0$. Coordinates are at most $10^9$ in absolute value, so $|\mathrm{cross}| \le 2 \cdot 10^{18}$, which fits in `long long`. Do not use `double`: the tests contain distinct directions closer than rounding error, and equal directions that come out different in floating point.

Complexity: $O(n \log n)$ per test, dominated by the sort.

</details>

## Code

<details>
<summary>Solution</summary>

See [`solution.cpp`](solution.cpp).

</details>
