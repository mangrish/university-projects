/* read_off.h
 *
 * Parses OFF files, returning an 'object' (more correctly, a polyhedron,
 * neither necessarily closed nor continuous).
 */


#include "structs2.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

#ifndef READ_OFF_H
#define READ_OFF_H

#define MAX_LENGTH_INPUT_LINE       256
#define COMMENT_CHAR                '#'
#define OBJECT_IDENTIFIER           "off"
#define LIGHT_IDENTIFIER            "light"
#define AMBIENT_IDENTIFIER          "ambient"

#endif


Polyhedron* read_off(const char*);

void get_line(FILE*, char*);
char* next_token(char*);
