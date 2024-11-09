#include <iostream>
#include <random>
#include <vector>
using namespace std;

//随机数设置
random_device rd;
mt19937 gen(rd());
int get_random(int n);


//使用并查集，邻接表存图
typedef struct G{
    int n;
    int m;
    vector<int> f;
    vector<int> rank;
    vector<bool> isEdge;
    vector<vector<int>> edge;
    G(int num_p, int num_e) {
        n = num_p;
        m = num_e;
        f.resize(n + 1);
        rank.resize(n + 1);
        isEdge.resize(m, true);
        edge.resize(m, vector<int>(3));
        for (int i = 1; i <= n; i++) {
            f[i] = i;
            rank[i] = 1;
        }
    }
    G(const G& other) : n(other.n), m(other.m) {
        // 深拷贝
        f = other.f;
        rank = other.rank;
        isEdge = other.isEdge;
        edge = other.edge;
    }
    int find(int x) {
        if (f[x] == x) return x;
        else return f[x] = find(f[x]);
    }
    void union_set(int n, int m) {
        int fn = find(n);
        int fm = find(m);
        if (fn == fm) return;
        else {
            if (rank[fn] > rank[fm]) f[fm] = fn;
            else {
                f[fn] = fm;
                if (rank[fn] == rank[fm]) rank[fm]++;
            }
        }
    }
    void contract(int num_p) {
        int cnt = this->n;
        while (cnt > num_p) {
            int random_e = get_random(m - 1);
            int u, v;
            u = edge[random_e][0];
            v = edge[random_e][1];
            if (find(u) == find(v)) {
                isEdge[random_e] = false;
            }
            while (!isEdge[random_e]) {
                random_e = (random_e + 1) % m;
            }
            u = edge[random_e][0];
            v = edge[random_e][1];
            union_set(u, v);
            isEdge[random_e] = false;
            cnt--;
        }
        for (int i = 0; i < m; ++i) {
            if (isEdge[i] && find(edge[i][0]) == find(edge[i][1])) 
                isEdge[i] = false;
        }
        this->n = num_p;
    } 
    int cnt_mincut() {
        // 当 n == 2时候使用这个函数求最小割;
        int sum = 0;
        for (int i = 0; i < m; ++i) {
            if (isEdge[i]) sum += edge[i][2];
        }
        return sum;
    }
} G;

int fast_mincut(G graph, int t);

int main() {
    int n, m;
    cin >> n >> m;
    G g1(n,m);
    for(int i = 0; i < m; i++) {
        cin >> g1.edge[i][0] >> g1.edge[i][1] >> g1.edge[i][2];
    }
    int min_cut = INT_MAX;
    int runs = ceil(log2(n));
    for(int i = 0; i < runs; i++) {
        min_cut = min(min_cut, fast_mincut(g1, n));
    }
    cout << min_cut;
}

int get_random(int n) {
    uniform_int_distribution<> dis(1,n);
    return dis(gen);
}

int fast_mincut(G graph, int t) {
    if (graph.n <= 6) {
        graph.contract(2);
        return graph.cnt_mincut();
    }
    G g1(graph);
    G g2(graph);
    t = ceil(t/sqrt(2.0));
    g1.contract(t);
    g2.contract(t);
    return min(fast_mincut(g1,t), fast_mincut(g2,t));
}