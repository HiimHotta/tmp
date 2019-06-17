/*
 * MAC0323 Algoritmos e Estruturas de Dados II
 * 
 * ADT Topological é uma "representação topológica" de digrafo.
 * Esta implementação usa ADT Digraph do EP13.
 *  
 * Busque inspiração em: 
 *
 *   https://algs4.cs.princeton.edu/42digraph/
 *   https://algs4.cs.princeton.edu/42digraph/DepthFirstOrder.java
 *   https://algs4.cs.princeton.edu/42digraph/Topological.java
 *   https://algs4.cs.princeton.edu/42digraph/DirectedCycle.java
 * 
 * TOPOLOGICAL
 *
 * Topological é uma ¨representação topológica" de um dado digrafo.
 * 
 * As principais operações são: 
 *
 *      - hasCycle(): indica se o digrafo tem um ciclo (DirectedCycle.java)
 *      - isDag(): indica se o digrafo é acyclico (Topological.java)
 *
 *      - pre(): retorna a numeração pré-ordem de um vértice em relação a uma dfs 
 *               (DepthFirstOrder.java)
 *      - pos(): retorna a numareção pós-ordem de um vértice em relação a uma dfs
 *               (DepthFirstOrder.java)
 *      - rank(): retorna a numeração topológica de um vértice (Topological.java)
 * 
 *      - preorder(): itera sobre todos os vértices do digrafo em pré-ordem
 *                    (em relação a uma dfs, DepthFirstOrder.java)
 *      - postorder(): itera sobre todos os vértices do digrafo em pós-ordem
 *                    (em relação a uma dfs, ordenação topologica reversa, 
 *                     DepthFirstOrder.java)
 *      - order(): itera sobre todos os vértices do digrafo em ordem  
 *                 topologica (Topological.java)
 *      - cycle(): itera sobre os vértices de um ciclo (DirectedCycle.java)
 *
 * O construtor e "destrutor" da classe consomem tempo linear..
 *
 * Cada chamadas das demais operações consome tempo constante.
 *
 * O espaço gasto por esta ADT é proporcional ao número de vértices V do digrafo.
 * 
 * Para documentação adicional, ver 
 * https://algs4.cs.princeton.edu/42digraph, Seção 4.2 de
 * Algorithms, 4th Edition por Robert Sedgewick e Kevin Wayne.
 *
 */

/* interface para o uso da funcao deste módulo */
#include "topological.h"

#include "digraph.h" /* Digraph, vDigraph(), eDigraph(), adj(), ... */
#include "bag.h"     /* add() e itens() */
#include "util.h"    /* emalloc(), ecalloc(), ERRO(), AVISO() */

#include <stdlib.h>  /* free() */

#undef DEBUG
#ifdef DEBUG
#include <stdio.h>   /* printf(): para debugging */
#endif

/*----------------------------------------------------------*/
/* 
 * Estrutura básica de um Topological
 * 
 */
struct topological {
    Digraph DG;
    int* rank;
    int *postOrder; //vector to show pre and post order
    int iPost     ;
    Bag Cycle, Order;
};

/*------------------------------------------------------------*/
/* 
 * Protótipos de funções administrativas: tem modificador 'static'
 * 
 */

/*------------------------------------------------------------*/
/* 
 * Funções auxiliares.
 * 
 */
void validateTopological (Topological T) {
    if (T == NULL) {
        ERROR ("SOCORRO! NAO CONSTRUIU Topological");
        exit (0);
    }
}

// DIRECTED CYCLE

// certify that digraph has a directed cycle if it reports one
Bool check(Topological T) {
    if (T->Cycle != NULL) {
        // verify cycle
        int first = -1, last = -1;
        for (int v = itens (T->Cycle, TRUE); v != -1; v = itens (T->Cycle, FALSE)) {
            if (first == -1) 
                first = v;

            last = v;
        }
        if (first != last) {
            System.err.printf ("cycle begins with %d and ends with %d\n", first, last);
            return FALSE;
        }
    }
    return TRUE;
}

