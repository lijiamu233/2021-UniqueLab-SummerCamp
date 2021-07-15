#include"../common.h"

int touch(int argc,char** argv) {
    int flag = access(argv[1],F_OK);
    if(flag==0) {
        printf("The file is already exsist!\n");
        return 1;
    } else {
        FILE *fp;
        if(argv[1][0]!='/'||argv[1][0]!='.') {
            char path[50];
            strcpy(path,"./");
            strcat(path,argv[1]);
            strcpy(argv[1],path);
        }
        fp = fopen(argv[1],"w+");
        if(fp==NULL) {
            printf("Failed to touch\n");
            return 1;
        }
        fclose(fp);
        return 0;
    }
}

int main(int argc,char** argv) {
    touch(argc,argv);
    return 0;
}