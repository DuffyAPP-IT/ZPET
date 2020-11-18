//
//  device.hpp
//  ZPET
//
//  Created by James Duffy on 17/11/2020.
//  Copyright © 2020 James Duffy. All rights reserved.
//

#ifndef device_hpp
#define device_hpp

#include <stdio.h>
#include <iostream>

class Device{
public:
    std::string connection_type; // SSH, TAR
    std::string display_name; // Research iPhone
    std::string ip_addr; // 127.0.0.1
    std::string port; // 7788
    std::string ssh_pw; // alpine
    
    std::string device_id; // Randomised ID for the current device within the current session
    
    int major_os_ver; // 12,13,14 etc.
    int crypt_state; // BFU=1, AFU=2
    
    int validate(); // checks current device data is valid and ready to be interpreted...
    
    bool can_connnect(); // checks for device access
    
    void info(); // pretty print device information
};

Device init_device(std::string connection_type);

#endif /* device_hpp */
