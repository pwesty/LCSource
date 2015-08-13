#include <stdio.h>
#include <stdarg.h>
#include <locale.h>

#include <boost/format.hpp>
#include <boost/thread/tss.hpp>

#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/helpers/exception.h>

#include "logsystem.h"

boost::thread_specific_ptr<CLogBufferForBnf> _ptr;

//////////////////////////////////////////////////////////////////////////////////////////
void LogSystem::setSubstitutedValue( const std::string variable, const std::string value )
{
	setlocale(LC_ALL, "");

#ifdef WIN32
	std::string t = boost::str(boost::format("%1%=%2%") % variable % value);
	::putenv(t.c_str());
#else
	::setenv(variable.c_str(), value.c_str(), 1);
#endif
}

void LogSystem::configureBasic()
{
	log4cxx::BasicConfigurator::configure();
}

void LogSystem::configureProperty( const std::string filename )
{
	log4cxx::PropertyConfigurator::configure( filename );
}

void LogSystem::configureXml( const std::string filename )
{
	log4cxx::xml::DOMConfigurator::configure( filename );
}

logger_t LogSystem::getLogger( const std::string name )
{
	return log4cxx::Logger::getLogger( name );
}

logger_t LogSystem::getRootLogger()
{
	return log4cxx::Logger::getRootLogger();
}

CLogBufferForBnf* LogSystem::getLogBuffer()
{
	if (_ptr.get() == 0)
	{
		_ptr.reset(new CLogBufferForBnf);
	}

	return _ptr.get();
}

//////////////////////////////////////////////////////////////////////////////////////////
// CLogMessage
#define CLOG_BUFFER_SIZE	(10240)

CLogBufferForBnf::CLogBufferForBnf()
{
	_buffer = new char[CLOG_BUFFER_SIZE];
}

CLogBufferForBnf::~CLogBufferForBnf()
{
	delete [] _buffer;
}

void CLogBufferForBnf::add( const char* fmt, ... )
{
	va_list args;
	va_start( args, fmt );
	vsnprintf( _buffer, CLOG_BUFFER_SIZE, fmt, args );
	va_end( args );
}

/*
#include "logsystem.h"

CLogger		myLogger( "spooky000" );

int main( int argc, char* argv[] )
{
	// 설정파일에 ${logfile} 으로 되있는 부분에 적당한 값을 넣어준다.
	LogSystem::setSubstitutedValuef( "logfile", "login_%s.log", argv[1] );

	// xml로 된 설정파일을 읽는다.
	LogSystem::configureXml( "log.xml" );

	// spooky00 로거에 디버그 로그를 출력한다.
	myLogger.debug( "ssss %d", 1 );

	// 루트 로거에 로그를 출력한다.
	LOG_ERROR( "test root log: %d, %d", 10, 100 );
	LOG_WARN( "test root log: %d, %d", 10, 100 );
	LOG_DEBUG( "test root log: %d, %d", 10, 100 );

	return 0;
}
*/