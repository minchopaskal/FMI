#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "smanip.h"
#include "stack.h"
#include "yay.h"

#define _NOT_IMPLEMENTED_

static stack bg_procs;

int handle_builtin(char **args);

void yay_loop(void) {
  pid_t pid;
  char *line, *cmd;
  char **args;
  int i, nread, nargs, status, is_bg;

  for (;;) {
    line = (char*) calloc(LINE_LEN, sizeof(char));
    if (!line) {
      perror("yay_loop line alloc");
      exit(1);
    }
  
    write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
    nread = read_line(&line);

    args = get_args(line, &nargs);

    cmd = args[0];
    if (!cmd) continue;
    
    int err;
    if (!(err = handle_builtin(args)) || err == -1)
      continue;
      
    if ((pid = fork()) < 0)
      perror("yay_loop fork");

    is_bg = 0;
    if (!strcmp(args[nargs - 2], "&")) {
      args[nargs - 2] = NULL;
      is_bg = 1;
    }

    if (!pid) {
      if (is_bg) setpgid(0, 0);
      
      if (execvp(cmd, args) == -1) {
        write(STDOUT_FILENO, NO_CMD_MSG, strlen(NO_CMD_MSG));
        exit(1);
      }
    }

    if (is_bg) {
      push(&bg_procs, pid);
    }

    waitpid(pid, &status, is_bg ? WNOHANG : 0);
    
    free(line);
    for (i = 0; i < nargs; ++i) {
      free(args[i]);
    }
    free(args);
  }  
}

int handle_builtin(char **args) {
  if (!strcmp(args[0], builtin_cmds[HELP])) {
    write(STDOUT_FILENO, HELP_MSG, strlen(HELP_MSG));
    return 0;
  }
  
  if (!strcmp(args[0], builtin_cmds[FAREWELL])) {
    write(STDOUT_FILENO, BYE_MSG, strlen(BYE_MSG));
    exit(0);
  }

  if (!strcmp(args[0], builtin_cmds[CD])) {
    chdir(args[1]);
    return 0;
  }

  if (!strcmp(args[0], builtin_cmds[FG])) {
    int bg_pid;
    int status;
    
    if (!args[1]) {
      if(pop(&bg_procs, &bg_pid) == -1) 
        return -1;
    } else if (get_nth(&bg_procs, &bg_pid, atoi(args[1])) == -1) {
      write(STDOUT_FILENO, WRG_JOB_MSG, strlen(WRG_JOB_MSG));
    }
    
    kill(-bg_pid, SIGCONT);
    tcsetpgrp(0, bg_pid);
    
    wait(&status);
    signal(SIGTTOU, SIG_IGN); // <Ctrl-D> ignore
    tcsetpgrp(0, getpid());
    signal(SIGTTOU, SIG_DFL); 

    return 0;
  }

  if (!strcmp(args[0], builtin_cmds[JOBS])) {
    int i;
    for (i = 0; i < stack_size(&bg_procs); ++i) {
      int j;
      get_nth(&bg_procs, &j, i);
      printf("[%d] PID: %d\n", i, j);
    }
    return 0;
  }

  return 1; /* cmd is not builtin */
}

int main(int argc, char **argv) {

  init_stack(&bg_procs);
  yay_loop();

  return 0;
}
