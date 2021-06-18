#ifndef __YET_ANOTHER_YASH_H
#define __YET_ANOTHER_YASH_H

/* TODO: Make it customizable */
#define PROMPT "> " 

#define LINE_LEN 256
#define CMD_LEN 32

#define EXIT_CMD "farewell"

#define BUILTIN_ERR_MSG "Built-in command error!\n"
#define BYE_MSG "Bye, bye, human!\n"
#define HELP_MSG "A couple of builtin commands are:\n" \
                 "\tcd - change directory\n" \
                 "\tjobs - list of the pids of all background jobs of current shell\n" \
                 "\tfg <idx> - continue a stopped background proccess, opt with index,\n" \
                 "\t           given by the `jobs` command\n" \
                 "\tfarewell - exit the shell\n" \
                 "\thelp - print this help message\n"
#define NO_CMD_MSG "No such command, human!\n"
#define WRG_JOB_MSG "Invalid job index!\n"

typedef enum cmd_t {
                    CD = 0,
                    JOBS,
                    FG,
                    FAREWELL,
                    HELP
} cmd_t;

extern const char *builtin_cmds[];

#endif /* __YET_ANOTHER_YASH_H */
