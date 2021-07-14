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
void DestProg(char** argv,int num);
int IfRedPip(int argc,char** argv);
int ExeCmd(int cmdnum,int argc,char** argv);
int POR(int argc,char** argv);
int Rout(int argc,char** argv);
int Rin(int argc,char** argv);
int Pip(int argc,char** argv);
//int SplitOptions(int argc,char** argv,int* num,char** opts);

#endif