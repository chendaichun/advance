#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cassert>
using namespace std;

class DAGGenerator {
private:
    mt19937 rng;
    
    // 生成[l, r]范围内的随机整数
    int randInt(int l, int r) {
        return uniform_int_distribution<int>(l, r)(rng);
    }
    
    // 生成一个DAG的边集
    vector<tuple<int,int,int>> generateEdges(int n, int m) {
        vector<tuple<int,int,int>> edges;
        
        // 确保图是连通的，先生成一条主链
        for(int i = 0; i < n-1; i++) {
            int w = randInt(0, 100);  // 修改：只生成非负权重
            edges.emplace_back(i, i+1, w);
        }
        
        // 生成剩余的边
        while(edges.size() < m) {
            int u = randInt(0, n-2);  // 起点不能是最后一个���点
            int v = randInt(u+1, n-1);  // 终点必须比起点大，保证DAG性质
            int w = randInt(0, 100);  // 修改：只生成非负权重
            
            // 检查是否已存在该边
            bool exists = false;
            for(const auto& e : edges) {
                if(get<0>(e) == u && get<1>(e) == v) {
                    exists = true;
                    break;
                }
            }
            
            if(!exists) {
                edges.emplace_back(u, v, w);
            }
        }
        
        return edges;
    }

public:
    DAGGenerator() : rng(random_device{}()) {}
    
    void generate() {
        // 生成节点数：10-50之间
        int n = randInt(3, 8);
        // 生成边数：保证连通的最小边数(n-1)到最大可能边数(n*(n-1)/2)之间
        int m = randInt(n-1, min(n*(n-1)/2, 3*n));
        // 生成k值：2到n之间
        int k = randInt(2, min(n, 20));
        
        // 生成边集
        auto edges = generateEdges(n, m);
        
        // 输出结果
        cout << n << " " << m << " " << k << endl;
        for(const auto& e : edges) {
            cout << get<0>(e) << " " << get<1>(e) << " " << get<2>(e) << endl;
        }
    }
};

int main() {
    DAGGenerator generator;
    generator.generate();
    return 0;
}