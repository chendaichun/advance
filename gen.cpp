#include <iostream>
#include <random>
#include <set>
#include <vector>
#include <utility>
using namespace std;

random_device rd;
mt19937 gen(rd());

// 并查集用于确保连通性
vector<int> parent;
void init_union(int n) {
    parent.resize(n + 1);
    for(int i = 0; i <= n; i++) parent[i] = i;
}
int find(int x) {
    return parent[x] == x ? x : (parent[x] = find(parent[x]));
}
void unite(int x, int y) {
    parent[find(x)] = find(y);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage: ./gen n m seed" << endl;
        return 1;
    }
    
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int seed = atoi(argv[3]);
    
    // 确保m在合理范围内：n-1 <= m <= n*(n-1)/2
    m = max(n-1, min(m, n*(n-1)/2));
    
    gen.seed(seed);
    uniform_int_distribution<> dis_val(1, 10);  // 边权值范围1-10
    uniform_int_distribution<> dis_vertex(1, n);
    
    set<pair<int,int> > used;
    vector<pair<int,int> > edges;
    
    // 先生成一棵生成树确保连通性
    init_union(n);
    int tree_edges = n - 1;
    while(tree_edges > 0) {
        int u = dis_vertex(gen);
        int v = dis_vertex(gen);
        if(u > v) swap(u, v);
        if(u != v && find(u) != find(v) && !used.count({u, v})) {
            unite(u, v);
            used.insert({u, v});
            edges.push_back({u, v});
            tree_edges--;
        }
    }
    
    // 添加剩余的随机边
    while(edges.size() < m) {
        int u = dis_vertex(gen);
        int v = dis_vertex(gen);
        if(u > v) swap(u, v);
        if(u != v && !used.count({u, v})) {
            used.insert({u, v});
            edges.push_back({u, v});
        }
    }
    
    // 输出结果
    cout << n << " " << m << endl;
    for(const auto& edge : edges) {
        cout << edge.first << " " << edge.second << " " << dis_val(gen) << endl;
    }
    
    return 0;
}
