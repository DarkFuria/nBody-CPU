#include "src/settings.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include "src/helpers.h"
#include "src/cpuModel.h"

int main(int argc, char* argv[]){
    if(argc != 4){
        fprintf(stderr, "Usage: ./nbody cat.csv framesAmount writeRate\n");
        exit(1);
    };
    
    frame* test = readFrame(argv[1]);
    int pathLen = sizeof("out/out000000000.csv");
    char path[pathLen];
    printFrame(test);
   
    double ** m = prepareGravitationalParameters(test->masses);
    
    unsigned int FRAMES_AMOUNT = atoi(argv[2]);
    if(FRAMES_AMOUNT < 1){
        fprintf(stderr, "ERROR: Too small frames amount\n");
        exit(2);
    };
    fprintf(stdout, "%d frames will be generated used\n", FRAMES_AMOUNT);
    int WRITE_STEP = atoi(argv[3]);
    if(WRITE_STEP < 1){
        fprintf(stderr, "ERROR: Too small writing step\n");
        exit(1);
    };
    fprintf(stdout, "Write step %d will be used\n", WRITE_STEP);
    fprintf(stdout, "Integration step %f will be used\n", DELTA_T);
    fprintf(stdout, "%d integration steps per virtual second will be used\n", (int)(1 / DELTA_T));
    sleep(3);
    tmpData* tmp = initTmpData();
   
    
    for(int i = 0; i < FRAMES_AMOUNT; i++){
        for(int j = 0; j < WRITE_STEP; j++){
			for(long int k = 0; k < 1000000; k++){
				updateFrame(test, (const double **)m, tmp, DELTA_T);
			};
        };
        if(snprintf(path, sizeof(path), "out/out%09d.csv", i) != pathLen - 1){
            fprintf(stderr, "ERROR: Can't generate filename\n");
            fprintf(stderr, "PathLen: %d\n", pathLen);
            exit(1);
        };
        path[pathLen - 1] = '\0';
        writeFrameShort(path, test);
        if(i % (FRAMES_AMOUNT / 100) == 0){
			fprintf(stdout, "%d %% of frames generated\n", i / (FRAMES_AMOUNT / 100));
		};
    };
    
    fprintf(stdout, "DONE\n");
    
    printFrame(test);
    
    writeFrameFull("result.csv", test);
	freeTmpData(tmp);
	free(tmp);
    freeSquareMatrix(m);
    freeFrame(test);
    free(test);
    return 0;
};
