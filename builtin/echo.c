#include "../common.h"

int echo(int argc,char** argv) {
    for(int i=1;i<argc;i++) {
        printf("%s ",argv[i]);
    } 
    if(argc!=1) printf("\n");
}