//
// File: Lookuptable.cpp
//
// Code generated for Simulink model 'Lookuptable'.
//
// Model version                  : 1.5
// Simulink Coder version         : 8.10 (R2016a) 10-Feb-2016
// C/C++ source code generated on : Wed Sep 18 10:46:48 2019
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
// Validation result: Not run
//
#include "Lookuptable.h"

void rt_Spline2Derivd(const real_T *x, const real_T *y, uint32_T n, real_T *u,
                      real_T *y2);
real_T look_SplNLinSZd(uint32_T numDims, const real_T* u, const rt_LUTSplineWork
  * const SWork);
real_T intrp_NSpld(uint32_T numDims, const rt_LUTSplineWork * const splWork,
                   uint32_T extrapMethod);
uint32_T plook_linx(real_T u, const real_T bp[], uint32_T maxIndex, real_T
                    *fraction);
uint32_T linsearch_u32d(real_T u, const real_T bp[], uint32_T startIndex);

//
// Second derivative initialization function for spline
// for last dimension.
//
void rt_Spline2Derivd(const real_T *x, const real_T *y, uint32_T n, real_T *u,
                      real_T *y2)
{
  real_T p, qn, sig, un;
  uint32_T n1, i, k;
  n1 = n - 1U;
  y2[0U] = 0.0;
  u[0U] = 0.0;
  for (i = 1U; i < n1; i++) {
    real_T dxm1 = x[i] - x[i - 1U];
    real_T dxp1 = x[i + 1U] - x[i];
    real_T dxpm = dxp1 + dxm1;
    sig = dxm1 / dxpm;
    p = (sig * y2[i - 1U]) + 2.0;
    y2[i] = (sig - 1.0) / p;
    u[i] = ((y[i + 1U] - y[i]) / dxp1) - ((y[i] - y[i - 1U]) / dxm1);
    u[i] = (((6.0 * u[i]) / dxpm) - (sig * u[i - 1U])) / p;
  }

  qn = 0.0;
  un = 0.0;
  y2[n1] = (un - (qn * u[n1 - 1U])) / ((qn * y2[n1 - 1U]) + 1.0);
  for (k = n1; k > 0U; k--) {
    y2[k-1U] = (y2[k-1U] * y2[k]) + u[k-1U];
  }

  return;
}

// n-D Spline interpolation function
real_T look_SplNLinSZd(uint32_T numDims, const real_T* u, const rt_LUTSplineWork
  * const SWork)
{
  //
  //    n-D column-major table lookup operating on real_T with:
  //        - Spline interpolation
  //        - Spline extrapolation
  //        - Linear breakpoint search
  //        - Index search starts at the same place each time

  rt_LUTnWork * const TWork_look = SWork->m_TWork;
  real_T* const fraction = (real_T*) TWork_look->m_bpLambda;
  uint32_T* const bpIdx = TWork_look->m_bpIndex;
  const uint32_T* const maxIndex = TWork_look->m_maxIndex;
  uint32_T k;
  for (k = 0U; k < numDims; k++) {
    const real_T* const bpData = ((const real_T * const *)
      TWork_look->m_bpDataSet)[k];
    bpIdx[k] = plook_linx(u[k], &bpData[0], maxIndex[k], &fraction[k]);
  }

  return(intrp_NSpld(numDims, SWork, 3U));
}

