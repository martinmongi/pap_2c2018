import functools


@functools.lru_cache()
def f(x, y, b):
    if x == y:
        return [x]
    # print("Called with", x, y, b)
    if b:               # punta en x
        tail = f((x + 1) % n, y, True)
        if tail and (x, (x + 1) % n) in e:
            return [x] + tail
        tail = f((x + 1) % n, y, False)
        if tail and (x, y) in e:
            return [x] + tail
    else:               # punta en y
        tail = f(x, (y - 1) % n, True)
        if tail and (x, y) in e:
            return [y] + tail
        tail = f(x, (y - 1) % n, False)
        if tail and ((y - 1) % n, y) in e:
            return [y] + tail
    # print("Returning",x,y,b, "--->", res)
    return None


global e, n
n = int(input())
m = int(input())
e = set()

for i in range(m):
    x, y = map(int, input().split(' '))
    e.add((x - 1, y - 1))
    e.add((y - 1, x - 1))
# print(e)

for i in range(n):
    if f(i, (i - 1) % n, True):
        for j in f(i, (i - 1) % n, True):
            print(j+1)
        exit()
        # print("============")
print(-1)

