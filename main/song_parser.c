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

        if (c < 128) { //ASCII
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
            }
        }
    }

    *out = 0;
}

void parse_file(char *file, insert_func insert, void *data_structure) {
    
    FILE *fp = fopen(file, "r");
    
    char line[256], stanza[2048] = "";
    song curr;
    memset(&curr, 0, sizeof(curr));
    
    if (!fgets(curr.title, sizeof(curr.title), fp)) return;
    curr.title[strcspn(curr.title, "\r\n")] = 0;
    
    if (!fgets(curr.artist, sizeof(curr.artist), fp)) return;
    curr.artist[strcspn(curr.artist, "\r\n")] = 0;
    
    while(fgets(line, sizeof(line), fp)) {
        if(strcmp(line, "\n") == 0 || strcmp(line, "\r\n") == 0) {
            if(strlen(stanza) > 0) {
                song stanza_song = curr;
                strncpy(stanza_song.lyric, stanza, sizeof(stanza_song.lyric)-1);
                stanza_song.lyric[sizeof(stanza_song.lyric)-1] = '\0';
				//pelo amor de deus diz que isso funciona
                char stanza_copy[2048];
                strncpy(stanza_copy, stanza, sizeof(stanza_copy)-1);
                stanza_copy[sizeof(stanza_copy)-1] = '\0';
                //espero que isso seja ruim o suficiente para que
				//nn me deixem fazer manipulacao de String em C
				//de novo
                char *token = strtok(stanza_copy, " \n");
                while(token) {
                    normalise_word(token);
                    if(strlen(token) > 3 && *token != 0) {
                        word_t word_in;
                        memset(&word_in, 0, sizeof(word_in));
                        strcpy(word_in.word, token);
                        word_in.sample = stanza_song;
                        word_in.freq = 1;

                        insert(word_in, data_structure);
                    }
                    token = strtok(NULL, " \n");
                }

                stanza[0] = 0;
            }
        } else {
            strncat(stanza, line, sizeof(stanza)-strlen(stanza)-1);
        }
    }
    //precisa ser repetido para a ultima estrofe
	//isso poderia ser uma funcao mas eu ja me enfiei aqui
    if(strlen(stanza) > 0) {
        song stanza_song = curr;
        strncpy(stanza_song.lyric, stanza, sizeof(stanza_song.lyric)-1);
        stanza_song.lyric[sizeof(stanza_song.lyric)-1] = '\0';

        char stanza_copy[2048];
        strncpy(stanza_copy, stanza, sizeof(stanza_copy)-1);
        stanza_copy[sizeof(stanza_copy)-1] = '\0';

        char *token = strtok(stanza_copy, " \n");
        while(token) {
            normalise_word(token);
            if(strlen(token) > 3 && *token != 0) {
                word_t word_in;
                memset(&word_in, 0, sizeof(word_in));
                strcpy(word_in.word, token);
                word_in.sample = stanza_song;
                word_in.freq = 1;

                insert(word_in, data_structure);
            }
            token = strtok(NULL, " \n");
        }
    }
    
    fclose(fp);
}