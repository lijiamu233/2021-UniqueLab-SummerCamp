#include <sys/stat.h>
#include <stdio.h>
int main(int argc,char* argv[]){
    int result=mkdir(argv[1],S_IRWXU|S_IRWXG|S_IRWXO);
    if(result==-1){
        printf("failed to create this directory\n");
    }else{
        printf("successfully create directory %s\n",argv[1]);
    }
}