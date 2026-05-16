#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <limits>
#include <algorithm>

using namespace std;

const int INF = 1e9;

// convert string to lowercase
string toLower(string s)
{
    for(char &c : s)
        c = tolower(c);
    return s;
}

// show all routes
void showRoutes(vector<vector<pair<int,int>>> &graph, vector<string> &names)
{
    cout << "\n--- Routes Status ---\n";
    for(int i = 0; i < graph.size(); i++)
        for(auto e : graph[i])
        {
            cout << names[i] << " -> " << names[e.first];
            if(e.second == INF)
                cout << " (BLOCKED)\n";
            else
                cout << " (Time: " << e.second << ")\n";
        }
}

// show only blocked routes
void showBlockedRoutes(vector<vector<pair<int,int>>> &graph, vector<string> &names)
{
    cout << "\n--- Blocked Routes ---\n";
    bool found = false;

    for(int i = 0; i < graph.size(); i++)
        for(auto e : graph[i])
            if(e.second == INF)
            {
                cout << names[i] << " -> " << names[e.first] << endl;
                found = true;
            }

    if(!found)
        cout << "No roads are blocked.\n";
}

// BFS
void BFS(int start, vector<vector<int>> &adj, vector<string> &names)
{
    vector<bool> visited(adj.size(), false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    cout << "BFS Traversal: ";
    while(!q.empty())
    {
        int node = q.front(); q.pop();
        cout << names[node] << " ";

        for(int next : adj[node])
            if(!visited[next])
            {
                visited[next] = true;
                q.push(next);
            }
    }
    cout << endl;
}

// DFS
void DFS(int node, vector<vector<int>> &adj,
         vector<bool> &visited, vector<string> &names)
{
    visited[node] = true;
    cout << names[node] << " ";
    for(int next : adj[node])
        if(!visited[next])
            DFS(next, adj, visited, names);
}

// Dijkstra
void dijkstra(int start, int dest,
              vector<vector<pair<int,int>>> &graph,
              vector<string> &names)
{
    int n = graph.size();
    vector<int> dist(n, INF), parent(n, -1);

    priority_queue<pair<int,int>,
        vector<pair<int,int>>, greater<pair<int,int>>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while(!pq.empty())
    {
        int node = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if(d > dist[node]) continue;

        for(auto e : graph[node])
        {
            if(e.second == INF) continue;

            if(dist[node] + e.second < dist[e.first])
            {
                dist[e.first] = dist[node] + e.second;
                parent[e.first] = node;
                pq.push({dist[e.first], e.first});
            }
        }
    }

    if(dist[dest] == INF)
    {
        cout << "No available path.\n";
        return;
    }

    cout << "\nShortest Time: " << dist[dest] << "\nPath: ";
    vector<int> path;
    for(int v = dest; v != -1; v = parent[v])
        path.push_back(v);

    for(int i = path.size()-1; i >= 0; i--)
        cout << names[path[i]] << " ";
    cout << endl;
}

int main()
{
    vector<string> names = {
        "saddar","isb","streetfood",
        "6throad","scheme3",
        "rajabazar","bluearea"
    };

    map<string,int> id;
    for(int i = 0; i < names.size(); i++)
        id[names[i]] = i;

    int n = names.size();
    vector<vector<pair<int,int>>> graph(n);
    vector<vector<int>> adj(n);

    // store original times
    map<pair<int,int>, int> originalTime;

    auto addRoad = [&](string a, string b, int w)
    {
        int u = id[a], v = id[b];
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
        adj[u].push_back(v);
        adj[v].push_back(u);
        originalTime[{u,v}] = w;
        originalTime[{v,u}] = w;
    };

    addRoad("saddar","isb",4);
    addRoad("saddar","streetfood",2);
    addRoad("isb","streetfood",1);
    addRoad("isb","6throad",5);
    addRoad("streetfood","6throad",8);
    addRoad("6throad","scheme3",3);
    addRoad("rajabazar","saddar",6);
    addRoad("bluearea","isb",3);

    int choice;
    do
    {
        cout << "\n--- Emergency Route Menu ---\n";
        cout << "1. Travel to destination\n";
        cout << "2. Block a road\n";
        cout << "3. Unblock a road\n";
        cout << "4. Show routes\n";
        cout << "5. BFS Traversal\n";
        cout << "6. DFS Traversal\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if(choice == 1)
        {
            showRoutes(graph, names);
            string d;
            cout << "Enter destination: ";
            cin >> d;
            d = toLower(d);

            if(id.find(d) == id.end())
                cout << "No such route exists. Try again.\n";
            else
                dijkstra(0, id[d], graph, names);
        }
        else if(choice == 2 || choice == 3)
        {
            if(choice == 3)
                showBlockedRoutes(graph, names);
            else
                showRoutes(graph, names);

            string u,v;
            cout << "Enter from to: ";
            cin >> u >> v;
            u = toLower(u); v = toLower(v);

            if(id.find(u) == id.end() || id.find(v) == id.end())
            {
                cout << "No such route exists. Try again.\n";
                continue;
            }

            bool found = false;
            for(auto &e : graph[id[u]])
                if(e.first == id[v])
                {
                    found = true;
                    if(choice == 2)
                        e.second = INF;
                    else
                        e.second = originalTime[{id[u],id[v]}];
                }

            for(auto &e : graph[id[v]])
                if(e.first == id[u])
                {
                    if(choice == 2)
                        e.second = INF;
                    else
                        e.second = originalTime[{id[v],id[u]}];
                }

            if(!found)
                cout << "No such route exists. Try again.\n";
            else
                cout << (choice==2 ? "Road blocked.\n" : "Road unblocked.\n");
        }
        else if(choice == 4)
            showRoutes(graph, names);

        else if(choice == 5)
            BFS(0, adj, names);

        else if(choice == 6)
        {
            vector<bool> visited(n,false);
            DFS(0, adj, visited, names);
            cout << endl;
        }

        else if(choice == 0)
        {
            cout<<"\tExiting Program See you soon\n";
        }

        else
        {
            cout<<"Invalid option Try Again!\n";
        }

    } while(choice != 0);

    return 0;
}
