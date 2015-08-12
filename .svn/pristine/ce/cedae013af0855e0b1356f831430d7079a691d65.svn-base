
#include <chsmd_api.h>
#include <chsmd_extern.h>

#include <con_def.h>
#include <shmutillib.h>


int GetMaskInfo( pst_Mask pstMask)
{
	FILE *fp;
	st_Mask  stMask;
	int  dRet;
	size_t	Ret;


	fp = fopen( DEF_MASK_FILE, "rb");

	if( fp == NULL )
	{
		dAppLog(LOG_CRI, "MASK_INWEBS.dat FILE OPEN ERROR [%s]", strerror(errno));

		if( errno != ENOENT )
			return -1;

		dRet = CreateMaskFile();		
		
		if( dRet < 0 )
		{
			return -4;
		}

		fp = fopen( DEF_MASK_FILE, "rb");

		if( fp == NULL )
		{
			dAppLog(LOG_CRI, "MASK_INWEBS.dat FILE REOPEN ERROR [%s]", strerror(errno));
			return -2;
		}
	}


	Ret = fread( &stMask, sizeof(st_Mask), 1, fp );

	if( Ret == 0 )
	{
		dAppLog(LOG_CRI, "MASK_INWEBS.dat FREAD ERROR [%s]", strerror(errno));
		return -3;
	}
	
	memcpy( pstMask, &stMask, sizeof(st_Mask) );
	log_hexa( (unsigned char *)pstMask, sizeof(st_Mask) );
	
	return 0;
}

int  UpdateMaskFile( pst_Mask pstMask)
{
	FILE *fp;
	size_t Ret;

	fp = fopen( DEF_MASK_FILE, "w+b");

	if( fp == NULL )
	{
		dAppLog( LOG_CRI, "MASK_INWEBS.dat FILE UPDATE ERROR [%s]", strerror(errno));
		return -1;
	}


	Ret  =  fwrite( pstMask, sizeof(st_Mask), 1, fp );	

	if( Ret == 0 )
	{
		fclose(fp);
		dAppLog(LOG_CRI, "MASK_INWEBS.dat FILE WRITE ERROR [%s]", strerror(errno));
		return -2;
	}

	fclose(fp);
	
	log_hexa( (unsigned char *)pstMask, sizeof(st_Mask) );

	return 0;
}

int  CreateMaskFile()
{
	FILE *fp;
	st_Mask  stMask;
	size_t Ret;

	fp = fopen( DEF_MASK_FILE, "wb");

	if( fp == NULL )
	{
		dAppLog( LOG_CRI, "MASK_INWEBS.dat FILE CREATE ERROR [%s]", strerror(errno));
		return -1;
	}

	memset( &stMask, 0x00, sizeof(st_Mask));

	Ret  =  fwrite( &stMask, sizeof(st_Mask), 1, fp );	

	if( Ret == 0 )
	{
		fclose(fp);
		dAppLog(LOG_CRI, "MASK_INWEBS.dat FILE WRITE ERROR [%s]", strerror(errno));
		return -2;
	}

	fclose(fp);
	return 0;
}


int FillMaskInfo( pst_Mask pstMask)
{
	int i;
	for( i=0; i<MAX_STATUS_COM; i++ )
	{
		fidb->stStatus[i].stSts.mask = pstMask->stStatus[i].stSts.mask;
	}

	return 0;
}
