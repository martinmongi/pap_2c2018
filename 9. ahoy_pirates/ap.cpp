/*
Queremos resolver el problema de https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=2397

Para esto, podemos utilizar una estructura de datos llamada segment tree. Esta es
una estructura en la cual podemos representar a los piratas como una tira de 1s
y 0s según el tipo de piratas que sean. Se representa como un árbol en el cual
la raíz representa al intervalo completo, y sus hijos representan subintervalos
disjuntos de él, y así sucesivamente. Por ejemplo, podríamos representar un 
conjunto de 8 piratas de la siguiente forma:

[0,8) --> [0,4) --> [0,2) --> [0,1)
                          --> [1,2)
                --> [2,4) --> [2,3)
                          --> [3,4)
      --> [4,8) --> [4,6) --> [4,5)
                          --> [5,6)
                --> [6,8) --> [6,7)
                          --> [7,8)

La gracia de este tipo de estructura es poder guardar información en cada nodo,
que nos permita hacer una cantidad constante de recorridos de la altura del
árbol.

Supongamos que armaramos el árbol, sin datos en los nodos intermedios. En este
caso, las 3 operaciones necesarias (asignación, inversión y consulta),
requerirían recorrer todas las hojas del árbol corresponiente al intervalo,
potencialmente O(n).

Podemos optimizar la consulta, por ejemplo, agregando la cantidad de piratas de
cada tipo que hay en el subintervalo para cada nodo. De esta forma, podemos
garantizar que esta operación tiene una complejidad de O(log n).

Luego, tenemos el problema de la asignación, que podemos solucionar con una
técnica llamada lazy propagation. Con esta técnica, si sabemos que todo el
intervalo representado por un nodo y sus descendientes, tiene asignado el mismo
valor, lo marcamos y no propagamos la operación para niveles inferiores.
Similarmente, garantizamos que esta operación también tiene complejidad O(log n)

Queda entonces la inversión, que podemos solucionar de forma similar; si todo el
subintervalo del nodo es invertido, lo marcamos en el nodo y no propagamos hasta
que sea necesario. También se puede hacer en O(log n).

Análisis de complejidad

Tenemos entonces:

  - Armado del segment tree         O(n)
  - Queries:                        O(q log n)
    - Asignación                    O(log n)
    - Inversión                     O(log n)
    - Consulta                      O(log n)

Podemos ver, que el programa completo tiene una complejidad de O(n + q log n)
*/
#define Martin using
#define Mongi namespace
#define Badia std

#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>

#define REGULAR 0
#define LAZY_0 1
#define LAZY_1 2
#define INVERTED 3

Martin Mongi Badia;

typedef struct
{
    int size, l, r, val, status;
} Node;

class SegmentTree
{
  private:
    vector<Node> ns;
    void __construct(const string &s, int i, int l, int r);
    void __set_range(int i, int l, int r, int newval);
    void __invert_range(int i, int l, int r);
    int __query(int i, int l, int r);
    void __normalize(int i);

  public:
    SegmentTree(const string &s);
    void SetRange(int l, int r);
    void ResetRange(int l, int r);
    void InvertRange(int l, int r);
    int Query(int l, int r);
};

SegmentTree::SegmentTree(const string &s)
{
    ns = vector<Node>(4 * s.size() - 1);
    __construct(s, 0, 0, s.size());
}

void SegmentTree::SetRange(int l, int r)
{
    __set_range(0, l, r, 1);
}
void SegmentTree::ResetRange(int l, int r)
{
    __set_range(0, l, r, 0);
}
void SegmentTree::InvertRange(int l, int r)
{
    __invert_range(0, l, r);
}
int SegmentTree::Query(int l, int r)
{
    return __query(0, l, r);
}

