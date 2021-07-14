#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "too few argument\n");
        return 0;
    }
    for (int i = 1; i < argc; i++)
    {
        if (mkdir(argv[1], 0777) == -1)
            fprintf(stderr, "mkdir error: %s", strerror(errno));
    }
}