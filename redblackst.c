/*
 * MAC0323 Estruturas de Dados e Algoritmo II
 * 
 * Tabela de simbolos implementada atraves de uma BST rubro-negra
 *
 *     https://algs4.cs.princeton.edu/33balanced/RedBlackBST.java.html
 * 
 * As chaves e valores desta implementaÃ§Ã£o sÃ£o mais ou menos
 * genÃ©ricos
 */

/* interface para o uso da funcao deste mÃ³dulo */
#include "redblackst.h"  
#include <stdlib.h>      /* free() */
#include <string.h>      /* memcpy() */
#include "util.h"        /* emalloc(), ecalloc() */

#undef DEBUG
#ifdef DEBUG
#include <stdio.h>   /* printf(): para debug */
#endif


// CONSTANTES 
 
#define RED   TRUE
#define BLACK FALSE 

/*----------------------------------------------------------*/
/* 
 * Estrutura de um noh da Arvore
 *
 */
typedef struct node Node;

struct node {
        void *key;           // key
        void *val;           // associated data
        Node *left, *right;  // links to left and right subtrees
        Bool color;          // color of parent link
        int size;            // subtree count
};

//"construtor"
Node *newNode (const void *key, size_t sizeKey, const void *val, size_t sizeVal, Bool color) {
    Node *tmp = emalloc (sizeof (Node));
    tmp->key = emalloc (sizeKey);
    memcpy (tmp->key, key, sizeKey);

    tmp->val = emalloc (sizeVal);
    memcpy (tmp->val, val, sizeVal);

    tmp->left = NULL;
    tmp->right = NULL;
    tmp->color = color;
    tmp->size = 1;
    return tmp;
}

void editVal (Node *node, const void *val, size_t sizeVal) {
    void *tmp = emalloc (sizeVal);
    memcpy (tmp, val, sizeVal);
    node->val = tmp;
}

//FIM NOH



/*----------------------------------------------------------*/
/* 
 * Estrutura BÃ¡sica da Tabela de SÃ­mbolos: 
 * 
 * implementaÃ§Ã£o com Ã¡rvore rubro-negra
 */
struct redBlackST {
    Node *root;
    int (*compareTo) (const void *key1, const void *key2);
};

/*------------------------------------------------------------*/
/* 
 *  ProtÃ³tipos de funÃ§Ãµes administrativas.
 * 
 *  Entre essa funÃ§Ãµes estÃ£o isRed(), rotateLeft(), rotateRight(),
 *  flipColors(), moveRedLeft(), moveRedRight() e balance().
 * 
 *  NÃ£o deixe de implmentar as funÃ§Ãµes chamadas pela funÃ§Ã£o 
 *  check(): isBST(), isSizeConsistent(), isRankConsistent(),
 *  is23(), isBalanced().
 *
 */

/*---------------------------------------------------------------*/
static Bool isBST(RedBlackST st);

/*---------------------------------------------------------------*/
static Bool isSizeConsistent(RedBlackST st);

/*---------------------------------------------------------------*/
static Bool isRankConsistent(RedBlackST st);

/*---------------------------------------------------------------*/
static Bool is23(RedBlackST st);

/*---------------------------------------------------------------*/
static Bool isBalanced(RedBlackST st);

/*-----------------------------------------------------------*/
/*
 *  initST(COMPAR)
 *
 *  RECEBE uma funÃ§Ã£o COMPAR() para comparar chaves.
 *  RETORNA (referÃªncia/ponteiro para) uma tabela de sÃ­mbolos vazia.
 *
 *  Ã‰ esperado que COMPAR() tenha o seguinte comportamento:
 *
 *      COMPAR(key1, key2) retorna um inteiro < 0 se key1 <  key2
 *      COMPAR(key1, key2) retorna 0              se key1 == key2
 *      COMPAR(key1, key2) retorna um inteiro > 0 se key1 >  key2
 * 
 *  TODAS OS OPERAÃ‡Ã•ES da ST criada utilizam a COMPAR() para comparar
 *  chaves.
 * 
 */
