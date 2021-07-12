#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int long_flag = 0; //"-l" option flag

int main(int argc, char **argv)
{
    struct dirent *entry;
    DIR *dir;
    char path[100];
    if (argc == 1)
    {
        argv[1] = getcwd(path, 100);
        argc++;
    }
    if (!strcmp(argv[1], "-l"))
        long_flag = 1;
    for (int i = 1 + long_flag; i < argc; i++)
    {
        if ((dir = opendir(argv[i])) == NULL)
        {
            fprintf(stderr, "opendir error: %s", strerror(errno));
            continue;
        }
        printf("%s:\n", argv[i]);
        while (entry = readdir(dir))
        {
            if (!strncmp(".", entry->d_name, 1))
                continue;
            printf("%s  ", entry->d_name);
        }
        closedir(dir);
        putchar('\n');
        fflush(stdout);
    }
    return 0;
}