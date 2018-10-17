"""
Queremos resolver el problema de https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=3670

Para este problema, vamos a modelar un diccionario como un grafo dirigido, donde
cada palabra es modelada por un vértice, y existe el eje a -> b sii b está en la
definición de a. Ahora veamos algunas ideas importantes:
  - Si existe un camino desde a hasta b, b tiene que estar definida antes (o en 
    el sub-diccionario inicial) que a.
  - Por lo tanto, si existe un camino de a a b, y otro de b a a, ambas tienen
    que estar definidas en el sub-diccionario inicial.
  - Es fácil ver que si existen componentes fuertemente conectadas (SCC), estas
    tienen que estar en el sub-diccionario inicial, ya que no pueden ser
    agregadas después.
  - Luego, puede ser que algunos vértices de estas SCC tengan ejes de salida a
    vértices fuera de la componente, estos también deben ser agregados, ya que
    deben estar definidos antes (o en el sd inicial).

Entonces, nuestro algoritmo hace lo siguiente:
  - Encontrar las componentes fuertemente conectadas.
  - Hacer BFS desde todos los nodos de las componentes.
  - Devolver todos los nodos visitados en el BFS como la solución

Análisis de complejidad:

Veamos cuales son las etapas del programa:

  - Input de ejes (tamaño de palabra acotado)                   O(E)
  - Kosaraju                                                    O(E)
  - BFS                                                         O(E)

Podemos usar que V <= E, ya que todas las palabras tienen una o más palabras en
la definición.

La complejidad del programa en total es O(E)
"""

from collections import Counter, defaultdict, deque
from itertools import groupby, chain

global graph

def kosaraju():
    def visit(w):
        if visited[w] == False:
            visited[w] = True
            for w2 in graph[w]:
                visit(w2)
            l.append(w)

    def assign(w, root):
        if assignment[w] == "":
            assignment[w] = root
            if w in graph_transpose:
                for w2 in graph_transpose[w]:
                    assign(w2, root)

    
    visited = {w:False for w in graph}
    l = []
    for w in graph:
        visit(w)
    l = l[::-1]

    assignment = {w: "" for w in graph}

    for w in l:
        assign(w,w)
    
    return assignment

while True:
    n = int(input())
    if n == 0:
        break

    graph = {}
    graph_transpose = {}
    for i in range(n):
        aux = input().rstrip().split(' ')
        graph[aux[0]] = set(aux[1:])
        for w in aux[1:]:
            if w not in graph_transpose:
                graph_transpose[w] = set()
            graph_transpose[w].add(aux[0])
    
    
    res = kosaraju()

    components = defaultdict(set)

    for w in res:
        components[res[w]].add(w)

    components = [comp for root, comp in components.items() if len(comp) > 1]

    q = deque(list(chain.from_iterable(components)))
    visited = {w:False for w in graph}
    while len(q) > 0:
        w = q.pop()
        if not visited[w]:
            visited[w] = True
            for w2 in graph[w]:
                q.append(w2)
    defres = [w for w in graph if visited[w]]

    print(len(defres))
    print(" ".join(sorted(defres)))
