#include <stdio.h>
#include <string.h>

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