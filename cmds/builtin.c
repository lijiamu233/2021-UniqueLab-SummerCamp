#include "../common.h"

BuiltinCmd builtincmd[MAX_CMD_NUMBER];

int FindBuiltinCmd(char* cmd) {
    if(cmd[0]=='.'||cmd[0]=='/') return -1;
    if(strcmp(cmd,"pwd")==0) return 1;
    if(strcmp(cmd,"echo")==0) return 2;
    if(strcmp(cmd,"exit")==0) return 3;
    if(strcmp(cmd,"ls")==0) return 4;
    if(strcmp(cmd,"cd")==0) return 5;
    if(strcmp(cmd,"touch")==0) return 6;
    if(strcmp(cmd,"mkdir")==0) return 7;
}

int IfBuiltinCmd(char* cmd) {
    ;
}

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

int makedir(int argc,char** argv) {
    
    if(argc==1) return 1;
    if(argv[1][0]!='.'||argv[1][0]!='/') {
        char path[100];
        strcpy(path,"./");
        strcat(path,argv[1]);
        strcpy(argv[1],path);
    // if(argv==2) {
    //     mkdir(argv[1],00775);
    //     return 0;
    // }
    // int mod = atoi(argv[2]);
    mkdir(argv[1],S_IRWXU);
    }
}