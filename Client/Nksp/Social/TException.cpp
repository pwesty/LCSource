#include "StdH.h"
#ifdef KALYDO
#include "TException.h"

TException::TException()
:	m_ErrorMessage(0)
,	m_SolutionMessage(0)
,	m_FileName(0)
,	m_TypeMessage(0)
,	m_SeverityLevel(SL_LOG)
,	m_LastError(-1)
,	m_Type(ET_UNKNOWN)
,	m_Line(-1)
{
}

TException::TException(const char* err, const char* solution, const char* typeMessage,
					   int type, int severityLevel, DWORD lastError, const char* file, int line)
:	m_ErrorMessage(err)
,	m_SolutionMessage(solution)
,	m_FileName(file)
,	m_TypeMessage(typeMessage)
,	m_SeverityLevel(severityLevel)
,	m_LastError(lastError)
,	m_Type(type)
,	m_Line(line)
{
}

TException::~TException()
{
}
#endif //KALYDO