#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>

void execute_cp(char *src, char *dst)
{
    int fd_src, fd_dst,l;
    char ch;
    char buf[100];
    fd_src = open(src, O_RDONLY);
    if (fd_src == -1)
    {
        fprintf(stderr, "cp error: %s", strerror(errno));
        return;
    }
    if (!access(dst, F_OK))
        remove(dst);
    fd_dst = open(dst, O_WRONLY | O_CREAT, 0644);
    if (fd_dst == -1)
    {
        fprintf(stderr, "cp error: %s\n", strerror(errno));
        return;
    }
    while(l=read(fd_src,buf,100)>0) write(fd_dst,buf,l);
    close(fd_dst);
    close(fd_src);
}

void main(int argc, char **argv)
{
    char dst[100];
    char *p, *ans;
    if (argc < 3)
        fprintf(stderr, "cp : too few argument\n");
    DIR *dir_des;
    if ((dir_des = opendir(argv[argc - 1])) == NULL)
    {
        fprintf(stderr, "%s: No such a file or directory\n", argv[argc - 1]);
        return;
    }
    else
        closedir(dir_des);
    for (int i = 1; i < argc - 1; i++)
    {
        memset(dst, 0, sizeof(dst));
        strcpy(dst, argv[argc - 1]);
        if (dst[strlen(dst) - 1] != '/')
            dst[strlen(dst)] = '/';
        p = argv[i] + 1;
        while (p = strchr(p + 1, '/'))
            ans = p;
        strcat(dst, ans + 1); ///以上部分合成拷贝终点名称
        execute_cp(argv[i], dst);
    }
}