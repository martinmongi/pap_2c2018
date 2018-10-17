#define Martin using
#define Mongi namespace
#define Badia std

#include <iostream>
#include <vector>
#include <set>

#define PM(i, n) (i % n + n) % n

Martin Mongi Badia;

/*
Queremos resolver el problema de http://coj.uci.cu/24h/problem.xhtml?pid=3555

Necesitamos encontrar un camino que pase por todos los nodos de nuestro grafo,
sin cruzarse entre sí. Veamos como podemos modelar el no cruce de las aristas:
  - Supongamos que empezamos el camino en el vértice 1; si fueramos por ej.
    a 4, dejaríamos a 2 y 3 de un lado del camino y a [5..n] del otro.
    Entonces, cualquier arista que agreguemos al camino, no debe dejar nodos 
    no visitados a los dos lados.
  - Entonces, supongamos que tenemos un camino que recorre los vértices en
    [3..7], terminando en 7. Este solo podría seguir con los vértices 2 u 8.
Vamos a crear nuestra función recursiva que nos permitirá calcular lo pedido
(omito módulo n para simplificar lectura):

f(x,y,b) =      x = y                                   OR  // camino vacio, ya esta conectado
                b = 1 AND (x,x+1) in E AND f(x+1,y,1)   OR  // punta en x, camino en [x+1..y] con punta en x+1 
                b = 1 AND (x,y) in E AND f(x+1,y,0)     OR  // punta en x, camino en [x+1..y] con punta en y
                b = 0 AND (x,y) in E AND f(x,y-1,1)     OR  // punta en y, camino en [x..y-1] con punta en x
                b = 0 AND (y-1,y) in E AND f(x,y-1,0)   OR  // punta en y, camino en [x..y-1] con punta en y-1

La funcion descripta arriba devuelve verdadero si existe de vertices [x..y] si x <= y
o [0..y] U [x..n] si no, tal que tiene la punta en x si b = 1 o en y si no.

Solo nos basta con llamar a f(i,i-1,1) para saber si existe un camino que recorra
todas las ciudades y empiece en i. Lo podemos llamar para todas las ciudades y si
existe alguno, lo devolvemos.

Análisis de complejidad:

Como no hay limite de acuerdos, mas que el inherente a la cantidad de ciudades,
preferí modelar el grafo como una matriz de adyacencia. Esto, a costa de un mayor
costo de inicialización, nos permite consultar en O(1).
Entonces, tenemos:
  - Inicializacion de matrices 3*n^2 in O(n^2)
  - Ingreso de acuerdos O(m) in O(n^2)
  - Para cada vertice, calcular si hay un camino que empieza en él.
    Estas llamadas a f(..), sumadas, a lo sumo tienen orden cuadrático, ya que
    con programación dinámica podemos ahorrar calcular subproblemas.

Podemos concluir que el algoritmo tiene complejidad O(n^2)
*/

int mem[1000][1000][2];
int n, m;
bool edges[1000][1000];

int f(int x, int y, int side)
{
    if (mem[x][y][side] == -1)
    {
        if (x == y)
            mem[x][y][side] = x;
        else
        {
            if (side)
            {
                if (edges[x][PM(x + 1, n)] && (f(PM(x + 1, n), y, 1) >= 0))
                    mem[x][y][side] = PM(x + 1, n);
                else if (edges[x][y] && (f(PM(x + 1, n), y, 0) >= 0))
                    mem[x][y][side] = y;
                else
                    mem[x][y][side] = -2;
            }
            else
            {
                if (edges[x][y] && (f(x, PM(y - 1, n), 1) >= 0))
                    mem[x][y][side] = x;
                else if (edges[PM(y - 1, n)][y] && (f(x, PM(y - 1, n), 0) >= 0))
                    mem[x][y][side] = PM(y - 1, n);
                else
                    mem[x][y][side] = -2;
            }
        }
    }
    // cout << x << '\t' << y << '\t' << side << '\t' << mem[x][y][side] << endl;
    return mem[x][y][side];
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> m;

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            for (int k = 0; k < 2; ++k)
            {
                mem[i][j][k] = -1;
            }
            edges[i][j] = false;
        }
    }

    while (m--)
    {
        int x, y;
        cin >> x >> y;
        edges[x - 1][y - 1] = true;
        edges[y - 1][x - 1] = true;
    }

    for (int i = 0; i < n; ++i)
    {
        if (f(i, PM(i - 1, n), 1) >= 0)
        {
            int start = i;
            int end = PM(start - 1, n);
            int side = 1;

            cout << start + 1 << '\n';
            int ct = n;
            while (start != end && ct--)
            {
                int next = f(start, end, side);
                if (side) // la punta esta en start
                    start = PM(start + 1, n);
                else // la punta esta en end
                    end = PM(end - 1, n);
                side = next == start;
                cout << next + 1 << '\n';
            }
            return 0;
        }
    }
    cout << "-1\n";
    return 0;
}