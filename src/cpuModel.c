#include "helpers.h"
#include "cpuModel.h"
#include "settings.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double** prepareGravitationalParameters(double * mass){
    if(mass == NULL){
        fprintf(stderr, "ERROR: mass array is NULL\n");
        exit(1);
    };
    
    double** res = protectedMallocF("Mass Array", sizeof(double*) * N_BODYS);
    for(int i = 0; i < N_BODYS; i++){
        res[i] = protectedMallocF("Mass Array[i]", sizeof(double) * N_BODYS);
        for(int j = 0; j < N_BODYS; j++){
            res[i][j] = (i == j) ? 0 : G * mass[i] * mass[j];
        };
    };

    return res;
};

vec3d* initVec3dMatr(){
	vec3d * res = malloc(sizeof(vec3d));
	res->x = malloc(sizeof(double*) * N_BODYS);
	if(res->x == NULL){
		fprintf(stderr, "ERROR: Couldn't allocate memory for res->x\n");
		exit(1);
	};
	res->y = malloc(sizeof(double*) * N_BODYS);
	if(res->y == NULL){
		fprintf(stderr, "ERROR: Couldn't allocate memory for res->y\n");
		exit(1);
	};
	res->z = malloc(sizeof(double*) * N_BODYS);
	if(res->z == NULL){
		fprintf(stderr, "ERROR: Couldn't allocate memory for res->z\n");
		exit(1);
	};
	res->len = malloc(sizeof(double*) * N_BODYS);
	if(res->len == NULL){
		fprintf(stderr, "ERROR: Couldn't allocate memory for res->len\n");
		exit(1);
	};
	for(int i = 0; i < N_BODYS; i++){
		res->x[i] = malloc(sizeof(double) * N_BODYS);
		if(res->x[i] == NULL){
			fprintf(stderr, "ERROR: Couldn't allocate memory for res->x[i]\n");
			exit(1);
		};
		
		res->y[i] = malloc(sizeof(double) * N_BODYS);
		if(res->y[i] == NULL){
			fprintf(stderr, "ERROR: Couldn't allocate memory for res->y[i]\n");
			exit(1);
		};
		
		res->z[i] = malloc(sizeof(double) * N_BODYS);
		if(res->z[i] == NULL){
			fprintf(stderr, "ERROR: Couldn't allocate memory for res->z[i]\n");
			exit(1);
		};
		
		res->len[i] = malloc(sizeof(double) * N_BODYS);
		if(res->len[i] == NULL){
			fprintf(stderr, "ERROR: Couldn't allocate memory for res->len[i]\n");
			exit(1);
		};
	};
	
	return res;
};

tmpData* initTmpData(){
	tmpData * res = malloc(sizeof(tmpData));
	// init distances
	res->distances = initVec3dMatr();
	res->interactions = initVec3dMatr();
	
	return res;
};

void freeVec3dMatr(vec3d* v){
	for(int i = 0; i < N_BODYS; i++){
		free(v->x[i]);
		free(v->y[i]);
		free(v->z[i]);
		free(v->len[i]);
	};
	free(v->x);
	free(v->y);
	free(v->z);
	free(v->len);
};

void freeTmpData(tmpData* tmp){
	freeVec3dMatr(tmp->distances);
	freeVec3dMatr(tmp->interactions);
	free(tmp->distances);
	free(tmp->interactions);
};

void calculateDistArray(double * X, double * Y, double * Z, vec3d * distances){
    if(X == NULL || Y == NULL || Z == NULL){
        fprintf(stderr, "ERROR: Coordinates array is NULL\n");
        exit(1);
    };
    for(int i = 0; i < N_BODYS; i++){
        for(int j = 0; j < N_BODYS; j++){
			double dx, dy, dz;
            dx = X[j] - X[i];
            dy = Y[j] - Y[i];
			dz = Z[j] - Z[i];
            
            distances->x[i][j] = dx;
            distances->y[i][j] = dy;
            distances->z[i][j] = dz;
            
            double r = sqrt(dx * dx + dy * dy + dz * dx);
            
            distances->len[i][j] = (r < EPSILON) ? EPSILON : r;
        };
    };
};

