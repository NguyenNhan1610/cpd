#include <stdarg.h>
#include <mex.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include <string.h> /* memset */
#include "misc.h"
#include "vdcpd.h"
#include "udcpd.h"

//static int verbose;

/*=========================================================
 * genVDCPDMex.c -
 * Code for uniform density complementary poisson-disc
 * or Poisson DISCO sampling
 * Usage:
 *
 * M = genVDCPDMex(numMasks, alph, int32(feasiblePoints), shapeOpt, verbose, C, mindist_scaling, Rmax, vd_exp);
 *
 * INPUTS: 
 * numMasks       = # regions over which to distribute samples
 * FOVRatio       = Square of anisotropy factor FOVz / FOVy
 * feasiblePoints = [ny nz] matrix of feasible points,
 *                  feasiblePoints(i,j) = 1 if (i,j) sample
 *                  should be in one mask of M, 0 otherwise
 * C              = dt_min / dky_min, parameter to balance min 
 *                  distance
 * mindistky      = scaling for min dist 
 * Rmax           = Reduction factor at krmax
 * vd_exp         = 1/kr^vd_exp density
 *
 * OUTPUTS:
 * M              = [ny nz nt] sampling pattern. sum(M,3)
 *                  should be equal to feasiblePoints
 *
 * References:
 *
 *
 * Evan Levine
 * egl@stanford.edu
 % Stanford University
 *
 * This is a MEX-file for MATLAB.
 *=======================================================*/

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{

if(nrhs != 9){ /* Check the number of arguments */
    mexErrMsgTxt("Wrong number of input arguments.");
}else if(nlhs > 1){
    mexErrMsgTxt("Too many output arguments.");
}

/* INPUTS */
const int nt = mxGetScalar(prhs[0]);
const double FOVRatio = mxGetScalar(prhs[1]);
const mxArray *feasiblePointsArr = prhs[2];
const long shapeOpt = mxGetScalar(prhs[3]);
verbose = mxGetScalar(prhs[4]);
const int *feasiblePoints = (int *) mxGetPr(feasiblePointsArr);
long i;
const int *size = mxGetDimensions(feasiblePointsArr);
const double C = mxGetScalar(prhs[5]);
const double mindist_scale  = mxGetScalar(prhs[6]);
const double Rmax = mxGetScalar(prhs[7]);
const double vd_exp = mxGetScalar(prhs[8]);

mwSize dimsMw[3];
long dims[3];
dims[Y_DIM] = dimsMw[0] = size[0];
dims[Z_DIM] = dimsMw[1] = size[1];
dims[T_DIM] = dimsMw[2] = nt;
const int isPeriodicInK = 0; /* use for periodic boundary conditions in k-dimension */
    

/* OUTPUTS */
mxArray *outArr;
if( (outArr = mxCreateNumericArray( 3, dimsMw, mxINT32_CLASS, mxREAL )) == NULL){
    debug_printf("Failed to allocate output\n");
    return;
}
int *out = (int *) mxGetPr(outArr);

genVDCPD(dims, out, feasiblePoints, FOVRatio, C, shapeOpt, mindist_scale, vd_exp, Rmax);

plhs[0] = outArr;
}


