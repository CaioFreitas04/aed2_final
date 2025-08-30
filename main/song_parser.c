/*
 *	Caio Figueiredo Freitas 	-- 12221BCC020
 *	Diogo Vieira Silva 			-- 12221BCC029
 *	Patrick Gomes de Oliveira	-- 12221BCC035
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "data_def.h"
#include "song_parser.h"

void normalise_word(char *input) {
    char *in = input, *out = input;
    while (*in) {
        unsigned char c = (unsigned char)*in;

        if (c < 128) { //ASCII;
            if (isalpha(c) || c == '-') {
                *out++ = tolower(c);
            }
            in++;
        } else {
            //podemos fingir que todas as instâncias de caracteres
			//não-ASCII são minúsculas, porque isso é preferível
			//a ter palavras sem-sentido e não requer a importação
			//de uma biblioteca terceira ou algo assim.
			//Ou o uso de C++ QUE TEM SUPORTE NATIVO PRA ISSO;
            int bytes = 0;
            if ((c & 0xE0) == 0xC0) bytes = 2;
            else if ((c & 0xF0) == 0xE0) bytes = 3;
            else if ((c & 0xF8) == 0xF0) bytes = 4;
            else bytes = 1;
            for (int i = 0; i < bytes && *in; i++) {
                *out++ = *in++;
			//nota: a ordenação lexicográfica é tecnicamente incorreta
			//mas as funções processam de forma idêntica, então...;
            }
        }
    }

    *out = 0;
}

void parse_file(char *file, insert_func insert, void *data_structure) {
    /*esse trambolho é um monumento aos pecados de todo mundo que trabalhou nesse código;
	 *como dizia um comentário antes da revisão e documentação desse arquivo:
	 * "Eu espero que isso seja tão ruim que não me façam
	 *	mexer com processamento de strings em C novamente."
	*/ 
    FILE *fp = fopen(file, "r");
    
    char line[256], estrofe[2048] = "";
    song curr;
    memset(&curr, 0, sizeof(curr));	//LIMPEZA;
    
    if (!fgets(curr.title, sizeof(curr.title), fp))
		return;
    curr.title[strcspn(curr.title, "\r\n")] = 0;
    //primeiro são extraídos o título e o artista da música. A parte fácil;
    if (!fgets(curr.artist, sizeof(curr.artist), fp))
		return;
    curr.artist[strcspn(curr.artist, "\r\n")] = 0;
    
	//o processamento de uma música é feito estrofe-por-estrofe. Isso permite a relativamente fácil
	//extração de palavras associadas às suas respectivas estrofes.
	//O tratamento de frequência é, felizmente, feito pelas funções de inserção;
	
    while(fgets(line, sizeof(line), fp)) {
        if(strcmp(line, "\n") == 0 || strcmp(line, "\r\n") == 0) {
			//a segunda cláusula de comparação foi adicionada porque Windows;
            if(strlen(estrofe) > 0) {
                song estrofe_song = curr;
				
                strncpy(estrofe_song.lyric, estrofe, sizeof(estrofe_song.lyric)-1);
                estrofe_song.lyric[sizeof(estrofe_song.lyric)-1] = '\0';
				
				//copiando a letra da estrofe para processamento para um buffer temporário;
                char estrofe_copy[2048];
                strncpy(estrofe_copy, estrofe, sizeof(estrofe_copy)-1);
                estrofe_copy[sizeof(estrofe_copy)-1] = '\0';
                
                char *token = strtok(estrofe_copy, " \n");
                while(token) {
                    normalise_word(token);
                    if(strlen(token) > 3 && *token != 0) {
						//garante que as entradas tenham mais de 3 caracteres;
                        word_t word_in;
                        memset(&word_in, 0, sizeof(word_in));	//LIMPEZA;
                        strcpy(word_in.word, token);
                        word_in.sample = estrofe_song;	//são utilizadas tanto uma estrofe_copy quanto uma estrofe_song
                        word_in.freq = 1;				//para preservar uma estrofe que necessariamente contenha a palavra
														//sendo processada. Isso consertava um erro onde todas as palavras
                        insert(word_in, data_structure);//tinham a mesma estrofe associada a todas elas. Senão, processaríamos
														//os tokens na própria estrofe_song mesmo;
                    }
                    token = strtok(NULL, " \n");	//LIMPEZA;
                }

                estrofe[0] = 0;
            }
        } else {
            strncat(estrofe, line, sizeof(estrofe)-strlen(estrofe)-1);
			//senão, continue processando a estrofe;
        }
    }
    //precisa ser repetido para a ultima estrofe para o caso do arquivo de letra
	//não conter uma última linha vazia;
	//esse código é quase idêntico ao acima;
    if(strlen(estrofe) > 0) {
        song estrofe_song = curr;
		
        strncpy(estrofe_song.lyric, estrofe, sizeof(estrofe_song.lyric)-1);
        estrofe_song.lyric[sizeof(estrofe_song.lyric)-1] = '\0';

        char estrofe_copy[2048];
        strncpy(estrofe_copy, estrofe, sizeof(estrofe_copy)-1);
        estrofe_copy[sizeof(estrofe_copy)-1] = '\0';

        char *token = strtok(estrofe_copy, " \n");
        while(token) {
            normalise_word(token);
            if(strlen(token) > 3 && *token != 0) {
				
                word_t word_in;
                memset(&word_in, 0, sizeof(word_in));	//LIMPEZA;
                strcpy(word_in.word, token);
                word_in.sample = estrofe_song;
                word_in.freq = 1;

                insert(word_in, data_structure);
            }
            token = strtok(NULL, " \n");	//LIMPEZA;
        }
    }
    
    fclose(fp);
}