#define relid 100
#include <stdio.h>
#include <sys/stat.h>
#include <zconf.h>
#include "res.h"
#include "scanhandler.h"

void cleanmenu(int note, char *notedata){
    system("clear");
    printf("--------------------\nZPET Zero-Pin Live Extraction Toolkit\nDuffyAPP_IT\nRelease ID %d\n--------------------\n", relid);
    if (note !=0){
        printf("%s\n--------------------\n", notedata);
    }
}

//Generate Menu Items
const char *mainMenu[] = {"Setup Intructions",
                         "Quick Scan - B205",
                         "Extended Scan - B205",
                         "MapperV2 - LIVE - B205",
                         "Custom Scan - Disabled"
};

int main() {
        cleanmenu(1,"Initialising");
        system("rm -rf DATA 2>/dev/null");
        system("mkdir DATA");
        //DATA folder create success
        //firstlaunchcheck
        if( access( ".flaunch", F_OK ) != -1 ) {
            // firstlaunch already passed...
            printf("Welcome Back!\n");
        } else {
            // exec first launch intro...
            printf("Welcome To ZPET First-Launch...\n\n");
            if( access("welcome", F_OK ) != -1 ) {
                system("cat welcome");
                system("touch .flaunch");
                //Continue exec...
            } else{
                printf("Welcome was not available... Did you copy the entire ZPET Build?\n");
                exit(1);
            }
            printf("Press Enter To Accept The User Agreement & Continue...\n");
            //while loop waits for \n/return to be entered to continue execution.
            while( getchar() != '\n' );
        }
        cleanmenu(1,"Main Menu");
        for (int i = 0; i < ARRAY_SIZE(mainMenu); i++) {
            printf("[%d]: %s\n", i + 1, mainMenu[i]);
        }
        char *optchar;
        printf("-> ");
        fgets(&optchar, 50, stdin);
        int opt;
        opt = atoi(&optchar);
        if (opt <= ARRAY_SIZE(mainMenu)) {
            switch (opt) {
                case 1:
                    printf("See ZPET Release Notes\n");
                    break;
                case 2:
                    printf("Quick Scan\n");
                    scanhandle(2);
                    break;
                case 3:
                    printf("Extended Scan\n");
                    scanhandle(3);
                    break;
                case 4:
                    printf("MapperV2 LIVE\n");
                    scanhandle(4);
                    break;
                default:
                    printf("Invalid Option..\n");
                    exit(1);
            }
            return 0;
        }
}