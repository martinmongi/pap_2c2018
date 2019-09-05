
from collections import defaultdict, Counter

n, l, k = list(map(int, input().split(" ")))
k = min(n, k)
prob = list(map((lambda x: float(x) / 100), input().split(" ")))
prizes = list(map(int, input().split(" ")))

stats = {0: Counter({(k, 0): 1})}

for round in range(n):
    stats[round + 1] = Counter()
    for j in stats[round]:
        cap = j[0]
        won = j[1]
        p = stats[round][j]
        stats[round + 1][(min(cap + prizes[round], k),
                          won + 1)] += p * prob[round]
        stats[round + 1][(cap, won)] += p * (1 - prob[round])

s = 0
for k in stats[n]:
    if k[0] >= 0 and k[1] >= l:
        s += stats[n][k]

# for i in range(n):
#     for k in stats[i]:
#         print(i, k, stats[i][k])

print(s)
