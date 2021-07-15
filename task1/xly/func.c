#include <stdio.h>
#include <string.h>
#include<stdlib.h>

void parseline(const char *cmdline, char **argv)
{
    static char array[250];
    char *buf = array;
    char *delim;
    int argc;

    strcpy(buf, cmdline);
    buf[strlen(buf)] = ' ';
    while (*buf && (*buf == ' '))
        buf++;

    argc = 0;
    delim = strchr(buf, ' ');

    while (delim)
    {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' '))
            buf++;
        delim = strchr(buf, ' ');
    }
    argv[argc] = NULL;
}

void init_mysh()
{
    FILE *init_fp = fopen("/home/zyz/project/xly/env", "r");
    char tmp[100];
    char *p;
    if (!init_fp)
    {
        fprintf(stderr, "Initialization failed\n");
        exit(0);
    }
    while (fscanf(init_fp, "%s", tmp) != -1)
    {
        p=strchr(tmp,'=');
        *p='\0';
        if(setenv(tmp,p+1,1))
            fprintf(stderr,"env error\n");
    }
}