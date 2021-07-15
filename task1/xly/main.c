#include "builtin.h"
#include "func.h"

char *argv[MAXLENGTH];
char tmp[100];
int redirec[2] = {-1, -1};
int exit_status = 0;
int child_status = 0;
int redirec_status = 0;     //重定向状态检测:0,   1:<,   2:>,    3:|
int fd = 0;                 //used for redirection
pid_t fgpid = 0, repid = 0; //foreground program pid
pid_t pid;
FILE *fp;

int main()
{
    int status;
    char cmdline[100];
    char *p_cmdline = cmdline;

    init_mysh();
    signal(SIGINT, sigint_handler);
    signal(SIGCHLD, sigchld_handler);

    while (1)
    {
        for (int i = 0; i < MAXLENGTH; i++)
        {
            argv[i] = NULL;
        }
        p_cmdline = readline("mysh> ");
        if (p_cmdline == NULL)
            exit(0);
        parseline(p_cmdline, argv);
        if (!argv[0])
            continue;

        for (int i = 0; argv[i]; i++)
        {
            if (!strcmp(argv[i], "<"))
                redirec_status = 1;
            if (!strcmp(argv[i], ">"))
                redirec_status = 2;
            if (!strcmp(argv[i], "|"))
                redirec_status = 3;
        }
        if (redirec_status == 1 || redirec_status == 2)
        {
            status = io_redirec(argv, redirec_status);
            if (status == 1)
                continue; //1：命令错误或者是父进程，且错误已经处理
            else if (status)
            {
                wait(NULL);
                redirec_status = 0;
                continue;
            }
            else
            {
                execute(argv);
                exit(0);
            }
        }
        else if (redirec_status == 3)
        {
            status = pipe_execute(argv);
            redirec_status = 0;
            redirec[0] = redirec[1] = -1;
            continue;
        }

        execute(argv);
    }

    return 0;
}

int builtin(char **argv)
{
    if (!argv[0])
        return 0;
    if (!strcmp(argv[0], "exit"))
        exit(exit_status);
    if (!strcmp(argv[0], "cd"))
    {
        cd_z(argv);
        return 1;
    }
    if (!strcmp(argv[0], "pwd"))
    {
        pwd_z(argv);
        return 1;
    }
    if (!strcmp(argv[0], "echo"))
    {
        echo_z(argv);
        return 1;
    }
    return 0;
}

void execute(char **argv)
{
    if (!builtin(argv))
    {
        if ((pid = fork()) == 0)
            if (execvp(argv[0], argv) < 0)
                fprintf(stderr, "%s: Command not found\n", argv[0]);

        if (pid < 0)
            return;
        else
            fgpid = pid;
        while (fgpid)
            ;
    }

    redirec_status = 0;
    if (fd)
    {
        close(fd);
        fd = 0;
    }
    fflush(stdout);
}

void sigint_handler(int sig)
{
    if (fgpid != 0)
    {
        kill(-fgpid, SIGINT);
    }
}

void sigchld_handler(int sig)
{
    pid_t pid;
    if (wait(&child_status))
        fgpid = 0;
}

int io_redirec(char **argv, int mode)
{
    char path[100];
    for (int i = 0; argv[i]; i++)
        if (!strcmp("<", argv[i]) || !strcmp(">", argv[i]))
        {
            if (!argv[i + 1])
            {
                fprintf(stderr, "too few argument\n");
                return 1;
            }
            strcpy(path, argv[i + 1]);
            for (int j = i; argv[j + 1]; j++)
                argv[j] = argv[j + 2];
        }
    fd = open(path, O_RDWR | O_CREAT, 0644);
    if (fd == -1 || mode == 1)
    {
        unix_error("openfile");
        return 1;
    }
    else if (fd == -1 || mode == 2)
    {
        fp = fopen(path, "w");
        fclose(fp);
        fd = open(path, O_RDWR | O_CREAT, 0644);
    }
    if ((repid = fork()) == 0)
    {
        if (mode == 1)
            dup2(fd, STDIN_FILENO);
        if (mode == 2)
            dup2(fd, STDOUT_FILENO);
        return 0;
    }
    return repid;
}

int pipe_execute(char **argv)
{
    int div; //将指令进行划分
    int pid1, pid2;
    for (div = 0; argv[div]; div++)
        if (!strcmp("|", argv[div]))
        {
            argv[div] = NULL;
            break;
        }
    pipe(redirec);
    if ((pid1 = fork()) == 0)
    {
        dup2(redirec[1], STDOUT_FILENO);
        execute(argv);
        exit(0);
    }
    if ((pid2 = fork()) == 0)
    {
        dup2(redirec[0], STDIN_FILENO);
        execute(argv + div + 1);
        exit(0);
    }
    waitpid(pid1, NULL, WNOHANG | WUNTRACED);
    waitpid(pid2, NULL, 0);
    return 0;
}