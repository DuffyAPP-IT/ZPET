//
// Created by 010010 on 28/05/2020.
//

#include <zconf.h>
#include "processor.h"
#include "res.h"

int jsonproc(module mod){
    {
        printf("Loading Plugin -> %s\n", mod.name);
        modinfo(mod);
//        printf("\n//////////OUTPUT START//////////\n");
        //recieve relevant file
        if(mod.parsetype!=1){
            printf("INCORRECT TYPE\n");
            exit(1);
        }
        if(ios_rec_f(mod.foi,"Data/import.json")==0){
            char comm[1200];
            sprintf(comm,"echo WAIT");
            system(comm);
        } else{
            printf("Module ERROR\n");
//            exit(1);
        }
        if(mod.useendcust==1){
            system(mod.endcust);
        }
//        printf("\n//////////OUTPUT END//////////\n");
//        system("rm Data/import.json");
    }
}

int sqliteproc(module mod){
    modinfo(mod);
    printf("Loading Plugin -> %s\n", mod.name);
    modinfo(mod);
//    printf("\n//////////OUTPUT START//////////\n");
    //recieve relevant file
    if(mod.parsetype!=2){
        printf("INCORRECT TYPE\n");
        exit(1);
    }
    ios_rec_f(mod.foi,"Data/import.sqlite");
    if(mod.usecust==1){
        system(mod.cust);
    }
    char comm[1200];
    sleep(10);
    sprintf(comm,"echo '<ul>' \>\> report.html ; sqlite3 Data/import.sqlite '%s' | awk '{print \"\<li\>\"$0\"\<\/li\>\"}' \>\> report.html ; echo '\<\/ul\>' \>\> report.html",mod.sql);
    system(comm);
//    printf("\n//////////OUTPUT END//////////\n");
    system("rm Data/import.sqlite");
    //add check for type before exec
}

int plistproc(module mod){
    printf("Loading Plugin -> %s\n", mod.name);
    modinfo(mod);
//    printf("\n//////////OUTPUT START//////////\n");
    //recieve relevant file
    if(mod.parsetype!=3){
        printf("INCORRECT TYPE\n");
        exit(1);
    }
    ios_rec_f(mod.foi,"Data/import.plist");
    if(mod.usecust==1){
        system(mod.cust);
    }
    char comm[1200];
    sprintf(comm,"echo '<ul>' \>\> report.html ; /usr/libexec/PlistBuddy -c 'print \"%s:\"' Data/import.plist | grep -a %s | cut -f2- -d'=' | awk '{print \"\<li\>\"$0\"\<\/li\>\"}' \>\> report.html ; echo '\<\/ul\>' \>\> report.html",mod.dict,mod.sval);
    system(comm);
//    printf("\n//////////OUTPUT END//////////\n");
    system("rm Data/import.plist");
}

int ptextproc(module mod){

}

int custproc(module mod){
    {
        printf("Loading Plugin -> %s\n", mod.name);
        modinfo(mod);
//        printf("\n//////////OUTPUT START//////////\n");
        //recieve relevant file
        if(mod.parsetype!=4){
            printf("INCORRECT TYPE\n");
            exit(1);
        }
        ios_rec_f(mod.foi,"Data/OUT");
        if(mod.usefoi2 == 1){
            ios_rec_f(mod.foi2,"Data/OUT");
        }
        char comm[1200];
        sprintf(comm,mod.cust);
        system(comm);
        if(mod.useendcust==1){
            system(mod.endcust);
        }
//        printf("\n//////////OUTPUT END//////////\n");
//        system("rm -rf Data/import.json");
    }
}
