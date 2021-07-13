#include <fcntl.h>
#include <stdio.h>
int main(int argc,char * argv[]){
    int result=open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,0644);
    if(result==-1){
        printf("failed to create this file\n");
    }else{
        printf("successfully create %s\n",argv[1]);
    }
}