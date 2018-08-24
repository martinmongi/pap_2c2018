#define Martin using
#define Mongi namespace
#define Badia std

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

Martin Mongi Badia;

enum Side
{
    starboard,
    port
};

typedef struct P
{
    int starboard_length;
    int port_length;
    vector<int> car_lengths;
} P;

map<P, vector<Side>> mem;

vector<Side> solve(P &problem)
{
    if (problem.starboard_length >= 0 &&
        problem.port_length >= 0 &&
        problem.car_lengths.size() == 0)
    {
        return vector<Side>();
    }

    auto it = mem.find(problem);
    if (it != mem.end())
    {
        return it->second;
    }
    P problem2;
    problem2.starboard_length = problem.port_length;
    problem2.port_length = problem.starboard_length;
    problem2.car_lengths = problem.car_lengths;
    auto it = mem.find(problem2);
    if (it != mem.end())
    {
        return it->second;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;

    for (int i = 0; i < t; ++i)
    {
        int boat_length;
        cin >> boat_length;
        boat_length *= 1000;
        vector<int> car_lengths;

        while (true)
        {
            int aux;
            cin >> aux;
            if (aux == 0)
                break;
            car_lengths.push_back(aux);
        }
        sort(car_lengths.begin(), car_lengths.end());

        P problem;
        problem.starboard_length = boat_length;
        problem.port_length = boat_length;
        problem.car_lengths = car_lengths;
        solve(problem);
    }
    return 0;
}


// Nueva idea:
// Cada auto que agarro, genero las 2 posibilidades (mandarlo a estribor y a babor)
// Me quedo con todas las posibilidades que se pueden ir haciendo (puedo tirar casi la mitad,
// solo guardando las de estribor >= babor), de esta forma no hago los mismos calculos
// a lo pavote varias veces