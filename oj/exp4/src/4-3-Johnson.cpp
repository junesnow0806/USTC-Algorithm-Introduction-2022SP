/* Johnson */
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <chrono>
using namespace std;
using namespace std::chrono;

const int MY_INFINITY = 268435455;
// const int MY_INFINITY = 2147483646;
const int NIL = -1;

struct edge {
    int u;
    int v;
    int w;
    edge() : u(0), v(0), w(0) {}
    edge(int u, int v, int w) : u(u), v(v), w(w) {}
};

struct node {
    int id;
    int pai;
    vector<int> Adj;
    node() : id(-1), pai(NIL) {}
    node(int id) : id(id), pai(NIL) {}
};

int min(const int &i1, const int &i2) {
    return (i1 < i2) ? i1 : i2;
}

int find_mind_inQ(unordered_set<int> &Q, vector<int> &d) {
    int min = MY_INFINITY+1;
    int min_index = -1;
    for (auto index : Q) {
        if (d[index] < min) {
            min = d[index];
            min_index = index;
        }
    }
    return min_index;
}

bool BellmanFord(int s, vector<vector<int>> &W, vector<edge> &E, vector<int> &d) {
    /*
     * 给定源点s, 如果存在负环就返回false, 否则返回true
     * 将从源点s出发到其他顶点的最短距离记录在d数组中
     */
    int n = W.size(); // 注意顶点数包括0号
    for (int i = 0; i < d.size(); i++) {
        d[i] = MY_INFINITY;
    }
    d[s] = 0;
    for (int i = 1; i <= n - 1; i++) { // 执行n-1次
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

void Dijkstra(int s, vector<vector<int>> &W, vector<node> &V, vector<int> &d) {
    int n = W.size()-1; // 计算的是1~n号顶点
    for (int i = 1; i <= n; i++) {
        d[i] = MY_INFINITY;
    }
    d[s] = 0;

    unordered_set<int> Q;
    for (int i = 1; i <= n; i++) { // 将1~n号顶点加入Q
        Q.emplace(i);
    }

    while (!Q.empty()) {
        int u = find_mind_inQ(Q, d);
        Q.erase(u);
        for (auto v : V[u].Adj) {
            if (d[v] > d[u] + W[u][v] && d[u] != MY_INFINITY) {
                d[v] = d[u] + W[u][v];
            }
        }
    }
}

bool Johnson(vector<vector<int>> &W, vector<node> &V, vector<edge> &E, vector<vector<int>> &D, double &time_cost) {

    auto start = steady_clock::now();


    int n = W.size()-1;
    // 从0号顶点出发的所有边的权重都为0
    for (int j = 0; j < W.size(); j++) {
        W[0][j] = 0;
    }

    vector<int> h(n+1);
    vector<vector<int>> W1(W);
    bool ret = BellmanFord(0, W, E, h); // 计算从0号顶点出发到其他所有顶点的最短距离, 记录在h
    if (!ret) {
        cout << "negative-weight cycle" << endl;
        return false;
    }

    for (int i = 0; i < E.size(); i++) {
        int u = E[i].u, v = E[i].v;
        E[i].w += h[u] - h[v];
        W1[u][v] += h[u] - h[v];
    }
    vector<int> d(n+1);
    for (int u = 1; u <= n; u++) {
        Dijkstra(u, W1, V, d);
        for (int v = 1; v <= n; v++) {
            D[u][v] = d[v] - h[u] + h[v];
        }
    }

    auto end = steady_clock::now();
    time_cost = duration<double, milli>(end-start).count();

    return true;
}


int main() {

    ifstream input("../testcase/4-3-gen.txt");
    
    int n, m;
    // cin >> n >> m;
    input >> n >> m;

    vector<vector<int>> W(n + 1);
    vector<edge> E;
    vector<node> V(n+1);
    for (int i = 0; i <= n; i++) {
        W[i].resize(n + 1);
        V[i] = node(i);
    }
    for (int i = 0; i < m; i++) {
        int u, v, w;
        input >> u >> v >> w;
        // cin >> u >> v >> w;
        W[u][v] = w;
        E.push_back(edge(u, v, w));
        V[u].Adj.push_back(v);
    }
    for (int i = 0; i < W.size(); i++) {
        for (int j = 0; j < W.size(); j++) {
            if (i != j && W[i][j] == 0) {
                W[i][j] = MY_INFINITY;
            }
        }
    }
    for (int i = 1; i <= n; i++) {
        E.push_back(edge(0, i, 0));
    }

    int q;
    // cin >> q;
    input >> q;
    vector<pair<int, int>> querys;
    while (q--) {
        int u, v;
        input >> u >> v;
        // cin >> u >> v;
        querys.push_back(make_pair(u, v));
    }

    vector<vector<int>> D(n + 1);
    for (int i = 0; i < D.size(); i++) {
        D[i].resize(n+1);
    }
    
    double time_cost = 0.0;

    bool ret = Johnson(W, V, E, D, time_cost);
    if (ret) {
        for (auto query : querys) {
            cout << D[query.first][query.second] << endl;
        }
    }

    cout << "running time: " << time_cost << "ms" << endl;

    input.close();
    return 0;
}