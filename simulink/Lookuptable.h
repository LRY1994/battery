//
// File: Lookuptable.h
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
#ifndef RTW_HEADER_Lookuptable_h_
#define RTW_HEADER_Lookuptable_h_
#ifndef Lookuptable_COMMON_INCLUDES_
# define Lookuptable_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // Lookuptable_COMMON_INCLUDES_

#include "rtsplntypes.h"

// Macros for accessing real-time model data structure

// Block signals and states (auto storage) for system '<Root>'
typedef struct {
  real_T m_bpLambda[2];                // '<Root>/2-D Lookup Table1'
  real_T m_yyA[5];                     // '<Root>/2-D Lookup Table1'
  real_T m_yyB[5];                     // '<Root>/2-D Lookup Table1'
  real_T m_yy2[5];                     // '<Root>/2-D Lookup Table1'
  real_T m_up[8];                      // '<Root>/2-D Lookup Table1'
  real_T m_y2[40];                     // '<Root>/2-D Lookup Table1'
  real_T m_bpLambda_o[2];              // '<Root>/2-D Lookup Table2'
  real_T m_yyA_i[10];                  // '<Root>/2-D Lookup Table2'
  real_T m_yyB_l[10];                  // '<Root>/2-D Lookup Table2'
  real_T m_yy2_k[10];                  // '<Root>/2-D Lookup Table2'
  real_T m_up_f[10];                   // '<Root>/2-D Lookup Table2'
  real_T m_y2_c[70];                   // '<Root>/2-D Lookup Table2'
  void* m_bpDataSet[2];                // '<Root>/2-D Lookup Table1'
  void* TWork[6];                      // '<Root>/2-D Lookup Table1'
  void* SWork[9];                      // '<Root>/2-D Lookup Table1'
  void* m_bpDataSet_p[2];              // '<Root>/2-D Lookup Table2'
  void* TWork_a[6];                    // '<Root>/2-D Lookup Table2'
  void* SWork_k[9];                    // '<Root>/2-D Lookup Table2'
  uint32_T m_bpIndex[2];               // '<Root>/2-D Lookup Table1'
  uint32_T m_bpIndex_n[2];             // '<Root>/2-D Lookup Table2'
} DW;

// Constant parameters (auto storage)
typedef struct {
  // Expression: reshape([62.33,77.61,86.6,89.51,93.42,97.24,100.4,91.13,60.23,77.6,86.25,89.29,92.7,96.56,100,90.82,40.68,59.57,76.58,85.41,90.13,94.43,98.06,89.05,20.82,38.87,69.35,82.54,85.95,90.44,95.66,86.51,0,10.56,54.67,79.48,84.13,88.81,92.38,83.89],8,5)
  //  Referenced by: '<Root>/2-D Lookup Table1'

  real_T uDLookupTable1_tableData[40];

  // Expression: [-30,-20,-10,0,10,25,40,50]
  //  Referenced by: '<Root>/2-D Lookup Table1'

  real_T uDLookupTable1_bp01Data[8];

  // Expression: [0.3,1,2,3,4]
  //  Referenced by: '<Root>/2-D Lookup Table1'

  real_T uDLookupTable1_bp02Data[5];

  // Expression: reshape([15.7284,10.4653,5.8162,2.8337,2.66,2.6,3.35,12.5613,8.5262,3.7898,1.7723,1.73,1.7,2.23,12.202,7.9038,3.0792,1.6757,1.65,1.62,1.93,12.1928,7.8950,2.8073,1.5792,1.55,1.48,1.82,12.2721,7.8862,2.7107,1.4827,1.47,1.45,1.75,12.0879,7.8774,2.7019,1.5616,1.53,1.44,1.67,11.9914,7.6931,2.6931,1.5528,1.51,1.48,1.63,12.158,7.8597,2.6843,1.5439,1.532,1.52,1.68,12.2369,7.9386,2.6755,1.5351,1.54,1.53,1.68,12.3162,8.1930,2.7548,1.6140,1.59,1.55,1.63],7,10)
  //  Referenced by: '<Root>/2-D Lookup Table2'

  real_T uDLookupTable2_tableData[70];

  // Expression: [-20,-10,0,25,30,40,50]
  //  Referenced by: '<Root>/2-D Lookup Table2'

  real_T uDLookupTable2_bp01Data[7];

  // Expression: [0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1]
  //  Referenced by: '<Root>/2-D Lookup Table2'

  real_T uDLookupTable2_bp02Data[10];

  // Computed Parameter: uDLookupTable1_maxIndex
  //  Referenced by: '<Root>/2-D Lookup Table1'

  uint32_T uDLookupTable1_maxIndex[2];

  // Computed Parameter: uDLookupTable1_dimSizes
  //  Referenced by: '<Root>/2-D Lookup Table1'

  uint32_T uDLookupTable1_dimSizes[2];

  // Computed Parameter: uDLookupTable1_numYWorkElts
  //  Referenced by: '<Root>/2-D Lookup Table1'

  uint32_T uDLookupTable1_numYWorkElts[3];

  // Computed Parameter: uDLookupTable2_maxIndex
  //  Referenced by: '<Root>/2-D Lookup Table2'

  uint32_T uDLookupTable2_maxIndex[2];

  // Computed Parameter: uDLookupTable2_dimSizes
  //  Referenced by: '<Root>/2-D Lookup Table2'

  uint32_T uDLookupTable2_dimSizes[2];

  // Computed Parameter: uDLookupTable2_numYWorkElts
  //  Referenced by: '<Root>/2-D Lookup Table2'

  uint32_T uDLookupTable2_numYWorkElts[3];
} ConstP;

// External inputs (root inport signals with auto storage)
typedef struct {
  real_T Tnex;                         // '<Root>/Tnex'
  real_T Beta;                         // '<Root>/Beta'
  real_T T;                            // '<Root>/T'
  real_T SOC;                          // '<Root>/SOC'
} ExtU;

// External outputs (root outports fed by signals with auto storage)
typedef struct {
  real_T Qt;                           // '<Root>/Qt'
  real_T R0;                           // '<Root>/R0'
} ExtY;

#ifdef __cplusplus

extern "C" {

#endif

#ifdef __cplusplus

}
#endif

// Constant parameters (auto storage)
extern const ConstP rtConstP;

// Class declaration for model Lookuptable
class LookuptableModelClass {
  // public data and function members
 public:
  // External inputs
  ExtU rtU;

  // External outputs
  ExtY rtY;

  // model initialize function
  void initialize();

  // model step function
  void step();

  // Constructor
  LookuptableModelClass();

  // Destructor
  ~LookuptableModelClass();

  // private data and function members
 private:
  // Block signals and states
  DW rtDW;
};

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'Lookuptable'

#endif                                 // RTW_HEADER_Lookuptable_h_

//
// File trailer for generated code.
//
// [EOF]
//
