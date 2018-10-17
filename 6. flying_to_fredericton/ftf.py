"""
Queremos resolver el problema de https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=2255

Este problema vamos a modelarlo como un grafo con ejes pesados, donde los
vértices son las ciudades, y los ejes los pares de ciudades unidas por uno o más
vuelos. Como dados dos vuelos entre ciudades a->b, siempre elegiríamos el de
menor costo, podemos no considerar el más costoso.

Algunas ideas importantes:
  - No hay vuelos de una ciudad más lejana a una más cercana. Esto hace que
    nuestro grafo sea un DAG
  - Si sólo quisieramos encontrar el camino de menor costo podríamos utilizar el
    algoritmo de Dijkstra. Como nos piden varias consultas con diferentes
    número de escalas, tenemos que ser más creativos.

Entonces, vamos a crear una estructura de datos que, para cada número de vuelos,
almacene los costos mínimos para llegar a las ciudades. Esto hace que podamos
generar los costos iterando sobre la cantidad de vuelos (recorriendo el grafo
con BFS, como es un DAG no es necesario controlar que los nodos no hayan sido 
visitados). Definimos la siguiente función:

    f(0,Calgary) = 0
    f(n+1, c) = min(c2 ciudad : f(n,c2) + peso(c2,c))

Vamos generando los resultados de esta función. Entonces, para un numero máximo
de escalas k, lo resolvemos haciendo g(k) = min(i in [0:k+1]f(i, Friedericton)).

Análisis de complejidad:

- Input del grafo               O(V+E)
- BFS                           O(V+E)
- Cálculos del resultado          O(V)
- Output de queries               O(Q)

La complejidad del programa es O(V+E+Q)
"""

def maxval():
    return float('inf')

from collections import defaultdict

t = int(input())

for case in range(t):
    g = {}
    n = input()
    n = int(input().rstrip())
    for i in range(n):
        g[input().rstrip()] = defaultdict(maxval)

    m = int(input())
    for i in range(m):
        o, d, c = input().rstrip().split(" ")
        c = int(c)
        g[o][d] = min(c, g[o][d])

    m = [defaultdict(maxval) for i in range(n)]
    m[0]['Calgary'] = 0
    for i in range(1, n):
        for orig in m[i - 1]:
            for dest in g[orig]:
                if m[i][dest] > m[i - 1][orig] + g[orig][dest]:
                    m[i][dest] = m[i - 1][orig] + g[orig][dest]

    res = [m[i]['Fredericton'] for i in range(n)]
    res = [min(res[:i + 1]) for i in range(len(res))]

    qn = list(map(int, input().rstrip().split(" ")))
    print("Scenario #" + str(case + 1))
    for i in qn[1:]:
        index = min(n-1, i+1)
        if res[index] != maxval():
            print("Total cost of flight(s) is $" + str(res[index]))
        else:
            print("No satisfactory flights")
    if case < t - 1:
        print("")
