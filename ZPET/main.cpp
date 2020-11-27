//
//  main.cpp
//  ZPETv2
//
//  Created by James Duffy on 21/10/2020.
//  Copyright © 2020 James Duffy, DuffyAPP_IT. All rights reserved.
//

#include "moduleproc/module.hpp"
#include "misc.h"
#include "device.hpp"

int main(int argc, char *argv[]) {
    int analytics = 0;
    std::string moduleDir = "modules/moduleloader";
    if(XC==1) system("pwd");
    std::cout << "[*] Initialising" << std::endl;
    std::cout << "\033[1;34m  ___________  ______ _______  \n"
    "\033[1;35m |___  /  __ \\|  ____|__   __|\n"
    "\033[1;31m    / /| |__) | |__     | |    \n"
    "\033[1;36m   / / |  ___/|  __|    | |    \n"
    "\033[1;34m  / /__| |    | |____   | |    \n"
    "\033[1;31m /_____|_|    |______|  |_|\033[0m v2-18500-RC" << std::endl;

    //Check if running as root... (folder permissions restricted for user safety)
    if(getuid()!=0){
        std::cout << "ZPETv2 Must Be Run As Root! (sudo ./ZPET)" << std::endl;
        return 1;
    }
    
    
    /*
     ModuleLoader - Basic Prerequesite Check
     'moduleloader' *must* be present for ZPETv2 core functionality
     External modules are listed inside moduleloader for, well, loading!
     */
    if (!is_file_exist(moduleDir.c_str())) {
        std::cout << "[!] ZPET component 'moduleloader' is missing!\nIf you downloaded the repository from GitHub, you will be missing components...\nThe extra components are bundled in the release build. Check releases in GitHub!" << std::endl;
        return 1;
    }
    
    //UserConsent Checker
    if(is_file_exist(".analytics")){
        if(load_consent_data()=="y"){
            if (XC==1) std::cout << "[@] user consent enabled" << std::endl;
            analytics=1;
        } else if(load_consent_data()=="n"){
            if (XC==1) std::cout << "[@] user consent disabled" << std::endl;
            analytics=0;
        } else{
            std::cout << "[!] Invalid Consent State" << std::endl;
            exit(1);
        }
    } else{ //first launch consent
        std::cout << "---------\nWelcome To ZPET\n---------" << std::endl;
        std::cout << "ZPET is actively developed by a single developer, myself, @J_Duffy01 !" << std::endl;
        std::cout << "I have implemented very basic, *non-attributable*, optional analytics - i've made this transparent, showing example requests for each event:\n" << std::endl;
        std::cout << "Would you like to submit the following analytics?\n[*] ZPETv2 Launch 'Ping'!\t'{device:'MacBookPro13,3',event:'userStart'}'" << std::endl;
        std::cout << "[*] ZPETv2 Internal Error\t'{device:'MacBookPro13,3',event:'userProcess:iOSRecieveErr'}'" << std::endl;
        std::cout << "[*] ZPETv2 Feature Entry\t'{device:'MacBookPro13,3',event:'userFeatureHit:SPIDER'}'" << std::endl;
        std::cout << "\ny/n [?] ";
        std::string consent_data;
        std::cin >> consent_data;
        if(consent_data=="y"){
            write_consent_data("y");
            analytics=1;
            submit_event("userAnalytic:enable");
            std::cout << "\n";
        } else if(consent_data=="n"){
            write_consent_data("n");
            analytics=0;
            std::cout << "\n";
        } else{
            std::cout << "Invalid Consent Response" << std::endl;
            exit(1);
        }
    }
    
    //BASIC INIT END
    
    //LOOP
    bool exitZPET = false;
    while (exitZPET==false){
    /*
     ZPETv2 - Main Menu
     Last Author: James Duffy
     Last Modified: 24-11-2020
     Purpose: Capture user input & direct execution flow based on user choice...
     Notes:
            * Menu item array size based on amount of items (dynamic array? not sure if you can do this easily in c++?)
     */
    
    if(analytics==1) submit_event("userStart");
    
    std::string Menu[8]={"[LIVE]\tCheckra1n CLI\t\t\t(Boot From DFU)","[LIVE]\tExecute SPIDER Live\t\t\t(User-Data Analysis)","[LIVE]\tExecute ZPETv2 Modules\t\t(Documentation!)","[LIVE]\tAcquire Device Root Filesystem\t(Encryption State Prompt Prior To Acquisition)","[ROOT-FS]\tExecute SPIDER Locally\t\t(User-Data Analysis)","[ROOT-FS]\tExecute Mapper Locally\t\t(Blind Analysis)","[MISC]\tExit ZPET"};
    
    
    std::cout << "[*] Main Menu" << std::endl;
    for(int i=0;i<((sizeof(Menu)/sizeof(Menu[0]))-1);i++){
        std::cout << "[" << (i+1) << "] " << Menu[i] << std::endl;
    }

    int userOpt=0;
    std::cout << "\n[?] ";
    std::cin >> userOpt; //replace me with something more secure!
    
    switch (userOpt) {
        case 1:
            if (analytics==1) submit_event("userFeatureHit:Checkra1n");
            if(is_file_exist("/Applications/checkra1n.app/Contents/MacOS/checkra1n")){
                std::cout << "[*] Launching checkra1n DFU shortcut" << std::endl;
                system("/Applications/checkra1n.app/Contents/MacOS/checkra1n -c");
            } else {
                std::cout << "[!] checkra1n does not exist (in /Applications...)" << std::endl;
                if (analytics==1) submit_event("userFeatureHit:Checkra1n");
                return 1;
            }
            break;
        /*
         SpiderLIVE
         Last Author: James Duffy
         Last Modified: 25-11-2020
         Process:
                * Initialise Device Object
                * Send Device Prerequesites (check al_v2)
                * Send Live Spider Agent To Device Via SCP
                * Execute Remotely
                * Zip SpiderOUT On RHOST
                * Return, Unzip In Sensitive
                * Clean From RHOST
         Notes:
                * Prerequesite Cleanup!!!
         */
        case 2:{
            if (analytics==1) submit_event("userFeatureHit:SpiderLIVE");
            //Basic Device Init
            std::cout << "\n[*] Initialising Device\n" << std::endl;
            //Initialise 'device' object...
            Device device = init_device("ssh");
            //Dump information for debugging purposes
            if(device.can_connnect){
                device.info();
                std::cout << "\n[*] Preparing Spider..." << std::endl;
                sleep(5);
                if(is_file_exist("resources/ios/spider-integration-live.sh")){
                    //mount device as rw
                    std::string remountDisk = "resources/sshpass -p " + device.ssh_pw + " ssh -o \"UserKnownHostsFile=/dev/null\" -o \"StrictHostKeyChecking=no\" root@" + device.ip_addr + " -p" + device.port + " 'mount -o rw,union,update / '";
                    system(remountDisk.c_str());
                    
                    
                    //send live prereqs...
                    iosSend("resources/ios/spider-integration-live.sh", "/spider-integration-live.sh", device);
                    iosSend("resources/ios/sqlite3", "/usr/bin/sqlite3", device);
                    iosSend("resources/ios/sqlite3.h", "/usr/include/sqlite3.h", device);
                    iosSend("resources/ios/sqlite3ext.h", "/usr/include/sqlite3ext.h", device);
                    iosSend("resources/ios/libreadline.7.dylib", "/usr/lib/libreadline.7.dylib", device);
                    iosSend("resources/ios/libncurses.6.dylib", "/usr/lib/libncurses.6.dylib", device);
                    iosSend("resources/ios/tr", "/usr/bin/tr", device);
                    iosSend("resources/ios/paste", "/usr/bin/paste", device);
                    
                    std::string spiderMenuArr[5]={"Database Schema Extraction w/Hidden Database Identification","User-Data Ingest - Keyword Search","Apple Photos Connected Album Data","Exit Spider Integration"};
                    bool spiderMenu = true;
                    while(spiderMenu){
                        
                        std::string kw; // must be defined outside of switch statement
                        
                        std::cout << "[*] Spider ZPET Integration Menu - Beta" << std::endl;
                        for(int i=0;i<((sizeof(spiderMenuArr)/sizeof(spiderMenuArr[0]))-1);i++){
                            std::cout << "[" << (i+1) << "] " << spiderMenuArr[i] << std::endl;
                        }

                        int userOpt=0;
                        std::cout << "\n[?] ";
                        std::cin >> userOpt; //replace me with something more secure!
                        
                        switch (userOpt) {
                            case 1:{
                                std::cout << "[*] Database Schema Extraction" << std::endl;
                                std::string dbSchemeSpiderLiveCMD = "resources/sshpass -p " + device.ssh_pw + " ssh -o \"UserKnownHostsFile=/dev/null\" -o \"StrictHostKeyChecking=no\" root@" + device.ip_addr + " -p" + device.port + " '/spider-integration-live.sh -f'";
                                system(dbSchemeSpiderLiveCMD.c_str());
                                if(iosReceive("/SpiderOUT", device.ip_addr.c_str(), device.ssh_pw.c_str(), device.port.c_str())==0){
                                    std::cout << "[*] Extraction Complete\n[@] Check SENSITIVE/ & set necessary open cmd depending on structure" << std::endl;
                                } else {
                                    std::cout << "[!] Spider Analysis Did Not Complete" << std::endl;
                                }
                                break;
                            }
                            case 2:{
                                std::cout << "[*] ZPET->Spider KWSearch..." << std::endl;
                                bool KWSearch = true;
                                while(KWSearch){
                                    std::cout << "[*] Please Enter A Single Keyword...\n[?] ";
                                    std::cin >> kw;
                                    char cmdbuf[400];
                                    snprintf(cmdbuf, 400, "resources/sshpass -p %s ssh -o \"UserKnownHostsFile=/dev/null\" -o \"StrictHostKeyChecking=no\" root@%s -p%s '/spider-integration-live.sh -i  %s'",device.ssh_pw.c_str(),device.ip_addr.c_str(),device.port.c_str(),kw.c_str());
                                    std::cout << cmdbuf << std::endl;
                                    system(cmdbuf);
                                    std::cout << "[*] Execution Complete" << std::endl;
                                }
                                break;
                            }
                            case 3:{
                                std::cout << "[*] Apple Photos - Connected Share Data Extraction" << std::endl;
                                std::string applePhotosSpiderLiveCMD = "resources/sshpass -p " + device.ssh_pw + " ssh -o \"UserKnownHostsFile=/dev/null\" -o \"StrictHostKeyChecking=no\" root@" + device.ip_addr + " -p" + device.port + " '/spider-integration-live.sh -p | tee /SpiderOUT/PhotoData.txt'";
                                std::cout << applePhotosSpiderLiveCMD;
                                system(applePhotosSpiderLiveCMD.c_str());
                                if(iosReceive("/SpiderOUT", device.ip_addr.c_str(), device.ssh_pw.c_str(), device.port.c_str())==0){
                                    std::cout << "[*] Extraction Complete\n[@] Check SENSITIVE/ & set necessary open cmd depending on structure" << std::endl;
                                } else {
                                    std::cout << "[!] Spider Analysis Did Not Complete" << std::endl;
                                }
                                break;
                            }
                                
                            case 4:
                                spiderMenu = false;
                            default:
                                std::cout << "[!] Invalid Option" << std::endl;
                                spiderMenu = false;
                        }
                    }
                } else{
                    std::cout << "[!] Internal Error" << std::endl;
                }
                
                //Clean Prerequesites
                iosRM("/spider-integration-live.sh", device);
                iosRM("/usr/bin/sqlite3", device);
                iosRM("/usr/include/sqlite3.h", device);
                iosRM("/usr/include/sqlite3ext.h", device);
                iosRM("/usr/lib/libncurses.6.dylib", device);
                iosRM("/usr/bin/tr", device);
                iosRM("/usr/bin/paste", device);
                
                if(device.connection_type == "ssh" && device.port != "3022") system("pkill iproxy");
            } else{
                if (analytics==1) submit_event("userProcess:deviceCanConnectERR");
                std::cout << "\n[!] Device Did Not Connect Successfully...";
                sleep(5);
                return 1;
            }
        break;
        }
        case 3:
            if (analytics==1) submit_event("userFeatureHit:ZPETModuleProc");
                //Create 'SENSITIVE' Directory For Device Data Processing/Handling
                //Notes - set permissions appropriately... currently set to world rw... root only!
                if (mkdir("SENSITIVE", 0777)) {
                    std::system("sudo rm -rf SENSITIVE 2>/dev/null");
                    if (mkdir("SENSITIVE", 0777)) {
                        if (analytics==1) submit_event("userProcess:sensitiveDirCreateErr");
                        std::cout << "Does The SENSITIVE Directory Already Exist? Back It Up & Delete It!..." << std::endl;
                        return 1;
                    } else{
                        std::cout << "\n[*] Initialising Device\n" << std::endl;
                        //Initialise 'device' object...
                        Device device = init_device("ssh");
                        //Dump information for debugging purposes
                        if(device.can_connnect){
                            device.info();
                            std::cout << "\n[*] Preparing To Extract Data..." << std::endl;
                            sleep(5);
                            process_modules(device);
                            if(device.connection_type == "ssh" && device.port != "3022") system("pkill iproxy");
                        } else{
                            if (analytics==1) submit_event("userProcess:deviceCanConnectERR");
                            std::cout << "\n[!] Device Did Not Connect Successfully...";
                            sleep(5);
                            return 1;
                        }
                }
                return 1;
            }
            break;
        /*
         Full Filesystem Acquisition
         Last Author: James Duffy, Kevin Mitchell
         Last Modified: 25-11-2020
         Purpose: Acquire Device Filesystem
         Notes:
                * offer to untar filesystem using tar -xf?
                * remember to use -C to specify custom tar output (SENSITIVE) as cwd will be root of zpet structure.
         */
        case 4:
            if(analytics==1) submit_event("userFeatureHit:FSAcquire");
            //Create 'SENSITIVE' Directory For Device Data Processing/Handling
            //Notes - set permissions appropriately... currently set to world rw... root only!
            if (mkdir("SENSITIVE", 0777)) {
                std::system("sudo rm -rf SENSITIVE 2>/dev/null");
                if (mkdir("SENSITIVE", 0777)) {
                    if (analytics==1) submit_event("userProcess:sensitiveDirCreateErr");
                    std::cout << "Does The SENSITIVE Directory Already Exist? Back It Up & Delete It!..." << std::endl;
                    return 1;
                } else{
                    std::cout << "\n[*] Initialising Device\n" << std::endl;
                    //Initialise 'device' object...
                    Device device = init_device("ssh");
                    device.info();
                    //Dump information for debugging purposes
                    if(device.can_connnect){
                        std::cout << "\n[*] Preparing To Extract Data..." << std::endl;
                        sleep(5);
                        char cwd[PATH_MAX];
                        if (getcwd(cwd, sizeof(cwd)) != NULL) {
                            printf("[+] Dumping rootfs in current working directory\n");
                            char commandBuffer[PATH_MAX];
                            snprintf(commandBuffer, sizeof(commandBuffer), "resources/sshpass -p %s ssh -o \"UserKnownHostsFile=/dev/null\" -o \"StrictHostKeyChecking=no\" root@%s -p%s 'tar zcf - / 2>/dev/null' | resources/pv > SENSITIVE/filesystem.tar",device.ssh_pw.c_str(),device.ip_addr.c_str(),device.port.c_str());
                            system(commandBuffer);
                            if(device.connection_type == "ssh" && device.port != "3022") system("pkill iproxy");
                        } else {
                            printf("[-] Error getting current working directory\n");
                            exit(1);
                       }
                    } else{
                        if (analytics==1) submit_event("userProcess:deviceCanConnectERR");
                        std::cout << "\n[!] Device Did Not Connect Successfully...";
                        sleep(5);
                    }
                }
            break;
        /*
         Spider - Local Integration
         Last Author: James Duffy
         Last Modified: 25-11-2020
         Purpose: Execute Local Spider w/rootfs
         Notes:
                * Verify rootFS exists locally instead of in sh!
         */
        case 5:
            if(analytics==1) submit_event("userFeatureHit:SpiderLOCAL");
                if(is_file_exist("./SENSITIVE/etc/hosts") && is_file_exist("./resources/spider-integration-local.sh")){ //verify iOS filesystem is captured & accessible
                    //Small Menu For Spider Analysis Options
                    std::string spiderMenuArr[7]={"Database Schema Extraction w/Hidden Database Identification","User-Data Ingest - Keyword Search","Apple Photos Connected Album Data","Exit Spider Integration"};
                    bool spiderMenu = true;
                    while(spiderMenu){
                        
                        std::string kw; // must be defined outside of switch statement
                        
                        std::cout << "[*] Spider ZPET Integration Menu - Beta" << std::endl;
                        for(int i=0;i<((sizeof(spiderMenuArr)/sizeof(spiderMenuArr[0]))-1);i++){
                            std::cout << "[" << (i+1) << "] " << spiderMenuArr[i] << std::endl;
                        }

                        int userOpt=0;
                        std::cout << "\n[?] ";
                        std::cin >> userOpt; //replace me with something more secure!
                        
                        switch (userOpt) {
                            case 1:
                                std::cout << "[*] Database Schema Extraction" << std::endl;
                                if(macOS_GetExit("resources/spider-local-integration.sh -f")==0){
                                    std::cout << "[*] Extraction Complete" << std::endl;
                                    system("open ./SENSITIVE/SpiderOUT");
                                } else {
                                    std::cout << "[!] Spider Analysis Did Not Complete" << std::endl;
                                }
                                break;
                            case 2:{
                                std::cout << "[*] ZPET->Spider KWSearch..." << std::endl;
                                bool KWSearch = true;
                                while(KWSearch){
                                    std::cout << "[*] Please Enter Keyword...\n[?] ";
                                    std::cin >> kw;
                                    char cmdbuf[sizeof(kw)];
                                    snprintf(cmdbuf, sizeof(kw), "./resources/spider-integration-local.sh -i %s",kw.c_str());
                                    if(macOS_GetExit("resources/spider-local-integration.sh")==0){
                                        std::cout << "[*] Extraction Complete" << std::endl;
                                        system("open ./SENSITIVE/SpiderOUT");
                                    } else {
                                        std::cout << "[!] Spider Analysis Did Not Complete" << std::endl;
                                    }
                                }
                                break;
                            }
                            case 3:
                                std::cout << "[*] Apple Photos - Connected Share Data Extraction" << std::endl;
                                if(macOS_GetExit("resources/spider-local-integration.sh -p")==0){
                                    std::cout << "[*] Extraction Complete" << std::endl;
                                    system("open ./SENSITIVE/SpiderOUT");
                                } else {
                                    std::cout << "[!] Spider Analysis Did Not Complete" << std::endl;
                                }
                                break;
                            default:
                                std::cout << "Invalid Option" << std::endl;
                                break;
                        }
                    }
                } else{
                    std::cout << "SPIDER Local ZPET Integration Requires A Full Filesystem Acquisition...Option [4] In ZPETv2!\nChoose [LIVE] Spider For Live Device SPIDER Analysis" << std::endl;
                }
                break;
        case 6:
            if(analytics==1) submit_event("userFeatureHit:MapperLocal");
            if(is_file_exist("./SENSITIVE/etc/hosts") && is_file_exist("./resources/mapper.sh")){
                system("resources/mapper.sh");
            } else{
                std::cout << "Mapper For ZPET Requires A Full Filesystem Acquisition...Option [4] In ZPET!" << std::endl;
            }
            break;
                
        case 7:
                exitZPET = true;
                break;
            
            
        default:
            if (analytics==1) submit_event("userProcess:invalidMenuClick");
            std::cout << "[!] Invalid Option!" << std::endl;
            break;
    }
    return 0;
}
}
}
