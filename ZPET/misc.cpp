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

#include "moduleproc/module.hpp"
#include "misc.h"
#include "moduleproc/processor.hpp"
#include <ctime>
#include "Base64.h"
#include <sys/utsname.h>
#include <unistd.h>
#include <string.h>

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

int iosReceive(std::string foi,std::string deviceip,std::string devicepwd, std::string deviceport){
    //remove old file because folder cannot overwrite file... possibly better solution for this.
    std::system("sudo rm -rf SENSITIVE 2>/dev/null && mkdir SENSITIVE");
    if(is_file_exist("resources/sshpass")) {
        std::string receive = "resources/sshpass -p " + devicepwd + " scp -r -P " + deviceport +
        " -o 'UserKnownHostsFile=/dev/null' -o 'StrictHostKeyChecking=no' root@" + deviceip + ":" +
        foi + " SENSITIVE/local 2>/dev/null";
        if(dbg==1) std::cout << "iosReceivex -> " << receive << std::endl;
        const char *exec = receive.c_str();
        int ret = system(exec);

        if (WEXITSTATUS(ret) == 0){
            std::cout << "[+] Finished Copy" << std::endl;
        return 0;
        } else {
            if(load_consent_data()=="y") submit_event("userProcess:iosReceiveErr");
            return 1;
        }
    }
    return 0;
}

int iosSend(std::string relative_path, std::string absolute_dest, Device device){
    if(is_file_exist("resources/sshpass")) {
        std::string receive = "resources/sshpass -p " + device.ssh_pw + " scp -r -P " + device.port +
        " -o 'UserKnownHostsFile=/dev/null' -o 'StrictHostKeyChecking=no' " + relative_path + " root@" + device.ip_addr + ":" + absolute_dest + " 2>/dev/null";
        if(dbg==1) std::cout << "iosSend -> " << receive << std::endl;
        const char *exec = receive.c_str();
        int ret = system(exec);

        if (WEXITSTATUS(ret) == 0){
            if(dbg==1) std::cout << "[@] Finished Copy" << std::endl;
        return 0;
        } else {
            if(load_consent_data()=="y") submit_event("userProcess:iosSendErr");
            return 1;
        }
    }
    return 0;
}

int iosRM(std::string absolute_path, Device device){
    if(is_file_exist("resources/sshpass")) {
        std::string rmCMD = "resources/sshpass -p " + device.ssh_pw + " ssh -o \"UserKnownHostsFile=/dev/null\" -o \"StrictHostKeyChecking=no\" root@" + device.ip_addr + " -p" + device.port + " 'rm " + absolute_path + "'";
        if(dbg==1) std::cout << "iosRM -> " << rmCMD << std::endl;
        const char *exec = rmCMD.c_str();
        int ret = system(exec);

        if (WEXITSTATUS(ret) == 0){
            if(dbg==1) std::cout << "[@] RM Operation Complete" << std::endl;
        return 0;
        } else {
            if(load_consent_data()=="y") submit_event("userProcess:iosRMErr");
            return 1;
        }
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
        return strtok(out,"\n");
}

std::string load_consent_data(){
    std::string data; //hold txt data per line during read
    std::string dataout;
    std::ifstream loadedtxt(".analytics"); //initialise stream to input txt file
    int current_linenum = 0; //init line number track
    if (loadedtxt.is_open())
    {
        while (getline(loadedtxt,data) )
        {
            dataout = dataout+data;
            current_linenum++;
        }
        loadedtxt.close(); //close file reading
    }
    return dataout;
};

int write_consent_data(std::string yn){
    std::ofstream log(".analytics");
    if(!log) return 1;
    log.write(yn.c_str(),yn.size());
    log.close();
    return 0;
}

std::string endProc(std::string str){
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    str.erase(remove(str.begin(), str.end(), ':'), str.end());
    str.erase(remove(str.begin(), str.end(), '\n'), str.end());
    return str;
}



void submit_event(std::string event){
//    std::string model = macos_run_get_fline("system_profiler SPHardwareDataType | awk '/Identifier/ {print $3}'");
//    std::string os_ver = macos_run_get_fline("system_profiler SPSoftwareDataType | awk '/System Version/ {print $4}'");
//    std::cout << "Fetching sysinfo struct...!" << std::endl;
    struct utsname systemstats;
//    std::cout << "Processing sysinfo...";
//    sleep(1);
//    std::cout << "!";
    uname(&systemstats);
    std::string z = systemstats.sysname;
    std::string kv = systemstats.machine;
//    std::cout << "Generating CryptVal...!";
    std::time_t cryptval = std::time(nullptr);
//    std::cout << "Generating SecureVal...";
//    sleep(1);
//    std::cout << "!" << std::endl;
    std::string outsecureval = std::asctime(std::localtime(&cryptval));
//    std::cout << "constructing..." << std::endl;
    std::string machine = endProc("zpet"+z+kv+"x49"+outsecureval+".local");
    std::string auth = Encode(machine);
    if(dbg==1) std::cout << "Machine Value: " << machine << std::endl;
    if(dbg==1) std::cout << "Generated Auth Key: " << auth << std::endl;
    
    std::string device = macos_run_get_fline("sysctl hw.model | cut -f2 -d':' | cut -f2 -d' '");
    
    std::string launchRequest = "curl -s --location --request POST 'http://alpha.external.duffy.app:8080/api/sandbox' --header 'Content-Type: application/json' --data-raw '{\"device\": \"" + device + "\",\"event\": \"" + event + "\",\"auth\": \"" + auth + "\"}' >/dev/null";
    
//    std::cout << launchRequest;
//    sleep(2);
    system(launchRequest.c_str());
//    std::cout << "sent!" << std::endl;
}

