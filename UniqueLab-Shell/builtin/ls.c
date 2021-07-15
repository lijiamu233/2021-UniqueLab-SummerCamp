#include "../common.h"

int ls(int argc,char** argv) {
    // printf("ls enter\n");
    // printf("%d\n",argc);
    // for(int i=0;i<argc;i++) printf("%s\n",argv[i]);      //--color=auto?
    DIR *dir;
    struct dirent *rent;
    struct stat fpstat;
    struct passwd *pwd;
    struct group *grp;
    struct tm *fatime,*fmtime;
    time_t ftime;
    char str[256];
    char res[256][50];
    static char *perm[]={"---","--x","-w-","-wx","r--","r-x","rw-","rwx"};
    dir = opendir(".");
    int num=0;
    if(argc==1) {
        while((rent = readdir(dir))) {
            strcpy(str,rent->d_name);
            if(*str=='.'||str==NULL) continue;
            strcpy(res[num++],str);
        }
        for(int i=0;i<num;i++) printf("%s\t",res[i]);
        printf("\n");
    } else if(argv[1][0]=='-') {
        if(strcmp(argv[1],"-a")==0) {
            while((rent = readdir(dir))) {
            strcpy(str,rent->d_name);
            if(str==NULL) continue;
            strcpy(res[num++],str);
        }
        for(int i=0;i<num;i++) printf("%s\t",res[i]);
        printf("\n");
        }
        if(strcmp(argv[1],"-l")==0) {
            unsigned int mask = 0700;
            while((rent = readdir(dir))) {
            strcpy(str,rent->d_name);
            if(str==NULL) continue;
            strcpy(res[num++],str);
        }
        for(int i=0;i<num;i++) {
            if(stat(res[i],&fpstat)!=-1) {
                mask = 0700;
                for(int k=3;k;--k) {
                    printf("%3s",perm[(fpstat.st_mode&mask)>>(k-1)*3]);   //ERROR!!!
                    mask>>=3;
                }
                pwd = getpwuid(fpstat.st_uid);
                grp = getgrgid(fpstat.st_gid);
                //ftime = localtime(fpstat.st_ctim.tv_sec);
                printf(" %d %s %s %5d ",fpstat.st_nlink,pwd->pw_name,grp->gr_name,fpstat.st_size);
                //printf("%d %d %d:%d ",fatime->tm_mon,fatime->tm_mday,fatime->tm_hour,fatime->tm_min);
                printf("%s",res[i]);
                printf("\n");
            }
        }
    }
    } else printf("ERROR:为定义的选项！\n");
    closedir(dir);
}

int main(int argc,char** argv) {
    ls(argc,argv);
    return 0;
}