RedBlackST initST (int (*compar) (const void *key1, const void *key2)) {
    RedBlackST st = emalloc (sizeof (struct redBlackST));
    st->root = NULL;
    st->compareTo = compar;
    return st;
}

/***************************************************************************
*  Red-black tree helper functions.
***************************************************************************/

/*-----------------------------------------------------------*/
/* 
 *  SIZE(ST)
 *
 *  RECEBE uma tabela de sÃ­mbolos ST.
 * 
 *  RETORNA o nÃºmero de itens (= pares chave-valor) na ST.
 *
 */
int size (RedBlackST st) {
    if (st->root == NULL)
        return 0;
    return st->root->size;
}

int sizeNode (Node *node) {
    if (node == NULL)
        return 0;
    return node->size;
}

// is node x red; false if x is NULL
static Bool isRed (Node *node) {
    if (node == NULL)
        return FALSE;
    return node->color == RED;
}

// make a left-leaning link lean to the right
static Node *rotateRight (Node *h) {
    // assert (h != NULL) && isRed(h->left);
    Node *x = h->left;
    h->left = x->right;
    x->right = h;
    x->color = x->right->color;
    x->right->color = RED;
    x->size = h->size;
    h->size = sizeNode (h->left) + sizeNode (h->right) + 1;
    return x;
}

// make a right-leaning link lean to the left
static Node *rotateLeft (Node *h) {
    // assert (h != NULL) && isRed(h->right);
    Node *x = h->right;
    h->right = x->left;
    x->left = h;
    x->color = x->left->color;
    x->left->color = RED;
    x->size = h->size;
    h->size = sizeNode (h->left) + sizeNode (h->right) + 1;
    return x;
}

// flip the colors of a node and its two children
static void flipColors (Node *h) {
    // h must have opposite color of its two children
    // assert (h != NULL) && (h->left != NULL) && (h->right != NULL);
    // assert (!isRed(h) &&  isRed(h->left) &&  isRed(h->right))
    //    || (isRed(h)  && !isRed(h->left) && !isRed(h->right));
    h->color = !h->color;
    h->left->color = !h->left->color;
    h->right->color = !h->right->color;
}

// Assuming that h is red and both h->left and h->left->left
// are black, make h->left or one of its children red->
static Node *moveRedLeft (Node *h) {
    // assert (h != NULL);
    // assert isRed(h) && !isRed(h->left) && !isRed(h->left->left);
    flipColors (h);
    if (isRed (h->right->left)) { 
        h->right = rotateRight (h->right);
        h = rotateLeft (h);
        flipColors (h);
    }
    return h;
}

    // Assuming that h is red and both h->right and h->right->left
    // are black, make h->right or one of its children red->
static Node *moveRedRight(Node *h) {
    // assert (h != NULL);
    // assert isRed(h) && !isRed(h->right) && !isRed(h->right->left);
    flipColors (h);
    if (isRed (h->left->left)) { 
        h = rotateRight (h);
        flipColors (h);
    }
    return h;
}

// restore red-black tree invariant
static Node *balance(Node *h) {
    // assert (h != NULL);
    if (isRed (h->right))                         h = rotateLeft (h);
    if (isRed (h->left) && isRed (h->left->left)) h = rotateRight (h);
    if (isRed (h->left) && isRed (h->right))      flipColors (h);

    h->size = sizeNode (h->left) + sizeNode (h->right) + 1;
    return h;
}

void *getKey (Node *node) {
    return node->key;
}

/*-----------------------------------------------------------*/
/* 
 *  ISEMPTY(ST, KEY)
 *
 *  RECEBE uma tabela de sÃ­mbolos ST.
 * 
 *  RETORNA TRUE se ST estÃ¡ vazia e FALSE em caso contrÃ¡rio.
 *
 */
