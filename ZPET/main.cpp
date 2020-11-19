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
    std::string moduleDir = "modules/moduleloader";
    
    system("pwd");
    
    std::cout << "\033[1;34m  ___________  ______ _______  \n"
    "\033[1;35m |___  /  __ \\|  ____|__   __|\n"
    "\033[1;31m    / /| |__) | |__     | |    \n"
    "\033[1;36m   / / |  ___/|  __|    | |    \n"
    "\033[1;34m  / /__| |    | |____   | |    \n"
    "\033[1;31m /_____|_|    |______|  |_|\033[0m v2-internal" << std::endl;

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
    
    //BASIC INIT END
    
    /*
     ZPETv2 - Main Menu
     Last Author: James Duffy
     Last Modified: 17-11-2020
     Purpose: Capture user input & direct execution flow based on user choice...
     Notes:
            * Ensure that only required dependencies are loaded based on user choice!
            * Menu item array size based on amount of items (dynamic array? not sure if you can do this easily in c++?)
     */
    
    std::string Menu[7]={"[LIVE]\tCheckra1n CLI\t\t\t(Boot From DFU)","[LIVE]\tExecute SPIDER\t\t\t(User-Data Analysis)","[LIVE]\tExecute ZPETv2 Modules\t\t(Documentation!)","[LIVE]\tAcquire Device Root Filesystem\t(Encryption State Prompt Prior To Acquisition)","[ROOT-FS]\tExecute SPIDER Locally\t\t(User-Data Analysis)","[ROOT-FS]\tExecute Mapper Locally\t\t(Blind Analysis)"};
    
    
    std::cout << "[*] Main Menu" << std::endl;
    for(int i=0;i<((sizeof(Menu)/sizeof(Menu[0]))-1);i++){
        std::cout << "[" << (i+1) << "] " << Menu[i] << std::endl;
    }

    int userOpt=0;
    std::cout << "\n[?] ";
    std::cin >> userOpt; //replace me with something more secure!
    
    switch (userOpt) {
        case 1:
            //check if checkra1n exists in /Applications, launch with '-c' (ctrl+c back to ZPET)
            break;
        case 2:
            //check if spider is present in resources/spiderkit... present mini menu in ZPET?
            break;
        case 3:
                //Create 'SENSITIVE' Directory For Device Data Processing/Handling
                //Notes - set permissions appropriately... currently set to world rw... root only!
                if (mkdir("SENSITIVE", 0777)) {
                    std::system("sudo rm -rf SENSITIVE 2>/dev/null");
                    if (mkdir("SENSITIVE", 0777)) {
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
                        } else{
                            std::cout << "\n[!] Device Did Not Connect Successfully...";
                            exit(1);
                        }
                }
                return 1;
            }
            break;
            
        default:
            std::cout << "[!] Invalid Option!" << std::endl;
            break;
    }
    
    
    
    exit(1);//debug
    
    //BASIC INIT ENDS HERE
    
    
    //Generic iproxy kill on exit - check *nix compatibility
    system("pkill iproxy");
    return 0;
}
