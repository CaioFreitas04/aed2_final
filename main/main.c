/*
 *	Caio Figueiredo Freitas 	-- 12221BCC020
 *	Diogo Vieira Silva 			-- 12221BCC029
 *	Patrick Gomes de Oliveira	-- 12221BCC035
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include "data_def.h"
#include "binary_search.h"
#include "binary_tree.h"
#include "song_parser.h"

//struct para medição de tempo;
typedef struct {
	int begin_c;
	int end_c;
	double time;
} time_s;

//enumeração das estruturas de dados;
typedef enum {ARRAY, BIN_T, AVL_T} ds_e;

//operações;
#define LOAD_FILE 1
#define SEARCH_WORD 2
#define SEARCH_FREQ 3
#define EXIT 4

//struct responsável por segurar uma variedade de dados e funções
//relevantes à execução sequencial de várias funções do código
//simplesmente porque permite a iteração de várias operações com um único laço;
typedef struct {
	ds_e label;				//código da ED;
	char name[32];			//nome da ED;
	void *ds_pointer;		//ponteiro (void*) para a ED;
	insert_func insert;		//função de inserção;
	search_func search;		//função de pesquisa;
	sort_func sort;			//função de ordenação;
	time_s time_data;		//estrutura de temporização;
} ds_s;


int main() {
	array *arr     = initialise_array();
	tree *bnt      =  initialise_tree();
	tree *avl      =  initialise_tree();
	
	tree *avl_freq; //essa árvore só é inicializada na hora;

	ds_s soe[] = {
		{ ARRAY, " Busca Binaria (array) ", arr, insert_array        , binary_search, array_quicksort, {0, 0, 0.0} },
		{ BIN_T, "Arvore de Busca Binaria", bnt, insert_node_void    , tree_search  , no_sort        , {0, 0, 0.0} },
		{ AVL_T, "  Arvore Binaria--AVL  ", avl, insert_node_avl_void, tree_search  , no_sort        , {0, 0, 0.0} }
	};	//soe = struct of everything;
		//um nome curto e descritivo de sua função;
	
	while(1) {
		int op = 0;
		printf("\n\n%s\n%s\n%s\n%s\n",
		" _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ ",	//texto gerado com https://patorjk.com/software/taag/
		"|  _  |  _  |   __|     | __  |     |     |  |  |   __|     |     |",
		"|     |   __|   __|-   -|    -|  |  | | | |  |  |__   |-   -|   --|",
		"|__|__|__|  |_____|_____|__|__|_____|_|_|_|_____|_____|_____|_____|");
		printf("Por Caio \"Endy\" Freitas, Diogo \"Kanekko\" Vieira, Patrick Gomes.\n\n\n");
		
		
		printf("\t\t1. Carregar Novo Arquivo.\n");
		printf("\t\t2. Pesquisar Palavra.\n");
		printf("\t\t3. Pesquisar Por Frequencia.\n");
		printf("\t\t4. Sair.\n");
		
		printf("\n\t\t>> ");
		scanf("%d", &op);
		
		switch(op) {
			case LOAD_FILE:
				printf("\n\nInsira o nome do arquivo a ser lido.\n>> ");
				
				char filepath_in[256];
				fgetc(stdin);
				fgets(filepath_in, 256, stdin);
				filepath_in[strcspn(filepath_in, "\n")] = 0;
				
				FILE *testfp = fopen(filepath_in, "r");
				if(testfp == NULL) {
					printf("Arquivo inexistente! Ele esta na pasta do programa?\n\n");
					break;
				}
				for(int i = 0; i < 3; i++) {
					soe[i].time_data.begin_c = clock();							//início do clock;
					parse_file(filepath_in, soe[i].insert, soe[i].ds_pointer);	//lendo arquivo;
					soe[i].sort(soe[i].ds_pointer);								//ordenando (se necessário);
					soe[i].time_data.end_c = clock();							//fim do clock; //vv cálculo do tempo;
					soe[i].time_data.time = ((double) soe[i].time_data.end_c - soe[i].time_data.begin_c) / CLOCKS_PER_SEC;
					printf("\t\t===%s============\n", soe[i].name);
					printf("\t\tInsercao concluida em: %lf segundos.\n", soe[i].time_data.time);
					printf("\t\t======================================\n\n");
				}
				
			break;
			
			case SEARCH_WORD:
				word_t *search_out;
				char word_in[32];
				
				printf("\n\nPesquise uma palavra.\n>> ");
				
				fgetc(stdin);
				fgets(word_in, 32, stdin);
				word_in[strcspn(word_in, "\n")] = 0;
				
				for(int i = 0; i < 3; i++) {
					soe[i].time_data.begin_c = clock();							//início do clock;
					search_out = soe[i].search(word_in, soe[i].ds_pointer);		//pesquisa;
					soe[i].time_data.end_c = clock();							//fim do clock; //vv cálculo do tempo;
					soe[i].time_data.time = ((double) soe[i].time_data.end_c - soe[i].time_data.begin_c) / CLOCKS_PER_SEC;
					if(search_out == NULL) {
						printf("Erro ao pesquisar. Essa palavra realmente existe?\n\n");
						break;
					}
					printf("\t\t===%s============\n", soe[i].name);
					printf("\t\tPesquisa concluida em: %lf segundos.\n", soe[i].time_data.time);
					printf("\t\t======================================\n");
					printf("%s\nFrequencia no repositorio: %d\n\n", search_out->word, search_out->freq);
					printf("\t\tInformacoes da musica:\nTitulo: %s\nCompositor: %s\n\n%s\n", search_out->sample.title, search_out->sample.artist, search_out->sample.lyric);
					printf("\t\t======================================\n\n");
				}
				
			break;
			
			case SEARCH_FREQ:
				if(search_index(0, (array*)soe[ARRAY].ds_pointer) == NULL) {
					printf("Repositorio vazio. Carregue um arquivo primeiro.\n\n");
					break;
				}
			
				avl_freq = initialise_tree();
				int index = 0;
				word_t *word_f_in;
				
				//armazenando no tempo do array só como medida temporária;
				//e, tecnicamente, o array está sendo temporizado, não?
				soe[ARRAY].time_data.begin_c = clock();
				do {
					
					word_f_in = search_index(index, (array*)soe[ARRAY].ds_pointer);
					//o uso do array para a extração dos dados para essa árvore se dá
					//devido à facilidade e eficiência de percorrer todo o array;
					if(word_f_in != NULL) {
						insert_node_avl_freq(*word_f_in, avl_freq);
					}
					index++;
					
				} while(word_f_in != NULL);
				soe[ARRAY].time_data.end_c = clock();
				soe[ARRAY].time_data.time = ((double) soe[ARRAY].time_data.end_c - soe[ARRAY].time_data.begin_c) / CLOCKS_PER_SEC;
				
				printf("\n");
				traversal(*avl_freq, INFIX);
				printf("\n");
				
				printf("\t\t======================================\n");
				printf("\t\tArvore construida em: %lf segundos.\n", soe[ARRAY].time_data.time);
				printf("\t\t======================================\n");
				
				destroy_tree(avl_freq);	//gerenciamento de memória é a minha paixão;
				
			break;
			
			case EXIT:
				printf("\n\n\t\tEncerrando...\n\n");
				return 0;
			break;
		}
    }                                                                                                                                      
	return 0;
}