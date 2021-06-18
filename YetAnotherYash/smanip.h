#ifndef __YAY_STRING_MANIPULATION_H
#define __YAY_STRING_MANIPULATION_H

#include <stddef.h>

/* Reads a (multi) line into 'line' and returns its length */
size_t read_line(char **line);

/* Returns pointer to newly allocated array of all the words
   from line and stores their count + 1 in *nargs. +1 because
   the last element of the array is NULL. */
char **get_args(char *line, int *nargs);

#endif /* __YAY_STRING_MANIPULATION_H */
