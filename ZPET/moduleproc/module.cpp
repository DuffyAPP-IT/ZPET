//
//  module.cpp
//  ZPET
//
//  Created by James Duffy on 21/10/2020.
//  Copyright © 2020 James Duffy. All rights reserved.
//

#include "module.hpp"
#include "misc.h"
#include "device.hpp"

//
// Created by 010011 on 06/10/2020.
//

#include <iostream>
#include <fstream>
#include <string>


bool is_file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

/*
 * Module::validate
 *
 * Module display name should be longer than 3 characters...
 * Module description should be longer than 10 characters...
 * foi should not be null
 * parsetype should be valid (sqlite,plist,json,etc)
 *
 */
int Module::validate() {
    if(displayname.length()>3){
        //Display Name Valid
        if(description.length()>10){
            //Description Valid
            if(foi.size()>0){
                //foi larger than 0 bytes
                if(parsetype == "sqlite" || parsetype == "json" || parsetype == "plist" || parsetype == "exec"){
                    //parsetype is valid
                    return 0;
                } else{
                    std::cout << "Parse Type Invalid\n";
                    return 1;
                }
            } else{
                std::cout << "FOI Not Defined\n";
                return 1;
            }
        } else{
            std::cout << "Module Description Invalid\n";
            return 1;
        }
    } else{
        std::cout << "Module Name Invalid\n";
        return 1;
    }
}

void Module::info() {
    std::cout << "Module Name: " << displayname << "\nDescription: " << description << "\nFOI: " << foi << "\nParser: " << parsetype << "\nFilter Val: " << filterval << "\nexecend: " << execend << "\npipeprocess: " << pipeprocess << std::endl;
}

Module loadModule(std::string modsrctxt){
    //hold txt data
    std::string line;
    //initialise stream to input module file
    std::ifstream modsrc (modsrctxt);
    //init line number track
    int linenum = 0;
    Module mod;
    if (modsrc.is_open())
    {
        while (getline(modsrc,line) )
        {
            switch (linenum) {
                    case 0:
                    mod.displayname = line;
                    case 1:
                    mod.description = line;
                    case 2:
                    mod.author = line;
                    case 3:
                    mod.foi = line;
                    case 4:
                    mod.parsetype = line;
                    case 5:
                    mod.filterval = line;
                    case 6:
                    mod.execend = line;
                    case 7:
                    mod.pipeprocess = line;
            }
            linenum++;
        }
        modsrc.close();
    }
    return mod;
};


void process_modules(Device device){
    int loadedmodcount=0;
    /*
     Counts modules listed in moduleloader txt to initialise
     a counter (how many modules to import)
     */
    std::cout << "\n[+] Importing Modules... ";
    sleep(1);
    Module mods[countLinesInTxt("modules/moduleloader")];
    //Populate mods array with modules while counting imported mods
    int linenum = 0;
    std::string line;
    //Open streams for the moduleloader (for read) - loadertxt used elsewhere!
    std::ifstream loadertxt("modules/moduleloader");
    std::ifstream popmodtxts("modules/moduleloader");
    if (popmodtxts.is_open()) {
        while (getline(popmodtxts, line)) {
            std::string addedsubdir = "modules/" + line; //adds folder reference + line contents (filename of module)... example would be 'modules/wifi'
            mods[linenum] = loadModule(addedsubdir);
            linenum++;
        }
        popmodtxts.close();
        loadedmodcount = linenum;
        std::cout << " Imported " << loadedmodcount << " modules!" << std::endl;
    }
    
    
    int err_count = 0; //if error limit is hit, stop execution.
    int err_limit = 3;
    for(int i=0;i<(loadedmodcount);i++){
        if(err_count>=err_limit){
            std::cout << "[!] Error Limit Hit...Exiting For Safety - (check moduleloader!)" << std::endl;
            exit(1);
        }
        if(mods[i].validate()==0){
            //            mods[i].info();
            std::cout << "\n[*] Module -> " << mods[i].displayname << "\n[*] Author -> " << mods[i].author << std::endl;
            sleep(1);
            std::cout << "[*] Module Output -> ...\n----------\n";
            if(scanHandler(mods[i],device.ip_addr,device.port,device.ssh_pw)!=0){
                err_count++;
                std::cout << "[!] Module " << i << " (" << mods[i].displayname << ") Sent For Processing But Did NOT Complete! Is it up to date? " << std::endl;
            }
            std::cout << "----------" << std::endl;
        } else{
            std::cout << "=========\n[!] Module" << i << " did not pass the initial validator!\n=========\n";
            if(load_consent_data()=="y") submit_event("userProcess:moduleValidateError");
        }
    }
}
