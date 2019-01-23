#ifndef CPU_MODEL_H
#define CPU_MODEL_H

vec3d calculateInteraction(vec3d bodyI, vec3d bodyJ, vec3d accel);

void calculateAlterations(vec3d * bodys, vec3d *accels, int N_BODYS);

void updateCoordinates(vec3d * bodys, vec3d * vels, vec3d * accels, float dt, int N_BODYS);

#endif
