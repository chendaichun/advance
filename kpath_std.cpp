#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

const int MAXN = 1005;
vector<pair<int, int>> adj[MAXN];
int dp[MAXN][MAXN];  // dp[i][j] 表示从起点到i，长度为j的最大路径值
int pre[MAXN][MAXN]; // 记录路径的前驱节点

void printPath(int end_node, int k) {
    vector<int> path;
    int curr = end_node;
    int len = k;
    while (len > 0) {
        path.push_back(curr);
        curr = pre[curr][len];
        len--;
    }
    reverse(path.begin(), path.end());
    cout << "Path: ";
    for (int i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i < path.size() - 1) cout << " -> ";
    }
    cout << endl;
}

int solve(int n, int k) {
    // 初始化
    for(int i = 0; i < n; i++) {
        for(int j = 0; j <= k; j++) {
            dp[i][j] = INT_MIN;
            pre[i][j] = -1;
        }
    }
    
    // 初始条件
    for(int i = 0; i < n; i++) {
        dp[i][1] = 0;
    }
    
    // 动态规划
    int ans = INT_MIN;
    int end_node = 0;
    for(int len = 2; len <= k; len++) {
        for(int u = 0; u < n; u++) {
            for(auto& e : adj[u]) {
                int v = e.first, w = e.second;
                if(dp[u][len-1] != INT_MIN && 
                   dp[u][len-1] + w > dp[v][len]) {
                    dp[v][len] = dp[u][len-1] + w;
                    pre[v][len] = u;  // 记录前驱节点
                }
            }
        }
    }
    
    // 找最大值
    for(int i = 0; i < n; i++) {
        if(dp[i][k] > ans) {
            ans = dp[i][k];
            end_node = i;  // 记录终点
        }
    }
    
    if(ans != INT_MIN) {
        //printPath(end_node, k);  // 打印路径
    }
    
    return ans == INT_MIN ? 0 : ans;
}

int main() {
    int n, m, k;
    cin >> n >> m >> k;
    
    for(int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }
    
    int result = solve(n, k);
    cout << result << endl;
    return 0;
}