Bool isEmpty (RedBlackST st) {
    return st->root == NULL;
}

/*------------------------------------------------------------*/
/*
 * OPERAÃ‡Ã•ES PARA TABELAS DE SÃMBOLOS ORDENADAS: isEmpty
 * min(), max(), rank(), select(), deleteMin() e deleteMax().
 */

/*-----------------------------------------------------------*/
/*
 *  MIN(ST)
 * 
 *  RECEBE uma tabela de sÃ­mbolos ST e RETORNA uma cÃ³pia/clone
 *  da menor chave na tabela.
 *
 *  Se ST estÃ¡ vazia RETORNA NULL.
 *
 */


Node *minNode (Node *node) {
    // assert node != NULL;
    if (node->left == NULL) 
        return node; 
    return minNode (node->left); 
}

void *min (RedBlackST st) {
    if (isEmpty (st))
      return NULL;
    return getKey (minNode (st->root));
}

/*-----------------------------------------------------------*/
/*
 *  deleteMIN(ST)
 * 
 *  RECEBE uma tabela de sÃ­mbolos ST e remove a entrada correspondente
 *  Ã  menor chave.
 *
 *  Se ST estÃ¡ vazia, faz nada.
 *
 */

// delete the key-value pair with the minimum key rooted at h
// delete the key-value pair with the minimum key rooted at h
Node* deleteMinNode (Node *h) { 
    if (h->left == NULL)
        return NULL;

    if (!isRed (h->left) && !isRed (h->left->left))
        h = moveRedLeft (h);

    h->left = deleteMinNode (h->left);
    return balance (h);
}

void deleteMin (RedBlackST st) {
    if (isEmpty (st))
        return;

    // if both children of root are black, set root to red
    if (!isRed (st->root->left) && !isRed (st->root->right))
        st->root->color = RED;

    st->root = deleteMinNode (st->root);

    if (!isEmpty (st))
        st->root->color = BLACK;
    // assert check();
}

/*-----------------------------------------------------------*/
/*
 *  MAX(ST)
 * 
 *  RECEBE uma tabela de sÃ­mbolos ST e RETORNA uma cÃ³pia/clone
 *  da maior chave na tabela.
 *
 *  Se ST estÃ¡ vazia RETORNA NULL.
 *
 */

// the largest key in the subtree rooted at x; NULL if no such key
Node *maxNode (Node *x) { 
    // assert x != NULL;
    if (x->right == NULL)
        return x; 
    return maxNode (x->right); 
} 

void *max (RedBlackST st) {
    if (isEmpty(st))
        return NULL;
    return getKey (maxNode (st->root));
} 

/*-----------------------------------------------------------*/
/*
 *  deleteMAX(ST)
 * 
 *  RECEBE uma tabela de sÃ­mbolos ST e remove a entrada correspondente
 *  Ã  maior chave.
 *
 *  Se ST estÃ¡ vazia, faz nada.
 *
 */

// delete the key-value pair with the maximum key rooted at h
Node *deleteMaxNode (Node *h) { 
    if (isRed (h->left))
        h = rotateRight (h);

    if (h->right == NULL)
        return NULL;

    if (!isRed (h->right) && !isRed (h->right->left))
        h = moveRedRight (h);

    h->right = deleteMaxNode (h->right);

    return balance (h);
}

void deleteMax (RedBlackST st) {
    if (isEmpty (st))
        return;

    // if both children of root are black, set root to red
    if (!isRed (st->root->left) && !isRed (st->root->right))
        st->root->color = RED;

    ST->root = deleteMaxNode (st->root);
    
    if (!isEmpty (st)) 
        st->root->color = BLACK;
    // assert check();
}

/*-----------------------------------------------------------*/
/*
 *  RANK(ST, KEY)
 * 
 *  RECEBE uma tabela de sÃ­mbolos ST e uma chave KEY.
 *  RETORNA o nÃºmero de chaves em ST menores que KEY.
 *
 *  Se ST estÃ¡ vazia RETORNA 0.
 *
 */

