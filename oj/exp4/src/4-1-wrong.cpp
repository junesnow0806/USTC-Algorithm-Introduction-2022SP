#include <iostream>
#include <vector>
using namespace std;

const int MY_INFINITY = 10000;
const int NIL = 0;
vector<vector<int>> h;           // h[u][v]MST中u和v路径上权值最大的边的权值
vector<vector<bool>> MST_edges;  // MST_edges[u][v] == true表示边(u, v)在MST中
vector<int> MST_vertex;          // MST中的顶点

struct vertex {
    int pai;
    int key;
    bool inTree;
};

struct edge {
    int u;
    int v;
    int w;
};

int ExtractMin(vector<vertex> &Q) {
    /*
     * 返回Q中key值最小的顶点编号, 并将该顶点加入到MST中
     */
    int n = Q.size() - 1;
    int min_key = MY_INFINITY;
    int min_index = 0;
    for (int i = 1; i <= n; i++) {
        if (Q[i].inTree == false && Q[i].key < min_key) {
            min_key = Q[i].key;
            min_index = i;
        }
    }
    if (min_index > 0) {
        Q[min_index].inTree = true;
        return min_index;
    } else {
        return 0;
    }
}

/*
bool IsEmpty(vector<vertex> &Q) {
    for (int i = 1; i < Q.size(); i++) {
        if (Q[i].inTree == false) {
            return false;
        }
    }
    return true;
}
*/

bool IsEmpty(vector<vertex> &Q) {
    int n = Q.size() - 1;
    return (MST_vertex.size() == n);
}

pair<int, int> Prim(vector<vector<int>> &w, vector<vertex> &Q, vector<edge> &E, vector<vector<int>> &adj) {
    /*
     * 使用Prim算法计算最小生成树和次小生成树的权重
     */
    int n = w.size() - 1;
    Q[1].key = 0;
    int MST_weight = 0;
    while (IsEmpty(Q) == false) {
        int u = ExtractMin(Q);
        MST_weight += Q[u].key;
        int pai = Q[u].pai;
        if (pai != NIL) {
            // 计算树中顶点到新加入的u的路径上的最大边的权值
            for (auto &t : MST_vertex) {
                h[t][u] = (Q[u].key > h[t][pai]) ? Q[u].key : h[t][pai];
                h[u][t] = (Q[u].key > h[pai][t]) ? Q[u].key : h[pai][t];
            }
            /*
            for (int i = 1; i < Q.size(); i++) {
                // 计算树中顶点到u的路径上的最大边的权值
                if (Q[i].inTree == true && i != u) {
                    h[i][u] = (Q[u].key > h[i][pai]) ? Q[u].key : h[i][pai];
                    h[u][i] = (Q[u].key > h[pai][i]) ? Q[u].key : h[pai][i];
                }
            }
            */
        }
        MST_vertex.push_back(u);  // 将u加入到MST中
        MST_edges[u][pai] = true;
        MST_edges[pai][u] = true;
        for (auto v : adj[u]) {
            if (Q[v].inTree == false && w[u][v] < Q[v].key) {
                Q[v].key = w[u][v];
                Q[v].pai = u;
            }
        }
    }
    int sub_MST_weight = MY_INFINITY;
    for (int i = 0; i < E.size(); i++) {
        // 遍历所有不在MST中的边, 考虑将这些边与MST中的边交换
        int u = E[i].u;
        int v = E[i].v;
        // if (E[i].inTree == false) {
        if (MST_edges[u][v] == false) {
            // 这条边不在MST中
            int tmp = MST_weight - h[u][v] + E[i].w;
            if (tmp < sub_MST_weight)
                sub_MST_weight = tmp;
        }
    }
    return make_pair(MST_weight, sub_MST_weight);
}

int main() {
    int n, m;
    cin >> n >> m;
    h.resize(n + 1);
    MST_edges.resize(n + 1);
    vector<vector<int>> w(n + 1);
    vector<vertex> Q(n + 1);
    vector<edge> E;
    vector<vector<int>> adj(n + 1);  // 记录每个顶点的相邻顶点
    for (int i = 0; i < n + 1; i++) {
        h[i].resize(n + 1);
        w[i].resize(n + 1);
        MST_edges[i].resize(n + 1);
    }
    for (int i = 0; i < m; i++) {
        int u, v, w1;
        cin >> u >> v >> w1;
        w[u][v] = w1;
        w[v][u] = w1;
        edge e;
        e.u = u;
        e.v = v;
        e.w = w1;
        E.push_back(e);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (w[i][j] == 0) {
                w[i][j] = MY_INFINITY;
            }
        }
    }
    for (int i = 1; i <= n; i++) {
        Q[i].pai = NIL;
        Q[i].key = MY_INFINITY;
        Q[i].inTree = false;
    }

    pair<int, int> result = Prim(w, Q, E, adj);
    cout << result.first << endl;
    cout << result.second << endl;

    return 0;
}