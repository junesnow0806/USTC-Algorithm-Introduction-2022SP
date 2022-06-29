#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

const int MY_INFINITY = 2147483640;

struct edge {
    int u;
    int v;
    int w;
    edge() : u(0), v(0), w(0) {}
    edge(int u, int v, int w) : u(u), v(v), w(w) {}
};

void Karp(vector<edge> &E, int n, int s) {
    vector<vector<int>> F(n+1); // F[k][v] denotes the shortest k-length path from s to v
    for (int i = 0; i < n + 1; i++) {
        F[i].resize(n+1);
    }
    for (int i = 0; i < n+1; i++) {
        for (int j = 0; j < n+1; j++) {
            F[i][j] = MY_INFINITY;
        }
    }
    F[0][s] = 0;

    // n*m relaxations like Bellman-Ford
    for (int k = 1; k <= n; k++) {
        for (auto e : E) {
            int u = e.u;
            int v = e.v;
            int w = e.w;
            if (F[k-1][u] < MY_INFINITY && F[k][v] > F[k-1][u] + w) {
                F[k][v] = F[k-1][u] + w;
            }
        }
    }

    // now F has been done
    double min = MY_INFINITY * 1.0;
    for (int v = 1; v <= n; v++) {
        double max = 0.0;
        for (int k = 0; k < n; k++) {
            if ((F[n][v] - F[k][v]) * 1.0 / (n-k) > max) {
                max = (F[n][v] - F[k][v]) * 1.0 / (n-k);
            }
        }
        if (max < min) {
            min = max;
        }
    }

    cout << fixed << setprecision(3) << min << endl;
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<edge> E;
    while (m--) {
        int u, v, w;
        cin >> u >> v >> w;
        E.push_back(edge(u, v, w));
    }

    int s = 1;
    Karp(E, n, s);

    return 0;
}