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

Martin Mongi Badia;

/*
Queremos resolver el problema de https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=1202

Para esto, vemos que el problema se puede modelar como un 0,1-knapsack. Funciona 
de la siguiente manera:
    - Tomamos c_1, ... c_n autos
    - Chequeamos si hay una particion en C_a y C_b tal que la suma de ambas sea 
      menor al largo del bote
    - Si la hay, terminamos. Si no, tomamos c_1, ... c_(n-1)

Para ver si es posible encontrar esta particion, nos fijamos cual es el
subconjunto C_a con suma máxima, pero menor al largo del bote. Si, además, la
suma de C_b también es menor, hemos encontrado la partición. Si no, no es
posible con esta cantidad de autos.

Para encontrar esta partición, usamos una técnica bottom up, aprovechando la
integralidad de los pesos. Creamos una matriz K en R^(cant_autos+1 x largo + 1)
donde K[i][j] representa la suma máxima <= j que se puede conseguir usando 
algunos de los autos de C_1 ... C_i. De esta forma, podemos aprovechar la 
suboptimalidad del problema y ahorrarnos calculos innecesarios.

Analicemos la complejidad:
    - El paso algorítmico de peor complejidad es recorrer la matriz de tamaño
      n x w, haciendo computos O(1) para cada posición de ella.
Por lo tanto, la complejidad del programa es O(n x w)
*/

int main()
{
    int t;
    cin >> t;
    while(t--)
    {
        int boat_length, car_length;
        cin >> boat_length;
        boat_length *= 100;
        vector<int> car_lengths;
        vector<bool> sol;

        while (cin >> car_length, car_length != 0)
            car_lengths.push_back(car_length);
        vector<vector<int>> k(car_lengths.size() + 1, vector<int>(boat_length + 1));
        vector<vector<bool>> u(car_lengths.size() + 1, vector<bool>(boat_length + 1));

        for (int i = 0; i <= car_lengths.size(); ++i)
        {
            for (int w = 0; w <= boat_length; ++w)
            {
                if (w == 0 || i == 0)
                {
                    k[i][w] = 0;
                }
                else if (car_lengths[i - 1] <= w)
                {
                    int using_i = k[i - 1][w - car_lengths[i - 1]] + car_lengths[i - 1];
                    int not_using_i = k[i - 1][w];
                    k[i][w] = max(using_i, not_using_i);
                    u[i][w] = using_i > not_using_i;
                }
                else
                {
                    k[i][w] = k[i - 1][w];
                }
            }
        }

        while (true)
        {
            int w = boat_length;
            sol = vector<bool>(car_lengths.size(), false);
            for (int i = car_lengths.size(); i > 0; --i)
            {
                if (u[i][w])
                {
                    sol[i - 1] = true;
                    w = w - car_lengths[i - 1];
                }
            }
            int whole_sum = 0, sol_sum = 0;
            for (int i = 0; i < car_lengths.size(); ++i)
            {
                whole_sum += car_lengths[i];
                if (sol[i])
                    sol_sum += car_lengths[i];
            }
            if (whole_sum - sol_sum > boat_length)
                car_lengths.pop_back();
            else
                break;
        }
        cout << car_lengths.size() << '\n';
        for (auto b : sol)
            cout << (b ? "starboard" : "port") << '\n';
        if(t) cout << '\n';
    }
    return 0;
}