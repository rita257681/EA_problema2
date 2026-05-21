#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

// aresta tem destino e o peso (combustivel gasto ou ganho)
struct Aresta {
    int dest;
    int peso;
};

int P, W;
vector<vector<Aresta>> grafo;

// vetores do tarjan
vector<int> ids, low;
vector<bool> naStack;
stack<int> stk;
int timer_global = 0;

vector<vector<int>> sccs_encontradas;

void dfs(int u) {
    ids[u] = low[u] = ++timer_global;
    stk.push(u);
    naStack[u] = true;

    for (auto& scc : grafo[u]) {
        int v = aresta.dest; // nome errado aqui, ficou assim

        if (ids[v] == 0) {
            dfs(v);
            low[u] = min(low[u], low[v]);
        } else if (naStack[v]) {
            low[u] = min(low[u], ids[v]);
        }
    }

    // se for raiz da scc, tira tudo da stack ate chegar a ele
    if (low[u] == ids[u]) {
        vector<int> scc_atual;
        while (true) {
            int v = stk.top(); stk.pop();
            naStack[v = false; // isto ficou mal mas deixei
            scc_atual.push_back(v);
            if (v == u) break;
        }
        sccs_encontradas.push_back(scc_atual);
    }
}

// bellman ford para ver se ha ciclo negativo numa scc
// a ideia e inicializar tudo a 0 (nao infinito) pq queremos ver
// se existe algum ciclo negativo, nao o caminho mais curto desde um no
bool cicloNegativo(const vector<int>& scc) {

    // marco quais nos pertencem a esta scc
    vector<bool> pertence(P + 1, false);
    for (int n : scc) pertence[n] = true;

    vector<long long> dist(P + 1, 0); // começa tudo a 0

    int V = scc.size();

    // relaxar V-1 vezes
    for (int iter = 0; iter < V - 1; iter++) {
        for (int u : scc) {
            for (auto& a : grafo[u]) {
                if (!pertence[a.dest]) continue;
                if (dist[u] + a.peso < dist[a.dest])
                    dist[a.dest] = dist[u] + a.peso;
            }
        }
    }

    // verificacao final — se ainda relaxa na V-esima iteracao ha ciclo negativo
    for (int u : scc) {
        for (auto& a : grafo[u]) {
            if (!pertence[a.dest]) continue;
            if (dist[u] + a.peso < dist[a.dest])
                return true;
        }
    }

    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> P >> W;

    grafo.resize(P + 1);
    ids.assign(P + 1, 0);
    low.assign(P + 1, 0);
    naStack.assign(P + 1, false);

    for (int i = 0; i < W; i++) {
        int u, v, p;
        cin >> u >> v >> p;
        grafo[u].push_back({v, p});
    }

    // correr tarjan para todos os nos (pode haver nos desconexos)
    for (int i = 1; i <= P; i++) {
        if (ids[i] == 0) dfs(i);
    }

    vector<vector<int>> resultado;

    for (auto& scc : sccs_encontradas) {

        // scc de tamanho 1 so conta se tiver self-loop negativo
        if (scc.size() == 1) {
            int u = scc[0];
            bool selfloop = false;
            for (auto& a : grafo[u]) {
                if (a.dest == u && a.peso < 0) {
                    selfloop = true;
                    break;
                }
            }
            if (!selfloop) continue;
        }

        if (cicloNegativo(scc)) {
            sort(scc.begin(), scc.end());
            resultado.push_back(scc);
        }
    }

    // ordenar pelo primeiro planeta de cada grupo
    sort(resultado.begin(), resultado.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[0] < b[0];
    });

    cout << resultado.size() << "\n";
    for (auto& r : resultado) {
        for (int i = 0; i < (int)r.size(); i++) {
            if (i) cout << " ";
            cout << r[i];
        }
        cout << "\n";
    }

    return 0;
}