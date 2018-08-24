#define Martin using
#define Mongi namespace
#define Badia std

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <tuple>

Martin Mongi Badia;

enum Side
{
    starboard,
    port
};

int main()
{
    int t;
    cin >> t;
    cout << "CASOS: " << t << '\n';

    for (int i = 0; i < t; ++i)
    {
        int boat_length;
        cin >> boat_length;
        boat_length *= 100;
        set<tuple<int,int>> sols;
        sols.insert(tuple<int,int>(0,0));

        while (true)
        {
            int aux;
            cin >> aux;
            if (aux == 0)
                break;
            set<tuple<int, int>> new_sols;
            for(auto sol : sols) {
                if (get<0>(sol) + aux <= boat_length)
                    new_sols.insert(tuple<int, int>(get<0>(sol) + aux, get<1>(sol)));
                if (get<1>(sol) + aux <= boat_length)
                    new_sols.insert(tuple<int, int>(get<0>(sol), get<1>(sol) + aux));
            }
            cout << "--------\n";
            sols = new_sols;
            for(auto sol : sols) {
                cout << get<0>(sol) << '\t' << get<1>(sol) << '\n';
            }
        }

    }
    return 0;
}


// Hay que toquetear un poco mas al respecto, guardar soluciones, dejar de duplicar casos y demas
// La base esta