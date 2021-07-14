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
char* makestring(char *buf){
    char *cp;
    buf[strlen(buf)]='\0';
    cp=malloc(strlen(buf));
    strcpy(cp,buf);
    return cp;
}
void parse(char *input,char *order,char *string){
        memset(order,0,sizeof(order));
        memset(input,0,sizeof(input));
        int j=0;
        for(int i=0;i<strlen(string)&&!isspace(string[i]);i++){
            input[i]=string[i];
            j=i;
        }
        j++;
        for(int i=0;i<strlen(string);i++){
            order[i]=string[++j];
        }
}
void parse_for_direction_and_pipeline(char *input,char *order,char *string){
        memset(order,0,sizeof(order));
        memset(input,0,sizeof(input));
        int j=0;
        for(int i=0;i<strlen(string)&&string[i+1]!='<'&&string[i+1]!='>'&&string[i+1]!='|';i++){
            input[i]=string[i];
            j=i;
        }
        j+=4;
        for(int i=0;j<strlen(string);i++){
            order[i]=string[j++];
        }
}
void parse_argv(char* arg[],char *input){
        char buf[5];
        memset(buf,0,sizeof(buf));
        int argc=0;
        int l=0;
        for(int i=0;input[i]!='\0';i++){
            if(isspace(input[i])){
                arg[argc]=makestring(buf);
                argc++;
                l=0;
                memset(buf,0,sizeof(buf));
                
            }else{
                buf[l]=input[i];
                l++;
            }
        }
        arg[argc]=makestring(buf);
}