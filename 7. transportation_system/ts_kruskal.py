class UnionFind:
    def __init__(self, n):
        self.parent = [i for i in range(n)]
        self.rank = [0 for i in range(n)]

    def find(self, x):
        while self.parent[x] != x:
            x, self.parent[x] = self.parent[x], self.parent[self.parent[x]]
        return x

    def union(self, x, y):
        xroot = self.find(x)
        yroot = self.find(y)
        if xroot == yroot:
            return
        if self.rank[xroot] < self.rank[yroot]:
            xroot, yroot = yroot, xroot
        self.parent[yroot] = xroot
        if self.rank[xroot] == self.rank[yroot]:
            self.rank[xroot] += 1


def dist_sq(c1, c2):
    return ((c1[0] - c2[0])**2 + (c1[1] - c2[1])**2)


t = int(input())

for case in range(1, t + 1):
    n, threshold = tuple(map(int, input().rstrip().split(' ')))
    threshold *= threshold
    cities = [tuple(map(int, input().rstrip().split(' '))) for i in range(n)]
    edges = [((i, j), dist_sq(cities[i], cities[j]))
             for i in range(n) for j in range(i + 1, n) ]
    edges.sort(key=(lambda c: c[1]))

    mst = []
    uf = UnionFind(n)
    for e in edges:
        if uf.find(e[0][0]) != uf.find(e[0][1]):
            mst.append(e)
            uf.union(e[0][0], e[0][1])
        if len(mst) == n - 1:
            break
    # print(mst)

    # for e in mst:
    #     print(e[1]**.5)
    roads = [e[1]**.5 for e in mst if e[1] <= threshold]
    railroads = [e[1]**.5 for e in mst if e[1] > threshold]

    states = len(railroads) + 1
    rl = round(sum(e for e in roads))
    rrl = round(sum(e for e in railroads))
    # print(roads, railroads)

    print("Case #" + str(case) + ":", states, rl, rrl)
