#include "helpers.h"

#include <stdlib.h>
#include <stdio.h>
#include "settings.h"

void printHelp(){
    printf("This is GPU version of program for N-body simulation\n");
    printf("Supported arguments :\n");
    printf("\t-s sets the ID of first frame[optional]\n");
    printf("\t-N sets bodys amount\n");
    printf("\t-f sets frames amount\n");
    printf("\t-w sets write step(frequency of file generation)\n");
    printf("\t-t sets threads amount\n");
    printf("\t-B runs program in bencmarking mode(deactivates processing progress && backups)[optional]\n");
    printf("\t-b writes backups files[optional]\n");
    printf("\t-c sets input catalogue name\n");
    printf("\t-? or -h prints this help\n");
};

void * protectedMallocF(char const* arrName, unsigned int size){
    vec3d * tmp = malloc(size);
    if(tmp == NULL){
        fprintf(stderr, "ERROR: Couldnt allocate memory for %s\n", arrName);
        exit(1);
    };
    return tmp;
};

frame * readFrame(char const* frameName, int N_BODYS){
    FILE *inp;
    inp = fopen(frameName, "r");
    if(inp == NULL){
        fprintf(stderr, "ERROR: Can't open file %s\n", frameName);
        exit(1);
    };
    
    frame *tmp = malloc(sizeof(frame));
    if(tmp == NULL){
        fprintf(stderr, "ERROR: Couldn't allocate memory for new frame\n");
        fclose(inp);
        exit(1);
    };
    
    tmp->coords = protectedMallocF("tmp->coords", sizeof(vec3d) * N_BODYS);
    tmp->vels = protectedMallocF("tmp->vels", sizeof(vec3d) * N_BODYS);
    tmp->accels = protectedMallocF("tmp->accels", sizeof(vec3d) * N_BODYS);
                    
                    
    for(int i = 0; i < N_BODYS; i++){
        if(fscanf(inp, "%e %E %E %E %E %E %E\n", &tmp->coords[i].len, &tmp->coords[i].x, &tmp->coords[i].y, &tmp->coords[i].z, &tmp->vels[i].x, &tmp->vels[i].y, &tmp->vels[i].z) != 7) {
            fprintf(stderr, "ERROR: Can't read file %s\n", frameName);
            free(tmp->coords);
            free(tmp->vels);
            free(tmp->accels);
            fclose(inp);
            exit(1);
        };
    };
    
    fclose(inp);
    return tmp;
};

void printFrame(frame const* fr, int N_BODYS){
    for(int i = 0; i < N_BODYS; i++){
        fprintf(stdout, "%e %E %E %E %E %E %E\n", fr->coords[i].len, fr->coords[i].x, fr->coords[i].y, fr->coords[i].z, fr->vels[i].x, fr->vels[i].y, fr->vels[i].z);
    };
};



void writeFrameFull(char const* frameName,const frame* fr, int N_BODYS){
    FILE * out = fopen(frameName, "w");
    if(out == NULL){
        fprintf(stderr, "ERROR: Can't open file %s\n", frameName);
        exit(1);
    };
    for(int i = 0; i < N_BODYS; i++){
        fprintf(out, "%le %E %E %E %E %E %E\n", fr->coords[i].len, fr->coords[i].x, fr->coords[i].y, fr->coords[i].z, fr->vels[i].x, fr->vels[i].y, fr->vels[i].z);
    };
    fclose(out);
};

void writeFrameShort(char const* frameName,const frame* fr, int N_BODYS){
    FILE * out = fopen(frameName, "w");
    if(out == NULL){
        fprintf(stderr, "ERROR: Can't open file %s\n", frameName);
        exit(1);
    };
    for(int i = 0; i < N_BODYS; i++){
        fprintf(out, "%E %E %E\n", fr->coords[i].x, fr->coords[i].y, fr->coords[i].z);
    };
    fclose(out);
};

void freeFrame(frame* fr){
    free(fr->coords);
    free(fr->vels);
    free(fr->accels);
};
