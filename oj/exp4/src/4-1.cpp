#include <iostream>
#include <unordered_set>
#include <vector>
using namespace std;

const int MY_INFINITY = 1000000;
const int NIL = 0;

struct edge {
    int u;
    int v;
    int w;
    bool inMST;
    edge() : u(0), v(0), w(MY_INFINITY), inMST(false) {}
    edge(int u, int v, int w) : u(u), v(v), w(w), inMST(false) {}
};

int ExtractMin(unordered_set<int> &Q, vector<int> &key) {
    int minv = 0;
    int minkey = MY_INFINITY;
    for (auto v : Q) {
        if (key[v] < minkey) {
            minv = v;
            minkey = key[v];
        }
    }
    Q.erase(minv);
    return minv;
}

pair<int, int> Prim(vector<vector<int>> &W, vector<edge> &E) {
    int n = W.size() - 1;
    unordered_set<int> Q;  // 保存未加入MST的顶点
    for (int i = 1; i <= n; i++) {
        Q.emplace(i);
    }

    vector<int> key(n + 1);
    vector<int> pai(n + 1);
    for (int i = 1; i <= n; i++) {
        key[i] = MY_INFINITY;
        pai[i] = NIL;
    }
    key[1] = 0;  // 以1号顶点为根

    int MST_weight = 0;
    unordered_set<int> T;                // 已经加入到MST中的顶点集合
    vector<vector<int>> T_edges(n + 1);  // 已经加入到MST中的边集合, 便于直接根据u和v查找该边是否在MST中和更新
    vector<vector<int>> h(n + 1);        // h[u][v]记录在MST中u和v路径上最大边的权值
    for (int i = 0; i <= n; ++i) {
        h[i].resize(n + 1);
        T_edges[i].resize(n + 1);
    }
    while (!Q.empty()) {
        int u = ExtractMin(Q, key);  // u是新加入到MST的新顶点
        int v = pai[u];
        if (v != NIL) {
            // 计算目前MST中所有顶点到新加入顶点的路径上最大边的权值
            for (auto t : T) {
                h[t][u] = (key[u] > h[t][v]) ? key[u] : h[t][v];
                h[u][t] = (key[u] > h[v][t]) ? key[u] : h[v][t];
            }
        }
        MST_weight += key[u];
        T.emplace(u);
        T_edges[u][v] = true;
        T_edges[v][u] = true;

        // 更新不在MST中顶点的key值
        for (int t = 1; t <= n; t++) {
            if (W[u][t] < MY_INFINITY && Q.find(t) != Q.end() && W[u][t] < key[t]) {
                key[t] = W[u][t];
                pai[t] = u;
            }
        }
    }

    // 计算次小生成树的权值
    int sub_MST_weight = MY_INFINITY;
    // 遍历所有不在MST中的边, 考虑将这些边与MST中的边交换
    for (auto edge : E) {
        int u = edge.u;
        int v = edge.v;
        // if (edge.inMST == false) { ! edge的inMST不方便更新
        if (T_edges[u][v] == false) {
            int after_change = MST_weight - h[u][v] + edge.w;
            if (after_change < sub_MST_weight) {
                sub_MST_weight = after_change;
            }
        }
    }

    return make_pair(MST_weight, sub_MST_weight);
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> W(n + 1);
    vector<edge> E;
    for (int i = 0; i <= n; i++) {
        W[i].resize(n + 1);
    }
    while (m--) {
        int u, v, w;
        cin >> u >> v >> w;
        W[u][v] = w;
        W[v][u] = w;
        E.push_back(edge(u, v, w));
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (i != j && W[i][j] == 0) {
                W[i][j] = MY_INFINITY;
            }
        }
    }

    pair<int, int> result = Prim(W, E);
    cout << result.first << endl;
    cout << result.second << endl;

    return 0;
}