/* Floyd-WarShall */
#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

const int MY_INFINITY = 268435455;
// const long MY_INFINITY = long(500000) * long(10000);

struct edge {
    int u;
    int v;
    int w;
    edge() : u(0), v(0), w(0) {}
    edge(int u, int v, int w) : u(u), v(v), w(w) {}
};

bool BellmanFord(int s, vector<vector<int>> &W, vector<edge> &E) {
    /*
     * 给定源点s, 如果存在负环就返回false, 否则返回true
     */
    int n = W.size() - 1;
    vector<long> d(n + 1);
    for (int i = 0; i < d.size(); i++) {
        d[i] = MY_INFINITY;
    }
    d[s] = 0;
    for (int i = 1; i <= n - 1; i++) {
        for (auto e : E) {
            if (d[e.u] == MY_INFINITY)
                continue;
            if (d[e.v] > d[e.u] + e.w) {
                d[e.v] = d[e.u] + e.w;
            }
        }
    }
    for (auto e : E) {
        if (d[e.v] > d[e.u] + e.w)
            return false;
    }
    return true;
}

int min(const int &i1, const int &i2) {
    return (i1 < i2) ? i1 : i2;
}

bool Floyd_WarShall(vector<vector<int>> &W, vector<edge> &E, vector<vector<int>> &d, double &time_cost) {

    auto start = steady_clock::now();


    bool ret = BellmanFord(1, W, E);
    if (ret == false) {
        cout << "negative-weight cycle" << endl;
        return false;
    }
    // d = W;
    d.assign(W.begin(), W.end());
    int n = W.size() - 1;
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
            }
        }
    }

    auto end = steady_clock::now();
    time_cost = duration<double, milli>(end-start).count();

    return true;
}

int main() {

    ifstream input("../testcase/4-3-gen.txt");

    int n, m;
    input >> n >> m;
    // cin >> n >> m;
    vector<vector<int>> W(n + 1);
    vector<edge> E;
    for (int i = 0; i < W.size(); i++) {
        W[i].resize(n + 1);
    }
    for (int i = 0; i < m; i++) {
        int u, v, w;
        // cin >> u >> v >> w;
        input >> u >> v >> w;
        W[u][v] = w;
        E.push_back(edge(u, v, w));
    }
    for (int i = 0; i < W.size(); i++) {
        for (int j = 0; j < W.size(); j++) {
            if (i != j && W[i][j] == 0) {
                W[i][j] = MY_INFINITY;
            }
        }
    }
    int q;
    input >> q;
    // cin >> q;
    vector<pair<int, int>> querys;
    while (q--) {
        int u, v;
        input >> u >> v;
        // cin >> u >> v;
        querys.push_back(make_pair(u, v));
    }

    vector<vector<int>> d(n + 1);

    double time_cost = 0.0;

    bool ret = Floyd_WarShall(W, E, d, time_cost);
    if (ret) {
        for (auto query : querys) {
            cout << d[query.first][query.second] << endl;
        }
    }

    cout << "running time: " << time_cost << "ms" << endl;

    input.close();
    return 0;
}