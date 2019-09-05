/*
Queremos resolver el problema de https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=2048

Para este problema, tenemos n strings {s_1, s_2, ...}, en los cuales tenemos que
buscar el substring más largo que esté en más de la mitad de ellos. Para esto,
creamos un string s = s_1$_1s_2$_2s_3$_3... Los símbolos $ que agregamos sirven
para tener en claro donde termina cada string, son lexicográficamente más chicos
que todos los caracteres y diferentes entre sí. Luego, buscamos el suffix array
de s.
El suffix array es un vector que representa el orden lexicográfico de los sufijos
de nuestro string. Es decir, para i < j, s[sa[i]..] < s[sa[j]..].
La idea es que en el suffix array, van a quedar "juntos" los sufijos que
compartan prefijos.
Luego, basándonos en el suffix array, generamos el longest common prefix array.
Definimos lcp[i] = max_k : s[sa[i]..sa[i] + k] = s[sa[i+1]..sa[i+1] + k].
Coloquialmente, el largo del prefijo que comparten dos sufijos contiguos en el 
suffix array.
Es fácil ver que, podemos computar el lcp(s1,s2) = min_(s1<= i < s2) lcp[i]
Ahora, tenemos que encontrar el prefijo más largo, que sea compartido entre
sufijos que empiecen en más de n/2 strings. Para esto, vamos corriendo una
ventana que tenga sufijos pertenecientes a más de n/2 strings, calculando el
mínimo de los lcp. De estos mínimos, calculamos el máximo, y esa será la 
solución de nuestro problema.

Análisis de complejidad (tomamos L como largo combinado de todos los strings):

  - Armado de s                 O(L)
  - Suffix Array                O(L)
  - LCP Array                   O(L)
  - Creación de ventanas        O(L log L)

La creación de las ventanas ejecuta L veces inserciones y borrado de estructuras
logarítmicas. Se podrían usar hash tables, pero necesitamos el mínimo de una de
estas estructuras. Por ende, la complejidad final del algoritmo es O(L log L)
*/

#define Martin using
#define Mongi namespace
#define Badia std

#include <cstddef>
#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <deque>
#include <vector>
#include <map>
#include <algorithm>
#include <assert.h>
#include <math.h>
#include <algorithm>

Martin Mongi Badia;

class Counter
{
private:
  map<int, int> m;
  int total_sum;

public:
  void add(int k, int v)
  {
    if (m.find(k) == m.end())
      m[k] = 0;
    m[k] += v;
    total_sum += v;
    if (m[k] == 0)
      m.erase(k);
  }
  int find(int k)
  {
    if (m.find(k) == m.end())
      return 0;
    return m[k];
  }
  int sum()
  {
    return total_sum;
  }
  int size()
  {
    return m.size();
  }
  int min()
  {
    if (size() != 0)
      return m.begin()->first;
    return -1;
  }
  void print()
  {
    for (auto kv : m)
    {
      if (kv.second != 0)
        cout << kv.first << ":" << kv.second << ", ";
    }
    cout << "\n";
  }
};

inline bool leq(int a1, int a2, int b1, int b2) // lexicographic order
{
  return (a1 < b1 || a1 == b1 && a2 <= b2);
} // for pairs
inline bool leq(int a1, int a2, int a3, int b1, int b2, int b3)
{
  return (a1 < b1 || a1 == b1 && leq(a2, a3, b2, b3));
} // and triples
// stably sort a[0..n-1] to b[0..n-1] with keys in 0..K from r
static void radixPass(int *a, int *b, int *r, int n, int K)
{                          // count occurrences
  int *c = new int[K + 1]; // counter array
  for (int i = 0; i <= K; i++)
    c[i] = 0; // reset counters
  for (int i = 0; i < n; i++)
    c[r[a[i]]]++;                       // count occurrences
  for (int i = 0, sum = 0; i <= K; i++) // exclusive prefix sums
  {
    int t = c[i];
    c[i] = sum;
    sum += t;
  }
  for (int i = 0; i < n; i++)
    b[c[r[a[i]]]++] = a[i]; // sort
  delete[] c;
}

