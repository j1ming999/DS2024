#include <iostream>
#include <vector>
#include <queue>
#include <climits> // for INT_MAX
using namespace std;

class Graph {
    int V; // 顶点数
    vector<vector<pair<int, int>>> adjList; // 邻接表 (邻居, 权重)

public:
    Graph(int V) : V(V) {
        adjList.resize(V);
    }

    // 添加边
    void addEdge(int u, int v, int weight) {
        adjList[u].push_back(make_pair(v, weight));
        adjList[v].push_back(make_pair(u, weight)); // 如果是无向图
    }

    // 打印图
    void printGraph() {
        for (int i = 0; i < V; ++i) {
            cout << "Node " << i << ": ";
            for (size_t j = 0; j < adjList[i].size(); ++j) {
                cout << "(" << adjList[i][j].first << ", " << adjList[i][j].second << ") ";
            }
            cout << endl;
        }
    }

    // 宽度优先搜索 (BFS)
    void BFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;
        q.push(start);
        visited[start] = true;

        cout << "BFS: ";
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            cout << node << " ";

            for (size_t i = 0; i < adjList[node].size(); ++i) {
                int neighbor = adjList[node][i].first;
                if (!visited[neighbor]) {
                    q.push(neighbor);
                    visited[neighbor] = true;
                }
            }
        }
        cout << endl;
    }

    // 深度优先搜索 (DFS)
    void DFSUtil(int node, vector<bool>& visited) {
        visited[node] = true;
        cout << node << " ";

        for (size_t i = 0; i < adjList[node].size(); ++i) {
            int neighbor = adjList[node][i].first;
            if (!visited[neighbor]) {
                DFSUtil(neighbor, visited);
            }
        }
    }

    void DFS(int start) {
        vector<bool> visited(V, false);
        cout << "DFS: ";
        DFSUtil(start, visited);
        cout << endl;
    }

    // Dijkstra 算法
    void dijkstra(int start) {
        vector<int> dist(V, INT_MAX);
        dist[start] = 0;

        // 最小优先队列：pair(距离, 节点)
        typedef pair<int, int> pii;
        priority_queue<pii, vector<pii>, greater<pii>> pq;
        pq.push(make_pair(0, start));

        while (!pq.empty()) {
            pii top = pq.top();
            pq.pop();
            int curDist = top.first;
            int node = top.second;

            if (curDist > dist[node]) continue;

            for (size_t i = 0; i < adjList[node].size(); ++i) {
                int neighbor = adjList[node][i].first;
                int weight = adjList[node][i].second;

                if (dist[node] + weight < dist[neighbor]) {
                    dist[neighbor] = dist[node] + weight;
                    pq.push(make_pair(dist[neighbor], neighbor));
                }
            }
        }

        cout << "Dijkstra distances from node " << start << ":\n";
        for (int i = 0; i < V; ++i) {
            if (dist[i] == INT_MAX)
                cout << "Node " << i << ": -1\n";
            else
                cout << "Node " << i << ": " << dist[i] << endl;
        }
    }
};

int main() {
    int V = 5;
    Graph g(V);

    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 1);
    g.addEdge(1, 2, 2);
    g.addEdge(1, 3, 5);
    g.addEdge(2, 3, 8);
    g.addEdge(3, 4, 6);

    g.printGraph();
    g.BFS(0);
    g.DFS(0);
    g.dijkstra(0);

    return 0;
}

