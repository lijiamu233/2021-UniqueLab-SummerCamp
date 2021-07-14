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
#include "built_in.h"
void cd(char order[]){
            int result=chdir(order);
            if(result==-1){
                printf("the path is wrong\n");
            }
}

void pwd(void){
            char show[100];
            getcwd(show,sizeof(show));
            printf("%s\n",show);
}
void export_(char order[]){
    char location[40];
    if(strstr(order,"PATH=$PATH:")==NULL){
        printf("the format is wrong\n");
        return;
    }
    int j=0;
    for(int i=11;order[i]!='\0';i++){
        location[j]=order[i];
        j++;
    }
    location[j]='\0';
    int result=setenv("PATH",location,0);
    if(result==-1){
        printf("failed to set environment variable\n");
    }if(result==0){
        printf("succeed to set environment variable\n");
    }
}
void echo(char order[]){
            if(order[0]=='"'||order[0]=='\''){
                for(int i=1;i<strlen(order)-1;i++)
                    printf("%c",order[i]);
                printf("\n");
            }else if (strcmp(order,"`date`")==0)
            {
                time_t now;
                struct tm *time_now;
                char *datetime;
                time(&now);
                time_now=localtime(&now);
                datetime=asctime(time_now);
                printf("%s",datetime);
            }
                else{
                for(int i=0;i<strlen(order);i++)
                    printf("%c",order[i]);
                printf("\n");
            }
}