// n-D natural spline calculation function
real_T intrp_NSpld(uint32_T numDims, const rt_LUTSplineWork * const splWork,
                   uint32_T extrapMethod)
{
  uint32_T il;
  uint32_T iu, k, i;
  real_T h, s, p, smsq, pmsq;

  // intermediate results work areas "this" and "next"
  const rt_LUTnWork *TWork_interp = (const rt_LUTnWork *)splWork->m_TWork;
  const real_T *fraction = (real_T *) TWork_interp->m_bpLambda;
  const real_T *yp = (real_T *) TWork_interp->m_tableData;
  real_T *yyA = (real_T *) splWork->m_yyA;
  real_T *yyB = (real_T *) splWork->m_yyB;
  real_T *yy2 = (real_T *) splWork->m_yy2;
  real_T *up = (real_T *) splWork->m_up;
  real_T *y2 = (real_T *) splWork->m_y2;
  const real_T **bpDataSet = (const real_T **) TWork_interp->m_bpDataSet;
  const real_T *xp = bpDataSet[0U];
  real_T *yy = yyA;
  uint32_T bufBank = 0U;
  uint32_T len = TWork_interp->m_maxIndex[0U] + 1U;

  // Generate at-point splines in each dimension
  for (k = 0U; k < numDims; k++ ) {
    // this dimension's input setup
    xp = bpDataSet[k];
    len = TWork_interp->m_maxIndex[k] + 1U;
    il = TWork_interp->m_bpIndex[k];
    iu = il + 1U;
    h = xp[iu] - xp[il];
    p = fraction[k];
    s = 1.0 - p;
    pmsq = p * ((p*p) - 1.0);
    smsq = s * ((s*s) - 1.0);

    //
    //  Calculate spline curves for input in this
    //  dimension at each value of the higher
    //  other dimensions\' points in the table.

    if ((p > 1.0) && (extrapMethod == 2U) ) {
      real_T slope;
      for (i = 0U; i < splWork->m_numYWorkElts[k]; i++) {
        slope = (yp[iu] - yp[il]) + ((y2[il]*h*h)*(1.0/6.0));
        yy[i] = yp[iu] + (slope * (p-1.0));
        yp = &yp[len];
        y2 = &y2[len];
      }
    } else if ((p < 0.0) && (extrapMethod == 2U) ) {
      real_T slope;
      for (i = 0U; i < splWork->m_numYWorkElts[k]; i++) {
        slope = (yp[iu] - yp[il]) - ((y2[iu]*h*h)*(1.0/6.0));
        yy[i] = yp[il] + (slope * p);
        yp = &yp[len];
        y2 = &y2[len];
      }
    } else {
      for (i = 0U; i < splWork->m_numYWorkElts[k]; i++) {
        yy[i] = yp[il] + p * (yp[iu] - yp[il]) +
          ((smsq * y2[il] + pmsq * y2[iu])*h*h)*(1.0/6.0);
        yp = &yp[len];
        y2 = &y2[len];
      }
    }

    // set pointers to new result and calculate second derivatives
    yp = yy;
    y2 = yy2;
    if (splWork->m_numYWorkElts[k+1U] > 0U ) {
      uint32_T nextLen = TWork_interp->m_maxIndex[k+1U] + 1U;
      const real_T *nextXp = bpDataSet[k+1U];
      for (i = 0U; i < splWork->m_numYWorkElts[k+1U]; i++) {
        rt_Spline2Derivd(nextXp, yp, nextLen, up, y2);
        yp = &yp[nextLen];
        y2 = &y2[nextLen];
      }
    }

    //
    //  Set work vectors yp, y2 and yy for next iteration;
    //  the yy just calculated becomes the yp in the
    //  next iteration, y2 was just calculated for these
    //  new points and the yy buffer is swapped to the space
    //  for storing the next iteration\'s results.

    yp = yy;
    y2 = yy2;

    //
    //  Swap buffers for next dimension and
    //  toggle bufBank for next iteration.

    if (bufBank == 0U) {
      yy = yyA;
      bufBank = 1U;
    } else {
      yy = yyB;
      bufBank = 0U;
    }
  }

  return( yp[0U] );
}

uint32_T plook_linx(real_T u, const real_T bp[], uint32_T maxIndex, real_T
                    *fraction)
{
  uint32_T bpIndex;

  // Prelookup - Index and Fraction
  // Index Search method: 'linear'
  // Extrapolation method: 'Linear'
  // Use previous index: 'off'
  // Use last breakpoint for index at or above upper limit: 'off'
  // Remove protection against out-of-range input in generated code: 'off'

  if (u <= bp[0U]) {
    bpIndex = 0U;
    *fraction = (u - bp[0U]) / (bp[1U] - bp[0U]);
  } else if (u < bp[maxIndex]) {
    bpIndex = linsearch_u32d(u, bp, maxIndex >> 1U);
    *fraction = (u - bp[bpIndex]) / (bp[bpIndex + 1U] - bp[bpIndex]);
  } else {
    bpIndex = maxIndex - 1U;
    *fraction = (u - bp[maxIndex - 1U]) / (bp[maxIndex] - bp[maxIndex - 1U]);
  }

  return bpIndex;
}

