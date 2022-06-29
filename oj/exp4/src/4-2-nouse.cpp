#include <iostream>
#include <unordered_set>
#include <vector>
using namespace std;

const int NIL = 0;

enum COLOR {
    WHITE,
    GRAY,
    BLACK
};

struct node {
    int id;
    int pai;
    COLOR color;
    vector<int> Adj;
    unordered_set<int> reachable;
    unordered_set<int> traceback;
    node() {}
    node(int id) : id(id), pai(NIL), color(COLOR::WHITE) { reachable.emplace(id); }
};

void DFS_EXPLORE(node &u, vector<node> &V) {
    u.color = COLOR::GRAY;
    for (auto &v : u.Adj) {
        // u的所有邻居都是u的可达节点
        // u的邻居的可达节点也是u的可达节点
        // ! 注意互相可达的顶点有遍历顺序问题
        u.reachable.emplace(v);
        if (V[v].color == COLOR::WHITE) {
            V[v].pai = u.id;
            DFS_EXPLORE(V[v], V);
        }
        if (V[v].color == COLOR::GRAY) {
            // v还没遍历完, u的可达节点还依赖于v的可达节点
            V[v].traceback.emplace(u.id);
        }
        if (V[v].color == COLOR::BLACK) {
            for (auto id : V[v].reachable) {
                u.reachable.emplace(id);
            }
        }
    }
    u.color = COLOR::BLACK;
    for (auto v : u.traceback) {
        for (auto w : u.reachable) {
            V[v].reachable.emplace(w);
        }
    }
}

void DFS(vector<node> &V) {
    for (auto &v : V) {
        if (v.color == COLOR::WHITE)
            DFS_EXPLORE(v, V);
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<node> V(n + 1);
    for (int i = 1; i <= n; i++) {
        node u(i);
        V[i] = u;
    }
    for (int i = 0; i < m; i++) {
        int v1, v2;
        cin >> v1 >> v2;
        V[v1].Adj.push_back(v2);
    }

    DFS(V);

    int q;
    cin >> q;
    vector<int> result;
    while (q--) {
        int u, v;
        cin >> u >> v;
        int flag = false;
        for (auto r : V[u].reachable) {
            if (v == r) {
                flag = true;
                result.push_back(1);
                break;
            }
        }
        if (flag == false) {
            result.push_back(0);
        }
    }
    for (auto res : result) {
        cout << res << endl;
    }
    return 0;
}