// number of keys less than key in the subtree rooted at x
int rankNode (RedBlackST st, const void *key, Node *x) {
    if (x == NULL)
        return 0; 
    int cmp = st->compareTo (key, x->key); 
    if      (cmp < 0) return rankNode (st, key, x->left); 
    else if (cmp > 0) return 1 + sizeNode (x->left) + rankNode (st, key, x->right); 
    else              return sizeNode (x->left); 
} 

int rank (RedBlackST st, const void *key) {
    if (key == NULL) 
        ERROR (KEY NULL);
    return rankNode (st, key, st->root);
} 

/*-----------------------------------------------------------*/
/*
 *  freeST(ST)
 *
 *  RECEBE uma RedBlackST  ST e devolve ao sistema toda a memoria 
 *  utilizada por ST.
 *
 */
void freeST (RedBlackST st) {
}


/*------------------------------------------------------------*/
/*
 * OPERAÃ‡Ã•ES USUAIS: put(), get(), contains(), delete(),
 * size() e isEmpty().
 */

/*-----------------------------------------------------------*/
/*
 *  put(ST, KEY, NKEY, VAL, NVAL)
 * 
 *  RECEBE a tabela de sÃ­mbolos ST e um par KEY-VAL e procura a KEY na ST.
 *
 *     - se VAL Ã© NULL, a entrada da chave KEY Ã© removida da ST  
 *  
 *     - se KEY nao e' encontrada: o par KEY-VAL Ã© inserido na ST
 *
 *     - se KEY e' encontra: o valor correspondente Ã© atualizado
 *
 *  NKEY Ã© o nÃºmero de bytes de KEY e NVAL Ã© o nÃºmero de bytes de NVAL.
 *
 *  Para criar uma copia/clone de KEY Ã© usado o seu nÃºmero de bytes NKEY.
 *  Para criar uma copia/clode de VAL Ã© usado o seu nÃºmero de bytes NVAL.
 *
 */

// insert the key-value pair in the subtree rooted at h
Node *auxPut (RedBlackST st, Node *h, const void *key, size_t sizeKey, const void *val, size_t sizeVal) { 
    if (h == NULL) {
        return newNode (key, sizeKey, val, sizeVal, RED);
    }

    if (h->key == NULL) {
        ERROR (AAAAHHH H->KEY NULL);
    }
    
    int cmp = st->compareTo (key, h->key);
    if      (cmp < 0) h->left  = auxPut (st, h->left,  key, sizeKey, val, sizeVal); 
    else if (cmp > 0) h->right = auxPut (st, h->right, key, sizeKey, val, sizeVal); 
    else              editVal (h, val, sizeVal);

    // fix-up any right-leaning links
    if (isRed (h->right) && !isRed (h->left))      h = rotateLeft  (h);
    if (isRed (h->left)  &&  isRed (h->left->left)) h = rotateRight (h);
    if (isRed (h->left)  &&  isRed (h->right))     flipColors (h);

    h->size = sizeNode (h->left) + sizeNode (h->right) + 1;
    return h;
}    

void put (RedBlackST st, const void *key, size_t sizeKey, const void *val, size_t sizeVal) {
    //printf("\nPUT %s\n\n", key);
    if (key == NULL) {
        ERROR ("KEY EH NULL");
        return;
    }

    if (val == NULL) {
        printf ("aaaaaaaah");
            //delete (st, key);
        return;
    }

    st->root = auxPut (st, st->root, key, sizeKey, val, sizeVal);
    st->root->color = BLACK;
        // assert check();
}

