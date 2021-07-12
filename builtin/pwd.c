#include "../common.h"

extern USERINFO user;

int pwd(int argc,char** argv) {
    printf("%s\n",user.pwd);
}