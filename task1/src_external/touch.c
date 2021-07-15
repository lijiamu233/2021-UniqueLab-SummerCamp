#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, char **argv)
{
    DIR *dir;
    FILE *fp;
    if (argc < 2)
    {
        fprintf(stderr, "too few argument\n");
        return 0;
    }
    for (int i = 1; i < argc; i++)
    {
        dir = opendir(argv[i]);
        if (dir == NULL)
        {
            fp = fopen(argv[i], "a");
            if (fp == NULL)
                fprintf(stderr, "touch error: %s", strerror(errno));
            else fclose(fp);
        }
        else closedir(dir);
    }
    return 0;
}