void calculateInteractions(const double** masses, vec3d * dist, vec3d* res){
    for(int i = 0; i < N_BODYS; i++){
        for(int j = 0; j < N_BODYS; j++){
            double force = masses[i][j] / (dist->len[i][j] * dist->len[i][j]);
            double fx = force * dist->x[i][j] / dist->len[i][j];
            double fy = force * dist->y[i][j] / dist->len[i][j];
            double fz = force * dist->z[i][j] / dist->len[i][j];
            
            res->x[i][j] = fx;
            res->y[i][j] = fy;
            res->z[i][j] = fz;
            res->len[i][j] = force;
        };
    };
};

double* calculateTotalForce(double ** F){
    double * totalForces = protectedMallocF("TotalForces", sizeof(double) * N_BODYS);
    
    // calculate total force
    for(int i = 0; i < N_BODYS; i++){
        totalForces[i] = 0; 
        for(int j = 0; j < N_BODYS; j++){
            totalForces[i] += F[i][j];
        };
    };
    
    return totalForces;
}; 

double* calculateAlteration(double* mass, double * force){
    double * alter = protectedMallocF("Alter", sizeof(double) * N_BODYS);
    for(int i = 0; i < N_BODYS; i++){
        alter[i] = force[i] / mass[i];
    };
    return alter;
};

void integrate(double * x, double * dx, double dt){
    for(int i = 0; i < N_BODYS; i++){
        x[i] += dt * dx[i];
    };
};

void initLeapfrog(frame * fr, tmpData *tmp, const double** masses, double dt){
	calculateDistArray(fr->x, fr->y, fr->z, tmp->distances);
	calculateInteractions(masses, tmp->distances, tmp->interactions);
	
	// update x position
	double* totalForceX = calculateTotalForce(tmp->interactions->x);
	double* altX = calculateAlteration(fr->masses, totalForceX);
	integrate(fr->vx, altX, dt / 2);
	free(totalForceX);
	free(altX);
	
	// update y position
	double* totalForceY = calculateTotalForce(tmp->interactions->y);
	double* altY = calculateAlteration(fr->masses, totalForceY);
	integrate(fr->vy, altY, dt / 2);
	free(totalForceY);
	free(altY);
	
	// update z position
	double* totalForceZ = calculateTotalForce(tmp->interactions->z);
	double* altZ = calculateAlteration(fr->masses, totalForceZ);
	integrate(fr->vz, altZ, dt / 2);
	free(totalForceZ);
	free(altZ);
};

void reverseLeapfrog(frame * fr, tmpData *tmp, const double** masses, double dt){
	initLeapfrog(fr, tmp, masses, -dt);
};

void updateFrame(frame * fr, const double** masses, tmpData* tmp, double dt){
	calculateDistArray(fr->x, fr->y, fr->z, tmp->distances);
	calculateInteractions(masses, tmp->distances, tmp->interactions);
	
	// update x position
	double* totalForceX = calculateTotalForce(tmp->interactions->x);
	double* altX = calculateAlteration(fr->masses, totalForceX);
	
	integrate(fr->vx, altX, DELTA_T);
	integrate(fr->x, fr->vx, DELTA_T);
	
	free(totalForceX);
	free(altX);
	
	// update y position
	double* totalForceY = calculateTotalForce(tmp->interactions->y);
	double* altY = calculateAlteration(fr->masses, totalForceY);

	integrate(fr->vy, altY, DELTA_T);
	integrate(fr->y, fr->vy, DELTA_T);

	free(totalForceY);
	free(altY);
	
	// update z position
	double* totalForceZ = calculateTotalForce(tmp->interactions->z);
	double* altZ = calculateAlteration(fr->masses, totalForceZ);

	integrate(fr->vz, altZ, DELTA_T);
	integrate(fr->z, fr->vz, DELTA_T);

	free(totalForceZ);
	free(altZ);
};

void freeSquareMatrix(double ** matrix){
    for(int i = 0; i < N_BODYS; i++){
        free(matrix[i]);
    };
    free(matrix);
};
