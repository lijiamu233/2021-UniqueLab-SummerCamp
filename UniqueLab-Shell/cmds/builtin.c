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
    if(strcmp(cmd,"mkdir")==0) return 8;
    else return 0;
}

// int IfBuiltinCmd(char* cmd) {
//     ;
// }

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
    if(argv[1][0]!='.'&&argv[1][0]!='/') {
        DestProg(argv,1);
    }
        mkdir(argv[1],S_IRWXU);
}

int cp(int argc,char** argv) {
    int status;     //判断地址对象：1为文件夹，0为文件
    struct stat st;
    if(argc!=3) {
        printf("参数输入有误！");
        return 1;
    }
    FILE *fpdes,*fpsrc;
    if(argv[1][0]!='.'&&argv[1][0]!='/') {
        DestProg(argv,1);
    }
    if(argv[2][0]!='.'&&argv[2][0]!='/') {
        DestProg(argv,2);
    }
    if(access(argv[2],F_OK)==0) {
        printf(
            "ERROR:找不到源文件！\n");
        return 1;
    }
    fpdes = fopen(argv[2],"w+");
    fpsrc = fopen(argv[1],"r");
    if(fpsrc==NULL) {
        printf("ERROR:文件打开失败！\n");
        return 1;
    }
    
}