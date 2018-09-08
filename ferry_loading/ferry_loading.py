from collections import Counter

t = int(input())
for case in range(t):
    cl = input()
    boat_length = int(input()) * 100

    car_lengths = []
    while True:
        cl = int(input())
        if cl == 0:
            break
        car_lengths.append(cl)

    while True:
        K = [[0 for i in range(boat_length + 1)]
             for i in range(len(car_lengths) + 1)]
        U = [[False for i in range(boat_length + 1)]
             for i in range(len(car_lengths) + 1)]

        for i in range(len(car_lengths) + 1):
            for w in range(boat_length + 1):
                if i == 0 or w == 0:
                    continue
                elif car_lengths[i - 1] <= w:
                    using = K[i - 1][w - car_lengths[i - 1]] + \
                        car_lengths[i - 1]
                    not_using = K[i - 1][w]
                    K[i][w] = max(using, not_using)
                    U[i][w] = using > not_using
                else:
                    K[i][w] = K[i - 1][w]

        i = len(car_lengths)
        w = boat_length
        sol = []

        while i > 0:
            if U[i][w]:
                sol.append(i - 1)
                w = w - car_lengths[i - 1]
            i -= 1
        if sum(car_lengths) - sum([car_lengths[i] for i in sol]) > boat_length:
            car_lengths = car_lengths[:-1]
        else:
            break
    # print(sol, [car_lengths[i] for i in sol])
    # print([i for i in range(len(car_lengths)) if i not in sol],
    #       [car_lengths[i] for i in range(len(car_lengths)) if i not in sol])
    print(len(car_lengths))
    for i in range(len(car_lengths)):
        if i in sol:
            print("starboard")
        else:
            print("port")
    print("")
