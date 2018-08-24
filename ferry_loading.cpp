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

int main()
{
    int t;

    cin >> t;

    for (int case_n = 0; case_n < t; ++case_n)
    {
        int boat_length;
        cin >> boat_length;
        boat_length *= 100;
        vector<int> car_lengths;
        vector<bool> sol;

        while (true)
        {
            int car_length;
            cin >> car_length;
            if (car_length == 0)
                break;
            car_lengths.push_back(car_length);
        }
        vector<vector<int>> k(car_lengths.size() + 1, vector<int>(boat_length + 1));
        vector<vector<bool>> u(car_lengths.size() + 1, vector<bool>(boat_length + 1));

        while (true)
        {
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
        cout << '\n';
    }
    return 0;
}
// Quiero dejar de vivir, no tira mas TLE, pero tira WA