#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void unix_error(char *msg)
{
    fprintf(stderr, "%s error:%s\n", msg, strerror(errno));
}

//builtin----cd
void cd_z(char **argv)
{
    if(!argv[1] || !strcmp(argv[1],"~")) argv[1]="/home/zyz";
    if (argv[2])
    {
        printf("too many argument\n");
        return;
    }
    if (chdir(argv[1]) == -1)
        unix_error("cd");
    fflush(stdout);
}

//builtin----pwd(�����ÿ��Ƕ������������)
void pwd_z(char **argv)
{
    char path[100];
    if (getcwd(path, 100) == NULL)
    {
        unix_error("pwd");
        fflush(stdout);
        return;
    }
    printf("%s\n", path);
    fflush(stdout);
}

//builtin----echo

void echo_z(char **argv)
{
    for(int i=1;argv[i]!=NULL;i++)
    {
        if(!strcmp(">",argv[i]) || !strcmp("<",argv[i]))
        {
            i++;
            continue;
        }
        printf("%s ",argv[i]);
    }
    putchar('\n');
    fflush(stdout);
}