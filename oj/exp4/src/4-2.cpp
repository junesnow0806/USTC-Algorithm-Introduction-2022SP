#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <vector>
using namespace std;

const int NIL = 0;

vector<bool> visited;

struct node {
    int id;
    int d;
    int f;
    int pai;
    int cno;  // component number
    vector<int> Adj;
    node() : id(0), d(0), f(0), pai(NIL), cno(0) {}
    node(int id) : id(id), d(0), f(0), pai(NIL), cno(0) {}
    bool operator() (node n1, node n2) { 
        return (n1.f < n2.f);
    }
};

void DFS_EXPLORE(node &u, int &t, vector<node> &V) {
    t++;
    u.d = t;
    visited[u.id] = true;
    for (auto &v : u.Adj) {
        if (visited[v] == false) {
            V[v].pai = u.id;
            DFS_EXPLORE(V[v], t, V);
        }
    }
    t++;
    u.f = t;
}

void DFS(vector<node> &V) {
    int t = 0;
    for (int i = 1; i < V.size(); i++) {
        if (visited[V[i].id] == false) {
            DFS_EXPLORE(V[i], t, V);
        }
    }
}

struct cmp {
    bool operator()(node v1, node v2) {
        return v1.f < v2.f;  // 以f值为key的大根堆
    }
};

bool compare(const node &n1, const node &n2) {
    return (n1.f > n2.f);
}

void DFS_EXPLORE_REVERSR(node &u, vector<node> &V, int cno) {
    visited[u.id] = true;
    u.cno = cno;
    for (auto &v : u.Adj) {
        if (visited[v] == false) {
            V[v].pai = u.id;
            DFS_EXPLORE_REVERSR(V[v], V, cno);
        }
    }
}

void DFS_REVERSR(vector<node> &V, vector<node> &reverseV) {
    // 需要按原图G中的节点f值降序执行DFS，使用优先队列
    // !注意优先队列中的节点都只是V的一份copy！！
    vector<node> nodes;
    for (int i = 1; i < V.size(); ++i) {
        nodes.push_back(V[i]);
    }
    sort(nodes.begin(), nodes.end(), compare);

    for (int i = 0; i < visited.size(); i++) {
        visited[i] = false;
    }

    int cno = 1;

    for (auto v : nodes) {
        if (visited[v.id] == false) {
            // ! DFS_EXPLORE_REVERSR(v, reverseV, cno);
            DFS_EXPLORE_REVERSR(reverseV[v.id], reverseV, cno);
            cno++;
        }
    }
}

vector<bool> component_visited(1050);
bool COMPONENT_DFS(vector<unordered_set<int>> &component_Adj, int ucno, int vcno) {
    // 将强连通分量作为点来进行DFS, 判断u是否可达v
    component_visited[ucno] = true;
    for (auto cno : component_Adj[ucno]) {
        if (vcno == cno) {
            return true;
        }
        if (component_visited[cno] == false) {
            bool result = COMPONENT_DFS(component_Adj, cno, vcno);
            if (result == true) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<node> V(n + 1);
    vector<node> reverseV(n + 1);
    vector<pair<int, int>> E;
    visited.resize(n + 1);  // 自动初始化为false
    for (int i = 1; i <= n; i++) {
        node u(i);
        V[i] = u;
        reverseV[i] = u;
    }
    for (int i = 0; i < m; i++) {
        int v1, v2;
        cin >> v1 >> v2;
        V[v1].Adj.push_back(v2);

        // edge
        E.push_back(make_pair(v1, v2));

        // reverse G
        reverseV[v2].Adj.push_back(v1);
    }

    // 计算图的强连通分量
    DFS(V);
    DFS_REVERSR(V, reverseV);

    // 创建分量图
    // components[i]表示i号连通分量的邻接分量
    // !注意是reverseV中的cno最后才是有效的，V中的cno无效
    // !注意考虑连通分量间接可达的情况
    vector<unordered_set<int>> component_Adj(1050);
    for (auto edge : E) {
        int u = edge.first;
        int v = edge.second;
        if (reverseV[u].cno != reverseV[v].cno) {
            component_Adj[reverseV[u].cno].emplace(reverseV[v].cno);
        }
    }

    int q;
    cin >> q;
    vector<int> result;
    while (q--) {
        int u, v;
        cin >> u >> v;
        if (reverseV[u].cno == reverseV[v].cno) {
            result.push_back(1);
        } else {
            int flag = false;
            for (auto adj_cno : component_Adj[reverseV[u].cno]) {
                if (reverseV[v].cno == adj_cno) {
                    flag = true;
                    result.push_back(1);
                    break;
                }
            }
            if (flag == false) {
                if (COMPONENT_DFS(component_Adj, reverseV[u].cno, reverseV[v].cno)) {
                    result.push_back(1);
                } else {
                    result.push_back(0);
                }
                for (int i = 0; i < component_visited.size(); i++) {
                    component_visited[i] = false;
                }
            }
        }
    }
    for (auto &res : result) {
        cout << res << endl;
    }
    return 0;
}