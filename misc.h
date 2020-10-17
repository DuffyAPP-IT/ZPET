//
// Created by 010011 on 06/10/2020.
//


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



#ifndef ZPETV2_MISC_H
#define ZPETV2_MISC_H

#include "module.h"

int verifyPrereqs();


int countLinesInTxt(char *txtname);
int scanHandler(Module mod,const std::string& DEVICEIP,const std::string& DEVICEPORT,const std::string& DEVICEPW);

int iosReceive(std::string foi,std::string deviceip,std::string devicepwd);

int macOS_GetExit(std::string command);

#endif //ZPETV2_MISC_H
