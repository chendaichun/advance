import networkx as nx
import matplotlib.pyplot as plt

def read_graph(filename):
    G = nx.DiGraph()
    with open(filename, 'r') as f:
        # 读取第一行的顶点数和边数
        n, m = map(int, f.readline().split())
        # 读取每条边
        for _ in range(m):
            u, v, w = map(int, f.readline().split())
            G.add_edge(u, v, weight=w)
    return G

def visualize_dag(G):
    # 设置画布大小，稍微调小一些
    plt.figure(figsize=(12, 8))
    
    # 使用分层布局，减小scale参数使图形更紧凑
    pos = nx.kamada_kawai_layout(G, scale=1.0)
    
    # 绘制节点，进一步减小节点大小
    nx.draw_networkx_nodes(G, pos, node_color='lightblue', 
                          node_size=200, alpha=0.6)
    
    # 绘制边
    nx.draw_networkx_edges(G, pos, edge_color='gray',
                          arrowsize=10, 
                          min_source_margin=15,
                          min_target_margin=15)
    
    # 添加节点标签，减小字体
    nx.draw_networkx_labels(G, pos, font_size=6)
    
    # 添加边权重标签
    edge_labels = nx.get_edge_attributes(G, 'weight')
    nx.draw_networkx_edge_labels(G, pos, edge_labels, font_size=5)
    
    plt.title("Directed Acyclic Graph (DAG) Visualization", pad=10)
    plt.axis('off')
    
    # 调整边距，确保图形在视图内
    plt.margins(0.15)
    
    # 自动调整布局以适应窗口
    plt.tight_layout()
    
    plt.show()

if __name__ == "__main__":
    # 读取图数据
    filename = "/Users/dyl/vsocde/advance/test_dag/test9.in"  # 修改为你的输入文件路径
    G = read_graph(filename)
    
    # 可视化图
    visualize_dag(G)