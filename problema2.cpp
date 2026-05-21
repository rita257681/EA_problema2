#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

// struct p/ as arestas do grafo
struct Aresta {
    int v, peso;
};

int P, W;
vector<vector<Aresta>> grafo;
vector<int> id, low;
vector<bool> no_stack;

stack<int> pilha_tarjan;
int tempo = 0;
vector<vector<int>> todas_sccs;

// algoritmo Tarjan p/ achar SCCs
void dfs_torjan(int u) {
    id[u] = low[u] = ++tempo;
    pilha_tarjan.push(u);
    no_stack[u] = true;

    for (vector<Aresta>::iterator it = grafo[u].begin(); it != grafo[u].end(); ++it) {
        int v = it->v;
        if (id[v] == 0) {
            dfs_torjan(v);
            low[u] = min(low[u], low[v]);
        } else if (no_stack[v]) {
            low[u] = min(low[u], id[v]);
        }
    }

    // se for raiz da scc, desempilha ate chegar a ele
    if (low[u] == id[u]) {
        vector<int> scc;
        while (true) {
            int v = pilha_tarjan.top();
            pilha_tarjan.pop();
            no_stack[v] = false;
            scc.push_back(v);
            if (u == v){
                break;
            }
        }
        todas_sccs.push_back(scc);
    }
}

// aqui tento ver se ha ciclo negativo c/ bellman-ford
bool tem_ciclo_negativo_scc(const vector<int>& scc) {
    static vector<bool> na_scc(P + 1, false);
    for (int nodo : scc) {
        na_scc[nodo] = true;
    }
    long long INF = 1e15; 
    vector<long long> dist(P + 1, INF);

    for (int nodo : scc){
        dist[nodo] = 0;
    }

    int n = scc.size();
    // relaxar V-1 vezes... padrao
    for (int i = 0; i < n - 1; ++i) {
        for (int u : scc) {
            if (dist[u] == INF){
                continue;
            }
            for (vector<Aresta>::iterator it = grafo[u].begin(); it != grafo[u].end(); ++it) {
                if (!na_scc[it->v]){
                    continue;
                }
                if (dist[u] + it->peso < dist[it->v]) {
                    dist[it->v] = dist[u] + it->peso;
                }
            }
        }
    }

    // check final de ciclo negativo
    for (int u : scc) {
        if (dist[u] == INF){
            continue;
        }
        for (vector<Aresta>::iterator it = grafo[u].begin(); it != grafo[u].end(); ++it) {
            if (!na_scc[it->v]){
                continue;
            }
            if (dist[u] + it->peso < dist[it->v]) {
                for (int n : scc){
                    na_scc[n] = false; // limpa p/ prox
                }
                return true;
            }
        }
    }

    for (int n : scc){
        na_scc[n] = false;
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> P >> W)){
        return 0;
    }

    grafo.resize(P + 1);
    id.assign(P + 1, 0);
    low.assign(P + 1, 0);
    no_stack.assign(P + 1, false);

    for (int i = 0; i < W; ++i) {
        int u, v, peso;
        cin >> u >> v >> peso;
        grafo[u].push_back({v, peso});
    }

    for (int i = 1; i <= P; ++i) {
        if (id[i] == 0){
            dfs_torjan(i);
        }
    }

    vector<vector<int>> res;
    // processar cada scc q o tarjan achou
    for (vector<vector<int>>::iterator it = todas_sccs.begin(); it != todas_sccs.end(); ++it) {
        vector<int>& scc = *it;
        
        // caso especial de scc com 1 elemento
        if (scc.size() == 1) {
            int u = scc[0];
            bool ok = false;
            for (vector<Aresta>::iterator a = grafo[u].begin(); a != grafo[u].end(); ++a) {
                if (a->v == u && a->peso < 0) { ok = true; break; }
            }
            if (!ok){
                continue;
            }
        }

        if (tem_ciclo_negativo_scc(scc)) {
            sort(scc.begin(), scc.end());
            res.push_back(scc);
        }
    }

    sort(res.begin(), res.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[0] < b[0];
    });

    cout << res.size() << "\n";
    for (vector<vector<int>>::iterator it = res.begin(); it != res.end(); ++it) {
        for (size_t i = 0; i < it->size(); ++i) {
            cout << (*it)[i] << (i == it->size() - 1 ? "" : " ");
        }
        cout << "\n";
    }

    return 0;
}