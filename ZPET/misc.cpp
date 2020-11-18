//
//  misc.cpp
//  ZPET
//
//  Created by James Duffy on 21/10/2020.
//  Copyright © 2020 James Duffy. All rights reserved.
//

//
// Created by 010011 on 06/10/2020.
//

#include "module.hpp"
#include "misc.h"
#include "processor.hpp"


#include <unistd.h>

/*
 check_binary_in_path
 Last Author: James Duffy
 Last Modified: 18-11-2020
 Purpose: Check binary exists in path... (prerequesite checker)
 Notes:
        * Potentially add option for absolute referencing of a binary?
 */
int check_binary_in_path(std::string bin){
    std::string exec_com = "which " + bin + " >/dev/null";
    if(macOS_GetExit(exec_com.c_str())==0){
        usleep(200 * 1000);
        return 0;
    } else return 1;
}


int countLinesInTxt(char *txtname){
    int mllinenum = 0;
    if(!is_file_exist(txtname)) {
        return 1;
    }
    std::string line; //temporary data storage
    std::ifstream loadertxt(txtname);
    //find amount of modules in loader txt
    if (loadertxt.is_open()) {
        while (getline(loadertxt, line)) {
            mllinenum++;
        }
        loadertxt.close();
    }
    return mllinenum;
}

int scanHandler(Module mod,const std::string& DEVICEIP,const std::string& DEVICEPORT,const std::string& DEVICEPW){
    if(mod.parsetype=="plist"){
        if(plistproc(mod.foi,mod.filterval,mod.pipeprocess,DEVICEIP,DEVICEPORT,DEVICEPW)==0){
            return 0;
        } else return 1;
    } else if(mod.parsetype=="json"){
        if(jsonproc(mod.foi,mod.filterval,DEVICEIP,DEVICEPORT,DEVICEPW)==0){
            return 0;
        } else return 1;
    } else if(mod.parsetype=="sqlite"){
        if(sqliteproc(mod.foi,mod.filterval,mod.pipeprocess,DEVICEIP,DEVICEPORT,DEVICEPW)==0){
            return 0;
        } else return 1;
    } else if(mod.parsetype=="exec"){
        if(execproc(mod.foi,mod.filterval,mod.pipeprocess,DEVICEIP,DEVICEPORT,DEVICEPW,mod.execend)==0){
            return 0;
        } else return 1;
    } else{
        return 1;
    }
}

int iosReceive(std::string foi,std::string deviceip,std::string devicepwd){
    //remove old file because folder cannot overwrite file... possibly better solution for this.
    std::system("sudo rm -rf SENSITIVE 2>/dev/null && mkdir SENSITIVE");
    if(is_file_exist("resources/sshpass")) {
        std::string receive = "resources/sshpass -p " + devicepwd + " scp -r -P 7788" +
        " -o 'UserKnownHostsFile=/dev/null' -o 'StrictHostKeyChecking=no' root@" + deviceip + ":" +
        foi + " SENSITIVE/local 2>/dev/null";
       
        const char *exec = receive.c_str();
        int ret = system(exec);

        if (WEXITSTATUS(ret) == 0){
//            std::cout << "Finished Copy" << std::endl;
        return 0;
        } else return 1;
    }
    return 0;
}

int macOS_GetExit(std::string command){
    const char *exec = command.c_str();
    int ret = system(exec);
    if (WEXITSTATUS(ret) == 0)
    return 0;
    else
    return 1;
}

char *macos_run_get_fline(char *command){
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



