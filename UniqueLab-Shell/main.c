#include "./common.h"

#define MAX_COMMAND_LENGTH 100

extern ChildPID CPID;
USERINFO user;
char PS1[MAX_PS1_LENGTH];

int init(){
    printf("Shell init complete!\nPress Ctrl-D to exit\n");
    return 0;
}


int main(int argc,char *argv[]) {
    char *line;

    fflush(stdout);

    //HANDLE_SIG_FNC_TYPE *CtrlC = &HandleCtrlC,CtrlD = &HandleCtrlD;
    signal(SIGINT,HandleCtrlC);


    if(init()) {
        return 1;
    }

    while(1) {
        SetPS1(PS1,&user);
        line = readline(PS1);
        SplitCmdInput(line);
        free(line);
    }
}

void HandleCtrlC(int sig) {
    if(CPID.status==0) {
        printf("\n");
        printf("不是子程序\n");
        printf("%s",PS1);
    } else {
        kill(CPID.PID,SIGINT);
        CPID.status==0;
        printf("\n");
        //printf("%s",PS1);
    }
    return ;
}