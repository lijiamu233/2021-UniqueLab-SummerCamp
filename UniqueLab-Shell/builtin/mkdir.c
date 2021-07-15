#include "../common.h"

void DestProg(char** argv,int num) {
    char path[100];
        strcpy(path,"./");
        strcat(path,argv[num]);
        strcpy(argv[num],path);
}

int makedir(int argc,char** argv) {
    
    if(argc==1) return 1;
    if(argv[1][0]!='.'&&argv[1][0]!='/') {
        DestProg(argv,1);
    }
        mkdir(argv[1],S_IRWXU);
}

int main(int argc,char** argv) {
    if(makedir(argc,argv)==1) printf("ERROR:参数过少！\n");
    return 0;
}