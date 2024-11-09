#include <climits>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
#define int long long //谁知道 15！int 就爆了啊？
const int maxn = 1e3;
typedef struct node{
    int to;
    int val;
    node(int t,int a) {
        to = t;
        val = a;
    }
} node;
vector<node> adj[maxn]; //邻接表，存储有向图
vector<int> deg(maxn, 0);//top 排序用
vector<int> top(maxn, 0);// top 序列
vector<vector<int>> W;//用于dag 动态规划，W[i][h] 表示到达第 i 个定点 h 步的最大权重和
vector<vector<int>> X;//用于回溯构建路径；
vector<node> graph[maxn];//用于存储无向图

bool top_sort(int n) {
    queue<int> q;
    int cnt = 0;
    int index = 0; // 修改变量名

    // 清空 deg 数组
    fill(deg.begin(), deg.begin() + n, 0);

    for (int i = 0; i < n; ++i) {
        for (auto edge: adj[i]) {
            deg[edge.to]++;
        }
    }

    for (int i = 0; i < n; ++i) {
        if (deg[i] == 0) {
            q.push(i);
            cnt++;
        }
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        top[index++] = u;
        for (auto edge: adj[u]) {
            deg[edge.to]--;
            if (deg[edge.to] == 0) {
                q.push(edge.to);
                cnt++;
            }
        }
    }

    return cnt == n;
}

void init(int n, int k) {
    W.resize(n, vector<int>(k + 1)); 
    X.resize(n, vector<int>(k + 1));
    for (int i = 0; i < n; ++i) {     
        W[i][1] = 0;
        X[i][1] = -1;
        for (int h = 2; h <= k; ++h) {
            W[i][h] = -INT_MAX;
            X[i][h] = -1;
        }
    }
}

int max_dag_path() {
    int max_dag = -INT_MAX; 
    for (int i = 1; i < W.size(); ++i) { 
        for (auto edge: adj[top[i]]) {
            for (int h = 1; h < W[0].size() - 1; ++h) {
                if (W[top[i]][h] != -INT_MAX && 
                    W[top[i]][h] + edge.val > W[edge.to][h + 1]) {
                    W[edge.to][h + 1] = W[top[i]][h] + edge.val;
                    X[edge.to][h + 1] = top[i];
                }
            }
        }
    }
    for (int j = 0; j < W.size(); ++j) {
        max_dag = max(max_dag, W[j][W[0].size() - 1]);
    }
    return max_dag == -INT_MAX ? 0 : max_dag;
}
long long factor(int n) {
    long long res = 1;
    for(int i = 2; i <= n; i++) {
        res *= i;
    }
    return res;
}

//一开始的不是那么好的实现
/*
int main() {
    int m, n, k, u, v, val;
    cin >> n >> m >> k;
    saved_edge.resize(m);
    for (int i = 0; i < m; i++) {
        cin >> u >> v >> val;
        saved_edge[i] = {u,v,val};

    }
    int max_k_path = -INT_MAX;
    int times = 1 * factor(k);

    for (int i = 0; i < times; ++i) {
        for (int i = 0; i < n; ++i) {
            adj[i].clear();
            W.clear();
            X.clear();
        }
        for (auto edge: saved_edge) {
            if (get_random(2) == 1) 
                adj[edge[0]].emplace_back(edge[1], edge[2]);
            else 
                adj[edge[1]].emplace_back(edge[0], edge[2]);
        }
        top_sort(n);
        init(n, k);
        max_k_path = max(max_k_path, max_dag_path());
        //cout << max_k_path << endl;
        //print_path(k);
    }
    cout << max_k_path ;
    return 0;
}
*/

signed main () {
    int n, m, k;
    int from, to, val;
    cin >> n >> m >> k;
    for (int i = 0; i < m; ++i) {
        cin >> from >> to >> val;
        //adj[from].emplace_back(to, val);
        graph[from].emplace_back(to, val);
        graph[to].emplace_back(from, val);
    } 
    /*
    if (!top_sort(n)) cout << "top failed";
    init(n, k);
    cout << max_dag_path() <<  endl;
    */
    vector<int> perm(n);
    for (int i = 0; i < n; ++i) perm[i] = i;
    int times = 50 * factor(k);
    int max_k_path = -INT_MAX;
    for (int i = 0; i < times; ++i) {
        random_shuffle(perm.begin(), perm.end());
        for (int i = 0; i < n; ++i) adj[i].clear();
        if (!top_sort(n)) {
            continue;
        }
        init(n, k);
        for (int u = 0; u < n; ++u) {
            for (auto edge: graph[u]) {
                if (perm[u] > perm[edge.to]) {
                    adj[u].emplace_back(edge.to, edge.val);
                }
            }
       }
        max_k_path = max(max_k_path, max_dag_path());
    }  
    cout << max_k_path << endl;
}