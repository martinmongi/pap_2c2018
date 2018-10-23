/*
Queremos resolver el problema de https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=4406

La idea del problema es como colocar la mayor cantidad de torres que no se
ataquen en un tablero cuadrado donde ya hay colocados peones. Los peones, además
de restringir casilleros en los cuales se podrían colocar torres, también
restringen el alcance de ellas, es decir, pueden ser colocadas dos torres en la
misma línea, si hay un peón en el medio.

Algunas ideas importantes:
  - En cada hilera vertical u horizontal ininterrumpida, se puede colocar 
    solamente una torre.
  - Cualquier torre que coloquemos, va a restringir exactamente dos hileras de
    las mencionadas arriba (una horizontal y una vertical)

Utilizando estas ideas, podemos modelar el problema como un matching máximo
sobre un grafo bipartito. Podemos tomar el siguiente grafo G = (V,E) con

    V = F U C       donde hay un vértice en F por cada hilera horizontal y un
                    vértice en C por cada hilera vertical
    E = {(f,c) | f in F, c in C, tienen un casillero en común y no está ocupado
                                 por un peón}

Por ejemplo, podemos ver la siguiente correspondencia entre el siguiente grafo y
su modelo:

    ...         V = {F1, F21, F22, F3} U {C1, C21, C22, C3}
    .X.         E = {(F1, C1), (F1, C21), (F1, C3), (F21, C1), (F22, C3),
    ...              (F3, C1), (F3, C22), (F3, C3)}

De esta forma, encontrando el matching máximo, encontramos la máxima cantidad de
torres que podemos colocar. Hacemos las adaptaciones necesarias como agregar
super-source y super-sink y usamos el algoritmo de Dinitz.

Análisis de complejidad:

  - Lectura del input y generación del grafo       O(n^2)
  - Algoritmo de Dinitz en grafo bipartito         O(V^.5*E) = O(n*n^2) = O(n^3)

Vemos que la complejidad total es O(n^3)

*/
#define Martin using
#define Mongi namespace
#define Badia std

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <tuple>
#include <numeric>
#include <math.h>
#include <unordered_map>

Martin Mongi Badia;

vector<set<int>> level_graph(const vector<set<int>> &rc, int s, int t)
{
    vector<int> dist(rc.size(), -1);
    vector<bool> visited(rc.size(), false);
    dist[s] = 0;
    queue<int> q;
    q.push(s);
    while (!q.empty())
    {
        int v = q.front();
        q.pop();
        visited[v] = true;
        for (auto &v2 : rc[v])
        {
            if (!visited[v2])
            {
                q.push(v2);
                dist[v2] = dist[v] + 1;
            }
        }
    }
    if (dist[t] == -1)
        return vector<set<int>>();
    vector<set<int>> level_graph(rc.size());
    for (int v = 0; v < rc.size(); ++v)
    {
        for (auto &v2 : rc[v])
        {
            if (dist[v2] == dist[v] + 1)
                level_graph[v].insert(v2);
        }
    }
    return level_graph;
}

vector<int> augmenting_path(vector<set<int>> &lg, int s, int t)
{
    int n = lg.size();
    vector<bool> visited(n, false);
    vector<int> prev(n, -1);

    stack<int> q;
    q.push(s);
    while (!q.empty() && prev[t] == -1)
    {
        int v = q.top();
        q.pop();
        visited[v] = true;
        for (auto &v2 : lg[v])
        {
            if (!visited[v2])
            {
                q.push(v2);
                prev[v2] = v;
            }
        }
    }
    if (prev[t] == -1)
        return vector<int>();
    int v = t;
    vector<int> path;
    while (prev[v] != -1)
    {
        path.push_back(v);
        v = prev[v];
    }
    path.push_back(s);
    reverse(path.begin(), path.end());
    return path;
}

int blocking_flow(vector<set<int>> &rc, int s, int t)
{
    vector<set<int>> lg = level_graph(rc, s, t);
    if (lg.size() == 0)
        return 0;
    int res = 0;
    vector<int> path;
    while ((path = augmenting_path(lg, s, t)).size() > 0)
    {
        for (int i = 0; i < path.size() - 1; ++i)
        {
            lg[path[i]].erase(path[i + 1]);
            rc[path[i]].erase(path[i + 1]);
            rc[path[i + 1]].insert(path[i]);
        }
        res++;
    }
    return res;
}

int dinitz(vector<set<int>> graph, int s, int t)
{
    auto rc = graph;
    int flow = 0, step = 0;

    while (step = blocking_flow(rc, s, t))
        flow += step;
    return flow;
}

int main()
{
    int n;
    while (cin >> n)
    {
        vector<string> board;
        string aux;
        for (int i = 0; i < n; ++i)
        {
            cin >> aux;
            board.push_back(aux);
        }
        map<pair<int, int>, int> sq_to_rowv, sq_to_colv;
        int vcount = 0;
        bool brand_new = true;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (board[i][j] == 'X')
                {
                    if (!brand_new)
                        vcount++;
                    brand_new = true;
                }
                else
                {
                    sq_to_rowv[make_pair(i, j)] = vcount;
                    brand_new = false;
                }
            }
            if (!brand_new)
                vcount++;
            brand_new = true;
        }
        int rowv = vcount;

        for (int j = 0; j < n; ++j)
        {
            for (int i = 0; i < n; ++i)
            {
                if (board[i][j] == 'X')
                {
                    if (!brand_new)
                        vcount++;
                    brand_new = true;
                }
                else
                {
                    sq_to_colv[make_pair(i, j)] = vcount;
                    brand_new = false;
                }
            }
            if (!brand_new)
                vcount++;
            brand_new = true;
        }
        if (!brand_new)
            vcount++;

        vector<set<int>> graph(vcount + 2);
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (board[i][j] != 'X')
                {
                    graph[sq_to_rowv[make_pair(i, j)]].insert(sq_to_colv[make_pair(i, j)]);
                }
            }
        }
        for (int i = 0; i < rowv; ++i)
        {
            graph[vcount].insert(i);
        }
        for (int i = rowv; i < vcount; ++i)
        {
            graph[i].insert(vcount + 1);
        }

        cout << dinitz(graph, vcount, vcount + 1) << '\n';
    }

    return 0;
}