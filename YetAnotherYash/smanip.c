#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "smanip.h"
#include "yay.h"

size_t read_line(char **line) {
  static size_t len = LINE_LEN;
  int nread;
  char *ret = *line, *nl;

  if ((nread = read(0, ret, LINE_LEN)) == -1) {
    perror("read_line read");
    exit(1);
  }

  while ((nl = strchr(ret, '\n')) == NULL || (nl != ret && *(nl-1) == '\\')) {
    *line = (char*) realloc(*line, len + LINE_LEN);
    /* write over last two chars if they were '\' and '\n' */
    ret = nl == NULL ? *line + len : *line + (len - 2); 
    len += LINE_LEN;

    if ((nread = read(0, ret, LINE_LEN)) == -1) {
      perror("read_line read");
      exit(1);
    }
  }
  nl = strchr(*line, '\n');
  *nl = '\0';

  return strlen(*line);
}

char **get_args(char *line, int *nargs) {
  int i = 0;
  char *curr;
  
  *nargs = strlen(line) / 4 + 1; /* We assume some number of arguments */
  char **args = (char**) calloc(*nargs,  sizeof(char*));
  if (!args) goto bad;

  for (i = 0; ;line = NULL) {
    curr = strtok(line, " \t");
    if (!curr) {
      args[i++] = NULL;
      break;
    }
    
    args[i] = (char*) malloc(strlen(curr) + 1);
    if (!args[i]) goto bad;
    strcpy(args[i++], curr);
    
    if (i == *nargs) {
      *nargs *= 2;
      args = (char**) realloc(args, *nargs * sizeof(char*));
      if (!args) goto bad;
    }
  }
  
  args = (char**) realloc(args, i * sizeof(char*));
  if (!args) goto bad;
  *nargs = i;
  return args;

 bad:
  *nargs = -1;
  return NULL;
}
