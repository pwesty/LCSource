/** @file "Exception.h"
 *  @author M.F.A.Schroders, R.Binnendijk, M.G.T.Titulaer, M.W.A.Roza
 *
 *  This file contains the handling of exceptions that are sent to the
 *  Kalydo Error Reporter. All errors should be sent in UTF-8 format.
 */

#ifdef KALYDO
#ifndef EXCEPTION_H
#define EXCEPTION_H

typedef unsigned int TSeverityLevel;
const TSeverityLevel SL_LOG		 = 0; // log message
const TSeverityLevel SL_WARNING	 = 1; // warning message
const TSeverityLevel SL_ERROR	 = 2; // error message, do not continue!
const TSeverityLevel SL_CRITICAL = 3; // critical error, do not continue!

typedef unsigned int TErrorType;
const TErrorType ET_UNKNOWN		= 0; // unknown programming error
const TErrorType ET_PROGGEN		= 1; // general programming error
const TErrorType ET_PROGSCRIPT	= 2; // programming error in the scripts
const TErrorType ET_PROGENGINE	= 3; // programming error in the engine
const TErrorType ET_INPUT		= 8; // error in the input handling
const TErrorType ET_HARDWARE	= 9; // hardware error

#define KALYDOASSERT	1

// The interface for the KASSERT macro is as follows:
// b: the boolean to check
// error: a description of the error
// solution: the possible solution of this error
// type: can be one of the types described above
// severityLevel: can be one of the levels described above
#undef KASSERT
#define KASSERT(b, error, solution, type, severityLevel) if (b) ; else ::KALYDOERROR(error, solution, type, severityLevel, __FILE__, __LINE__)

// this assert is only defined in debug
#undef DEBUGASSERT
#ifdef _DEBUG
#define DEBUGASSERT(b, error, solution, type, severityLevel) if (b) ; else ::KALYDOERROR(error, solution, type, severityLevel, __FILE__, __LINE__)
#else
#define DEBUGASSERT(b, error, solution, type, severityLevel)
#endif

void KALYDOERROR(const char* err, const char* solution, int type,
					int severityLevel, const char* file, int line);

#endif //EXCEPTION_H
#endif //KALYDO
