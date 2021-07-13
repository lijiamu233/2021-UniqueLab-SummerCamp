#include <stdio.h>
#include <dirent.h>
#include <string.h>
void show_ls(char filename[]){
   DIR* dir_ptr;
   struct dirent* dirent_ptr;
   
   if((dir_ptr=opendir(filename))==NULL){
       printf("failed to open this dirctory");
   }else{      
        while((dirent_ptr=readdir(dir_ptr))!=NULL){
            printf("%s\n",dirent_ptr->d_name);
        }
        closedir(dir_ptr);
   } 
}

int main(int argc,char* argv[]){
    show_ls(".");
}
