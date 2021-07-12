#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>

#include "./cmds/builtin.h"
#include "./cmds/cmd.h"

#define MAX_CMD_NUMBER 50
#define bool int

typedef struct ChildPID
{
    pid_t PID;
    bool status;
}ChildPID;
//typedef int HANDLE_SIG_FNC_TYPE();


void HandleCtrlC(int sig);
void HandleCtrlD(int sig);

#endif