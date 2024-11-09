#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

const int MAXN = 1005;
const int INF = 0x3f3f3f3f;

int n, m;
int graph[MAXN][MAXN];  // 原图
int rGraph[MAXN][MAXN]; // 残余网络
vector<int> parent;     // BFS使用的父节点数组

// 使用BFS查找增广路径
bool bfs(int s, int t) {
    vector<bool> visited(n + 1, false);
    parent.assign(n + 1, -1);
    
    queue<int> q;
    q.push(s);
    visited[s] = true;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int v = 1; v <= n; v++) {
            if (!visited[v] && rGraph[u][v] > 0) {
                parent[v] = u;
                visited[v] = true;
                q.push(v);
                if (v == t) return true;
            }
        }
    }
    return false;
}

// 计算从s到t的最大流
int maxFlow(int s, int t) {
    // 初始化残余网络
    memcpy(rGraph, graph, sizeof(graph));
    
    int max_flow = 0;
    
    // 当还存在增广路径时
    while (bfs(s, t)) {
        // 找到这条路径上的最小残余容量
        int path_flow = INF;
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }
        
        // 更新残余网络
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }
        
        max_flow += path_flow;
    }
    return max_flow;
}

int main() {
    memset(graph, 0, sizeof(graph));
    
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        int from, to, val;
        cin >> from >> to >> val;
        graph[from][to] += val;
        graph[to][from] += val;  // 无向图
    }
    
    // 计算所有顶点对之间的最小割
    int min_cut = INF;
    for (int s = 1; s <= n; s++) {
        for (int t = s + 1; t <= n; t++) {
            min_cut = min(min_cut, maxFlow(s, t));
        }
    }
    
    cout << min_cut << endl;
    return 0;
}