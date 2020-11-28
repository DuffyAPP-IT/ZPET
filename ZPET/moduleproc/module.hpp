//
//  module.hpp
//  ZPET
//
//  Created by James Duffy on 21/10/2020.
//  Copyright © 2020 James Duffy. All rights reserved.
//

#ifndef module_hpp
#define module_hpp

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "../device.hpp"

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

void process_modules(Device device);

#endif /* module_hpp */
