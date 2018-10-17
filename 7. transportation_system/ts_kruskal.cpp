/*
Queremos resolver el problema de https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=2169

Queremos unir todas las ciudades dadas con rutas o ferrocarriles construyendo la
menor cantidad de ambos. Primero, supongamos que la diferenciación entre rutas y
ferrocarriles no existe. Entonces, necesitamos que entre cada par de ciudades de
país hay al menos un país. Dado que queremos ahorrar, una forma de unir todas
las ciudades, sería pedir sólo un camino entre cada par. Si en un grafo, entre
cada par de vértices hay exactamente un camino, el grafo es un árbol. Podemos
generar este árbol con algoritmos como Prim o Kruskal. Usamos este último.

Dado el MST (minimum spanning tree), tenemos que decidir que ejes de este vamos
a construir como rutas y cuales como ferrocarriles. Esto lo hacemos según el
límite dado para la construcción de rutas. Luego, contamos la cantidad de
estados diferentes del país (si hay m ferrocarriles, hay m + 1 estados), y 
devolvemos el resultado.

Análisis de complejidad:

- Generación de grafo y pesos de ejes       O(n^2)
- Ordenamiento de ejes                      O(n^2 * log n)
- Creación del árbol                        O(n^2 * log n) (es posible acotar mejor)
- División entre ruta y ferrocarril         O(n)

Vemos que la complejidad total del algoritmo es O(n^2 * log n)

*/

#define Martin using
#define Mongi namespace
#define Badia std

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <tuple>
#include <numeric>
#include <math.h>

Martin Mongi Badia;

typedef struct point
{
    double x, y;
} City;

typedef struct edge
{
    int o, d;
    double w;
} Edge;

inline double sq_dist(City c1, City c2)
{
    return pow(pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2),0.5);
}

class UnionFind
{
    vector<int> parent, rank;

  public:
    UnionFind(int);
    int Find(int);
    void Union(int, int);
};

UnionFind::UnionFind(int n)
{
    for (int i = 0; i < n; ++i)
    {
        parent.push_back(i);
        rank.push_back(0);
    }
}

int UnionFind::Find(int x)
{
    int aux1, aux2;
    while (parent[x] != x)
    {
        aux1 = parent[x];
        aux2 = parent[parent[x]];
        x = aux1;
        parent[x] = aux2;
    }
    return x;
}

void UnionFind::Union(int x, int y)
{
    int xroot = Find(x);
    int yroot = Find(y);
    if (xroot == yroot)
        return;
    if (rank[xroot] < rank[yroot])
    {
        int aux = xroot;
        xroot = yroot;
        yroot = aux;
    }
    parent[yroot] = xroot;
    if (rank[xroot] == rank[yroot])
        rank[xroot]++;
}

int main()
{
    int t, n;
    double threshold;
    City p;
    Edge e;
    cin >> t;
    for (int case_number = 1; case_number <= t; case_number++)
    {
        cin >> n >> threshold;
        vector<City> cities;
        for (int i = 0; i < n; ++i)
        {
            cin >> p.x >> p.y;
            cities.push_back(p);
        }
        vector<Edge> edges;
        for (int i = 0; i < n; ++i)
        {
            for (int j = i + 1; j < n; ++j)
            {
                e.o = i;
                e.d = j;
                e.w = sq_dist(cities[i], cities[j]);
                edges.push_back(e);
            }
        }
        sort(edges.begin(), edges.end(),
             [](Edge e1, Edge e2) { return e1.w < e2.w; });

        vector<Edge> mst;
        UnionFind uf(n);
        for (const auto &e : edges)
        {
            if (uf.Find(e.o) != uf.Find(e.d))
            {
                mst.push_back(e);
                uf.Union(e.o, e.d);
            }
            if (mst.size() == n - 1)
                break;
        }
        int states = 1;
        double rl = 0, rrl = 0;
        for (const auto &e : mst)
        {
            // cout << e.w << endl;
            if (e.w > threshold)
            {
                states++;
                rrl += e.w;
            }
            else
            {
                rl += e.w;
            }
        }
        cout << "Case #" << case_number << ": " << states << " "
             << round(rl) << " " << round(rrl) << "\n";
    }
    return 0;
}