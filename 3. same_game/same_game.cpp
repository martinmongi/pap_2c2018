#define Martin using
#define Mongi namespace
#define Badia std

#include <iostream>
#include <vector>

Martin Mongi Badia;
/*
Queremos resolver el problema de https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=699

En este problema, para cada tablero se nos pide simular la jugada de una estrategia
en particular, y ver cuantos puntos podemos sacar. La estrategia es la siguiente:
  - Remuevo el bloque con más bolas
  - Si hay empate, el que tenga una bola más a la izquierda
  - Si hay empate, el que tenga una bola más abajo
Primero, vamos a necesitar identificar estos bloques. Podríamos pensar el
tablero como un grafo, donde cada bola está conectada con sus vecinas, a menos
que difieran en color. De esta forma, los bloques serían las componentes conexas
de nuestro modelo. Esto se puede encontrar fácilmente con DFS. Jugamos con el
orden de búsqueda de las componentes para estar seguros que manejamos bien los
empates.
Luego, tenemos que ejecutar la jugada. Usando el resultado anterior, podemos
simplemente cambiar esas bolas por espacios vacios. Después, comprimimos el 
tablero como se pide, y hacemos los cálculos de puntajes pertinentes.

Análisis de complejidad:

Vamos a tomar como n y m los lados del tablero, ya que si no sería todo
constante y pierde la gracia :)

El programa consta de los siguientes pasos para cada tablero:
  - Ingreso de datos                O(nm)
  - Para cada jugada:
      - Búsqueda de bloques (DFS)   O(nm)
        (tiene cantidad de ejes acotada linealmente por cantidad de vertices)
      - Búsqueda del bloque máximo  O(nm)
        (a lo sumo nm bloques)
      - Cálculos varios             O(1)
      - Liberar bloque              O(nm)
      - Comprimir tablero           O(nm)

Tenemos que para cada jugada, requerimos un número de operaciones O(nm). Como
podemos tener O(nm) jugadas, la complejidad final del algoritmo es O(n^2*m^2)

*/
char board[10][15];
char seen[10][15];

inline void clear_seen()
{
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 15; j++)
            seen[i][j] = 'W';
}
vector<pair<int, int>> dfs(int i, int j)
{
    vector<pair<int, int>> res, aux;
    if (seen[i][j] != 'W')
        return res;
    seen[i][j] = 'G';
    res.insert(res.end(), make_pair(i, j));
    if ((j + 1 < 15) && (board[i][j + 1] == board[i][j]))
    {
        aux = dfs(i, j + 1);
        res.insert(res.end(), aux.begin(), aux.end());
    }
    if ((j - 1 >= 0) && (board[i][j - 1] == board[i][j]))
    {
        aux = dfs(i, j - 1);
        res.insert(res.end(), aux.begin(), aux.end());
    }
    if ((i + 1 < 10) && (board[i + 1][j] == board[i][j]))
    {
        aux = dfs(i + 1, j);
        res.insert(res.end(), aux.begin(), aux.end());
    }
    if ((i - 1 >= 0) && (board[i - 1][j] == board[i][j]))
    {
        aux = dfs(i - 1, j);
        res.insert(res.end(), aux.begin(), aux.end());
    }
    seen[i][j] = 'B';
    return res;
}
vector<vector<pair<int, int>>> block_search()
{
    vector<vector<pair<int, int>>> res;
    clear_seen();
    for (int j = 0; j < 15; j++)
    {
        for (int i = 9; i >= 0; i--)
        {
            // cout << i << ' ' << j << '\n';
            if (seen[i][j] == 'W' && board[i][j] != '-')
            {
                res.insert(res.end(), dfs(i, j));
            }
        }
    }
    return res;
}
void compress()
{
    for (int j = 0; j < 15; ++j)
    {
        int ci = 9;
        for (int i = 9; i > 0; --i)
        {
            if (board[i][j] == '-')
            {
                while (board[ci][j] == '-' && ci >= 0)
                {
                    ci--;
                }
                if (ci < 0)
                    break;
                board[i][j] = board[ci][j];
                board[ci][j] = '-';
            }
            ci--;
        }
    }

    int cj = 0;
    for (int j = 0; j < 15; ++j)
    {
        if (board[9][j] == '-')
        {
            while (board[9][cj] == '-' && cj < 15)
            {
                cj++;
            }
            if (cj == 15)
                break;
            for (int i = 0; i < 10; ++i)
            {
                board[i][j] = board[i][cj];
                board[i][cj] = '-';
            }
        }
        cj++;
    }
}
int main()
{
    int t;
    cin >> t;
    for (int casen = 1; casen <= t; casen++)
    {
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 15; j++)
                cin >> board[i][j];

        cout << "Game " << casen << ":\n\n";

        int ball_count = 150, score = 0, move_count = 0;
        while (true)
        {
            auto res = block_search();
            unsigned int max_s = 0;
            vector<pair<int, int>> biggest_set;
            for (auto s : res)
            {
                if (s.size() > max_s)
                {
                    biggest_set = s;
                    max_s = s.size();
                }
            }
            if (biggest_set.size() < 2)
                break;
            ball_count -= biggest_set.size();
            score += (biggest_set.size() - 2) * (biggest_set.size() - 2);
            move_count++;
            cout << "Move " << move_count
                 << " at (" << 10 - biggest_set[0].first
                 << "," << biggest_set[0].second + 1
                 << "): removed " << biggest_set.size()
                 << " balls of color " << board[biggest_set[0].first][biggest_set[0].second]
                 << ", got " << (biggest_set.size() - 2) * (biggest_set.size() - 2)
                 << " points.\n";

            for (auto p : biggest_set)
                board[p.first][p.second] = '-';
            compress();
        }
        if (ball_count == 0)
            score += 1000;
        cout << "Final score: " << score
             << ", with " << ball_count
             << " balls remaining.\n";

        if (casen < t)
            cout << '\n';
    }
}