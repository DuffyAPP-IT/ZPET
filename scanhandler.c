//
// Created by 010010 on 28/05/2020.
//

#include "scanhandler.h"
#include "res.h"
#include "processor.h"
#include <stdio.h>

int scanhandle(int opt){
    cleanmenu(0,"");

    if(macos_run_ge("which iproxy")==0){
        printf("Starting Device Proxy\n");
        //could set custom port here.
        //add autokill
        if(macos_run_ge("iproxy 7788 44 &")==0){
            printf("Started Device Proxy\n");
        }
    } else{
        printf("iProxy Not Found...\nInstalling iProxy...\nDetecting Brew Installation...\n");
        if(macos_run_ge("brew test")==1){
            printf("Brew Detected... Installing libimobiledevice (iProxy)");
            system("brew install libimobiledevice");

            if(macos_run_ge("which iproxy")==0){
                printf("Starting Device Proxy\n");
                //could set custom port here.
                //add autokill
                if(macos_run_ge("iproxy 7788 44 &")==0){
                    printf("Started Device Proxy\n");
                }
            } else{
                printf("System Issue. Exiting For Safety...\n");
                exit(1);
            }
        } else{
            printf("Please head to https://brew.sh 1/3\nCopy the command in the center of the page 2/3\nLaunch macOS terminal, paste & run 3/3\nYou may then safely return to ZPET\n");
            system("open https://brew.sh");
        }
    }

    //Begin SecureLog
    if(securelog()==0){
        if(ios_fetch_access("127.0.0.1","7788")==0){
            switch(opt){
                case 2:
                    printf("Quick - B205\n");
                    macos_run_comm("rm report.html 2>/dev/null");
                    macos_run_comm("echo '\<html\> \<head\> \<style\> body \{font-family: arial\;\} \<\/style\> \<\/head\> \<body\>\<h1\>ZPET REPORT - DuffyAPP_IT - Quick Scan - B205\</h1\>' \>\> report.html");
                    //Here we define which processors to use for each struct defining each module.
                    plistproc(wifi());
                    sqliteproc(accounts());
                    plistproc(bsn());
                    plistproc(vpnd());
                    custproc(mailextract());
                    sqliteproc(wifiloc());
                    printf("Report available at -> %s/report.html\n", strtok(macos_run_comm("pwd"),"\n"));
                    break;
                case 3:
                    printf("Extended - B205\n");
                    macos_run_comm("rm report.html 2>/dev/null");
                    macos_run_comm("echo '\<html\> \<head\> \<style\> body \{font-family: arial\;\} \<\/style\> \<\/head\> \<body\>\<h1\>ZPET REPORT - DuffyAPP_IT - Extended - B205\</h1\>' \>\> report.html");
                    plistproc(wifi());
                    custproc(photocom());
                    sqliteproc(accounts());
                    sqliteproc(PCInfo());
                    plistproc(bsn());
                    sqliteproc(PCInfo());
                    sqliteproc(callhist());
                    plistproc(vpnd());
                    sqliteproc(appids());
                    jsonproc(wallet());
                    custproc(wapp());
                    custproc(mailvip());
                    printf("Report available at -> %s/report.html\n", strtok(macos_run_comm("pwd"),"\n"));
                    break;
                case 4:
                    printf("MapperV2 LIVE\nExecuting Media Finder...\n");
                    if(ios_send_f("Resources/MapperV2-Live.sh","/MAP")==0){
                        printf("Send MapperV2 LIVE! This can take an extended amount of time...\n");
                        ios_run_comm("chmod +x /MAP");
                        system("sshpass -p alpine ssh root@127.0.0.1 -p7788 /MAP");
                        ios_run_comm("rm /MAP");
                    } else{
                        printf("FAIL");
                    }
                    break;
                default:
                    printf("Invalid Value -> ScanHandler\n");
                    exit(1);
            }
        } else{
            printf("iDevice Connection Issue...\nCheck Setup Instructions..\n");
            exit(1);
        }
    } else{
        printf("Failed to send Secure Monitor\nExiting For Safety!\n");
    }

    if(rlogios()==0){
        printf("Logs Retrieved\n");
    } else{
        printf("Failed Retrieving Logs\n");
    }


    return 1;
}
