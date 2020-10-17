//
// Created by 010011 on 06/10/2020.
//

#ifndef ZPETV2_MODULE_H
#define ZPETV2_MODULE_H

#include <iostream>
#include <fstream>
#include <string>

bool is_file_exist(const char *fileName);

class Module{
public:
    std::string displayname;
    std::string description;
    std::string foi;
    std::string execend;
    std::string pipeprocess;
    std::string parsetype;
    std::string filterval;
    std::string author;
    int validate();
    void info();
};

Module loadModule(std::string modsrctxt);

#endif //ZPETV2_MODULE_H