void dfs (Topological T, Bool* marked, Bool* onBag, int* edgeTo, int v) {
    onStack[v] = TRUE;
    marked[v]  = TRUE;
    for (int w = adj (T->DG, v, TRUE); w != -1; w = adj (T->DG, v, FALSE)) {

        // short circuit if directed cycle found
        if (T->Cycle != NULL) 
            return;

        // found new vertex, so recur
        else if (!marked[w]) {
            edgeTo[w] = v;
            dfs (G, marked, onBag, edgeTo, w);
        }

        // trace back directed cycle
        else if (onStack[w]) {
            T->Cycle = newBag ();
            for (int x = v; x != w; x = edgeTo[x]) {
                add (T->Cycle, x);
            }
            add (T->Cycle, w);
            add (T->Cycle, v);
            if (check (T));
        }
    }
    onStack[v] = FALSE;
}

Bool checkCycle (Topological T) {
    Bool* marked = emalloc (vDigraph (T->DG) * sizeof (Bool));
    Bool* onBag  = emalloc (vDigraph (T->DG) * sizeof (Bool));
    int*  edgeTo = emalloc (vDigraph (T->DG) * sizeof (int));

    for (int v = 0; v < vDigraph (T->DG); v++) {
        if (!marked[v] && cycle == NULL) 
            dfs (T, marked, onBag, edgeTo, v);
    }

    free (marked);
    free (onBag);
    free (edgeTo);

    if (T->Cycle == NULL)
        return FALSE;

    return TRUE;
}

// DEPTH FIRST ORDER


Bool checkOrder (Topological T) {
    int*  post      = emalloc (vDigraph (T->DG) * sizeof (int));
    //postorder = new Queue<Integer>();
    Bool* marked    = emalloc (vDigraph (T->DG) * sizeof (Bool));

    for (int v = 0; v < vDigraph (T->DG); v++) {
        if (!marked[v])
            dfso (G, v);
    }

}

/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/
/*
 *  newTopologica(G)
 *
 *  RECEBE um digrafo G.
 *  RETORNA uma representação topológica de G.
 * 
 */
Topological newTopological (Digraph G) {
    validateDigraph (G);

    Topological aux = emalloc (sizeof (struct topological));

    aux->DG         = G;
    if (checkCycle (aux)) {
        aux->rank      = NULL;
        aux->postOrder = NULL;
        return aux;
    }
    if (!checkOrder (aux)) {
        aux->rank      = NULL;
        aux->postOrder = NULL;
        return aux;
    }
    aux->rank          = emalloc (vDigraph (G) * sizeof (int));
    aux->postOrder     = emalloc (vDigraph (G) * sizeof (int));
    return aux;
}

/*-----------------------------------------------------------*/
/*
 *  freeTopological(TS)
 *
 *  RECEBE uma representação topologica TS.
 *  DEVOLVE ao sistema toda a memória usada por TS.
 *
 */
void freeTopological (Topological ts) {
    validateTopological (ts);
    free (rank);
    free (postOrder);
    free (ts);
}    

/*------------------------------------------------------------*/
/*
 *  OPERAÇÕES: 
 *
 */

/*-----------------------------------------------------------*/
/* 
 *  HASCYCLE(TS)
 *
 *  RECEBE uma representação topológica TS de um digrafo;
 *  RETORNA TRUE se o digrafo possui um ciclo e FALSE em caso 
 *  contrário.
 *
 */
Bool hasCycle (Topological ts) {
    validateTopological (ts);
    return T->Cycle != NULL;
}

/*-----------------------------------------------------------*/
/* 
 *  ISDAG(TS)
 *
 *  RECEBE um representação topológica TS de um digrafo.
 *  RETORNA TRUE se o digrafo for um DAG e FALSE em caso 
 *  contrário.
 *
 */
Bool isDag (Topological ts) {
    validateTopological (ts);
    return T->Order != NULL;
}

/*-----------------------------------------------------------*/
/* 
 *  PRE(TS, V)
 *
 *  RECEBE uma representação topológica TS de um digrafo e um 
 *  vértice V.
 *  RETORNA a numeração pré-ordem de V em TS.
 *
 */
