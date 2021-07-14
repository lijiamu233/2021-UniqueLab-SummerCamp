#include "../common.h"

int echo(int argc,char** argv) {
    if(argc>3) {
        if(argv[2][0]=='>') {   //有重定向

        }
    }
    for(int i=1;i<argc;i++) {
        printf("%s ",argv[i]);
    } 
    if(argc!=1) printf("\n");
}