uint32_T linsearch_u32d(real_T u, const real_T bp[], uint32_T startIndex)
{
  uint32_T bpIndex;

  // Linear Search
  for (bpIndex = startIndex; u < bp[bpIndex]; bpIndex--) {
  }

  while (u >= bp[bpIndex + 1U]) {
    bpIndex++;
  }

  return bpIndex;
}

// Model step function
void LookuptableModelClass::step()
{
  // local block i/o variables
  real_T rtb_uDLookupTable2;

  // Lookup_n-D: '<Root>/2-D Lookup Table1'
  //  About '<Root>/2-D Lookup Table1':
  //        Table size:  8 x 5
  //     Interpolation:  Spline
  //     Extrapolation:  Spline
  //    Breakpt Search:  Linear
  //     Breakpt Cache:  OFF

  {
    real_T rt_LUTuVect[2];
    rt_LUTuVect[0] = rtU.Tnex;
    rt_LUTuVect[1] = rtU.Beta;
    rtb_uDLookupTable2 = look_SplNLinSZd(2U, rt_LUTuVect, (rt_LUTSplineWork*)
      &rtDW.SWork[0]);
  }

  // Outport: '<Root>/Qt'
  rtY.Qt = rtb_uDLookupTable2;

  // Lookup_n-D: '<Root>/2-D Lookup Table2'
  //  About '<Root>/2-D Lookup Table2':
  //        Table size:  7 x 10
  //     Interpolation:  Spline
  //     Extrapolation:  Spline
  //    Breakpt Search:  Linear
  //     Breakpt Cache:  OFF

  {
    real_T rt_LUTuVect[2];
    rt_LUTuVect[0] = rtU.T;
    rt_LUTuVect[1] = rtU.SOC;
    rtb_uDLookupTable2 = look_SplNLinSZd(2U, rt_LUTuVect, (rt_LUTSplineWork*)
      &rtDW.SWork_k[0]);
  }

  // Outport: '<Root>/R0'
  rtY.R0 = rtb_uDLookupTable2;
}