/*-----------------------------------------------------------*/
/*
 *  get(ST, KEY)
 *
 *  RECEBE uma tabela de sÃ­mbolos ST e uma chave KEY.
 *
 *     - se KEY estÃ¡ em ST, RETORNA uma cÃ³pia/clone do valor
 *       associado a KEY;
 *
 *     - se KEY nÃ£o estÃ¡ em ST, RETORNA NULL.
 * 
 */
void *get (RedBlackST st, const void *key) {
    Node *n = st->root;
    while (n != NULL) {
        int cmp = st->compareTo (key, n->key);
        if      (cmp < 0) n = n->left;
        else if (cmp > 0) n = n->right;
        else              return n->val;
    } 
    return NULL;
}

/*-----------------------------------------------------------*/
/* 
 *  CONTAINS(ST, KEY)
 *
 *  RECEBE uma tabela de sÃ­mbolos ST e uma chave KEY.
 * 
 *  RETORNA TRUE se KEY estÃ¡ na ST e FALSE em caso contrÃ¡rio.
 *
 */
Bool contains (RedBlackST st, const void *key) {
    return get(st, key) != NULL;
}

/*-----------------------------------------------------------*/
/* 
 *  DELETE(ST, KEY)
 *
 *  RECEBE uma tabela de sÃ­mbolos ST e uma chave KEY.
 * 
 *  Se KEY estÃ¡ em ST, remove a entrada correspondente a KEY.
 *  Se KEY nÃ£o estÃ¡ em ST, faz nada.
 *
 */

// delete the key-value pair with the given key rooted at h
Node *deleteNode (RedBlackST st, Node *h, const void *key) { 
    // assert get(h, key) != NULL;
    if (st->compareTo (key, h->key) < 0)  {
        if (!isRed (h->left) && !isRed (h->left->left))
            h = moveRedLeft (h);
        h->left = deleteNode (st, h->left, key);
    }

    else {
        if (isRed (h->left))
            h = rotateRight (h);
        if (st->compareTo (key, h->key) == 0 && (h->right == NULL))
            return NULL;
        if (!isRed (h->right) && !isRed (h->right->left))
            h = moveRedRight (h);
        if (st->compareTo (key, h->key) == 0) {
            Node *x = minNode (h->right);
            h->key = x->key;
            h->val = x->val;
            // h->val = get(h->right, min(h->right)->key);
            // h->key = min(h->right)->key;
            h->right = deleteMinNode (h->right);
        }
        else h->right = deleteNode (st, h->right, key);
    }
    return balance(h);
}

void delete (RedBlackST st, const void *key) {
    if (key == NULL) 
        ERROR (Delete com KEY NULL);

    if (isEmpty (st))
        ERROR (BST underflow);

    if (!contains (st, key)) 
        return;

    // if both children of root are black, set root to red
    if (!isRed (st->root->left) && !isRed (st->root->right))
        st->root->color = RED;

    st->root = deleteNode (st, st->root, key);
    if (!isEmpty (st))
        st->root->color = BLACK;
        // assert check();
}

/*-----------------------------------------------------------*/
/*
 *  SELECT(ST, K)
 * 
 *  RECEBE uma tabela de sÃ­mbolos ST e um inteiro K >= 0.
 *  RETORNA a (K+1)-Ã©sima menor chave da tabela ST.
 *
 *  Se ST nÃ£o tem K+1 elementos RETORNA NULL.
 *
 */
void *select (RedBlackST st, int k) {
    return NULL;
}


/*-----------------------------------------------------------*/
/* 
 *  KEYS(ST, INIT)
 * 
 *  RECEBE uma tabela de sÃ­mbolos ST e um Bool INIT.
 *
 *  Se INIT Ã© TRUE, KEYS() RETORNA uma cÃ³pia/clone da menor chave na ST.
 *  Se INIT Ã© FALSE, KEYS() RETORNA a chave sucessora da Ãºltima chave retornada.
 *  Se ST estÃ¡ vazia ou nÃ£o hÃ¡ sucessora da Ãºltima chave retornada, KEYS() retorna NULL.
 *
 *  Se entre duas chamadas de KEYS() a ST Ã© alterada, o comportamento Ã© 
 *  indefinido. 
 *  
 */
