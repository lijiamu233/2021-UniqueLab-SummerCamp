#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "builtin.h"
#include "func.h"

char *argv[MAXLENGTH];
int exit_status = 0;
int child_status = 0;
pid_t fgpid = 0; //foreground program pid

extern char **environ;

int main()
{
    char cmdline[100];
    char tmp[100];
    char *p_cmdline = cmdline;
    pid_t pid;

    signal(SIGINT, sigint_handler);
    signal(SIGCHLD, sigchld_handler);

    while (1)
    {
        for (int i = 0; i < MAXLENGTH; i++)
        {
            argv[i] = NULL;
        }
        p_cmdline = readline("mysh> ");
        parseline(p_cmdline, argv);
        if(!argv[0]) continue;

        if (!builtin(argv))
        {
            if ((pid = fork()) == 0)
            {
                sprintf(tmp, "../bin/%s", argv[0]);
                if (execve(tmp, argv, environ) < 0)
                    fprintf(stderr, "%s: Command not found\n", argv[0]);
            }
            if (pid < 0)
                continue;
            else
                fgpid = pid;
            while (fgpid) ;
        }
        fflush(stdout);
    }

    return 0;
}

int builtin(char **argv)
{
    if (!argv[0])
        return 0;
    if (!strcmp(argv[0], "exit"))
        exit(exit_status);
    if (!strcmp(argv[0], "cd"))
    {
        cd_z(argv);
        return 1;
    }
    if (!strcmp(argv[0], "pwd"))
    {
        pwd_z(argv);
        return 1;
    }
    if (!strcmp(argv[0], "echo"))
    {
        echo_z(argv);
        return 1;
    }
    return 0;
}

void sigint_handler(int sig)
{
    if (fgpid != 0)
    {
        kill(-fgpid, SIGINT);
    }
}

void sigchld_handler(int sig)
{
    pid_t pid;
    if (wait(&child_status))
        fgpid = 0;
}