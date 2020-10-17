#include <iostream>
#include <cstdlib>
#include <string>
#include "module.h"
#include "misc.h"
#include <sys/stat.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    //Basic Init
    //Basic Costant Strings - Could Setup User Input
    std::string deviceip = "127.0.0.1";
    std::string deviceport;
    std::string devicepw;

    std::cout << "\033[1;34m  ___________  ______ _______  \n"
                 "\033[1;35m |___  /  __ \\|  ____|__   __|\n"
                 "\033[1;31m    / /| |__) | |__     | |    \n"
                 "\033[1;36m   / / |  ___/|  __|    | |    \n"
                 "\033[1;34m  / /__| |    | |____   | |    \n"
                 "\033[1;31m /_____|_|    |______|  |_|\033[0m v2-" << rand() << std::endl;


    if (!is_file_exist("modules/moduleloader")) {
        std::cout << "ZPET component 'moduleloader' is missing!\n" << std::endl;
        return 1;
    }

    if (mkdir("SENSITIVE", 0777) != 0) {
//        rmdir("SENSITIVE"); //rmdir doesn't seem to work properly?
        //using system temporarily...
        std::system("sudo rm -rf SENSITIVE 2>/dev/null");
        if (mkdir("SENSITIVE", 0777) != 0) {
            std::cout << "Does The SENSITIVE Directory Already Exist? Back It Up & Delete It!..." << std::endl;
            return 1;
        }
    }

    if (verifyPrereqs() != 0) {
        std::cout << "prerequisites not fulfilled\n" << std::endl;
        return 1;
    }

    //BEGIN PROCESSING

    //TODO - Fix return character not being detected/escaping from input.
    std::cout << "--------\nDevice SSH Port? (44): ";
    std::cin >> deviceport;
    if(deviceport.length()==0) deviceport = "44";
    std::cout << "Device Root Password? (alpine): ";
    std::cin >> devicepw;
    if(devicepw.length()==0) devicepw = "alpine";

    Module mods[countLinesInTxt("modules/moduleloader")];
    //Populate mods array with modules while counting imported mods
    int linenum = 0;
    std::string line;
    std::ifstream loadertxt("modules/moduleloader");
    std::ifstream popmodtxts("modules/moduleloader");
    if (popmodtxts.is_open()) {
        while (getline(popmodtxts, line)) {
            std::string addedsubdir = "modules/" + line;
            mods[linenum] = loadModule(addedsubdir);
            linenum++;
        }
        popmodtxts.close();
    }
    int loadedmodcount = linenum;


    //Initialise SSH
    std::cout << "----------\nInitialising SSH...\n----------\n";
    std::string ssh_init = "iproxy 7788 " + deviceport + " 2>/dev/null >/dev/null &";
    const char *exec = ssh_init.c_str();
    int ret = system(exec);
    if (WEXITSTATUS(ret) != 0){
        std::cout << "USB-SSH Proxy Could Not Be Initialised...\nIs libimobiledevice Installed?";
        return 1;
    }
    sleep(2);
    std::cout << "----------\nSSH Tunnel Initialised\n----------\n";
    sleep(1);
    //Check device is connected
    if(macOS_GetExit("ideviceinfo | grep DeviceName | cut -f2 -d':'")!=0) return 1;

    //process loaded mods
    for(int i=0;i<(loadedmodcount);i++){
        if(mods[i].validate()==0){
//            mods[i].info();
            std::cout << "\nModule -> " << mods[i].displayname << "\nAuthor -> " << mods[i].author << std::endl;
            sleep(1);
            std::cout << "Module Output -> ...\n----------\n";
            if(scanHandler(mods[i],deviceip,deviceport,devicepw)!=0){
                std::cout << "Module " << i << " Sent For Processing But Did NOT Complete!" << std::endl;
            }
            std::cout << "----------" << std::endl;
        } else{
            std::cout << "=========\nModule" << i << " did not pass the initial validator!\n=========\n";
        }
    }

    //Generic iproxy kill on exit - check *nix compatibility
    system("pkill iproxy");
    return 0;
}
