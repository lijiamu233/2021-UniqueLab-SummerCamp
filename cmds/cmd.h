#ifndef CMD_H
#define CMD_H

#define MAX_ARGV_LENGTH 256
#define MAX_PS1_LENGTH 256
#define MAX_PWD_LENGTH 256
#define MAX_UID_LENGTH 50

typedef struct USERINFO
{
    char pwd[MAX_PWD_LENGTH];      //pwd
    char *usn;      //username
}USERINFO;


int SplitCmdInput(char*);
int SetPS1(char*,USERINFO*);
int CmdParser(int,char**);

#endif