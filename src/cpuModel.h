#ifndef CPU_MODEL_H
#define CPU_MODEL_H

typedef struct{
	double ** x;
	double ** y;
	double ** z;
	double ** len;
} vec3d;

vec3d* initVec3dMatr();
void freeVec3dMatr(vec3d* v);

typedef struct {
	vec3d * distances;
	vec3d * interactions;
} tmpData;

tmpData* initTmpData();
void freeTmpData(tmpData * tmp);

// creates matrix, each element is G * mass[i] * mass[j], there i and j - indexes of interacting bodys
// if i == j, matrix[i][j] == 0 
double** prepareGravitationalParameters(double * mass);

// creates v(1/2) from v(0)
void initLeapfrog(frame * fr, tmpData *tmp, const double** masses, double dt);

// reverces leeapfrog for 1/2 step to save correct values
void reverseLeapfrog(frame * fr, tmpData *tmp, const double** masses, double dt);

// calculates distance between elements in projection onto X, Y or Z axis
void calculateDistArray(double * X, double * Y, double * Z, vec3d * distances);

// calculates interactions based on gravitation law
// F = G * M1 * M2 / R^2
void calculateInteractions(const double** masses, vec3d * dist, vec3d* res);

// Calculates vector sum of forces acting on each body
double* calculateTotalForce(double ** F);

// Calculates alteration of the body
// a[i] = force[i] / mass[i]
double* calculateAlteration(double* mass, double * force);

// Integrates x using numerical methods
void integrate(double * x, double * dx, double dt);

// updates frame
void updateFrame(frame * fr, const double** masses, tmpData * tmp, double  dt);

// frees created matrix
void freeSquareMatrix(double ** matrix);

#endif
