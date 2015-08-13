#include "StdH.h"
#ifdef KALYDO
#include "THandler.h"

#if _MSC_VER <= 1200
THandler::THandler( TCallbackClass* base, TClassCallback method, void* id)
	{
		m_CbClass = base;
		m_Callback = *reinterpret_cast<void**>(&method);
		m_Id = id;
	}


void THandler::setCallback( TCallbackClass* base, void* method)
	{
		m_CbClass = reinterpret_cast<TCallbackClass*>(base);
		m_Callback = method;
	}
#endif

THandler::THandler( void (*function)(unsigned int errorType, 
		unsigned int errorCode, void* id, void* message, unsigned int length), void* id)
{
	m_CbClass = 0;
	m_Callback = function;
	m_Id = id;
}

	
void THandler::setCallback( void (*function)(unsigned int errorType, 
	unsigned int errorCode, void* id, void* message, unsigned int length))
{
	m_CbClass = 0;
	m_Callback = function;
}

void THandler::operator() (unsigned int errorType, unsigned int errorCode, void* message, unsigned int length)
{
	if (m_CbClass)
	{
		TDelegate dgate = *reinterpret_cast<TDelegate*>(&m_Callback);
		(m_CbClass->*dgate)(errorType, errorCode, m_Id, message, length);
	}
	else 
	{
		TCallback cback = *reinterpret_cast<TCallback*>(&m_Callback);
		cback(errorType, errorCode, m_Id, message, length);
	}
}

void THandler::setId(void* id) 
{
	m_Id = id;
}
#endif //KALYDO