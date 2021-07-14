#include <stdio.h>
int main(int argc,char* argv[]){
    FILE *read_file_ptr=fopen(argv[1],"r");
    FILE *write_file_ptr=fopen(argv[2],"w");
    int content=0;
    char file[10000];
    while(1){
        content=fread(file,1,sizeof(file),read_file_ptr);
        if(content==0){
            break;
        }
        fwrite(file,1,content,write_file_ptr);
    }
    fclose(read_file_ptr);
    fclose(write_file_ptr);
}
