#include "../common.h"

int ls(int argc,char** argv) {
    DIR *dir;
    struct dirent *rent;
    struct stat fpstat;
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
                for(int k=3;k;--k) {
                    printf("%3s",perm[(fpstat.st_mode&mask)>>(k-1)*3]);   //ERROR!!!
                    mask>>=3;
                }
                printf("\n");
            }
        }
    }
    } 
    closedir(dir);
}