// find the suffix array SA of s[0..n-1] in {1..K}ˆn
// require s[n]=s[n+1]=s[n+2]=0, n>=2
void suffixArray(int *s, int *SA, int n, int K)
{
  int n0 = (n + 2) / 3, n1 = (n + 1) / 3, n2 = n / 3, n02 = n0 + n2;
  int *s12 = new int[n02 + 3];
  s12[n02] = s12[n02 + 1] = s12[n02 + 2] = 0;
  int *SA12 = new int[n02 + 3];
  SA12[n02] = SA12[n02 + 1] = SA12[n02 + 2] = 0;
  int *s0 = new int[n0];
  int *SA0 = new int[n0];
  // generate positions of mod 1 and mod 2 suffixes
  // the "+(n0-n1)" adds a dummy mod 1 suffix if n%3 == 1
  for (int i = 0, j = 0; i < n + (n0 - n1); i++)
    if (i % 3 != 0)
      s12[j++] = i;
  // lsb radix sort the mod 1 and mod 2 triples
  radixPass(s12, SA12, s + 2, n02, K);
  radixPass(SA12, s12, s + 1, n02, K);
  radixPass(s12, SA12, s, n02, K);
  // find lexicographic names of triples
  int name = 0, c0 = -1, c1 = -1, c2 = -1;
  for (int i = 0; i < n02; i++)
  {
    if (s[SA12[i]] != c0 || s[SA12[i] + 1] != c1 || s[SA12[i] + 2] != c2)
    {
      name++;
      c0 = s[SA12[i]];
      c1 = s[SA12[i] + 1];
      c2 = s[SA12[i] + 2];
    }
    if (SA12[i] % 3 == 1)
    {
      s12[SA12[i] / 3] = name;
    } // left half
    else
    {
      s12[SA12[i] / 3 + n0] = name;
    } // right half
  }
  // recurse if names are not yet unique
  if (name < n02)
  {
    suffixArray(s12, SA12, n02, name);
    // store unique names in s12 using the suffix array
    for (int i = 0; i < n02; i++)
      s12[SA12[i]] = i + 1;
  }
  else // generate the suffix array of s12 directly
    for (int i = 0; i < n02; i++)
      SA12[s12[i] - 1] = i;
  // stably sort the mod 0 suffixes from SA12 by their first character
  for (int i = 0, j = 0; i < n02; i++)
    if (SA12[i] < n0)
      s0[j++] = 3 * SA12[i];
  radixPass(s0, SA0, s, n0, K);
  // merge sorted SA0 suffixes and sorted SA12 suffixes
  for (int p = 0, t = n0 - n1, k = 0; k < n; k++)
  {
#define GetI() (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2)
    int i = GetI();    // pos of current offset 12 suffix
    int j = SA0[p];    // pos of current offset 0 suffix
    if (SA12[t] < n0 ? // different compares for mod 1 and mod 2 suffixes
            leq(s[i], s12[SA12[t] + n0], s[j], s12[j / 3])
                     : leq(s[i], s[i + 1], s12[SA12[t] - n0 + 1], s[j], s[j + 1], s12[j / 3 + n0]))
    { // suffix from SA12 is smaller
      SA[k] = i;
      t++;
      if (t == n02) // done --- only SA0 suffixes left
        for (k++; p < n0; p++, k++)
          SA[k] = SA0[p];
    }
    else
    { // suffix from SA0 is smaller
      SA[k] = j;
      p++;
      if (p == n0) // done --- only SA12 suffixes left
        for (k++; t < n02; t++, k++)
          SA[k] = GetI();
    }
  }
  delete[] s12;
  delete[] SA12;
  delete[] SA0;
  delete[] s0;
}

vector<int> lcp_array(const vector<int> &s, const vector<int> &sa)
{
  int n = s.size();
  vector<int> rank(n), res(n - 1);
  for (int i = 0; i < n; ++i)
    rank[sa[i]] = i;
  int h = 0;
  for (int i = 0; i < n; ++i)
  {
    if (rank[i] > 1)
    {
      int j = sa[rank[i] - 1];
      while (s[i + h] == s[j + h])
        h++;
      res[rank[i] - 1] = h;
      if (h > 0)
        h--;
    }
  }
  return res;
}

int main()
{
  int sn;
  string ss;
  bool nl = false;
  while (cin >> sn, sn != 0)
  {
    if (nl)
      cout << endl;
    else
      nl = true;
    vector<int> s, suffix_to_sn;
    for (int i = 1; i <= sn; ++i)
    {
      cin >> ss;
      for (auto c : ss)
      {
        s.push_back(c - 'a' + sn + 1);
        suffix_to_sn.push_back(i);
      }
      s.push_back(i);
      suffix_to_sn.push_back(0);
    }
    if (sn == 1)
    {
      cout << ss << "\n";
      continue;
    }

    int n = s.size(), k = 'z' - 'a' + sn + 2;

    s.push_back(0), s.push_back(0), s.push_back(0);
    vector<int> sa(s.size());
    suffixArray(s.data(), sa.data(), n, k);
    for (int i = 0; i < 3; ++i)
      s.pop_back(), sa.pop_back();

    vector<int> lcp = lcp_array(s, sa);

    Counter shs;    //segment has string
    Counter window; // lcp window
    vector<int> lcp_k(n);

    // Busco el primer K-good segment
    k = (sn + 2) / 2;
    int right = sn, left = sn;

    for (; shs.size() < k; ++right)
      shs.add(suffix_to_sn[sa[right]], 1);
    for (int i = left; i < right - 1; ++i)
      window.add(lcp[i], 1);

    while (right <= n)
    {
      lcp_k[left] = window.min();

      //saco el de mas a la izquierda
      shs.add(suffix_to_sn[sa[left]], -1);
      window.add(lcp[left], -1);
      left++;

      for (; shs.size() < k && right <= n; ++right)
      {
        shs.add(suffix_to_sn[sa[right]], 1);
        window.add(lcp[right - 1], 1);
      }
    }

    int goal = *max_element(lcp_k.begin(), lcp_k.end());
    if (goal == 0)
    {
      cout << "?\n";
      continue;
    }
    set<string> res;
    for (int i = 0; i < lcp_k.size(); ++i)
    {
      if (lcp_k[i] == goal)
      {
        string res_s;
        for (int j = sa[i]; j < sa[i] + goal; ++j)
          res_s.push_back(s[j] + 'a' - sn - 1);
        res.insert(res_s);
      }
    }
    for (auto w : res)
    {
      cout << w << endl;
    }
  }
}