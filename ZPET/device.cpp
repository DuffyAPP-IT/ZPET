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
        /*
         Prerequesite Checks - SSH
         Notes:
                * iProxy checked for USB-SSH initialisation
                * plutil checked for plist processing within function
         */
        if(check_binary_in_path("iproxy")==0){
            std::cout << "[+] Loaded iProxy" << std::endl;
            if(check_binary_in_path("plutil")==0){
                std::cout << "[+] Loaded plutil" << std::endl;
            } else return connected;
        } else return connected;
        std::cout << "\n[@] Prerequesite Checks Passed\n" << std::endl;
        
        /*
         Storing Device IP...
         Notes:
                * Where device IP is set to 'usb', the value of ip_addr becomes '127.0.0.1' - a little easier for the end user!
                * If an IP is manually specified, leave as is.
         */
        std::cout << "[?] Device IP? ('usb' For USB-SSH Transmission): ";
        std::cin >> connected.ip_addr;
        if(connected.ip_addr=="usb") connected.ip_addr="127.0.0.1";
        
        std::cout << "[?] Device Port? ('44' For USB-SSH Checkra1n): ";
        std::cin >> connected.port;
        
        /*
         Initialising USB SSH Proxy
         Notes:
                * Checks if ip_addr is 127.0.0.1 - If it's anything else, we're not connecting via USB
                * Redirect device port to localhost '7788' for ZPET to use as a communication channel
                * Exit code of iProxy checked, will assume error if return code was anything other than 0.
                * Finally re-assign 'port' variable to 7788, as the device port is now irrelevant due to the new mapping.
         */
        if(connected.ip_addr=="127.0.0.1"){
            if(connected.port != "3022"){ //EIFT Port Exclusion
                std::string usbproxyconfig = "sudo iproxy 7788 " + connected.port + "  2>/dev/null >/dev/null &";
    //            system(usbproxyconfig.c_str());
                const char *exec = usbproxyconfig.c_str();
                int ret = system(exec);
                if (WEXITSTATUS(ret) != 0){
                    std::cout << "\n[!] iProxy Could Not Be Initialised Correctly...\nIs libimobiledevice Installed? (try 'brew install libimobiledevice')";
                    connected.can_connnect=false;
                    return connected;
                } else std::cout << "[+] USB Proxy Initialised" << std::endl;
                if(connected.port=="44") connected.port="7788"; //Device To Locally Mapped Port
            }
        }
        
        std::cout << "[?] Device SSH Password? ('alpine' Is The Default): ";
        std::cin >> connected.ssh_pw;
        
        // Call to iosRecieve, pull SystemVersion.plist - if that's a success, we have access!
        std::cout << "[@] Testing Device Connection" << std::endl;
        if(iosReceive("/System/Library/CoreServices/SystemVersion.plist", connected.ip_addr, connected.ssh_pw, connected.port)==0){
            std::cout << "[+] Connection Established" << std::endl;
            connected.can_connnect=true;
        } else{
            std::cout << "[!] didnt retrieve :(" << std::endl;
            connected.can_connnect=false;
            return connected;
        }
        
        //Using the just retrieved SystemVersion.plist, grab the major iOS version!
        connected.major_os_ver=std::atoi((macos_run_get_fline("plutil -extract ProductVersion xml1 -o -  SENSITIVE/local 2>&1 | grep string | cut -f2 -d'>' | cut -f1 -d'<' | cut -f1 -d'.'")));
        
        //BFU/AFU *basic* Analysis
        //Attempt to retrieve a 'protected' file as per nsfileprotection rules.
        //Should fail if BFU!
        if(iosReceive("/private/var/mobile/Library/CoreDuet/Knowledge/knowledgeC.db", connected.ip_addr, connected.ssh_pw, connected.port)==0){
            connected.crypt_state=2;
        } else connected.crypt_state=1;
        
        //Device ID + Analysis Session
        //Randomised ID for the current device within the current session
        //Need to configure this! placeholder for now. maybe hash output sent to a file too.
        connected.device_id=(rand() % 100000 + 1);
        
        connected.display_name="Research iPhone";
        
        //complete!
        return connected;
        
    } else if(connection_type=="tar"){
        // Check tar validity
        connected.can_connnect=false;
        return connected;
    } else {
        std::cout << "[!] Internal Error - Invalid Connection Method" << std::endl;
        connected.connection_type="ERROR";
        return connected;
    }
}

void Device::info(){
    std::cout << "\n[*]\tCurrent Session ID\t" << device_id << std::endl;
    std::cout << "[*]\tConnection Type\t\t" << connection_type << std::endl;
    std::cout << "[*]\tConnection State\t";
    if(can_connnect) std::cout << "Connected!" << std::endl;
    if(!can_connnect) std::cout << "Disconnected!" << std::endl;
    
    std::cout << "\n[*]\tDisplay Name\t" << display_name << std::endl;
    std::cout << "[*]\tiOS Version\t" << major_os_ver << std::endl;
    std::cout << "[*]\tEncryption State\t";
    if(crypt_state==1) std::cout << "Encrypted (BFU)" << std::endl;
    if(crypt_state==2) std::cout << "Decrypted (AFU)" << std::endl;
    
    std::cout << "\n[*]\tIP Address\t" << ip_addr << std::endl;
    std::cout << "[*]\tDevice Port\t" << port << std::endl;
    std::cout << "[*]\tSSH Password\t" << ssh_pw << std::endl;
}
