#include "helpers.h"
#include "cpuModel.h"
#include "settings.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>


vec3d calculateInteraction(vec3d bodyI, vec3d bodyJ, vec3d accel){
	// calculating distances [8 FLOPS]
	vec3d dist;
	dist.x = bodyJ.x - bodyI.x;
	dist.y = bodyJ.y - bodyI.y;
	dist.z = bodyJ.z - bodyI.z;
	dist.len = sqrtf(dist.x * dist.x + dist.y * dist.y + dist.z * dist.z + EPSILON * EPSILON);
	
	// calculating alteration [3 FLOPS]
	float a = bodyJ.len * G / (dist.len * dist.len);
	
	// updating acceleration [9 FLOPS]
    accel.x += a * dist.x / dist.len;
    accel.y += a * dist.y / dist.len;
    accel.z += a * dist.z / dist.len;
    return accel;
};

void calculateAlterations(vec3d * bodys, vec3d *accels, int N_BODYS){
	for(int i = 0; i < N_BODYS; i++){
		vec3d accel = {0.0f, 0.0f, 0.0f, 0.0f};
		for(int j = 0; j < N_BODYS; j++){
			calculateInteraction(bodys[i], bodys[j], accel);
		};
		accels[i] = accel;
	};
};

void updateCoordinates(vec3d * coords, vec3d * vels, vec3d * accels, float dt, int N_BODYS){
	for(int i = 0; i < N_BODYS; i++){
		// updating velosity
        vels[i].x += accels[i].x * dt;
        vels[i].y += accels[i].y * dt;
        vels[i].z += accels[i].z * dt;

        // updating coords
        coords[i].x += vels[i].x * dt;
        coords[i].y += vels[i].y * dt;
        coords[i].z += vels[i].z * dt;

	};	
};