#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct P { ll x, y; };
ll cross(P a, P b) { return a.x * b.y - a.y * b.x; }
int sgn(ll v) { return (v > 0) - (v < 0); }

// signs of cross(d, p) and dot(d, p) for d = (1, eps)
int sideFirst(P p)  { return p.y ? sgn(p.y) : sgn(-p.x); }
int sideSecond(P p) { return p.x ? sgn(p.x) : sgn(p.y); }

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t--) {
        int n;
        if (scanf("%d", &n) != 1) return 0;
        vector<ll> w(n);
        vector<int> mask(n);
        vector<tuple<P, int, int>> ev;   // (direction, cable, bit)
        for (int i = 0; i < n; i++) {
            P a, b;
            if (scanf("%lld %lld %lld %lld %lld", &a.x, &a.y, &b.x, &b.y, &w[i]) != 5) return 0;
            mask[i] = (sideFirst(a) * sideFirst(b) < 0)
                    | (sideSecond(a) * sideSecond(b) < 0) << 1;
            for (P p : {a, b}) {
                int k = 0;
                while (!(p.x > 0 && p.y >= 0)) { p = {p.y, -p.x}; k++; }
                if (p.y > 0) ev.push_back({p, i, k % 2 ? 2 : 1});
            }
        }
        sort(ev.begin(), ev.end(), [](auto &u, auto &v) {
            return cross(get<0>(u), get<0>(v)) > 0;
        });

        ll sum = 0; int bad = 0;
        auto add = [&](int i, int s) {   // add (s = +1) or remove (s = -1) cable i
            if (mask[i] == 3) bad += s;
            else if (mask[i]) sum += s * w[i];
        };
        for (int i = 0; i < n; i++) add(i, +1);
        ll ans = bad ? -1 : sum;

        for (size_t l = 0, r; l < ev.size(); l = r) {
            for (r = l; r < ev.size() && cross(get<0>(ev[l]), get<0>(ev[r])) == 0; r++) {
                auto [p, i, bit] = ev[r];
                add(i, -1);
                mask[i] ^= bit;
                add(i, +1);
            }
            if (!bad) ans = max(ans, sum);
        }
        printf("%lld\n", ans);
    }
}
