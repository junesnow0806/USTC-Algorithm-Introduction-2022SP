#include <memory.h>
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

#define MAX_VERTEX_NUM 2000
#define MAX_COST 1000

bool visited[MAX_VERTEX_NUM];
int match[MAX_VERTEX_NUM];

struct neighbor {
    int v;
    int c;
    neighbor() : v(0), c(0) {}
    neighbor(int v, int c) : v(v), c(c) {}
};

bool neighbor_cmp(neighbor n1, neighbor n2) {
    return (n1.c < n2.c);
}

bool greedy_dfs(vector<vector<neighbor>> &adj, int u) {
    // 沿交替路做DFS, 看是否存在增广路, 如果存在则返回true, 否则返回false
    int minc = MAX_COST;
    int minv = 0;
    sort(adj[u].begin(), adj[u].end(), neighbor_cmp); // 先对所有邻边按从小到大代价排序
    for (auto nei : adj[u]) {
        if (visited[nei.v] == false) {
            visited[nei.v] = true;
            if (match[nei.v] == 0 || greedy_dfs(adj, match[nei.v])) {
                match[nei.v] = u;
                return true;
            }
        }
    }
    return false;
}

int Hungarian(vector<vector<neighbor>> &adj, int s) {
    int ans = 0;
    for (int i = 1; i <= s; i++) {
        memset(visited, 0, sizeof(visited));
        if (greedy_dfs(adj, i)) {
            ans++;
        }
    }
    return ans;
}

int main() {
    int s, t, m;
    cin >> s >> t >> m;
    vector<vector<neighbor>> adj(s + t + 1);
    vector<vector<int>> C(s+t+1);
    for (int i = 0; i < C.size(); i++) {
        C[i].resize(s+t+1);
    }
    while (m--) {
        int u, v, c;
        cin >> u >> v >> c;
        C[u][v] = c;
        C[v][u] = c;
        adj[u].push_back(neighbor(v, c));
        adj[v].push_back(neighbor(u, c));
    }
    memset(match, 0, sizeof(match));
    int match_count = Hungarian(adj, s);
    int min_cost = 0;
    for (int i = s + 1; i <= s + t; i++) {
        if (match[i] > 0) {
            min_cost += C[match[i]][i];
        }
    }
    cout << match_count << " " << min_cost << endl;
}