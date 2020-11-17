//
//  misc.h
//  ZPET
//
//  Created by James Duffy on 21/10/2020.
//  Copyright © 2020 James Duffy. All rights reserved.
//

#ifndef misc_h
#define misc_h



//Standard Library Imports
#include <iostream>
#include <cstdlib>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

//Platform specific definitions
#if __APPLE__
//Compat Defines
#define PLIST_COMPAT 1
#define SQLITE_COMPAT 1
#define EXEC_COMPAT 1
#define JSON_COMPAT 1
#define OS 1
#define plist "/usr/libexec/plistbuddy"

#endif
#if __linux__
//Compat Defines
#define PLIST_COMPAT 0
#define SQLITE_COMPAT 1
#define EXEC_COMPAT 1
#define JSON_COMPAT 1
#define plist "plutil"
#define OS 2
#endif


#include "module.hpp"
int verifyPrereqs();


int countLinesInTxt(char *txtname);
int scanHandler(Module mod,const std::string& DEVICEIP,const std::string& DEVICEPORT,const std::string& DEVICEPW);

int iosReceive(std::string foi,std::string deviceip,std::string devicepwd);

int macOS_GetExit(std::string command);


#endif /* misc_h */
