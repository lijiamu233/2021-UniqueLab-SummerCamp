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
        if(*now1=='\0') {
            flag = 1;
            break;
        } else flag=0;
    }
    if(flag==0) argc++;
    int ret = CmdParser(argc,argv);
    while(argc--) if(argv[argc]) free(argv[argc]);
    return ret;
}

int CmdParser(int argc,char**argv) {
    int cmdnum;
    if(POR(argc,argv)==0) return 0;
    //for(int i=0;i<argc;i++) if(argv[i]!=NULL) printf("%s\n",argv[i]);
    //printf("%d\n",argc);
    // RedPip = IfRedPip(argc,argv);    //如果包含重定向
    cmdnum = FindBuiltinCmd(argv[0]);
    ExeCmd(cmdnum,argc,argv);
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

void DestProg(char** argv,int num) {
    char path[100];
        strcpy(path,"./");
        strcat(path,argv[num]);
        strcpy(argv[num],path);
}

int IfRedPip(int argc,char** argv) {
    for(int i=1;i<argc;i++) {
        if(strcmp(argv[i],"<")==0) return 1;
        if(strcmp(argv[i],">")==0) return 2;
        if(strcmp(argv[i],"|")==0) return 3;
    }
    return 0;
}

int ExeCmd(int cmdnum,int argc,char** argv) {
    pid_t PID;
    int status;
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
        case 8:
            cp(argc,argv);
            break;
        
        default:
        printf("Undefined command!\n");
            break;
        }
    }
    return 0;
}

int POR(int argc,char** argv) {
    int out=0,in=0,pip=0;
    for(int i=1;i<argc;i++) {
        if(strcmp(argv[i],">")==0) out = 1;
        if(strcmp(argv[i],"<")==0) in = 1;
        if(strcmp(argv[i],"|")==0) pip = 1;
    }
    if((out+in+pip)>0) {
        printf("ERROR:操作符过多！\n");
        return 0;
    } else if((out+in+pip)==0) return 1;
    if(out==1) {
        Rout(argc,argv);
    } else if(in==1) {
        Rin(argc,argv);
    }else if(pip==1) {
        Pip(argc,argv);
    }
    return 0;
}

int Rout(int argc,char** argv) {
    ;
}

int Rin(int argc,char** argv) {
    ;
}

int Pip(int argc,char** argv) {
    int fd[2];
    pid_t PID;
    PID = fork();
    if(PID==0) {
        execvp(argv[0],)
    }
}

// int SplitOptions(int argc,char** argv,int* optnum,char** opts) {
//     *optnum = 0;
//     int j,optnumb=0;
//     for(int i=1;i<argc;i++) {
//         if(argv[i][0]=='-') {
//             printf("ok\n");
//             j=1;
//             while(argv[i][j]!='\0') {
//                 //strcpy(opts[(*optnum)],*(argv+i)+1);
//                 printf("循环进入\n");
//                 strncpy(opts[optnumb],*(argv+i)+1,1);   //ERROR!!!
//                 printf("cpy complete\n");
//                 //opts[(*optnum)][0] = 'a';//argv[i][j++];
//                 opts[optnumb][1] = '\0';
//                 printf("\0addcomplete\n");
//                 optnumb++;
//                 //printf("%s",opts[*optnum]);
//             }
//             //strcpy(opts[(*optnum)++],)
//         } else {
//             strcpy(opts[(*optnum)++],argv[i]);
//         }
//     }
//     printf("exit for optnum:%d\n",*optnum);
// }