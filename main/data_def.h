/*
 *	Caio Figueiredo Freitas 	-- 12221BCC020
 *	Diogo Vieira Silva 			-- 12221BCC029
 *	Patrick Gomes de Oliveira	-- 12221BCC035
*/

#ifndef DATA_DEF_H
#define DATA_DEF_H

//definição dos dados usados pelo programa todo;
//begin;
typedef struct {
	char title[50];
	char artist[50];
	char lyric[256];
	int freq;
} song;

typedef struct {
	char word[32];
	song sample;
	int freq;
} word_t;
//end;

//typedefs utilizados para formar a ds_s na main;
//o motivo pelo qual existem wrappers para as funções das estruturas de dados que recebem (void*);
//begin;
typedef int (*insert_func)(word_t input, void *data_structure);
typedef word_t *(*search_func)(char *key, void *data_structure);
typedef void (*sort_func)(void *data_structure);
//end;

#endif