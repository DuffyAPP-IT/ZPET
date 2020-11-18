//
//  device.cpp
//  ZPET
//
//  Created by James Duffy on 17/11/2020.
//  Copyright © 2020 James Duffy. All rights reserved.
//

#include "device.hpp"
#include "misc.h"



Device init_device(std::string connection_type){
    //initialise device object, set connection type
    Device connected;
    connected.connection_type=connection_type;
    
    if(connection_type=="ssh"){
        // Initial Prerequesite Check
        if(check_binary_in_path("iproxy")==0){
            std::cout << "[+] Loaded iProxy" << std::endl;
            if(check_binary_in_path("plutil")==0){
                std::cout << "[+] Loaded plutil" << std::endl;
            } else return connected;
        } else return connected;
        
        std::cout << "[@] Prerequesite Checks Passed" << std::endl;
        
        std::cout << "[?] Device IP? ('usb' For USB-SSH Transmission): ";
        std::cin >> connected.ip_addr;
        if(connected.ip_addr=="usb") connected.ip_addr="127.0.0.1";
        
        std::cout << "[?] Device Port? ('44' For USB-SSH Checkra1n): ";
        std::cin >> connected.port;
        
        // Initialise iProxy if locally connected
        if(connected.ip_addr=="127.0.0.1"){
            std::string usbproxyconfig = "sudo iproxy 7788 " + connected.port + "  2>/dev/null >/dev/null &";
//            system(usbproxyconfig.c_str());
            const char *exec = usbproxyconfig.c_str();
            int ret = system(exec);
            if (WEXITSTATUS(ret) != 0){
                std::cout << "\n[!] iProxy Could Not Be Initialised Correctly...\nIs libimobiledevice Installed? (try 'brew install libimobiledevice')";
                return connected;
            } else std::cout << "[+] USB Proxy Initialised" << std::endl;
            if(connected.port=="44") connected.port="7788"; //Device To Locally Mapped Port
        }
        
        std::cout << "[?] Device SSH Password? ('alpine' Is The Default): ";
        std::cin >> connected.ssh_pw;
        
        std::cout << "[@] Testing Device Connection" << std::endl;
        if(iosReceive("/System/Library/CoreServices/SystemVersion.plist", connected.ip_addr, connected.ssh_pw)==0){
            std::cout << "[+] Connection Established" << std::endl;
        } else return connected;
        
        
        std::cout << "getting ios" << std::endl;
        connected.major_os_ver=std::atoi((macos_run_get_fline("plutil -extract ProductVersion xml1 -o -  SENSITIVE/local 2>&1 | grep string | cut -f2 -d'>' | cut -f1 -d'<' | cut -f1 -d'.'")));
        
        std::cout << "ios is major..." << connected.major_os_ver;
        
//        //BFU/AFU *basic* Analysis
//        if(iosReceive("/private/var/mobile/Library/CoreDuet/Knowledge/knowledgeC.db", connected.ip_addr, connected.ssh_pw)==0){
//            connected.crypt_state=2;
//        } else connected.crypt_state=1;
        
        
        //Device ID + Analysis Session
        connected.device_id="this_is_a_random_value";
        
        return connected;
        
        
    } else if(connection_type=="tar"){
        // Check tar validity
        
        return connected;
    } else {
        std::cout << "[!] Internal Error - Invalid Connection Method" << std::endl;
        connected.connection_type="ERROR";
        return connected;
    }
}
