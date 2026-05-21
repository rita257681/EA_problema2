#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

//estrutura para representar aresta
struct Aresta{
    int v;      // destino
    int peso;   // delta do combustivel
};

int P, W;
vector<vector<Aresta>> grafo;
vector<int> id, low;
vector<bool> no_stack;

// pilha vazia
stack<int> pilha_tarjan;

int tempo = 0;

// lista que vai guardar todas as CSS's encontradas pelo Tarjan
vector<vector<int>> todas_sccs;

// algoritmo de Tarjan para identificar componentes fortemente conexas
void dfs_torjan(int u){
    id[u] = low[u] = ++tempo;
    pilha_tarjan.push(u);
    no_stack[u] = true;

    //vai para os vizinhos
    for (auto& scc : grafo[u]) {
        int v = aresta.v;

        if(id[v] == 0){ //se nao foi visitado
            dfs_torjan(v);
            low[u] = min(low[u], low[v]);
        }
        else if(no_stack[v]){   // se está na pilha
            low[u] = min(low[u], id[v]);
        }
    }

    // se 'u' for a raiz de uma SCC, retira os elementos da pilha
    if(low[u] == id[u]){
        vector<int> scc;

        while(true){
            int v = pilha_tarjan.top();
            pilha_tarjan.pop();
            no_stack[v = false;
            scc.push_back(v);
            if(u == v){
                break;
            }
        }
        todas_sccs.push_back(scc);
    }
}

// algoritmo BELLMAN-FORD (verifica ciclos negativos dentro de uma SCC)
bool tem_ciclo_negativo_scc(const vector<int>& scc) {
    static vector<bool> na_scc(P + 1, false);
    for (int nodo : scc) na_scc[nodo] = true;

    long long INF = 1e15; 
    vector<long long> dist(P + 1, INF);

    // Inicializa os nodos da SCC
    for (int nodo : scc) dist[nodo] = 0;

    int n_nodos = scc.size();
    
    // Relaxar as arestas (V - 1) vezes
    for (int i = 0; i < n_nodos - 1; ++i) {
        for (int u : scc) {
            if (dist[u] == INF) continue;
            for (auto& aresta : grafo[u]) {
                int v = aresta.v;
                if (!na_scc[v]) continue; // ignora nós fora desta SCC
                
                if (dist[u] + aresta.peso < dist[v]) {
                    dist[v] = dist[u] + aresta.peso;
                }
            }
        }
    }

    // Verificação de ciclo negativo
    for (int u : scc) {
        if (dist[u] == INF) continue;
        for (auto& aresta : grafo[u]) {
            int v = aresta.v;
            if (!na_scc[v]) continue;
            
            if (dist[u] + aresta.peso < dist[v]) {
                for (int nodo : scc) na_scc[nodo] = false; // Limpa o vetor estático
                return true; // Há ciclo de combustível infinito!
            }
        }
    }

    for (int nodo : scc){
        na_scc[nodo] = false;
    }
    return false;
}

int main() {
    // Otimização de I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> P >> W)) return 0;

    // Redimensionar os vetores com base no número de planetas P
    grafo.resize(P + 1);
    id.assign(P + 1, 0);
    low.assign(P + 1, 0);
    no_stack.assign(P + 1, false);

    // Leitura do Grafo
    for (int i = 0; i < W; ++i) {
        int u, v, peso;
        cin >> u >> v >> peso;
        grafo[u].push_back({v, peso});
    }

    // Rodar Tarjan para todos os planetas não visitados
    for (int i = 1; i <= P; ++i) {
        if (id[i] == 0) {
            dfs_torjan(i);
        }
    }

    vector<vector<int>> loops_infinitos;

    // Processar cada SCC encontrada
    for (auto& scc : todas_sccs) {
        // Se só tem 1 planeta, só valida se tiver auto-loop de ganho de combustível (peso < 0)
        if (scc.size() == 1) {
            int u = scc[0];
            bool auto_loop_valido = false;
            for (auto& aresta : grafo[u]) {
                if (aresta.v == u && aresta.peso < 0) {
                    auto_loop_valido = true;
                    break;
                }
            }
            if (!auto_loop_valido) continue;
        }

        // Rodar Bellman-Ford na SCC atual
        if (tem_ciclo_negativo_scc(scc)) {
            sort(scc.begin(), scc.end()); // Ordenar planetas da SCC por ordem crescente
            loops_infinitos.push_back(scc);
        }
    }

    // Ordenar os grupos pelo ID do primeiro planeta de cada um
    sort(loops_infinitos.begin(), loops_infinitos.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[0] < b[0];
    });

    // OUTPUT FINAL
    cout << loops_infinitos.size() << "\n";
    for (auto& scc_valida : loops_infinitos) {
        for (size_t i = 0; i < scc_valida.size(); ++i) {
            cout << scc_valida[i] << (i == scc_valida.size() - 1 ? "" : " ");
        }
        cout << "\n";
    }

    return 0;
}