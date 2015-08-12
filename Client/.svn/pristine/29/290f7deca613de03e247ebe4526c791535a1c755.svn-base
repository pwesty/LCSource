/** @class TException "TException.h"
*  @author M.G.T.Titulaer
*
*	An exception class that contains information on the exception/assert 
*  that occurred.
*/

#ifdef KALYDO
#ifndef TEXCEPTION_H
#define TEXCEPTION_H

#include "Exception.h"

typedef unsigned long       DWORD;

class TException
{
public:
	/** @name Constructor & Destructor Functions
	*	This list contains all constructor and destructor methods.
	*/
	//@{	
	TException();
	TException(const char* err, const char* solution, const char* typeMessage, 
		int type, int severityLevel, DWORD lastError, const char* file, int line);

	virtual ~TException();
	//@}

	inline const char* getErrorMessage() const { return m_ErrorMessage; }
	inline const char* getSolutionMessage() const { return m_SolutionMessage; }
	inline const char* getTypeMessage() const { return m_TypeMessage; }
	inline const char* getFileName() const { return m_FileName; }
	inline int getSeverityLevel() const { return m_SeverityLevel; }
	inline int getLine() const { return m_Line; }
	inline int getType() const { return m_Type; }
	inline DWORD getLastError(){ return m_LastError; }

protected:
	const char* m_FileName;
	const char* m_TypeMessage;
	const char* m_SolutionMessage;
	const char* m_ErrorMessage;
	int m_Type;
	int m_Line;
	int m_SeverityLevel;
	DWORD m_LastError;
};

#endif //TEXCEPTION_H
#endif //KALYDO