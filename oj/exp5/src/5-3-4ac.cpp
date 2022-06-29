#include <iostream>
#include <vector>
using namespace std;

long long extend_gcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    long long gcd = extend_gcd(b, a%b, y, x);
    y = y - a / b * x;
    return gcd;
}

long long extend_gcd(long long a, long long b, int &x, int &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    long long gcd = extend_gcd(b, a%b, y, x);
    y = y - a / b * x;
    return gcd;
}

long long ChinaRemainder(vector<int> &r, vector<int> &p, int k) {
    long long M = 1;
    for (int i = 0; i < k; i++) {
        M *= p[i];
    }

    long long ans = 0;
    // long long s, t, m;
  	int s, t;
  	long long m;
    for (int i = 0; i < k; i++) {
        m = M / p[i];
        // extend_gcd(p[i], m, s, t);
      	extend_gcd(m, p[i], t, s);
        ans = (ans + r[i] * t * m + M) % M;
    }
    return (ans + M) % M;
}

int main() {
    int k;
    cin >> k;
    vector<int> p(k);
    vector<int> r(k);
    for (int i = 0; i < k; i++) {
        cin >> r[i] >> p[i];
    }

    cout << ChinaRemainder(r, p, k) << endl;

    return 0;
}