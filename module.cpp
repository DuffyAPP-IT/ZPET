//
// Created by 010011 on 06/10/2020.
//

#include "module.h"
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
