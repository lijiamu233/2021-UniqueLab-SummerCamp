#include "../common.h"

BuiltinCmd builtincmd[MAX_CMD_NUMBER];
extern USERINFO user;

int FindBuiltinCmd(char* cmd) {
    if(cmd[0]=='.'||cmd[0]=='/') return -1;
    if(strcmp(cmd,"pwd")==0) return 1;
    //if(strcmp(cmd,"echo")==0) return 2;
    if(strcmp(cmd,"exit")==0) return 3;
    //if(strcmp(cmd,"ls")==0) return 4;
    if(strcmp(cmd,"cd")==0) return 5;
    if(strcmp(cmd,"touch")==0) return 6;
    if(strcmp(cmd,"mkdir")==0) return 7;
    if(strcmp(cmd,"mkdir")==0) return 8;
    if(strcmp(cmd,"rm")==0) return 9;
    else return 0;
}

// int IfBuiltinCmd(char* cmd) {
//     ;
// }

int pwd(int argc,char** argv) {
    printf("%s\n",user.pwd);
}

int echo(int argc,char** argv) {
    if(argc>3) {
        if(argv[1][0]=='$') {   //环境变量
            char*path;
            path = getenv("PATH");
            printf("%s",path);
            return 0;
        }
    }
    for(int i=1;i<argc;i++) {
        printf("%s ",argv[i]);
    } 
    if(argc!=1) printf("\n");
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

int cd(int argc,char** argv) {
    chdir(argv[1]);
}