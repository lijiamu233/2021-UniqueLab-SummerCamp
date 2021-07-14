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
#include "parse.h"
#include "direction_and_pipe.h"
void input_direct(char input[],char order[]){
    char* argv[5];
    for(int i=0;i<5;i++){
        argv[i]=NULL;
    }
    parse_argv(argv,input);
    pid_t pid;
    switch (pid=fork())
    {
    case -1:{
        printf("failed to create a new process\n");
        break;
    }
    case 0:{
        int fd;
        fd=open(order,O_RDONLY,0644);
        dup2(fd,STDIN_FILENO);
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
void output_direct(char input[],char order[]){
    char* arg[5];
    for(int i=0;i<5;i++){
        arg[i]=NULL;
    }
    parse_argv(arg,input);
    pid_t pid;
    pid=fork();
    switch (pid)
    {
        case -1:{
            printf("failed to create a new process\n");
            break;
        }
        case 0:{
            int fd=0;
            fd=open(order,O_WRONLY|O_CREAT|O_TRUNC,0644);
            dup2(fd,STDOUT_FILENO);
            int j=0;
             j=execvp(arg[0],arg);
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
void pip(char input[],char order[]){
    char *argv_first[5];
    char *argv_second[5];
    for(int i=0;i<5;i++){
        argv_first[i]=NULL;
    }
    for(int i=0;i<5;i++){
        argv_second[i]=NULL;
    }
    parse_argv(argv_first,input);
    parse_argv(argv_second,order);
    pid_t pid;
    switch (pid=fork())
    {
        case -1:{
            printf("failed to create a new process\n");
            break;
        }
        case 0:{
            int pip[2];
            if(pipe(pip)<0)
                perror("fail to build pipe");
            pid_t child_pid;
            switch (child_pid=fork())
            {
                case -1:{
                    printf("failed to create a new process\n");
                    break;
                }
                case 0:{
                    close(pip[0]);
                    dup2(pip[1],STDOUT_FILENO);
                    int j=0;
                    j=execvp(argv_first[0],argv_first);
                    if(j==-1){
                        printf("fail to execvp");
                        exit(1);
                    }
                    break;
                }           
                default:{
                    int status;
                    waitpid(child_pid,&status,0);
                    int err=WEXITSTATUS(status);
                    if(err){
                        printf("%s\n",strerror(err));
                    }
                    close(pip[1]);
                    dup2(pip[0],STDIN_FILENO);
                    int j=0;
                    j=execvp(argv_second[0],argv_second);
                    if(j==-1){
                        printf("fail to execvp");
                        exit(1);
                    }
                    break;           
                }
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