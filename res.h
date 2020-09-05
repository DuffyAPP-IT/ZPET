//
// Created by 010010 on 27/05/2020.
//

#ifndef SRC_RES_H
#define SRC_RES_H
#define ARRAY_SIZE(x) ( sizeof(x) / sizeof((x)[0]) )
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>

typedef struct module{
    int version;
    char *name;
    char *description;
    char *foi;
    int parsetype;
    char *sval;
    char *dict;
    char *sql;
    int usecust;
    char *cust;
    char *custloc;
    int useendcust;
    char *endcust;
    int usefoi2;
    char *foi2;
} module;


//Module Definitions
module wifi();
module accounts();
module bsn();
module vpnd();
module appids();
module wallet();
module mailextract();
module wapp();
module ktxproc();
module mailvip();
module PCInfo();
module callhist();
module wifiloc();
module photocom();

void cleanmenu(int note, char *notedata);
void modinfo(module mod);
int ios_fetch_access(char *ipin,char *port);

//secureLog
int securelog();
int rlogios();

char *ios_run_comm(char *command);
char *ios_run_ge(char *command);

int macos_run_ge(char *command);
char *macos_run_comm(char *command);

int ios_makedir(char *absolutedirectory);
int ios_send_f(char *filetosend, char *remotedir);
int ios_rec_f(char *remotefiledir, char *localfname);
int ios_checkdirexists(char *dir);

#endif //SRC_RES_H
