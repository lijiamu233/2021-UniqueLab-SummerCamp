#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "external.h"
void ls(){
    pid_t pid;
    switch (pid=fork())
    {
        case -1:{
            printf("failed to create a new process\n");
            break;
        }
        case 0:{
            char* argv[]={"ls",NULL};
            int j=0;
            j=execvp(argv[0],argv);
            if(j==-1){
            printf("fail to execvp");
            exit(1);
            }
            break;
        }
        default:{
            int status;
            waitpid(pid,&status,0);
            int err=WEXITSTATUS(status);
            if(err){
                printf("%s\n",strerror(err));
            }
            break;
        }
    }
}
void touch(char* order){
    pid_t pid;
    switch (pid=fork())
    {
        case -1:{
            printf("failed to create a new process\n");
            break;
        }
        case 0:{
            char* argv[]={"touch",order,NULL};
            int j=0;
            j=execvp(argv[0],argv);
            if(j==-1){
            printf("fail to execvp");
            exit(1);
            }
            break;
        }
        default:{
            int status;
            waitpid(pid,&status,0);
            int err=WEXITSTATUS(status);
            if(err){
                printf("%s\n",strerror(err));
            }
            break;
        }
    }
}
void mkdirectory(char* order){
    pid_t pid;
    switch (pid=fork())
    {
        case -1:{
            printf("failed to create a new process\n");
            break;
        }
        case 0:{
            char* argv[]={"mkdir",order,NULL};
            int j=0;
            j=execvp(argv[0],argv);
            if(j==-1){
            printf("fail to execvp");
            exit(1);
            }
            break;
        }
        default:{
            int status;
            waitpid(pid,&status,0);
            int err=WEXITSTATUS(status);
            if(err){
                printf("%s\n",strerror(err));
            }
            break;
        }
    }
}