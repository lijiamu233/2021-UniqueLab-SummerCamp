#include "../common.h"

void DestProg(char** argv,int num) {
    char path[100];
        strcpy(path,"./");
        strcat(path,argv[num]);
        strcpy(argv[num],path);
}

int rm(int argc,char** argv) {
    struct stat fpstat;
    if(argv[1][0]!='.'&&argv[1][0]!='/') {
        DestProg(argv,1);
    }
    if(access(argv[1],F_OK)==-1) {
        printf("ERROR:rm找不到文件！\n");
        return 1;
    }
    stat(argv[1],&fpstat);
    if(__S_IFDIR&fpstat.st_mode) {
        printf("ERROR:不能删除文件夹！\n");
        return 1;
    }
    remove(argv[1]);
    return 0;
}

int main(int argc,char** argv) {
    rm(argc,argv);
    return 0;
}