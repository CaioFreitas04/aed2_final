/*
 *	Caio Figueiredo Freitas 	-- 12221BCC020
 *	Diogo Vieira Silva 			-- 12221BCC029
 *	Patrick Gomes de Oliveira	-- 12221BCC035
*/

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include "data_def.h"

//definições gerais da árvore;
//begin;
typedef struct node *tree;

typedef struct node {
	word_t data;
	int factor;
	tree l;
	tree r;
} node;

#define PREFIX 0
#define INFIX 1
#define POSTFIX 2

tree *initialise_tree();
int is_empty(tree *t);
word_t *word_tsearch(char *key, tree *t);
int freq_tsearch(char *key, tree *t);
int tree_height(tree *t);
void traversal(tree t, int type);
void print_node(tree t);
void destroy_tree(tree *t);
//end;

//funções para árvore convencional;
//begin;
int insert_node(word_t input, tree *t);
//end;

//funções para árvore AVL;
//begin;
void rotate_left(tree *t);
void rotate_right(tree *t);
void rotate(int rot, tree *t);
int factor(tree *t);
int insert_node_avl(word_t input, tree *t);
int insert_node_avl_freq(word_t input, tree *t);
//end;

//funções extras necessárias para o parse_file;
int insert_node_void(word_t input, void *t);
int insert_node_avl_void(word_t input, void *avl);
word_t *tree_search(char *key, void *data_structure);

#endif