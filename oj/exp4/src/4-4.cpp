#include <iostream>
#include <queue>
#include <vector>
#include <chrono>
#include <fstream>
using namespace std;
using namespace std::chrono;

const int MY_INFINITY = 1000000;

struct vertex {
    int id;
    vector<int> adj;
    vertex() : id(0) {}
    vertex(int id) : id(id) {}
};

struct edge {
    int u;
    int v;
    int c;
    int f;
    edge() : u(0), v(0), c(0), f(0) {}
    edge(int u, int v, int c) : u(u), v(v), c(c), f(0) {}
};

int min(int a, int b) {
    return (a < b) ? a : b;
}

bool BFS(vector<vector<int>> &Cf, vector<vertex> &V, int s, int t, vector<int> &pai) {
    /*
     * BFS搜索Gf, 如果存在一条从s到t的增广路径
     * 如果存在就返回true, 否则返回false
     * 这样找到的路径是最短的(边数最少)
     * 将路径上每个顶点的前驱记录在数组pai中
     */
    int n = V.size() - 1;
    vector<bool> visited(n + 1);

    queue<int> Q;
    Q.push(s);
    visited[s] = true;
    pai[s] = 0;

    while (!Q.empty()) {
        int u = Q.front();
        Q.pop();
        // for (auto v : V[u].adj) {
        for (int v = 1; v <= n; v++) {
            if (!visited[v] && Cf[u][v] > 0) {
                // v还没有访问过且从u到v的边上有剩余容量
                Q.push(v);
                pai[v] = u;
                visited[v] = true;
            }
        }
    }

    return visited[t] == true;  // 如果存在从s到t的一条增广路径就返回true
}

bool DFS(vector<vector<int>> &Cf, vector<int> &pai, int s, int t) {
    /**
     * @brief DFS找可push流量最多的增广路径, 如果找到就返回true, 否则返回false
     * @param Cf 剩余流量矩阵
     * @param s source
     * @param t sink
     * @return bool
     */

    int n = Cf.size() - 1;
    vector<bool> visited(n + 1);

    vector<int> stack;
    stack.push_back(s);
    visited[s] = true;
    pai[s] = 0;

    while (!stack.empty()) {
        int u = stack.back();
        int flag = false;
        int max_v = 0;
        int max_edge_flow = 0;
        for (int v = 1; v <= n; v++) {
            // greedy: 选择剩余容量最大的
            if (!visited[v] && Cf[u][v] > max_edge_flow) {
                max_v = v;
                max_edge_flow = Cf[u][v];
            }
        }
        if (max_v > 0) {
            flag = true;
            visited[max_v] = true;
            pai[max_v] = u;
            stack.push_back(max_v);
            if (max_v == t) {
                break;
            }
        }
        if (!flag) {
            stack.pop_back();
        }
    }

    return visited[t] == true;
}

int FordFulkerson(vector<vertex> &V, vector<vector<int>> &C, int s, int t, int mode, double &time_cost, int &path_count) {
    // while exists aumenting paths in the residual network Gf do
    //      Find the aumenting path P with the largest residual capacity
    //      Push flow f on P

    auto start = steady_clock::now();

    int n = V.size() - 1;
    vector<vector<int>> Cf(C);
    vector<int> pai(n + 1);
    int maxflow = 0;

    bool ret = false;
    if (mode == 0) {
        ret = BFS(Cf, V, s, t, pai);
    } else if (mode == 1) {
        ret = DFS(Cf, pai, s, t);
    }

    while (ret) {

        path_count++;

        // 找到瓶颈边的流量
        int pathflow = MY_INFINITY;
        for (int v = t; v != s; v = pai[v]) {
            int u = pai[v];
            pathflow = min(pathflow, Cf[u][v]);
        }

        // 更新剩余容量
        for (int v = t; v != s; v = pai[v]) {
            int u = pai[v];
            Cf[u][v] -= pathflow;
            Cf[v][u] += pathflow;
        }

        maxflow += pathflow;

        if (mode == 0) {
            ret = BFS(Cf, V, s, t, pai);
        } else if (mode == 1) {
            ret = DFS(Cf, pai, s, t);
        }
    }

    auto end = steady_clock::now();
    time_cost = duration<double, milli>(end-start).count();


    return maxflow;
}

int main(int argc, char* argv[]) {

    fstream input("../testcase/4-4-gen.txt");

    int n, m;
    input >> n >> m;
    // cin >> n >> m;
    vector<vertex> V(n + 1);
    vector<edge> E;
    vector<vector<int>> C(n + 1);
    vector<vector<int>> F(n + 1);
    for (int i = 0; i < C.size(); i++) {
        C[i].resize(n + 1);
        F[i].resize(n + 1);
        V[i] = vertex(i);
    }
    while (m--) {
        int u, v, c;
        input >> u >> v >> c;
        // cin >> u >> v >> c;
        E.push_back(edge(u, v, c));
        V[u].adj.push_back(v);
        C[u][v] = c;
    }
    int s, t;
    input >> s >> t;
    // cin >> s >> t;

    int mode = 0; // 0表示使用最短路径贪心, 1表示使用最快路径贪心
    mode = atoi(argv[1]);
    double time_cost = 0.0;
    int path_count = 0;
    if (mode == 0) {
        cout << "***** shortest greedy *****" << endl;
    } else if (mode == 1) {
        cout << "***** largest greedy *****" << endl;
    }
    cout << FordFulkerson(V, C, s, t, mode, time_cost, path_count) << endl;

    cout << "time: " << time_cost << " ms" << endl;
    cout << "number of augmenting paths: " << path_count << endl;

    return 0;
}