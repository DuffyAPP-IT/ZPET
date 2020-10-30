//
//  processor.hpp
//  ZPET
//
//  Created by James Duffy on 21/10/2020.
//  Copyright © 2020 James Duffy. All rights reserved.
//

#ifndef processor_hpp
#define processor_hpp

#include <iostream>

#define plistprocver 1
#define sqliteprocver 1
#define jsonprocver 1
#define execprocver 1

int plistproc(std::string foi, std::string key, std::string piperesult, std::string IP, std::string PORT, std::string DEVICEPW);

int sqliteproc(std::string foi, std::string query, std::string piperesult, std::string IP, std::string PORT, std::string DEVICEPW);

int jsonproc(std::string foi, std::string sval, std::string IP, std::string PORT, std::string DEVICEPW);

int execproc(std::string foi, std::string exec, std::string piperesult, std::string IP, std::string PORT, std::string DEVICEPW, std::string EXECEND);

#endif /* processor_hpp */
