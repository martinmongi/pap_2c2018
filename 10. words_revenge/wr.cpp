/*
Queremos resolver el problema de https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=2397

*/
#define Martin using
#define Mongi namespace
#define Badia std

#include <cstddef>
#include <iostream>
#include <vector>
#include <set>
#include <deque>
#include <algorithm>
#include <assert.h>
#include <math.h>
#include <algorithm>

Martin Mongi Badia;

typedef struct node
{
    bool terminal;
    int next[2];
    int fail, dict_fail;
    string s;
} Node;

class AhoCorasickTrie
{
  private:
    vector<Node> ns;

  public:
    set<string> buffered_words, words_in_trie;
    AhoCorasickTrie();
    void BufferWord(const string &s);
    void FlushBuffer();
    int MatchString(const string &s);
    void Print();
};

AhoCorasickTrie::AhoCorasickTrie()
{
    Node n;
    n.terminal = false;
    n.next[0] = -1;
    n.next[1] = -1;
    n.fail = 0;
    n.dict_fail = 0;
    n.s = "";
    ns.push_back(n);
}

int AhoCorasickTrie::MatchString(const string &s)
{
    int cn = 0, res = 0;
    for (auto c : s)
    {
        int i = (c == '1') ? 1 : 0;
        while (ns[cn].next[i] == -1 && cn > 0)
        {
            cn = ns[cn].fail; // go up until I have a way down
        }
        if (ns[cn].next[i] == -1)
            continue; //reached the top and theres no path
        cn = ns[cn].next[i];
        if (ns[cn].terminal)
        {
            // cout << "FOUND " << ns[cn].s << endl;
            res++;
        }
        int dsn = ns[cn].dict_fail;
        while (dsn > 0)
        {
            // cout << "FOUND " << ns[dsn].s << endl;
            dsn = ns[dsn].dict_fail;
            res++;
        }
    }
    return res;
}

void AhoCorasickTrie::BufferWord(const string &s)
{
    buffered_words.insert(s);
}

void AhoCorasickTrie::FlushBuffer()
{
    // Trie set up
    words_in_trie.insert(buffered_words.begin(), buffered_words.end());
    buffered_words.clear();
    for (const auto &s : words_in_trie)
    {
        int cn = 0;
        for (char c : s)
        {
            int i = (c == '1') ? 1 : 0;
            if (ns[cn].next[i] == -1)
            {
                Node nn;
                nn.terminal = false;
                nn.next[0] = -1;
                nn.next[1] = -1;
                nn.fail = -1;
                nn.dict_fail = -1;
                nn.s = ns[cn].s + c;
                ns.push_back(nn);
                ns[cn].next[i] = ns.size() - 1;
            }
            cn = ns[cn].next[i];
        }
        ns[cn].terminal = true;
    }

    // Fail BFS
    deque<int> q;
    q.push_back(ns[0].next[0]);
    if (ns[0].next[0] >= 0)
        ns[ns[0].next[0]].fail = 0;
    q.push_back(ns[0].next[1]);
    if (ns[0].next[1] >= 0)
        ns[ns[0].next[1]].fail = 0;

    while (!q.empty())
    {
        int cn = q.front();
        q.pop_front();
        if (cn == -1)
            continue;
        for (int i = 0; i < 2; ++i)
        {
            int fsn = ns[cn].next[i]; // fail seeking node
            if (fsn == -1)
                continue;
            q.push_back(fsn);
            int fail_node = ns[cn].fail;
            while (fail_node > 0 && ns[fail_node].next[i] == -1)
                fail_node = ns[fail_node].fail;
            ns[fsn].fail = (ns[fail_node].next[i] >= 0)
                               ? ns[fail_node].next[i]
                               : 0;
        }
        // Print();
    }

    // Dict Fail BFS
    q.clear();
    q.push_back(ns[0].next[0]);
    if (ns[0].next[0] >= 0)
        ns[ns[0].next[0]].dict_fail = 0;
    q.push_back(ns[0].next[1]);
    if (ns[0].next[1] >= 0)
        ns[ns[0].next[1]].dict_fail = 0;

    while (!q.empty())
    {
        int cn = q.front();
        q.pop_front();
        if (cn == -1)
            continue;
        for (int i = 0; i < 2; ++i)
        {
            int fsn = ns[cn].next[i]; // fail seeking node
            if (fsn == -1)
                continue;
            q.push_back(fsn);
            if (ns[ns[fsn].fail].terminal)
                ns[fsn].dict_fail = ns[fsn].fail;
            else
                ns[fsn].dict_fail = ns[ns[fsn].fail].dict_fail;
        }
        // Print();
    }
}

void AhoCorasickTrie::Print()
{
    cout << "STR\t\t";
    for (auto n : ns)
        cout << n.s << '\t';
    cout << '\n';
    cout << "NEXT(0)\t\t";
    for (auto n : ns)
        cout << n.next[0] << '\t';
    cout << '\n';
    cout << "NEXT(1)\t\t";
    for (auto n : ns)
        cout << n.next[1] << '\t';
    cout << '\n';
    cout << "TERMINAL\t";
    for (auto n : ns)
        cout << (n.terminal ? 'T' : 'F') << '\t';
    cout << '\n';
    cout << "FAIL\t\t";
    for (auto n : ns)
        cout << n.fail << '\t';
    cout << '\n';
    cout << "DICT FAIL\t";
    for (auto n : ns)
        cout << n.dict_fail << '\t';
    cout << '\n'
         << '\n';
}

string shift(const string &s, int l)
{
    int n = s.size();
    l = l % n;
    // return s.substr(n-l,l) + s.substr(0,n-l);
    return s.substr(l, n - l) + s.substr(0, l);
}

int main()
{
    ios::sync_with_stdio(false);
    int t, q;
    char type;
    string s;
    cin >> t;
    AhoCorasickTrie big_aht, small_aht;
    for (int casen = 1; casen <= t; casen++)
    {
        int l = 0;
        cout << "Case #" << casen << ":\n";
        big_aht = AhoCorasickTrie();
        small_aht = AhoCorasickTrie();
        cin >> q;
        int th = max((int)pow(q, 0.5), 100);
        while (q--)
        {
            cin >> type >> s;
            // cout << s << "-->" << shift(s,l) << endl;
            s = shift(s, l);
            switch (type)
            {
            case '?':
                l = big_aht.MatchString(s) + small_aht.MatchString(s);
                cout << l << endl;
                break;
            case '+':
                if (big_aht.words_in_trie.find(s) == big_aht.words_in_trie.end())
                {
                    big_aht.BufferWord(s);
                    small_aht.BufferWord(s);
                    small_aht.FlushBuffer();
                }
                break;
            }
            if (q % th == th - 1)
            {
                small_aht = AhoCorasickTrie();
                big_aht.FlushBuffer();
            }
            // small_aht.Print();
            // big_aht.Print();
        }
    }

    return 0;
}