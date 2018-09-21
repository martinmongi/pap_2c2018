"""
Queremos resolver el problema de https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=1140

Necesitamos encontrar los lugares por donde es necesario pasar si o si uniendo
cualquier par de lugares. Es decir, necesitamos encontrar los vértices que si 
los sacaramos, aumentaríamos el número de componentes conexas. Este tipo de
vértices se llaman puntos de articulación. Hay un algoritmo que los puede 
encontrar en tiempo lineal utilizando DFS. La idea es mientras que vamos 
recorriendo el grafo, mantener dos datos:
  - La distancia del nodo actual a la raíz (donde empezamos el DFS)
  - La mínima profundidad de los vecinos del nodo actual, llamado punto bajo
La distancia es algo fácil de calcular. Para el punto bajo, es necesario esperar
a que se hayan hecho todas las llamadas recursivas (por esto se usa DFS).
La idea es que un nodo no-raíz x va a ser punto de articulación sii tiene un 
vecino y tal que el punto bajo de y sea mayor o igual a la profundidad de x.
Para la raíz, alcanza con ver que tenga más de 2 hijos para ser punto de 
articulación.

Análisis de complejidad:

Veamos cuales son las etapas del programa:

  - Input de vértices (usamos hash tables de palabra a nro)     O(V)
  - Input de ejes (usamos hash tables de palabra a nro)         O(E)
  - Inicialización de vectores                                  O(V)
  - Tarjan-Hopcroft DFS                                         O(V+E)
  - Output de puntos de articulación                            O(V)

La complejidad del programa en total es O(V+E)
"""

import copy
from collections import deque, Counter

global visited, depth, low, n, graph, parent, aps

c = 0

def articulation_points(i, d):
    visited[i] = True
    depth[i] = d
    low[i] = d
    child_count = 0
    is_articulation = False
    for ni in graph[i]:
        if not visited[ni]:
            parent[ni] = i
            articulation_points(ni, d + 1)
            child_count += 1
            if low[ni] >= depth[i]:
                is_articulation = True
            low[i] = min(low[i], low[ni])
        elif ni != parent[i]:
            low[i] = min(low[i], depth[ni])
    if parent[i] != -1 and is_articulation or parent[i] == -1 and child_count > 1:
        # print(i)
        aps[i] = True

while True:

    n = int(input())
    if n == 0:
        break

    if c > 0:
        print("")
    c += 1

    ctn = ntc = {}
    for i in range(n):
        ntc[i] = str(input()).rstrip()
        ctn[ntc[i]] = i

    graph = [[] for i in range(n)]

    m = int(input())
    for i in range(m):
        v1, v2 = input().rstrip().split(' ')
        graph[ctn[v1]].append(ctn[v2])
        graph[ctn[v2]].append(ctn[v1])

    aps = [False for i in graph]
    visited = [False for i in graph]
    depth = [0 for i in graph]
    low = [0 for i in graph]
    parent = [-1 for i in graph]
    for i in range(n):
        if not visited[i]:
            articulation_points(i, 0)

    print("City map #" + str(c) + ": " +
          str(len([i for i in aps if i])) + " camera(s) found")
    for i in sorted([ntc[i] for i in range(n) if aps[i]]):
        print(i)
