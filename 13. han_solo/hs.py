## Queremos resolver el problema de https://codeforces.com/problemset/problem/514/B

# Para esto, debemos calcular cuantas lineas diferentes necesitamos para poder
# cubrir todos los puntos. Luego, podemos caracterizar cada linea que necesitamos
# disparar con la pendiente, calculamos cada una y las agregamos a un conjunto.
# Con este conjunto podemos saber cuantas lineas se necesitan. La complejidad es
# O(n) usando un hash table como conjunto.

n, x0, y0 = list(map(int,input().split(" ")))

ms = set()

for i in range(n):
    x,y = list(map(int,input().split(" ")))
    x -= x0
    y -= y0
    m = y/x if x != 0 else float('+inf')
    ms.add(m)

print(len(ms))
