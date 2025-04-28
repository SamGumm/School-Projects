// C++ program for Breadth-first-traversal

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

// Define a class named Graph to represent the graph
// structure.
class Graph {
    // Number of vertices in the graph.
    int V;
    // Adjacency list representation of the graph.
    vector<vector<int> > adjList;

public:
    // Constructor for initializing the graph with a given
    // number of vertex
    Graph(int vertices)
    {
        V = vertices;
        adjList.resize(vertices);
    }

    // Function for adding an edge to the graph.
    void addEdge(int src, int dest)
    {
        // Add destination vertex to source vertex's list.
        adjList[src].push_back(dest);
        // Add source vertex to destination vertex's list
        // (for undirected graph).
        adjList[dest].push_back(src);
    }

    // Function to perform Breadth-First Traversal (BFS)
    // starting from a given vertex.
    void bfs(int startVertex)
    {
        // Vector to keep track of visited vertices.
        vector<bool> visited(V, false);
        // Queue to help with the BFS traversal.
        queue<int> q;
        // Mark the start vertex as visited.
        visited[startVertex] = true;
        // Enqueue the start vertex.
        q.push(startVertex);

        while (!q.empty()) {
            // point currentVertex at front vertex from the queue.
            int currentVertex = q.front();
            // Print the current vertex.
            cout << currentVertex << " ";
            // Remove the front vertex from the queue.
            q.pop();

            // Iterate through all the neighbors of the
            // current vertex.
            for (int neighbor : adjList[currentVertex]) {
                // If the neighbor vertex has not been
                // visited yet.
                if (!visited[neighbor]) {
                    // Mark the neighbor vertex as visited.
                    visited[neighbor] = true;
                    // Enqueue the neighbor vertex.
                    q.push(neighbor);
                }
            }
        }
    }
};

int main()
{
    // Create a graph with 7 vertices.
    Graph g(7);

    // Add edges to the graph.
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);

    // Perform BFS starting from vertex 2.
    cout << "Breadth-First Traversal (starting from vertex "
            "2): "
         << endl;
    g.bfs(2);

    return 0;
}
