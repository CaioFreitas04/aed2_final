/*
 *	Caio Figueiredo Freitas 	-- 12221BCC020
 *	Diogo Vieira Silva 			-- 12221BCC029
 *	Patrick Gomes de Oliveira	-- 12221BCC035
*/

#ifndef SONG_PARSER_H
#define SONG_PARSER_H

void normalise_word(char *input);
void parse_file(char *file, insert_func insert, void *data_structure);

#endif