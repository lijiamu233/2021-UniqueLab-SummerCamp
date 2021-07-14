#ifndef BUILTIN_H
#define BUILTIN_H

typedef int BUILTIN_CMD(int argc,char** argv);

typedef struct builtincmd
{
    char name[20];

}BuiltinCmd;


int FindBuiltinCmd(char*);
int IfBuiltinCmd(char*);

BUILTIN_CMD pwd;
BUILTIN_CMD cd;
BUILTIN_CMD echo;
BUILTIN_CMD touch;
BUILTIN_CMD cp;

#endif