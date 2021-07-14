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
    //printf("Enter loop\n");
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
    argv[argc]=NULL;
    int ret = CmdParser(argc,argv);
    while(argc--) if(argv[argc]) free(argv[argc]);
    return ret;
}

int CmdParser(int argc,char**argv) {
    int cmdnum;
    if(POR(argc,argv)==0) return 0;
    
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
    //printf("IfRedPip\n");
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
        //PID = fork();
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
        case 9:
            rm(argc,argv);
            break;
        
        default:
        printf("Undefined command!\n");
            break;
        }
    }
    return 0;
}

int POR(int argc,char** argv) {
    int out=0,in=0,pip=0,i=1;
    //printf("进入POR\n");
    for(i=1;i<argc;i++) {
        //printf("for\n");
        if(strcmp(argv[i],">")==0) out += 1;
        if(strcmp(argv[i],"<")==0) in += 1;
        if(strcmp(argv[i],"|")==0) pip += 1;
    }
    if((out+in+pip)>1) {
        printf("ERROR:操作符过多！\n");
        return 0;
    }
    if((out+in+pip)!=1) return 1;
    if(out==1) {
        Rout(argc,argv);
    }
    if(in==1) {
        Rin(argc,argv);
    }
    if(pip==1) {
        fflush(stdout);
        Pip(argc,argv);
    }
    
    return 0;
}

int Rout(int argc,char** argv) {
    pid_t PID;
    int status,i;
    char** now1,**now2;
    now1=argv;
    for(i=0;(i<argc)&&(strcmp(argv[i],">")!=0);i++) {
        ;
    }
    argv[i]=NULL;
    now2 = argv+i+1;
    now1 = argv;
    // printf("i=%d\t%s$$",i,*now2);
    argv[argc]=NULL;
    PID = fork();
    switch (PID)
    {
    case -1:
        printf("ERROR:子进程创建失败！\n");
        break;
    case 0:
        {int fd=0;
        if(now2[0][0]!='.'&&now2[0][0]!='/') {
            char path[50];
            strcpy(path,"./");
            strcat(path,now2[0]);
            strcpy(now2[0],path);
            //printf("path prod complete");
        }
        fd = open(now2[0],O_WRONLY|O_CREAT|O_TRUNC,0664);
        if(fd==-1) {
            printf("ERROR:新建文件失败！\n");
        }
        // printf("path open complete");
        dup2(fd,STDOUT_FILENO);
        if(execvp(now1[0],now1)==-1) printf("ERROR:命令执行失败！\n");
        close(fd);
        break;
    }
    default: {
        waitpid(PID,&status,0);
    }
        break;
    }
}

int Rin(int argc,char** argv) {
    pid_t PID;
    int status,i;
    char** now1,**now2;
    now1=argv;
    for(i=0;(i<argc)&&(strcmp(argv[i],"<")!=0);i++) {
        ;
    }
    argv[i]=NULL;
    now2 = argv+i+1;
    now1 = argv;

    // printf("i=%d\t$$%s$$\n",i,*now2);
    argv[argc]=NULL;
    PID = fork();
    switch (PID)
    {
    case -1:
        printf("ERROR:子进程创建失败！\n");
        break;
    case 0:
        {int fd=0;
        // if(now2[0][0]!='.'&&now2[0][0]!='/') {
        //     char path[50];
        //     strcpy(path,"./");
        //     strcat(path,now2[0]);
        //     strcpy(now2[0],path);
        //     // printf("path prod complete");
        // }
        printf("i=%d\t$$%s$$\n",i,*now2);
        fd = open(now2[0],O_RDONLY);
        if(fd=-1) {
            printf("ERROR:找不到文件！\n");
            return 1;
        }
        printf("path open complete");
        dup2(fd,STDIN_FILENO);
        if(execvp(now1[0],now1)==-1) printf("ERROR:命令执行失败！\n");
        close(fd);
        break;
    }
    default: {
        waitpid(PID,&status,0);
    }
        break;
    }
}

int Pip(int argc,char** argv) {
    int fd[2],status,i;
    pid_t PID;
    //printf("进入Pip\n");
    //char argvpip1[MAX_ARGV_LENGTH][20];
    //char argvpip2[MAX_ARGV_LENGTH][20];
    char** now1,**now2;
    now1=argv;
    for(i=0;(i<argc)&&(strcmp(argv[i],"|")!=0);i++) {
        ;
    }
    argv[i]=NULL;
    now2 = argv+i+1;
    now1 = argv;
    argv[argc]=NULL;
    //printf("now1=%s\n",now1[0]);

    PID = fork();
    if(PID==-1) {
        printf("ERROR:进程建立失败！\n");
        return 1;
    }
    if(PID==0) {
        pid_t CPID;
        int ret = pipe(fd);
        CPID = fork();
        if(CPID==-1) return 1;
        if(CPID==0) {
            close(fd[0]);       //关闭读
            //printf("now1=%s",now1[0]);
            dup2(fd[1],STDOUT_FILENO);
            if(execvp(now1[0],now1)==-1) {
                fflush(stdout);
                printf("ERROR:命令1执行失败！\n");
                exit(1);
            }
        } else if(CPID>0) {
            waitpid(CPID,&status,0);
            close(fd[1]);
            dup2(fd[0],STDIN_FILENO);
            //fflush(stdout);
            //printf("now2=%s\n",now2[0]);
            if(execvp(now2[0],now2)==-1) {
                printf("ERROR:命令2执行失败！\n");
                exit(1);
            }
        }
        //execvp(argv[0],)
    } else if(PID>0) {
        int status1;
        waitpid(PID,&status1,0);
    }
}

// int SplitForPOR(int argc,char** argv,char** argvPOR1,char** argvPOR2) {
//     int i,j;
//     fflush(stdout);
//     printf("1");
//     for(i=0;(i<argc)&&(argv[i][0]!='|')&&(argv[i][0]!='<')&&(argv[i][0]!='>');i++) {
//         strcpy(argvPOR1[i],argv[i]);
//     }
//     argvPOR1[i+1]=NULL;
//     i++;
//     printf("2");
//     for(j=0;j<argc&&i<argc;j++) {
//         strcpy(argvPOR2[j],argv[i++]);
//     }
//     printf("3");
//     argvPOR2[j+1]=NULL;
// }

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