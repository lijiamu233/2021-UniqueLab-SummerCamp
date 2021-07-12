#include "../common.h"

int ls(int argc,char** argv) {
    DIR *dir;
    struct dirent *rent;
    char str[256];
    char res[256][50];
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
    } else {
        if(strcmp(argv[1],"-a")==0) {
            while((rent = readdir(dir))) {
            strcpy(str,rent->d_name);
            if(str==NULL) continue;
            strcpy(res[num++],str);
        }
        for(int i=0;i<num;i++) printf("%s\t",res[i]);
        printf("\n");
        }
        
    }
    closedir(dir);
}