void *keys (RedBlackST st, Bool init) {
    return NULL;
}



/*------------------------------------------------------------*/
/* 
 * FunÃ§Ãµes administrativas
 */

/***************************************************************************
 *  Utility functions.
 ***************************************************************************/

/*
 * HEIGHT(ST)
 * 
 * RECEBE uma RedBlackST e RETORNA a sua altura. 
 * Uma BST com apenas um nÃ³ tem altura zero.
 * 
 */
int height (RedBlackST st) {
        return 0;
}


/***************************************************************************
 *  Check integrity of red-black tree data structure.
 ***************************************************************************/

/*
 * CHECK(ST)
 *
 * RECEBE uma RedBlackST ST e RETORNA TRUE se nÃ£o encontrar algum
 * problema de ordem ou estrutural. Em caso contrÃ¡rio RETORNA 
 * FALSE.
 * 
 */
Bool check (RedBlackST st) {
    if (!isBST(st))            ERROR("check(): not in symmetric order");
    if (!isSizeConsistent (st)) ERROR("check(): subtree counts not consistent");
    if (!isRankConsistent (st)) ERROR("check(): ranks not consistent");
    if (!is23 (st))             ERROR("check(): not a 2-3 tree");
    if (!isBalanced (st))       ERROR("check(): not balanced");
    return isBST (st) && isSizeConsistent (st) && isRankConsistent (st) && is23 (st) && isBalanced (st);
}


/* 
 * ISBST(ST)
 * 
 * RECEBE uma RedBlackST ST.
 * RETORNA TRUE se a Ã¡rvore Ã© uma BST.
 * 
 */
    // is the tree rooted at x a BST with all keys strictly between min and max
    // (if min or max is NULL, treat as empty constraint)
    // Credit: Bob Dondero's elegant solution
Bool isBSTNode (RedBlackST st, Node *x, void *min, void *max) {
    if (x == NULL) 
        return TRUE;
    if (min != NULL && st->compareTo (min, x->key) <= 0)
        return FALSE;
    if (max != NULL && st->compareTo (max, x->key) >= 0) 
        return FALSE;
    return isBSTNode (st, x->left, min, x->key) && isBSTNode (st, x->right, x->key, max);
} 

static Bool isBST (RedBlackST st) {
    return isBSTNode (st, st->root, NULL, NULL);;
}


/* 
 *  ISSIZECONSISTENT(ST) 
 *
 *  RECEBE uma RedBlackST ST e RETORNA TRUE se para cada nÃ³ h
 *  vale que size(h) = 1 + size(h->left) + size(h->right) e 
 *  FALSE em caso contrÃ¡rio.
 */
static Bool isSizeConsistent (RedBlackST st) {
    return FALSE;
}


/* 
 *  ISRANKCONSISTENT(ST)
 *
 *  RECEBE uma RedBlackST ST e RETORNA TRUE se seus rank() e
 *  select() sÃ£o consistentes.
 */  
/* check that ranks are consistent */
static Bool isRankConsistent (RedBlackST st) {
    return FALSE;
}

/* 
 *  IS23(ST)
 *
 *  RECEBE uma RedBlackST ST e RETORNA FALSE se hÃ¡ algum link RED
 *  para a direta ou se ha dois links para esquerda seguidos RED 
 *  Em caso contrÃ¡rio RETORNA TRUE (= a ST representa uma Ã¡rvore 2-3). 
 */
static Bool is23 (RedBlackST st) {
    return FALSE;
}


/* 
 *  ISBALANCED(ST) 
 * 
 *  RECEBE uma RedBlackST ST e RETORNA TRUE se st satisfaz
 *  balanceamento negro perfeiro.
 */ 
static Bool isBalanced (RedBlackST st) {
    return FALSE;
}