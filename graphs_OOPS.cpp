#include <bits/stdc++.h>
using namespace std;

void printVector(vector<int> &v)
{
    for (int i = 0; i < v.size(); i++)
    {
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
    bool isCyclicHelper(int node, vector<int>&visited, int parent);

public:
    Graph(int vertices); // constructor
    virtual ~Graph() {}
    virtual void addEdge(int v, int w);
    virtual void removeEdge(int v, int w);
    // this display function will be used both in Graph and DirectedGraph classes
    virtual void display();
    virtual vector<int> bfs(int vertex);
    virtual vector<int> dfs(int vertex);

    // FOR YOU----> // used call by reference, for just using the address, instead of unnecessary copying
    virtual void explore(int vertex, vector<int> &visited, vector<int> &ans);

    virtual bool isCyclic();
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
void Graph::explore(int vertex, vector<int> &visited, vector<int> &ans)
{
    visited[vertex] = 1;
    ans.push_back(vertex);
    for (auto neighbour : adjList[vertex])
    {
        if (!visited[neighbour])
        {
            explore(neighbour, visited, ans);
        }
    }
}
vector<int> Graph::dfs(int vertex)
{
    vector<int> ans;
    vector<int> visited(numVertices, 0);
    visited[vertex] = 1;
    ans.push_back(vertex);
    for (auto neighbour : adjList[vertex])
    {
        explore(neighbour, visited, ans);
    }
    return ans;
}
bool Graph::isCyclicHelper(int node, vector<int>& visited, int parent){
    visited[node] = 1;
    for(auto neighbour : adjList[node]){
        if(!visited[neighbour]){
            if(isCyclicHelper(neighbour, visited, node)){
                return true;
            }
        }else if(neighbour != parent){
            return true;
        }
    }
    return false;
}
bool Graph::isCyclic(){
    vector<int> visited(numVertices, 0);
    for(int node = 0; node < numVertices; node++){
        if(!visited[node]){
            if(isCyclicHelper(node, visited, -1)==true){
                return true;
            }
        }
    }
    return false;
}

class DirectedGraph : public Graph
{
public:
    DirectedGraph(int vertices);
    void addEdge(int v, int w);
    void removeEdge(int v, int w);
    // display function of Graph class will be used
    // topological sort using Kahn algorithm
    virtual vector<int> topoSort();
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

vector<int> DirectedGraph::topoSort()
{
    vector<int> ans;
    vector<int> indegree(numVertices, 0);
    queue<int> q;
    for (int node = 0; node < numVertices; node++)
    {
        for (auto neighbour : adjList[node])
        {
            indegree[neighbour]++;
        }
    }
    for (int node = 0; node < numVertices; node++)
    {
        if (indegree[node] == 0)
        {
            q.push(node);
        }
    }
    while (!q.empty())
    {
        int node = q.front();
        q.pop();
        ans.push_back(node);
        for (auto neighbour : adjList[node])
        {
            indegree[neighbour]--;
            if (indegree[neighbour] == 0)
            {
                q.push(neighbour);
            }
        }
    }
    return ans;
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
    for (auto itr = adjList[v].begin(); itr != adjList[v].end(); itr++)
    {
        if (itr->first == w)
        {
            adjList[v].erase(itr);
            break;
        }
    }
    for (auto itr = adjList[w].begin(); itr != adjList[w].end(); itr++)
    {
        if (itr->first == v)
        {
            adjList[w].erase(itr);
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
    for (auto itr = adjList[v].begin(); itr != adjList[v].end(); itr++)
    {
        if (itr->first == w)
        {
            adjList[v].erase(itr);
            break;
        }
    }
}

int main()
{
    Graph g(3);
    g.addEdge(0, 1);
    g.addEdge(2, 1);
    g.addEdge(2, 0);

    bool ans = g.isCyclic();
    cout << ans << endl;
    g.display();
    return 0;
}