int pre (Topological ts, vertex v) {
    validateTopological (ts);
    validateVertex (v);
    return -1;
}

/*-----------------------------------------------------------*/
/* 
 *  POST(TS, V)
 *
 *  RECEBE uma representação topológica TS de um digrafo e um 
 *  vértice V.
 *  RETORNA a numeração pós-ordem de V em TS.
 *
 */
int post (Topological ts, vertex v) {
    validateTopological (ts);
    validateVertex (v);
    return -1;
}

/*-----------------------------------------------------------*/
/* 
 *  RANK(TS, V)
 *
 *  RECEBE uma representação topológica TS de um digrafo e um 
 *  vértice V.
 *  RETORNA a posição de V na ordenação topológica em TS;
 *  retorna -1 se o digrafo não for um DAG.
 *
 */
int rank (Topological ts, vertex v) {
    validateTopological (ts);
    validateVertex (v);
    return -1;
}

/*-----------------------------------------------------------*/
/* 
 *  PREORDER(TS, INIT)
 * 
 *  RECEBE uma representação topológica TS de um digrafo e um 
 *  Bool INIT.
 *
 *  Se INIT é TRUE,  PREORDER() RETORNA o primeiro vértice na ordenação pré-ordem de TS.
 *  Se INIT é FALSE, PREORDER() RETORNA o vértice sucessor do último vértice retornado
 *                   na ordenação pré-ordem de TS; se todos os vértices já foram retornados, 
 *                   a função retorna -1.
 */
vertex preorder (Topological ts, Bool init) {
    validateTopological (ts);
    return -1;
}

/*-----------------------------------------------------------*/
/* 
 *  POSTORDER(TS, INIT)
 * 
 *  RECEBE uma representação topológica TS de um digrafo e um 
 *  Bool INIT.
 *
 *  Se INIT é TRUE,  POSTORDER() RETORNA o primeiro vértice na ordenação pós-ordem de TS.
 *  Se INIT é FALSE, POSTORDER() RETORNA o vértice sucessor do último vértice retornado
 *                   na ordenação pós-ordem de TS; se todos os vértices já foram retornados, 
 *                   a função retorna -1.
 */
vertex postorder (Topological ts, Bool init) {
    validateTopological (ts);
    return -1;
}

/*-----------------------------------------------------------*/
/* 
 *  ORDER(TS, INIT)
 * 
 *  RECEBE uma representação topológica TS de um digrafo e um Bool INIT.
 *
 *  Se INIT é TRUE,  ORDER() RETORNA o primeiro vértice na ordenação topológica 
 *                   de TS.
 *  Se INIT é FALSE, ORDER() RETORNA o vértice sucessor do último vértice retornado
 *                   na ordenação topológica de TS; se todos os vértices já foram 
 *                   retornados, a função retorna -1.
 *
 *  Se o digrafo _não_ é um DAG, ORDER() RETORNA -1.
 */
vertex order (Topological ts, Bool init) {
    validateTopological (ts);
    if (!isDag (ts))
        return -1;

    if (init == TRUE) 
        return itens (ts->Order, TRUE);

    return itens (ts->Order, FALSE);
}

/*-----------------------------------------------------------*/
/* 
 *  CYCLE(TS, INIT)
 * 
 *  RECEBE uma representação topológica TS de um digrafo e um Bool INIT.
 *
 *  Se INIT é TRUE,  CYCLE() RETORNA um vértice em um ciclo do digrafo.
 *  Se INIT é FALSE, CYCLE() RETORNA o vértice  no ciclo que é sucessor do 
 *                   último vértice retornado; se todos os vértices no ciclo já 
 *                   foram retornados, a função retorna -1.
 *
 *  Se o digrafo é um DAG, CYCLE() RETORNA -1.
 *
 */
vertex cycle (Topological ts, Bool init) {
    validateTopological (ts);

    if (isDag (ts))
        return -1;

    if (init == TRUE) 
        return itens (ts->Cycle, TRUE);
    
    return itens (ts->Cycle, FALSE);
}


/*------------------------------------------------------------*/
/* 
 * Implementaçao de funções administrativas: têm o modificador 
 * static.
 */
