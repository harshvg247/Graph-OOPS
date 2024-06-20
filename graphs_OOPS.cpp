#include <bits/stdc++.h>
using namespace std;

void printVector(vector<int>&v){
    for(int i=0; i<v.size(); i++){
        cout << v[i] << " ";
    }
    cout << endl;
}
// This class is the base class for other graph-related classes such as DirectedGraph, WeightedGraph, and WeightedDirectedGraph
class Graph
{
private:
    // making this private so only friend class i.e. DirectedGraph can access it
    vector<vector<int>> adjList;

protected:
    //  making this protected as numVertices will be common for all graph types
    int numVertices;

public:
    Graph(int vertices); // constructor
    virtual ~Graph() {}
    virtual void addEdge(int v, int w);
    virtual void removeEdge(int v, int w);
    // this display function will be used both in Graph and DirectedGraph classes
    virtual void display();
    virtual vector<int> bfs(int vertex);
    virtual vector<int> dfs(int vertex);
    // this way only DerictedGraph class can access adjList
    friend class DirectedGraph;
};
// Graph::Graph(int vertices): This is the constructor for the Graph class, which takes an integer argument vertices to specify the number of vertices in the graph.
// : numVertices(vertices): This is a member initialization list, which initializes the numVertices attribute with the value of the vertices argument.
Graph::Graph(int vertices) : numVertices(vertices)
{
    // Resizes the adjacency list to accommodate the specified number of vertices
    adjList.resize(vertices);
}
void Graph::addEdge(int v, int w)
{
    adjList[v].push_back(w);
    adjList[w].push_back(v);
}
void Graph::removeEdge(int v, int w)
{
    adjList[v].erase(find(adjList[v].begin(), adjList[v].end(), w));
    adjList[w].erase(find(adjList[w].begin(), adjList[w].end(), v));
}
void Graph::display()
{
    for (int i = 0; i < numVertices; i++)
    {
        cout << i << " -> ";
        for (int j = 0; j < adjList[i].size(); j++)
        {
            cout << adjList[i][j] << " ";
        }
        cout << endl;
    }
}
vector<int> Graph::bfs(int vertex)
{
    vector<int> ans;
    queue<int> q;
    q.push(vertex);
    vector<int> visited(numVertices, 0);
    visited[vertex] = 1;
    while (!q.empty())
    {
        int node = q.front();
        q.pop();
        ans.push_back(node);
        for (auto neighbour : adjList[node])
        {
            if (!visited[neighbour])
            {
                visited[neighbour] = 1;
                q.push(neighbour);
            }
        }
    }
    return ans;
}
vector<int> Graph::dfs(int vertex){

}
class DirectedGraph : public Graph
{
public:
    DirectedGraph(int vertices);
    void addEdge(int v, int w);
    void removeEdge(int v, int w);
    // display function of Graph class will be used
};
DirectedGraph::DirectedGraph(int vertices) : Graph(vertices) {}
void DirectedGraph::addEdge(int v, int w)
{
    adjList[v].push_back(w); // v:source  w:destination
}
void DirectedGraph::removeEdge(int v, int w)
{
    adjList[v].erase(find(adjList[v].begin(), adjList[v].end(), w));
}

class WeightedGraph : public Graph
{
    // protected so that WeightedDirectedGraph class can access it
protected:
    vector<vector<pair<int, int>>> adjList;

public:
    WeightedGraph(int vertices);
    // since number of arguments are different, it is the case of method overloading(consider it different function all together)
    // so we need to again define it to be virtual here(for appropriate binding behaviour in this class and WeightedDirectedGraph class)
    virtual void addEdge(int v, int w, int weight);
    virtual void removeEdge(int v, int w);
    // display has same number of arguments so no need to define it to be virtual here(as already done in Graph class)
    void display();
};
WeightedGraph::WeightedGraph(int vertices) : Graph(vertices)
{
    // since adjList of this class is different from Graph class, we need to resize it here
    adjList.resize(numVertices);
}
void WeightedGraph::addEdge(int v, int w, int weight)
{
    adjList[v].push_back(make_pair(w, weight));
    adjList[w].push_back(make_pair(v, weight));
}
void WeightedGraph::removeEdge(int v, int w)
{
    for (auto neighbour : adjList[v])
    {
        if (neighbour.first == w)
        {
            adjList[v].erase(find(adjList[v].begin(), adjList[v].end(), neighbour));
            break;
        }
    }
    for (auto neighbour : adjList[w])
    {
        if (neighbour.first == v)
        {
            adjList[w].erase(find(adjList[w].begin(), adjList[w].end(), neighbour));
            break;
        }
    }
}
void WeightedGraph::display()
{
    for (int i = 0; i < numVertices; i++)
    {
        cout << i << " -> ";
        for (int j = 0; j < adjList[i].size(); j++)
        {
            cout << adjList[i][j].first << " " << adjList[i][j].second << "  ";
        }
        cout << endl;
    }
}

class WeightedDirectedGraph : public WeightedGraph
{
public:
    WeightedDirectedGraph(int vertices);
    void addEdge(int v, int w, int weight);
    void removeEdge(int v, int w);
};
WeightedDirectedGraph::WeightedDirectedGraph(int vertices) : WeightedGraph(vertices) {}
void WeightedDirectedGraph::addEdge(int v, int w, int weight)
{
    adjList[v].push_back(make_pair(w, weight));
}
void WeightedDirectedGraph::removeEdge(int v, int w)
{
    for (auto neighbour : adjList[v])
    {
        if (neighbour.first == w)
        {
            adjList[v].erase(find(adjList[v].begin(), adjList[v].end(), neighbour));
            break;
        }
    }
}

int main()
{
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 3);
    g.addEdge(2, 3);
    vector<int> ans = g.bfs(0);
    printVector(ans);
    g.display();
    return 0;
}