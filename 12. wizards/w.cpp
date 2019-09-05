/*
Queremos resolver el problema de https://codeforces.com/contest/167/problem/B

Tenemos que calcular la probabilidad de terminar todos los juegos con capacidad
suficiente y con las suficientes victorias para tener buena performance. Para 
esto definimos la siguiente función:

    f(p,w,k) = probabilidad de que despues de las primeras p jugadas,
               haya ganado w y tenga capacidad restante k

Podemos recursivamente calcular esta probabilidad:

    f(0,0,K_inicial) = 1
    f(p,w,k) = f(p-1,w-1,k-prizes[p-1])*p[i-1] +
               f(p-1,w,k)*(1-p[i-1])

Esto lo podemos resolver dinámicamente, con una matriz que almacene los valores
ya computados. Luego, el resultado es sum_(k in [0..n]) sum_(w in [l..n]) f(n,w,k).

Análisis de complejidad:

Tenemoz una matriz de O(n^2) unordered_maps, como podemos acotar la capacidad
restante por n, en cada map hay como mucho n pares kv. Luego, recorrer toda la
matriz tiene una complejidad de O(n^3)
*/

#define Martin using
#define Mongi namespace
#define Badia std

#include <iostream>
#include <vector>
#include <unordered_map>

Martin Mongi Badia;

int main()
{
    int n, l, k;
    cin >> n >> l >> k;
    vector<double> p(n), pl(n);
    k = n < k ? n : k;
    for (int i = 0; i < n; ++i)
    {
        cin >> p[i];
        p[i] /= 100;
    }
    for (int i = 0; i < n; ++i)
        cin >> pl[i];

    // f[played][won][cap] = p
    unordered_map<int,double> f[201][201];

    f[0][0][k] = 1.;
    for (int played = 0; played < n; ++played)
    {
        for (int won = 0; won <= played; won++)
        {
            for (auto kv : f[played][won])
            {
                int cap = kv.first;
                double cprob = kv.second;
                // cout << won << '/' << played << ':'
                //      << cap << '\t' << cprob << endl;

                int won_cap = cap + pl[played];
                won_cap = won_cap > n ? n : won_cap;
                if (f[played + 1][won + 1].find(won_cap) ==
                    f[played + 1][won + 1].end())
                    f[played + 1][won + 1][won_cap] = 0;
                f[played + 1][won + 1][won_cap] += cprob * p[played];

                if (f[played + 1][won].find(cap) ==
                    f[played + 1][won].end())
                    f[played + 1][won][cap] = 0;
                f[played + 1][won][cap] += cprob * (1 - p[played]);
            }
        }
    }

    double res = 0;

    for (int won = l; won <= n; won++)
    {
        for (auto kv : f[n][won])
        {
            if (kv.first >= 0)
                res += kv.second;
        }
    }
    cout << res << endl;
}