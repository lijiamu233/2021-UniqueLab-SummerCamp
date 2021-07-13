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
                //buf[l]='\0';
                arg[argc]=makestring(buf);
                argc++;
                l=0;
                memset(buf,0,sizeof(buf));
                
            }else{
                buf[l]=input[i];
                l++;
            }
        }
        //buf[l]='\0';
        arg[argc]=makestring(buf);
       // for(int i=0;i<argc+1;i++){
        //printf("%s\n",arg[i]);
    //}
}
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
        //printf("the order is wrong\n");
       
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
            //dup2(STDOUT_FILENO,fd);
            //printf("the order is wrong\n");
            
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
                 //printf("%s < %s\n",input,order);
                input_direct(input,order);  
            }else if(pipeline==1)
            {
                //printf("%s | %s\n",input,order);
                pip(input,order);
            }else if (output_dirction==1)
            {
                //printf("%s > %s\n",input,order);
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
            
        }
}



int main(){
    while (1)
    {
        char *string;
        string=readline("[new shell]");
        char input[20];
        
        char order[20];
        
        parse(input,order,string);
        command(input,order,string);
    }
}