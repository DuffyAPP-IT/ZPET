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

int verifyPrereqs(){
    switch (OS) {
            case 1:
            if(macOS_GetExit("which iproxy >/dev/null")==0){
                std::cout << "----------\nLOADED iProxy" << std::endl;
                usleep(200 * 1000);
                if(macOS_GetExit("which scp >/dev/null")==0){
                    std::cout << "LOADED SCP" << std::endl;
                    usleep(200 * 1000);
                    if(macOS_GetExit("ls /usr/libexec/plistbuddy >/dev/null ")==0){
                        std::cout << "LOADED plistbuddy (for plist processing)" << std::endl;
                        if(macOS_GetExit("which truncate >/dev/null ")==0){
                            std::cout << "LOADED truncate (for file manipulation)" << std::endl;
                            usleep(200 * 1000);
                            return 0;
                        } else return 1;
                    } else return 1;
                } else return 1;
            } else return 1;
            break;
            case 2:
            if(macOS_GetExit("which iproxy")==0){
                std::cout << "Found iProxy" << std::endl;
                if(macOS_GetExit("which scp")==0){
                    std::cout << "Found SCP" << std::endl;
                    if(macOS_GetExit("plutil")==0){
                        std::cout << "Found Plist Processor" << std::endl;
                        usleep(200 * 1000);
                        return 0;
                    } else return 1;
                } else return 1;
            } else return 1;
            break;
        default:
            return 2;
    }
    return 1;
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
        
//       std::cout << receive;
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



