// Small functions which can be used in both versions: CPU and GPU

// All frame data will be saved in this structure
#ifndef HELPERS_H
#define HELPERS_H

typedef struct {
	float x;
	float y;
	float z;
	float len;
}vec3d;

typedef struct{
	vec3d *coords;
	vec3d *vels;
	vec3d *accels;
} frame;

// prints info about programm
void printHelp();

// function reads frame from csv file
frame * readFrame(char const* frameName, int N_BODYS);

void * protectedMallocF(char const* arrName, unsigned int size);

// function writes frame into csv file(with velocitys and masses)
void writeFrameFull(char const* frameName, const frame* fr, int N_BODYS);
// fucntion writes only coordinates into file
void writeFrameShort(char const* frameName, const frame* fr, int N_BODYS);

// finction prints frame
void printFrame(frame const* fr, int N_BODYS);

// function free's frame
void freeFrame(frame* fr);

#endif
