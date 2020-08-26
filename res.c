//
// Created by 010010 on 27/05/2020.
//

#include "res.h"

int securelog(){
    printf("Sending Filesystem Monitor To iDevice\n");
    if(ios_send_f("Resources/fsmon-ios","/")!=50){
        ios_run_ge("\/fsmon-ios \\> \/.fslog-ios \\&");
        return 0;
    } else{
        printf("Failed Transmitting Filesystem Monitor...\n");
        return 1;
    }
}


int rlogios(){
    printf("Returning Filesystem Logs\n");
    if(ios_rec_f("/.fslog-ios","Data/fslog-ios")==0){
        ios_run_ge("rm /fsmon-ios");
//        ios_run_ge("rm /.fslog-ios");
        return 0;
    } else{
        return 1;
    }
}

void modinfo(module mod){
    printf("Executing Module (Non-Verbose) -> %s\n\n",mod.name);
//    printf("Module Name : %s\n", mod.name);
//    printf("Module Description : %s\n", mod.description);
//    printf("Module Version : %d\n", mod.version);
//    printf("Module FOI Target : %s\n", mod.foi);
//    printf("Parse Type (Check Documentation) : %d\n", mod.parsetype);
//    if(strlen(mod.sval) == 0){
//        printf("Specific Value : %s", mod.sval);
//    }
}

int ios_fetch_access(char *ipin,char *port){
    char *ping = "nc -z";
    char *silence = ">/dev/null 2>/dev/null";
    char command[800];
    sprintf(command,"%s %s %s %s",ping,ipin,port,silence);
    FILE * shell;
    shell = popen(command, "r");
    int r = pclose(shell);
    if(WEXITSTATUS(r)==0)
    {
        return 0;
    } else {
        return 1;
    }
}

//iOS Exec Handling
char *ios_run_comm(char *command){
    char *com1 = "Resources/sshpass -p";
    char *com2 = "ssh -o StrictHostKeyChecking=no root@127.0.0.1 -p 7788";
    char commout[2400];
    sprintf(commout, "%s alpine %s %s", com1, com2, command);
    char *com = commout;
    char out[4048];
    FILE *shell = popen(com, "r");
    fgets(out, sizeof(out), shell);
    pclose(shell);
    return out;
}
char *ios_run_ge(char *command){
    char *com1 = "Resources/sshpass -p";
    char *com2 = "ssh -o StrictHostKeyChecking=no root@127.0.0.1 -p 7788";
    char *silence = ">/dev/null 2>/dev/null \; echo $?";
    char commout[2400];
    sprintf(commout, "%s alpine %s %s %s", com1, com2, command, silence);
    char *com = commout;
    char out[4048];
    FILE *shell = popen(com, "r");
    fgets(out, sizeof(out), shell);
    pclose(shell);
    return out;
}

//macOS Exec Handling
char *macos_run_comm(char *command){
    char com2[1024];
    strcpy(com2, command);
    char commout[1024];
    sprintf(commout, "%s", com2);
    char *com = commout;
    char out[4096];
    FILE *shell = popen(com, "r");
    fgets(out, sizeof(out), shell);
    pclose(shell);
    return out;
}
int macos_run_ge(char *command){
    char *com2 = ">/dev/null 2>/dev/null;echo $?";
    char com1[2400];
    strcpy(com1, command);
    char commout[2400];
    sprintf(commout, "%s %s", com1, com2);
    char *com = commout;
    char out[2048];
    FILE *shell = popen(com, "r");
    fgets(out, sizeof(out), shell);
    pclose(shell);
    return atoi(out);
}

//iOS FS Management
int ios_makedir(char *absolutedirectory){
    if (ios_fetch_access("127.0.0.1","7788") == 0){
        char *com1 = ("mkdir");
        char *com2 = (">/dev/null 2>/dev/null\; echo \$?");
        char dirmake[800];
        
        sprintf(dirmake, "%s %s %s", com1, absolutedirectory, com2);
        return (atoi(ios_run_ge(dirmake)));
    }
}
//ios_send_f("fstab","/mnt1/private/etc");
int ios_send_f(char *filetosend, char *remotedir){
    FILE *fileout;
    if((fileout = fopen(filetosend,"r"))!=NULL)
    {
        fclose(fileout);
        char commout[800];
        sprintf(commout, "Resources/sshpass -p alpine scp -P 7788 %s root@127.0.0.1:%s 2>/dev/null \; echo $?", filetosend, remotedir);
        char *com = commout;
        char out[2048];
        FILE *shell = popen(com, "r");
        fgets(out, sizeof(out), shell);
        pclose(shell);
        if (atoi(out)==0){
            return 0;
        } else{
            return 1;
        }
    }
}
//ios_rec_f("/mnt1/private/etc/fstab","fstab");
int ios_rec_f(char *remotefiledir, char *localfname){
    char commout[800];
    sprintf(commout, "Resources/sshpass -p alpine scp -r -P 7788 root@127.0.0.1:%s %s 2>/dev/null \; echo $?", remotefiledir, localfname);
    char *com = commout;
    char out[2048];
    FILE *shell = popen(com, "r");
    fgets(out, sizeof(out), shell);
    pclose(shell);
    if (atoi(out)==0){
        return 0;
    } else{
        return 1;
    }
}

//returns 0 on success 1 if dir doesnt exist
//dbl check this one
int ios_checkdirexists(char *dir){
    char *com1 = ("mkdir");
    char *com2 = (">/dev/null 2>/dev/null\; echo \$?");
    char dircheck[500];
    sprintf(dircheck, "%s %s %s", com1, dir, com2);
    if (atoi(ios_run_comm(dircheck))==0){
        char rmdir[600];
        sprintf(rmdir,"rm %s",dir);
        ios_run_comm(rmdir);
        return 1;
    } else{
        return 0;
    }
}
