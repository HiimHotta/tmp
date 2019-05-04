/*
 * MAC0323 Algoritmos e Estruturas de Dados II
 * 
 * NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ARQUIVO
 *
 * Interface levemente modificada para a tabela de simbolos 
 * implementada atraves de uma BST rubro-negra (RedBlackST)
 * descrita em 
 *  
 *   https://algs4.cs.princeton.edu/33balanced/
 */


/* 
 * REDBLACKST
 *
 * Representa uma tabela de sÃ­mbolos (ST) ordenada "genÃ©rica".
 *
 * Permite as operaÃ§Ãµes usuais: put(), get(), contains(), delete(), size() 
 * e isEmpty().
 *
 * Fornece algumas operaÃ§Ãµes para tabelas de sÃ­mbolos ordenadas: min(), 
 * max(), rank(), select(), deleteMin() e deleteMax().
 *
 * TambÃ©m fornece a operaÃ§Ã£o keys() para iterar todas as chaves.
 *
 * Uma tabela de sÃ­mbolos implementa a abstraÃ§Ã£o "array associativo":
 * ao associar um valor a uma chave que jÃ¡ estÃ¡ na tabela de sÃ­mbolos,
 * a convenÃ§Ã£o Ã© substituir o valor antigo pelo novo valor.
 *
 * Ao contrÃ¡rio de java.util.Map, essa implementaÃ§Ã£o usa a convenÃ§Ã£o de que
 * os valores nÃ£o podem ser NULL - associar o valor de uma chave para NULL
 * Ã© equivalente a excluir a chave da tabela de sÃ­mbolos.
 * 
 * Esta implementaÃ§Ã£o usa uma Ã¡rvore rubro-negra.
 * Requer que uma funÃ§Ã£o compar() seja argumento do construtor.
 *
 * As operaÃ§Ãµes put(), get(), delete(), deleteMin(), deleteMax(), 
 * contains(), select(), rank(), min(), max() consomem  tempo 
 * logarÃ­tmico.
 * As operaÃ§Ãµes size(), isEmpty() consomem tempo constante.
 *
 * Para documentaÃ§Ã£o adicional, veja 
 * https://algs4.cs.princeton.edu/33balanced, SeÃ§Ã£o 3.3
 * Algorithms, 4th Edition, por Robert Sedgewick e Kevin Wayne.
 */

#ifndef _REDBLACKST_H
#define _REDBLACKST_H

#include <stddef.h> /* size_t */
#include "util.h"   /* Bool */

typedef struct redBlackST *RedBlackST;

/*-----------------------------------------------------------*/
/*  prototipos das funcoes que manipulam a tabela de simbolos */

/*-----------------------------------------------------------*/
/* 
 *  Construtor e "destrutor"
 */
/* 
 *  INIT(COMPAR)
 *
 *  cria uma ST vazia 
 *  compar() Ã© ponteiro para uma funÃ§Ã£o que retorna um inteiro tal que
 *
 *      compar(key1, key2) retorna um inteiro < 0 se key1 <  key2
 *      compar(key1, key2) retorna um 0           se key1 == key2
 *      compar(key1, key2) retorna um inteiro > 0 se key1 >  key2
 */
RedBlackST
initST(int (*compar)(const void *this, const void *that));

/* libera todo o espaÃ§o usado pela ST */
void  
freeST(RedBlackST st);

/*------------------------------------------------------------*/
/*
 * Permite as operaÃ§Ãµes usuais: put(), get(), contains(), delete(),
 * size() e isEmpty().
 */

/* coloca um par KEY-VAL na ST */
void  
put(RedBlackST st, const void *key, size_t nKey, const void *val, size_t nVal);

/* retorna o valor associado a KEY */
void * 
get(RedBlackST st, const void *key);

/* retorna TRUE se KEY esta na ST e FALSE em caso contrÃ¡rio */
Bool
contains(RedBlackST st, const void *key);

/* remove da ST a chave KEY */
void
delete(RedBlackST st, const void *key);

/* retorna o numero de itens na ST */
int
size(RedBlackST st);

/* retorna TRUE se ST esta vazia e FALSE em caso contrÃ¡rio */
Bool
isEmpty(RedBlackST st);

/*------------------------------------------------------------*/
/*
 * Fornece algumas operaÃ§Ãµes para tabelas de sÃ­mbolos ordenadas: 
 * min(), max(), rank(), select(), deleteMin() e deleteMax().
 */

/* menor chave na ST */
void *
min(RedBlackST st);

/* maior chave na ST */
void *
max(RedBlackST st);

/* numero de chaves menores que key */
int
rank(RedBlackST st, const void *key);

/* remove menor chave */
void
deleteMin(RedBlackST st);

/* remove menor chave */
void
deleteMax(RedBlackST st);

/* a k-Ã©sima chave da ST */
void *
select(RedBlackST st, int k);

/*------------------------------------------------------------*/
/*
 * TambÃ©m fornece a operaÃ§Ã£o keys() para iterar todas as chaves.
 */

/* all of the keys, as an Iterable */
void *
keys(RedBlackST st, Bool init);

/***************************************************************************
 *  Check integrity of red-black tree data structure.
 ***************************************************************************/
Bool 
check(RedBlackST st);

#endif /* _REDBLACKST_H */
