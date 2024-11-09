#include <bitset>
#include <iostream>
#include <random>
#include <vector>
using namespace std;
const int maxn = 1e3;
random_device rd;
mt19937 gen(rd());
typedef struct {
    bitset<32> s;
    int w;
    int x;
} status;

typedef struct {
    int to;
    int val;
} edge;

vector<edge> G[maxn];
int color[maxn];
vector<status> A[maxn];

int color_path(int n, int k) {
    for (int i = 0 ; i < n; ++i) {
        A[i].clear();
    }
    int max_w = -1;
    for (int i = 0; i < n; ++i) {
        bitset<32> tmp;
        tmp.set(color[i] - 1);
        A[i].push_back({tmp, 0, -1});
    }
    for (int h = 1; h < k; ++h) {
        for (int i = 0; i < n; ++i) {
            for (const auto& edges: G[i]) {
                for (const auto& s: A[i]) {
                    bitset<32> tmp;
                    tmp.set(color[edges.to] - 1);
                    if ((s.s.count() == h ) && !(s.s & tmp).any()) {
                        bitset<32> new_s = s.s | tmp;
                        bool found_new_s = false;
                        for (auto &s1: A[edges.to]) {
                            if (s1.s == new_s ) {
                                found_new_s = true;
                                s1.w = max(s1.w, s.w + edges.val);
                                s1.x = i;
                                break;
                            }
                        }
                        if (!found_new_s) {
                            A[edges.to].push_back({new_s, s.w + edges.val, i});
                        }
                        if (h == k - 1) {
                            max_w = max(s.w + edges.val, max_w);
                        }
                    }
                }
            }
        }
    }

    return max_w;
}

 void random_color(int n, int k) {
    for (int i = 0; i < n; ++i) {
        color[i] = i % k + 1;
    }
    shuffle(color, color + n, gen);
}  

int pathcc(int n, int k) {
    int times = exp(k) * 10;
    int max_k = -1;
    for (int i = 0 ;i < times; ++i) {
        random_color(n, k);
        max_k = max(color_path(n, k),max_k);
    }
    return max_k;
}

int main() {
    int n, m, k;
    cin >> n >> m >> k;
    int from, to, val;
    for (int i = 0; i < m; ++i) {
        cin >> from >> to >> val;
        G[from].push_back({to, val});
        G[to].push_back({from, val});
    }
    cout << pathcc(n, k);
    return 0;
}