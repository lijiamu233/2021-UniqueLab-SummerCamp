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
#include <signal.h>
#include "built_in.h"
#include "direction_and_pipe.h"
#include "parse.h"
#include "external.h"
typedef void (* signal_handler)(int);
int pipeline_or_direction(char *string){
        int input_direction=0;
        int output_dirction=0;
        int pipeline=0;
        char input[20];
        char order[20];
        for(int i=0;i<strlen(string);i++){
            if(isspace(string[i])&&string[i+1]=='<'&&isspace(string[i+2])){
                input_direction++;
            }
            if(isspace(string[i])&&string[i+1]=='>'&&isspace(string[i+2])){
                output_dirction++;
            }
            if(isspace(string[i])&&string[i+1]=='|'&&isspace(string[i+2])){
                pipeline++;
            }
        }
        if((input_direction+output_dirction+pipeline)>=2){
            printf("too many >|<\n");
            return 0;
        }else if((input_direction+output_dirction+pipeline)==0){
            return 1;
        }else{
            parse_for_direction_and_pipeline(input,order,string);
            if(input_direction==1){

                input_direct(input,order);
            }else if(pipeline==1)
            {

                pip(input,order);
            }else if (output_dirction==1)
            {

                output_direct(input,order);
            }
            return 0;
        }
}
void command(char input[],char order[],char *string){
        int t=pipeline_or_direction(string);
        if(t==0){
            return ;
        }
        if(strcmp(input,"exit")==0){
            exit(0);

        }else if (strcmp(input,"pwd")==0){
            pwd();

        }else if (strcmp(input,"echo")==0){
            echo(order);

        }else if(strcmp(input,"cd")==0){
            cd(order);
        }else if(strcmp(input,"export")==0){
            export_(order);
        }else if(strcmp(input,"ls")==0){
            ls();
        }else if(strcmp(input,"touch")==0){
            touch(order);
        }else if(strcmp(input,"mkdir")==0){
            mkdirectory(order);
        }else if(strcmp(input,"cp")==0){

        }
}
void loop(){
    while (1)
    {
        char *string;
        string=readline("[new shell]");
        if(string==NULL)
            exit(0);
        char input[20];
        char order[20];
        parse(input,order,string);
        command(input,order,string);
    }
}
void ctrl_c(){
    //printf("\n");
    return;
}

int main(){
    signal(SIGINT,ctrl_c);
    loop();
}
