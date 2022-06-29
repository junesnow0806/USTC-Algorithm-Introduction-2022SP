#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
using ll = long long;

struct Fenwick {
    const int n;
    std::vector<int> tr;
    Fenwick(int n) : n(n), tr(n + 1, 0) {}
    void add(int x, int v) {
        for (int i = x; i <= n; i += i & -i) {
            tr[i] += v;
        }
    }
    int sum(int x) {
        int res = 0;
        for (int i = x; i; i -= i & -i) {
            res += tr[i];
        }
        return res;
    }
};

int main()
{
    int n; cin >> n;

    std::vector<int>a(n);

    for(int i = 0; i < n; ++i) cin >> a[i];

    Fenwick fen(n);

    auto s = a;
    sort(s.begin(), s.end());
    auto q = [&](int x) {
        return lower_bound(s.begin(), s.end(), x) - s.begin() + 1;
    };

    std::vector<int>G(n);
    for(int i = 0; i < n; ++i) {
        int x = q(a[i]);
        G[i] = fen.sum(n) - fen.sum(x);
        fen.add(x, 1);
    }

    Fenwick fen1(n);
    std::vector<int>L(n);
    for(int i = n - 1; i >= 0; --i){
        int x = q(a[i]);
        L[i] = fen1.sum(x - 1);
        fen1.add(x, 1);
    }

    ll res = 0;
    for(int i = 0; i < n; ++i) {
        res += 1ll * G[i] * (G[i] - 1) / 2 - 1ll * L[i] * G[i];
    }
    
    cout << res << endl;

    return 0;
}
