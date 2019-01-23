#include "src/settings.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>

#include "src/helpers.h"
#include "src/cpuModel.h"

int main(int argc, char* argv[]){
    int startID = 0;
    int N_BODYS = 0;
    int FRAMES_AMOUNT = 0;
    int WRITE_STEP = 0;
    bool benchmark = false;
    bool writeBackups = 0;
    char *catName = NULL;

    // supported options
    const char *optString = "s:N:f:w:Bbc:h?";
    
    int opt = getopt(argc, argv, optString);
    while(opt != -1){
        switch(opt){
            case 's':
                startID = atoi(optarg);
                break;
            case 'N':
                N_BODYS = atoi(optarg);
                break;
            case 'f':
                FRAMES_AMOUNT = atoi(optarg);
                break;
            case 'w':
                WRITE_STEP = atoi(optarg);
                break;
            case 'B':
                benchmark = true;
                break;
            case 'b':
                writeBackups = true;
                break;
            case 'c':
                catName = optarg;
                break;
            case 'h':
            case '?':
                printHelp();
                return 0;
        };
        opt = getopt(argc, argv, optString);
    };

    if(N_BODYS < 1){
        fprintf(stderr, "ERROR: Too few bodys: %d\n", N_BODYS);
        return -1;
    };
    if(FRAMES_AMOUNT < 1){
        fprintf(stderr, "ERROR: Too few frames: %d\n", FRAMES_AMOUNT);
        return -1;
    };
    if(WRITE_STEP < 1){
        fprintf(stderr, "ERROR: Wrong writing frequency: %d\n", WRITE_STEP);
        return -1;
    };
    
    frame* test = readFrame(catName, N_BODYS);
    
    int pathLen = sizeof("out/out000000000.csv");
    char path[pathLen];

    // for backups
    int backupPathLen = sizeof("backup/back00000000.csv");
    char backupPath[backupPathLen];
    
    for(int i = startID; i < startID + FRAMES_AMOUNT; i++){
        for(int j = 0; j < WRITE_STEP; j++){
			calculateAlterations(test->coords, test->accels, N_BODYS);
            updateCoordinates(test->coords, test->vels, test->accels, DELTA_T, N_BODYS);
        };
        if(snprintf(path, sizeof(path), "out/out%09d.csv", i) != pathLen - 1){
            fprintf(stderr, "ERROR: Can't generate filename\n");
            fprintf(stderr, "PathLen: %d\n", pathLen);
            exit(1);
        };
        path[pathLen - 1] = '\0';
        writeFrameShort(path, test, N_BODYS);

        if(!benchmark){
            printf("\r");
            int progress = (int)(((float)i / FRAMES_AMOUNT) * 100);
            int i;
            for(i = 0; i < progress; i+=5){
                printf("#");
            };
            for(; i < 100; i+=5){
                printf(" ");
            };

            if(progress % 5 == 0 && writeBackups && !benchmark){
                if(sprintf(backupPath, "backup/back%08d.csv", i) != backupPathLen- 1){
                    fprintf(stderr, "\nERROR: Can't generate filename\n");
                    fprintf(stderr, "PathLen: %d\n", pathLen);
                    exit(1);
                };
                backupPath[backupPathLen - 1] = '\0';
                writeFrameFull(backupPath, test, N_BODYS);

            };
        };


    };
    
    
    writeFrameFull("result.csv", test, N_BODYS);
    freeFrame(test);
    free(test);
    fprintf(stdout, "\nDONE\n");
    return 0;
};
