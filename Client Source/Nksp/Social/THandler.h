/** @class THandler "THandler.h"
 *	@author M.F.A.Schroders, R.Binnendijk, M.G.T.Titulaer, M.W.A.Roza
 *
 *	This class is an convenience class for calling C or C++ functions,
 *  used when integrating an application into the Kalydo technology.
 */

#ifdef KALYDO
#ifndef THANDLER_H
#define THANDLER_H

class TCallbackClass{};
typedef void (TCallbackClass::* TClassCallback)(unsigned int errorType, 
		unsigned int errorCode, void* id, void* message, unsigned int length);

#if (_MSC_VER <= 1200 )
	#define MAKECLASSHANDLER(object, function, id) new THandler(reinterpret_cast<TCallbackClass*>(object), reinterpret_cast<TClassCallback>(function), id); 
#else
	#define MAKECLASSHANDLER(object, function, id) new THandler(object, function, id); 
#endif

#define MAKEHANDLER(function, id) new THandler(funcion, id);

class THandler
{
	typedef void (TCallbackClass::* TDelegate)(unsigned int errorType, 
			unsigned int errorCode, void* id, void* message, unsigned int length);

	typedef void (*TCallback)(unsigned int errorType, 
			unsigned int errorCode, void* id, void* message, unsigned int length);
public:

#if _MSC_VER <= 1200
	THandler( TCallbackClass* base, TClassCallback callback, void* id);

	void setCallback( TCallbackClass* base, void* method);
#else 
	template <class C>
	THandler( C* base, void (C::* method)(unsigned int errorType, 
		unsigned int errorCode, void* id, void* message, unsigned int length), void* id)
	{
		m_CbClass = reinterpret_cast<TCallbackClass*>(base);
		m_Callback = *reinterpret_cast<void**>(&method);
		m_Id = id;
	}

	template <class C>
	void setCallback( C* base, void (C::* method)(unsigned int errorType, 
		unsigned int errorCode, void* id, void* message, unsigned int length))
	{
		m_CbClass = reinterpret_cast<TCallbackClass*>(base);
		m_Callback = method;
	}
#endif

	THandler( void (*function)(unsigned int errorType, 
		unsigned int errorCode, void* id, void* message, unsigned int length), void* id);
	
	void setCallback( void (*function)(unsigned int errorType, 
		unsigned int errorCode, void* id, void* message, unsigned int length));

	void operator() (unsigned int errorType, unsigned int errorCode, void* message, unsigned int length);

	void setId(void* id);
	void* getId() const { return m_Id; }

protected:
	TCallbackClass* m_CbClass;
	void* m_Callback;
	void* m_Id;
};

#endif //THANDLER_H
#endif //KALYDO