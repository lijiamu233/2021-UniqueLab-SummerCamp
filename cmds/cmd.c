#include "../common.h"

ChildPID CPID;
//分割输入命令
int SplitCmdInput(char* line) {
    int argc = 0,flag;
    char *argv[MAX_ARGV_LENGTH] = {0};
    char *now1 = line;
    char *now0 = line;
    if(line==NULL) exit(0);
    if(strlen(line)==0) return 1;
    while(*now1==' ') now1++;
    if(*now1=='\0') return 1;
    while(*now1!='\0') {
        while(*now1==' '&&*now1!='\0') now1++;
        now0 = now1;
        if(*now1=='\0') {
            flag = 1;
            break;
        } else flag = 0;
        while(*now1!=' ' && *now1!='\0') {
            now1++;
        }
        argv[argc] = malloc(now1-now0+1);
        strncpy(argv[argc],now0,now1-now0);
        argv[argc][now1-now0] = '\0';
        if(*now1=='\0') break;
        now1++;
        argc++;
    }
    if(flag==0) argc++;
    int ret = CmdParser(argc,argv);
    while(argc--) if(argv[argc]) free(argv[argc]);
    return ret;
}

int CmdParser(int argc,char**argv) {
    int cmdnum,status;
    pid_t PID;
    for(int i=0;i<argc;i++) if(argv[i]!=NULL) printf("%s\n",argv[i]);
    printf("%d\n",argc);
    cmdnum = FindBuiltinCmd(argv[0]);
    if(cmdnum==-1) {
        PID = fork();
        if(PID > 0) {
                CPID.PID = PID;
                CPID.status = 1;
                wait(&status);
                CPID.status = 0;
            }
        if(PID==0) {
            if(execve(argv[0],argv,__environ)<0) {
                printf("AN ERROR OCCURED\n");
                exit(0);
            }
        }
    } else {
        switch (cmdnum)
        {
        case 1:
            pwd(argc,argv);
            break;
        case 2:
            echo(argc,argv);
            break;
        case 3:
            exit(0);
            break;
        case 4:
            ls(argc,argv);
            break;
        case 5:
            cd(argc,argv);
            break;
        case 6:
            touch(argc,argv);
            break;
        case 7:
            makedir(argc,argv);
            break;
        
        default:
        printf("Undefined command!\n");
            break;
        }
    }
    return 0;
}

int SetPS1(char* PS1,USERINFO* user) {
    strcpy(PS1,"\0");
    getcwd(user->pwd,MAX_PWD_LENGTH);
    user->usn = getlogin();
    strcpy(PS1,user->usn);
    strcat(PS1,"@wbjshell:");
    //strcat(PS1,user->pwd);
    strcat(PS1,"$ ");
}