void SegmentTree::__construct(const string &s, int i, int l, int r)
{
    ns[i].size = r - l;
    ns[i].l = l;
    ns[i].r = r;
    ns[i].status = REGULAR;
    if (ns[i].size <= 1)
    {
        ns[i].val = s[l] == '1' ? 1 : 0;
        return;
    }
    int mid = (int)(r + l) / 2;
    __construct(s, 2 * i + 1, l, mid);
    __construct(s, 2 * i + 2, mid, r);
    ns[i].val = ns[2 * i + 1].val + ns[2 * i + 2].val;
}

void SegmentTree::__set_range(int i, int l, int r, int newval)
{
    if ((r <= ns[i].l) || (ns[i].r <= l))
        return;
    __normalize(i);
    if (ns[i].size == 1)
    {
        ns[i].val = newval;
        return;
    }
    if ((l <= ns[i].l) && (ns[i].r <= r))
    {
        ns[i].status = newval ? LAZY_1 : LAZY_0;
        ns[i].val = newval ? ns[i].size : 0;
        return;
    }
    __set_range(2 * i + 1, l, r, newval);
    __set_range(2 * i + 2, l, r, newval);
    ns[i].val = ns[2 * i + 1].val + ns[2 * i + 2].val;
}

void SegmentTree::__invert_range(int i, int l, int r)
{
    if ((r <= ns[i].l) || (ns[i].r <= l))
        return;
    __normalize(i);
    if (ns[i].size == 1)
    {
        ns[i].val = 1 - ns[i].val;
        return;
    }
    if ((l <= ns[i].l) && (ns[i].r <= r))
    {
        assert(ns[i].status == REGULAR);
        ns[i].status = INVERTED;
        ns[i].val = ns[i].size - ns[i].val;
        return;
    }
    __invert_range(2 * i + 1, l, r);
    __invert_range(2 * i + 2, l, r);
    ns[i].val = ns[2 * i + 1].val + ns[2 * i + 2].val;
}

int SegmentTree::__query(int i, int l, int r)
{
    if ((r <= ns[i].l) || (ns[i].r <= l))
        return 0;
    __normalize(i);
    if (ns[i].size == 1)
        return ns[i].val;
    if ((l <= ns[i].l) && (ns[i].r <= r))
        return ns[i].val;
    return __query(2 * i + 1, l, r) +
           __query(2 * i + 2, l, r);
}

void SegmentTree::__normalize(int i)
{
    if (ns[i].size > 1)
    {
        for (int ci = 2 * i + 1; ci <= 2 * i + 2; ++ci)
        {
            switch (ns[i].status)
            {
            case LAZY_1:
                ns[ci].status = LAZY_1;
                ns[ci].val = ns[ci].size;
                break;
            case LAZY_0:
                ns[ci].status = LAZY_0;
                ns[ci].val = 0;
                break;
            case INVERTED:
                switch (ns[ci].status)
                {
                case LAZY_1:
                    ns[ci].status = LAZY_0;
                    break;
                case LAZY_0:
                    ns[ci].status = LAZY_1;
                    break;
                case REGULAR:
                    ns[ci].status = INVERTED;
                    break;
                case INVERTED:
                    ns[ci].status = REGULAR;
                    break;
                }
                ns[ci].val = ns[ci].size - ns[ci].val;
                break;
            }
        }
    }
    ns[i].status = REGULAR;
}

int main()
{
    ios::sync_with_stdio(false);
    int t;
    cin >> t;
    for (int casen = 1; casen <= t; casen++)
    {
        int m, k, q, qc = 1, i, j;
        string s, ss;
        char qt;
        cin >> m;
        while (m--)
        {
            cin >> k;
            cin >> ss;
            while (k--)
                s.append(ss);
        }

        SegmentTree st(s);
        cout << "Case " << casen << ":\n";
        cin >> q;
        while (q--)
        {
            cin >> qt >> i >> j;
            j++;
            switch (qt)
            {
            case 'F':
                st.SetRange(i, j);
                break;
            case 'E':
                st.ResetRange(i, j);
                break;
            case 'I':
                st.InvertRange(i, j);
                break;
            case 'S':
                cout << "Q" << qc << ": " << st.Query(i, j) << '\n';
                qc++;
                break;
            }
        }
    }

    return 0;
}