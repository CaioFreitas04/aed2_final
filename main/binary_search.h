/*
 *	Caio Figueiredo Freitas 	-- 12221BCC020
 *	Diogo Vieira Silva 			-- 12221BCC029
 *	Patrick Gomes de Oliveira	-- 12221BCC035
*/

#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include "data_def.h"

//implementação de quicksort para ordenação de palavras;
//begin;

void swap(word_t *x, word_t *y);
int wordcmp(word_t *x, word_t *y);
int partition(word_t *arr, int p, int r);
void word_qsort(word_t *arr, int p, int r);

//end;

word_t *word_bsearch(word_t *arr, char *key, int p, int r); //a busca binária propriamente dita;


//defs e funções adicionadas para funcionar com parse_file;
//begin;
#define BASE_ARRAY_SIZE 128

typedef struct {
	word_t *arr;
	int size;
	int size_max;
} array;

array *initialise_array();
word_t *search_index(int index, array *warr);			//busca direta por índice;
int insert_array(word_t input, void *data_structure);	//função de inserção para garantir acesso direto;
word_t *binary_search(char *key, void *data_structure);	//wrapper para a busca binária;
void array_quicksort(void *data_structure);				//wrapper para o quicksort;
void no_sort(void *data_structure);						//função necessária para garantir a existência da ds_s na main;			
//end;

#endif