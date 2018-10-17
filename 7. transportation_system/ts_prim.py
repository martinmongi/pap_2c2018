
t = int(input())

for case in range(1, t + 1):
    n, threshold = tuple(map(int, input().rstrip().split(' ')))
    cities = {}
    for i in range(n):
        cities[i] = tuple(map(int, input().rstrip().split(' ')))
    print(cities)
    
