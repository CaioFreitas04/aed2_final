/*
 *	Caio Figueiredo Freitas 	-- 12221BCC020
 *	Diogo Vieira Silva 			-- 12221BCC029
 *	Patrick Gomes de Oliveira	-- 12221BCC035
*/

#include <stdlib.h>
#include <string.h>
#include "binary_search.h"

//implementação de quicksort para ordenação de palavras;
//begin;

void swap(word_t *x, word_t *y) {
	word_t temp = *x;
	*x = *y;
	*y = temp;
}

int wordcmp(word_t *x, word_t *y) {
	return strcmp(x->word, y->word);
}

int partition(word_t *arr, int p, int r) {	//utilizar r-1 no programa;
	word_t pivot = arr[r];
	int i = p-1;
	for(int j = p; j < r; j++) {
		if(wordcmp(&arr[j], &pivot) < 0) {
			i++;
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i+1], &arr[r]);
	return i+1;
}

void word_qsort(word_t *arr, int p, int r) {
	if(p < r) {
		int q = partition(arr, p, r);
		word_qsort(arr, p, q-1);
		word_qsort(arr, q+1, r);
	}
}

//end;

word_t *word_bsearch(word_t *arr, char *key, int p, int r) {
	if(r >= p) {
		int q = (p + r) / 2;
		
		int cmp_res = strcmp(key, arr[q].word);
		
		if(cmp_res == 0)
			return &arr[q];
		
		if(cmp_res > 0)
			return word_bsearch(arr, key, q+1, r);
		else
			return word_bsearch(arr, key, p, q-1);
	}
	return NULL;
}

//defs e funções adicionadas para funcionar com parse_file;
array *initialise_array() {
	array *warr = calloc(1, sizeof(array));
	warr->size = 0;
	warr->size_max = BASE_ARRAY_SIZE;
	warr->arr = calloc(warr->size_max, sizeof(word_t));
	
	return warr;
}

int insert_array(word_t input, void *warray) {
	array *warr = (array*) warray;
	
	//testar se uma palavra já existe e atualizar, se necessário;
	for (int i = 0; i < warr->size; i++) {
        if (strcmp(warr->arr[i].word, input.word) == 0) {
            warr->arr[i].freq += input.freq;

            if (input.sample.freq > warr->arr[i].sample.freq) {
                warr->arr[i].sample = input.sample;
            }
            return 0;
        }
    }
	
	if(warr->size >= warr->size_max) {
		warr->size_max *= 2;
		warr->arr = realloc(warr->arr, warr->size_max * sizeof(word_t));
	}
	//inserção normal;
	warr->arr[warr->size] = input;
	warr->size++;
	
	return 0;
}

word_t *binary_search(char *key, void *data_structure) {
	array *warr = (array*) data_structure;
	return word_bsearch(warr->arr, key, 0, warr->size-1);
}

void array_quicksort(void *data_structure) {
	array *warr = (array *) data_structure;
	word_qsort(warr->arr, 0, warr->size-1);
}

void no_sort(void *data_structure) {
	return;
}

word_t *search_index(int index, array *warr) {
	if(index < warr->size) {
		return &(warr->arr[index]);
	}
	return NULL;
}