#define Martin using
#define Mongi namespace
#define Badia std

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <string>
#include <climits>

Martin Mongi Badia;

// void print_v(const vector<int> &v)
// {
//     for (auto &i : v)
//         cout << (i != 0 ? to_string(i) : "") << '\t';
//     cout << '\n';
// }

// void print_g(const vector<vector<int>> &g)
// {
//     cout << "-----------------------------------------------\n";
//     for (auto &row : g)
//         print_v(row);
//     cout << "-----------------------------------------------\n";
// }
vector<int> augmentation_path(vector<vector<int>> graph,
                              vector<vector<int>> f, int s, int t)
{
    int n = graph.size();
    vector<bool> seen(n, false);
    vector<int> previous(n, -1);
    queue<int> q;
    q.emplace(s);
    while (!q.empty())
    {
        int v = q.front();
        q.pop();
        seen[v] = true;
        for (int i = 0; i < n; ++i)
        {
            if ((graph[v][i] > 0) && (!seen[i]) &&
                (graph[v][i] - f[v][i] > 0))
            {
                // cout << v << '\t' << i << '\t'
                //      << graph[v][i] - f[v][i] << '\n';
                q.emplace(i);
                previous[i] = v;
                if (i == t)
                {
                    vector<int> path(1, t);
                    while (previous[path[path.size() - 1]] != -1)
                        path.push_back(previous[path[path.size() - 1]]);
                    reverse(path.begin(), path.end());
                    return path;
                }
            }
        }
    }
    return vector<int>();
}

typedef struct
{
    set<vector<int>> paths;
    int vertex_cut;
} FlowAnalysis;

FlowAnalysis max_flow(vector<vector<int>> graph, int s, int t)
{
    int n = graph.size() / 2;
    vector<vector<int>> f(2 * n, vector<int>(2 * n, 0));
    FlowAnalysis res;
    res.vertex_cut = 0;
    res.paths = set<vector<int>>();

    vector<int> path;
    while (path = augmentation_path(graph, f, s, t), !path.empty())
    {
        res.paths.insert(path);
        int a = graph[path[0]][path[1]] -
                f[path[0]][path[1]];
        // print_v(path);
        for (int i = 0; i < path.size() - 1; ++i)
        {
            a = min(a, graph[path[i]][path[i + 1]] -
                           f[path[i]][path[i + 1]]);
            // cout << graph[path[i]][path[i + 1]] << '\t' << f[path[i]][path[i + 1]] << '\t' << a << endl;
        }
        for (int i = 0; i < path.size() - 1; ++i)
        {
            f[path[i]][path[i + 1]] += a;
            f[path[i + 1]][path[i]] -= a;
        }
    }
    if (res.paths.size() == 1)
    {
        path = *res.paths.begin();
        // print_v(path);
        for (int i = 1; i < path.size() - 2; i += 2)
        {
            if (graph[path[i]][path[i + 1]] -
                    f[path[i]][path[i + 1]] ==
                0)
            {
                res.vertex_cut = path[i] / 2;
                // cout << res.vertex_cut << '\n';
                break;
            }
        }
    }
    return res;
}

int main()
{
    int n, m, c = 0;
    string aux, aux2;
    while (cin >> n, n != 0)
    {
        if (c != 0)
            cout << '\n';
        c++;
        unordered_map<string, int> ctn;
        unordered_map<int, string> ntc;
        for (int i = 0; i < n; ++i)
        {
            cin >> aux;
            ntc[i] = aux;
            ctn[aux] = i;
        }
        vector<vector<int>> graph(2 * n, vector<int>(2 * n, 0));
        for (int i = 0; i < 2 * n; i += 2)
            graph[i][i + 1] = 1;

        cin >> m;
        while (m--)
        {
            cin >> aux >> aux2;
            graph[2 * ctn[aux] + 1][2 * ctn[aux2]] = 2;
            graph[2 * ctn[aux2] + 1][2 * ctn[aux]] = 2;
        }
        set<string> cameras;
        for (int i = 1; i < 2 * n; i += 2)
        {
            for (int j = i + 1; j < 2 * n; j += 2)
            {
                if (graph[i][j] == 0)
                {
                    FlowAnalysis res = max_flow(graph, i, j);
                    // cout << res.vertex_cut << endl;
                    cameras.insert(ntc[res.vertex_cut]);
                }
            }
        }
        cout << "City map #" << c << ": " << cameras.size() << " camera(s) found.\n";
        for (auto cam : cameras)
        {
            cout << cam << '\n';
        }
    }
}