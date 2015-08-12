// FileSecure.cpp: implementation of the CFileSecure class.
//
//////////////////////////////////////////////////////////////////////

#include "stdh.h"
#include "FileSecure.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileSecure::CFileSecure() :
JUMPSIZE(20), ENDCODE(-9999)
{

}

CFileSecure::~CFileSecure()
{

}

const int CFileSecure::CalculateSum( FILE *file, const int fileSize )
{
	int buffer = 0;
	int fileSum = 0;
	int readbyte = 0;

	fseek( file, 0, SEEK_SET );
	
	for( int i =0; i < fileSize; i += JUMPSIZE )
	{
		fseek( file, i, SEEK_SET );

		readbyte = sizeof( int );
		
		if( i + readbyte > fileSize )
			readbyte = fileSize - i;

		fread( &buffer, readbyte, 1, file );

		fileSum += buffer;
	}

	return fileSum;
}

void CFileSecure::EncodeFile( FILE *file )
{
	// end pointer.
	int fileSize = ftell( file );

	int encryptNumber = CalculateSum( file, fileSize );
	// go to end in file.	
	fseek( file, 0, SEEK_END );
	fwrite( &encryptNumber, sizeof( int ), 1, file );
}

bool CFileSecure::DecodeFile( FILE *file )
{
	int encryptNumber = 0;
	
	fseek( file, -4, SEEK_END );
	// read file size & encrypt key value.	
	int fileSize = ftell(file);
	fread( &encryptNumber, sizeof( int ), 1, file );
	// end pointer.
	int result = CalculateSum( file, fileSize );
	
	fseek( file, 0, SEEK_SET) ;
	
	return ( result == encryptNumber );
}