#ifndef __LOGSYSTEM_H__
#define __LOGSYSTEM_H__

#ifdef WIN32
#pragma warning ( disable: 4819 )
#pragma warning ( disable: 4231 )
#pragma warning ( disable: 4996 )
#endif

#include <log4cxx/log4cxx.h>
#include <log4cxx/logger.h>
#include <stdarg.h>

typedef log4cxx::LoggerPtr		logger_t;

class CLogBufferForBnf;

class LogSystem
{
public:
	// 설정 파일을 읽기전에 substituted value를 세팅해야 한다.
	static void setSubstitutedValue(const std::string variable, const std::string value);

	static void configureBasic();
	static void configureProperty( const std::string filename );
	static void configureXml( const std::string filename );

	static logger_t getLogger( const std::string name );
	static logger_t getRootLogger();

	static CLogBufferForBnf* getLogBuffer();
};

class CLogBufferForBnf
{
public:
	CLogBufferForBnf();
	~CLogBufferForBnf();

	void add( const char* fmt, ... );
	const char* getMessage()
	{
		return _buffer;
	}

private:
	char* _buffer;
};

#define LOGGER_LOG_FATAL( logger, ... )			do { \
	if (logger->isFatalEnabled()) \
	{ \
		CLogBufferForBnf* p = LogSystem::getLogBuffer(); \
		p->add( __VA_ARGS__  ); \
		LOG4CXX_FATAL( logger, p->getMessage() ); \
	} \
} while( 0 )

#define LOGGER_LOG_ERROR( logger, ... )			do { \
	if (logger->isErrorEnabled()) \
	{ \
		CLogBufferForBnf* p = LogSystem::getLogBuffer(); \
		p->add( __VA_ARGS__  ); \
		LOG4CXX_ERROR( logger, p->getMessage() ); \
	} \
} while( 0 )

#define LOGGER_LOG_WARN( logger, ... )			do { \
	if (logger->isWarnEnabled()) \
	{ \
		CLogBufferForBnf* p = LogSystem::getLogBuffer(); \
		p->add( __VA_ARGS__  ); \
		LOG4CXX_WARN( logger, p->getMessage() ); \
	} \
} while( 0 )

#define LOGGER_LOG_INFO( logger, ... )			do { \
	if (logger->isInfoEnabled()) \
	{ \
		CLogBufferForBnf* p = LogSystem::getLogBuffer(); \
		p->add( __VA_ARGS__  ); \
		LOG4CXX_INFO( logger, p->getMessage() ); \
	} \
} while( 0 )

#define LOGGER_LOG_DEBUG( logger, ... )			do { \
	if (logger->isDebugEnabled()) \
	{ \
		CLogBufferForBnf* p = LogSystem::getLogBuffer(); \
		p->add( __VA_ARGS__  ); \
		LOG4CXX_DEBUG( logger, p->getMessage() ); \
	} \
} while( 0 )

#define LOGGER_LOG_TRACE( logger, ... )			do { \
	if (logger->isTraceEnabled()) \
	{ \
		CLogBufferForBnf* p = LogSystem::getLogBuffer(); \
		p->add( __VA_ARGS__  ); \
		LOG4CXX_TRACE( logger, p->getMessage() ); \
	} \
} while( 0 )

#define LOG_FATAL( ... )			LOGGER_LOG_FATAL( LogSystem::getRootLogger(), __VA_ARGS__ )
#define LOG_ERROR( ... )			LOGGER_LOG_ERROR( LogSystem::getRootLogger(), __VA_ARGS__ )
#define LOG_WARN( ... )				LOGGER_LOG_WARN( LogSystem::getRootLogger(), __VA_ARGS__ )
#define LOG_INFO( ... )				LOGGER_LOG_INFO( LogSystem::getRootLogger(), __VA_ARGS__ )
#define LOG_DEBUG( ... )			LOGGER_LOG_DEBUG( LogSystem::getRootLogger(), __VA_ARGS__ )
#define LOG_TRACE( ... )			LOGGER_LOG_TRACE( LogSystem::getRootLogger(), __VA_ARGS__ )

#endif // #ifndef __LOGSYSTEM_H__