// Model initialize function
void LookuptableModelClass::initialize()
{
  // Start for Lookup_n-D: '<Root>/2-D Lookup Table1'
  {
    rt_LUTnWork *TWork_start = (rt_LUTnWork *) &rtDW.TWork[0];
    void **bpDataSet = (void **) &rtDW.m_bpDataSet[0];
    TWork_start->m_dimSizes = (const uint32_T *)
      rtConstP.uDLookupTable1_dimSizes;
    TWork_start->m_tableData = (void *) rtConstP.uDLookupTable1_tableData;
    TWork_start->m_bpDataSet = bpDataSet;
    TWork_start->m_bpIndex = &rtDW.m_bpIndex[0];
    TWork_start->m_bpLambda = &rtDW.m_bpLambda[0];
    TWork_start->m_maxIndex = (const uint32_T *)
      rtConstP.uDLookupTable1_maxIndex;
    bpDataSet[0] = (void *) rtConstP.uDLookupTable1_bp01Data;
    bpDataSet[1] = (void *) rtConstP.uDLookupTable1_bp02Data;
  }

  {
    const real_T **bpDataSet;
    const real_T *xp, *yp;
    real_T *up, *y2;
    uint32_T i, len;
    const rt_LUTnWork *TWork_interp;
    rt_LUTSplineWork *rt_SplWk = (rt_LUTSplineWork*)&rtDW.SWork[0];
    rt_SplWk->m_TWork = (rt_LUTnWork*)&rtDW.TWork[0];
    rt_SplWk->m_yyA = &rtDW.m_yyA[0];
    rt_SplWk->m_yyB = &rtDW.m_yyB[0];
    rt_SplWk->m_yy2 = &rtDW.m_yy2[0];
    rt_SplWk->m_up = &rtDW.m_up[0];
    rt_SplWk->m_y2 = &rtDW.m_y2[0];
    rt_SplWk->m_numYWorkElts = rtConstP.uDLookupTable1_numYWorkElts;

    // If table and bps are not-tunable then precalculate 1st dim 2nd deriv
    TWork_interp = (const rt_LUTnWork *)rt_SplWk->m_TWork;
    bpDataSet = (const real_T **) TWork_interp->m_bpDataSet;
    xp = bpDataSet[0U];
    yp = (real_T *) TWork_interp->m_tableData;
    len = TWork_interp->m_maxIndex[0U] + 1U;
    up = (real_T *) rt_SplWk->m_up;
    y2 = (real_T *) rt_SplWk->m_y2;

    // Generate first dimension's second derivatives
    for (i = 0U; i < rt_SplWk->m_numYWorkElts[0U]; i++) {
      rt_Spline2Derivd(xp, yp, len, up, y2);
      yp = &yp[len];
      y2 = &y2[len];
    }
  }

  // Start for Lookup_n-D: '<Root>/2-D Lookup Table2'
  {
    rt_LUTnWork *TWork_start = (rt_LUTnWork *) &rtDW.TWork_a[0];
    void **bpDataSet = (void **) &rtDW.m_bpDataSet_p[0];
    TWork_start->m_dimSizes = (const uint32_T *)
      rtConstP.uDLookupTable2_dimSizes;
    TWork_start->m_tableData = (void *) rtConstP.uDLookupTable2_tableData;
    TWork_start->m_bpDataSet = bpDataSet;
    TWork_start->m_bpIndex = &rtDW.m_bpIndex_n[0];
    TWork_start->m_bpLambda = &rtDW.m_bpLambda_o[0];
    TWork_start->m_maxIndex = (const uint32_T *)
      rtConstP.uDLookupTable2_maxIndex;
    bpDataSet[0] = (void *) rtConstP.uDLookupTable2_bp01Data;
    bpDataSet[1] = (void *) rtConstP.uDLookupTable2_bp02Data;
  }

  {
    const real_T **bpDataSet;
    const real_T *xp, *yp;
    real_T *up, *y2;
    uint32_T i, len;
    const rt_LUTnWork *TWork_interp;
    rt_LUTSplineWork *rt_SplWk = (rt_LUTSplineWork*)&rtDW.SWork_k[0];
    rt_SplWk->m_TWork = (rt_LUTnWork*)&rtDW.TWork_a[0];
    rt_SplWk->m_yyA = &rtDW.m_yyA_i[0];
    rt_SplWk->m_yyB = &rtDW.m_yyB_l[0];
    rt_SplWk->m_yy2 = &rtDW.m_yy2_k[0];
    rt_SplWk->m_up = &rtDW.m_up_f[0];
    rt_SplWk->m_y2 = &rtDW.m_y2_c[0];
    rt_SplWk->m_numYWorkElts = rtConstP.uDLookupTable2_numYWorkElts;

    // If table and bps are not-tunable then precalculate 1st dim 2nd deriv
    TWork_interp = (const rt_LUTnWork *)rt_SplWk->m_TWork;
    bpDataSet = (const real_T **) TWork_interp->m_bpDataSet;
    xp = bpDataSet[0U];
    yp = (real_T *) TWork_interp->m_tableData;
    len = TWork_interp->m_maxIndex[0U] + 1U;
    up = (real_T *) rt_SplWk->m_up;
    y2 = (real_T *) rt_SplWk->m_y2;

    // Generate first dimension's second derivatives
    for (i = 0U; i < rt_SplWk->m_numYWorkElts[0U]; i++) {
      rt_Spline2Derivd(xp, yp, len, up, y2);
      yp = &yp[len];
      y2 = &y2[len];
    }
  }
}

// Constructor
LookuptableModelClass::LookuptableModelClass()
{
}

// Destructor
LookuptableModelClass::~LookuptableModelClass()
{
  // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
