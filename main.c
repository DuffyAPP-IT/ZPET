#define relid 91
#include <stdio.h>
#include "res.h"
#include "scanhandler.h"

void cleanmenu(){
    system("clear");
    printf("--------------------\nZPET - DuffyAPP_IT\nRelease ID %d\n--------------------\n", relid);
}

//Generate Menu Items
const char *mainMenu[] = {"Setup Intructions",
                         "Quick Scan - B91",
                         "Extended Scan - B91",
                         "Custom Scan - Disabled"
};

int main() {
    cleanmenu();
    system("rm Data >/dev/null 2>/dev/null");
    system("mkdir Data >/dev/null 2>/dev/null");
    printf("\nMain Menu:\n");
    printf("");
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
                printf("See ZPET B9X RELNOTES PDF\n");
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
                printf("Spider\n");
                scanhandle(4);
                break;
            default:
                printf("Invalid Option..\n");
                exit(1);
        }
        return 0;
    }
}