#include <iostream>
#include <utility>
#include <random>
using namespace std;
const int maxn = 1e3;
//随机数设置
random_device rd;
mt19937 gen(rd());
int get_random(int n) {
    uniform_int_distribution<> dis(1,n);
    return dis(gen);
}
//graph
int G[maxn][maxn];
int W[maxn];
int sum_W;

pair<int,int> random_pick(int n);
void contract(int u, int v, int n);
void update_w(int n);
int karger(int n, int m);

int main() {
    int n, m;
    int from, to, val;
    memset(G, 0, sizeof(G));
    cin >> n >> m;
    for(int i = 0; i < m; i++) {
        cin >> from >> to >> val;
        G[from][to] += val;
        G[to][from] += val;
    }
    
    cout << karger(n, m) << endl;
    return 0;
}

void contract(int u, int v, int n) {
    //首先更新 w 数组
    sum_W -= W[u]; W[u] = 0; 
    for (int i = u + 1; i < v; ++i) {
        //这个地方本来应该是 G[i][u],考虑到空间局部性，故
        sum_W += G[u][i];
        W[i] += G[u][i];
    }
    sum_W -= W[v];
    for (int i = v + 1; i <= n; i++) {
        W[v] += G[u][i];
    }
    sum_W += W[v];
    //接着合并边
    for (int i = 1; i <= n; i++) {
        G[v][i] += G[u][i];
    }
    for (int i = 1; i <= n; i++) {
        G[i][v] += G[i][u];
    }
    for (int i = 1; i <= n; i++) {
        G[i][u] = 0;
        G[u][i] = 0;
    }
    G[v][v] = 0;
}

pair<int, int> random_pick(int n) {
    int t = get_random(sum_W);
    int i = 1;
    int j = 0;
    int w = W[1];
    while (t > w) {
        i++;
        w += W[i];
    }
    t = t - (w - W[i]);
    j = i;
    while (t > 0) {
        j = j + 1;
        t -= G[i][j];
    }
    return make_pair(i, j);
}

void update_w(int n) {
    sum_W = 0;
    for (int i = 1; i <= n; ++i) {
        int sum = 0;
        for (int j = i + 1; j <= n; j++) {
            sum+=G[i][j];
        }
        W[i] = sum;
        sum_W += sum;
    }
}

int karger(int n, int m) {
    int min_cut = INT_MAX;
    int times = (n * n * log(n) / 2) ;
    for(int t = 0; t < times; t++) {
        int backup[maxn][maxn];
        memcpy(backup, G, sizeof(G));
        int new_n = n;
        update_w(n);
        while(new_n > 2) {
            pair<int,int> edge = random_pick(n);
            contract(edge.first, edge.second, n);
            //update_w(n);
            new_n--;
        }
        //cout << "this time:" << sum_W << endl;
        min_cut = min(min_cut, sum_W);
        memcpy(G, backup, sizeof(G));
    }
    
    return min_cut;
}