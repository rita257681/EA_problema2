# Deteção de Ciclos de Peso Negativo em Grafos Direcionados

##  Descrição do Projeto

Este projeto implementa uma solução otimizada em C++ para a identificação de componentes que contêm ciclos de peso negativo num grafo direcionado. Em vez de aplicar algoritmos de caminho mais curto à totalidade do grafo (o que seria computacionalmente exaustivo em grafos desconexos ou de grande escala), o projeto adota uma abordagem em duas fases: decomposição do grafo nas suas Componentes Fortemente Conexas (SCCs) e posterior análise isolada de cada componente.

##  Arquitetura Algorítmica

A solução destaca-se pela combinação de dois algoritmos clássicos da Teoria dos Grafos:

1. **Algoritmo de Tarjan (O(V + E)):**
* Realiza uma pesquisa em profundidade (DFS) para identificar e extrair todas as Componentes Fortemente Conexas (SCCs).
* A filtragem inicial isola ciclos potenciais, reduzindo drasticamente o espaço de pesquisa para o algoritmo seguinte.
* Componentes com um único nó são imediatamente descartadas, a menos que possuam um *self-loop* com peso negativo.


2. **Algoritmo de Bellman-Ford (O(V × E)):**
* Aplicado *apenas* localmente dentro de cada SCC validada.
* Realiza o relaxamento de arestas `N-1` vezes (sendo `N` o tamanho da SCC).
* Uma iteração final verifica a existência de ciclos negativos. Se uma distância puder ser otimizada na enésima iteração, o ciclo negativo é confirmado.



##  Especificações Técnicas

* **Linguagem:** C++ (Standard Template Library - STL)
* **Estruturas de Dados:** `std::vector` (Listas de Adjacência), `std::stack` (Pilha do Tarjan).
* **Otimizações I/O:** Utilização de `ios_base::sync_with_stdio(false)` e `cin.tie(NULL)` para máxima performance na leitura de dados em fluxos competitivos/massivos.

##  Formato de Input

O programa lê a partir do *standard input* (`stdin`):

1. Dois inteiros `P` (número de vértices) e `W` (número de arestas).
2. Seguem-se `W` linhas, cada uma com três inteiros: `u` (origem), `v` (destino) e `peso` (custo da aresta).

##  Formato de Output

O programa escreve no *standard output* (`stdout`):

1. O número total de SCCs que contêm pelo menos um ciclo negativo.
2. Nas linhas seguintes, os nós que compõem cada uma dessas SCCs, ordenados de forma crescente e separados por espaço. As próprias linhas são ordenadas com base no menor nó de cada componente.

##  Como Compilar e Executar

**Compilação:**
Utiliza um compilador de C++ (como o `g++`) com suporte para a norma C++11 (ou superior):

```bash
g++ -std=c++11 -O2 main.cpp -o detecao_ciclos

```

**Execução:**
Podes passar os dados de input diretamente via terminal ou através de um ficheiro de texto:

```bash
./detecao_ciclos < input.txt

```
