//
// MATLAB Compiler: 8.6 (R2023a)
// Date: Tue Oct 10 21:47:03 2023
// Arguments:
// "-B""macro_default""-W""cpplib:PhaseSignalGen,all""-T""link:lib""PhaseSignalG
// en.mlx"
//

#ifndef PhaseSignalGen_h
#define PhaseSignalGen_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" { // sbcheck:ok:extern_c
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_PhaseSignalGen_C_API 
#define LIB_PhaseSignalGen_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_PhaseSignalGen_C_API 
bool MW_CALL_CONV PhaseSignalGenInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_PhaseSignalGen_C_API 
bool MW_CALL_CONV PhaseSignalGenInitialize(void);
extern LIB_PhaseSignalGen_C_API 
void MW_CALL_CONV PhaseSignalGenTerminate(void);

extern LIB_PhaseSignalGen_C_API 
void MW_CALL_CONV PhaseSignalGenPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_PhaseSignalGen_C_API 
bool MW_CALL_CONV mlxPhaseSignalGen(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_PhaseSignalGen
#define PUBLIC_PhaseSignalGen_CPP_API __declspec(dllexport)
#else
#define PUBLIC_PhaseSignalGen_CPP_API __declspec(dllimport)
#endif

#define LIB_PhaseSignalGen_CPP_API PUBLIC_PhaseSignalGen_CPP_API

#else

#if !defined(LIB_PhaseSignalGen_CPP_API)
#if defined(LIB_PhaseSignalGen_C_API)
#define LIB_PhaseSignalGen_CPP_API LIB_PhaseSignalGen_C_API
#else
#define LIB_PhaseSignalGen_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_PhaseSignalGen_CPP_API void MW_CALL_CONV PhaseSignalGen(int nargout, mwArray& t, mwArray& sig);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
