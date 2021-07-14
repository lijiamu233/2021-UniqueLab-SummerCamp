#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    char path[100];
    DIR *dir;
    if (argc < 2)
    {
        fprintf(stderr, "rm: lack of arguments\n");
        return 0;
    }
    for (int i = 1; i < argc; i++)
    {
        memset(path, 0, sizeof(path));
        if (!getcwd(path, 100))
        {
            fprintf(stderr, "rm error : %s", strerror(errno));
            continue;
        }
        if (access(path, F_OK))
        {
            path[strlen(path)]='/';
            strcat(path,argv[i]);
            if(access(path,F_OK))
            {
                fprintf(stderr,"rm :%s doesen't exist\n",argv[i]);
                continue;
            }
            remove(path);
        }
    }
    return 0;
}