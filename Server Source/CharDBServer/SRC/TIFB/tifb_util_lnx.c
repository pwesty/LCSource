#include <fcntl.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <sys/procfs.h>

#include <tifb.h>
#include <tifb_api.h>
#include <tifb_extern.h>


char *STR_TSW_COM[CURR_SW_COM] = {
    "SI_TCP"
};

void makelower(char *str)
{
    while (*str) {
        *str = tolower(*str);
        str++;
    }
}

void makeupper(char *str)
{
    while (*str) {
        *str = toupper(*str);
        str++;
    }
}


int is_registered_block( char *blkname )
{
	int i;
	int  idx=-1;


	for(i=0; i < CURR_SW_COM; i++ )
	{
		if( !strcasecmp(blkname, STR_TSW_COM[i] ) )
		{ 
			idx = i;
			break;
		}
	}

	return idx;
}


char *mtime_tifb()
{
	time_t t;
	t = time(&t);

	strftime(mtime_str, 32, "%Y-%m-%d %T %a", localtime((time_t*)&t));
	mtime_str[21] = toupper(mtime_str[21]);
	mtime_str[22] = toupper(mtime_str[22]);
	mtime_str[23] = 0x00;

	return mtime_str;
}

int GetProcessID( char *name )
{
	int fd;
	DIR  *dirp;
    int         dPCnt = 0;
	FILE        *fp_pro;
    FILE        *fp_tot;
	int         dReadSize;
    long long  	llTotVal;
    long long	llProcVal;
	char        szBuffer[20480];
	struct dirent *direntp;

	/* changed by uamyd 050425 : PROC_NAME_LEN -> PROC_DIR_LEN */
	char   		pname[PROC_DIR_LEN];
	char   		tempbuf[BUF_LEN];
    char        *szTmp;

    memset(szBuffer, 0x00, 20480 );

    if( (fp_tot = fopen( SYS_STAT, "r")) == NULL)
    {
        fprintf(stderr, "CANNOT OPEN STAT FILE\n");
        return -1;
    }

    dReadSize = fread( szBuffer, 20480, 1, fp_tot);

    szTmp = strstr(szBuffer, "btime");

    sscanf( szTmp, "%*s %lld", &llTotVal );

    fclose(fp_tot);

	if( (dirp = opendir(PROC_PATH)) == (DIR*)NULL )
	{
		fprintf(stderr, "\n\tCan't Access Porcess Directory (%s)\n", PROC_PATH);
		exit(0);
	}


	while( (direntp = readdir(dirp)) != NULL )
	{
        dPCnt++;
        if( dPCnt > 1024 )
            break;

		if( !strcmp(direntp->d_name, PARENT_PATH) ||
			!strcmp(direntp->d_name, HOME_PATH) ) continue;


        if( !atoi(direntp->d_name) )
        {
            continue;
        }


		sprintf(pname, "%s/%s/cmdline", PROC_PATH, direntp->d_name );

		if( (fd = open(pname, O_RDONLY)) < 0 )
		{
			close(fd);
			continue;
		}

        memset( pname, 0x00, PROC_DIR_LEN);
        if( read(fd, pname, PROC_DIR_LEN-1) < 0 )
        {
            close(fd);
            continue;
        }
        else
        {
            close(fd);
        }

        pname[PROC_DIR_LEN-1] = 0x00;


        if( !strcmp(name, pname) )
        {
            sprintf(tempbuf, "%s/%s/stat", PROC_PATH, direntp->d_name);


            fp_pro = fopen( tempbuf, "r");

            dReadSize = fread( szBuffer, 20480, 1, fp_pro);

            sscanf(szBuffer, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u %*u %*d %*d %*d %*d %*d %*d %lld %*u %*d %*u %*u %*u %*u %*u %*u %*u %*u %*u%*u %*u %*u %*u %*d %*d",
&llProcVal );

            fclose(fp_pro);

            llProcVal = llProcVal/100;

            pid_stime = llTotVal + llProcVal;

            return atoi(direntp->d_name);

        }

	}

	closedir(dirp);

	return -1;
}



int id_of_process( char *name )
{
	int i;


	for(i=0; i < CURR_SW_COM; i++ )
	{
		if( !strcasecmp(name, STR_TSW_COM[i]) )
		{
			return i;
		}
	}

	return -1;
}


int GetMcInitInfo()
{
    FILE *fa;
    char    szBuf[1024];
    char    ProcName[64];
    char    szDumy[64];
    int     dActFlag;
    int     i, idx;
	int		dIndex;

	/* changed by uamyd 050426: CURR_SW_COM -> MAX_SW_COM */
    for(i=0; i<MAX_SW_COM; i++ )
    {
		/* changed by uamyd 050426: SWInit[i] -> st_SWInit[i].dActFlag */
        st_SWInit[i].dActFlag = 0;
    }

    fa = fopen(MCINIT_FILE, "r");

    if( fa == NULL )
    {

        printf( "%s FILE OPEN FAIL (%s)\n", MCINIT_FILE, strerror(errno)) ;
        return -1;
    }

	idx = 0; 	/* added by uamyd 050426: count initialize for while compute */

    while(fgets(szBuf, 1024, fa) != NULL)
    {
        if(szBuf[0] != '#')
        {
            printf("GetMcInitInfo fun, [%s] File [%d] row format error\n", MCINIT_FILE, idx+1);
            fclose(fa);
            return -2;
        }

        if(szBuf[1] == '#')
            continue;
        else if(szBuf[1] == 'E')
            break;
        else
        {
            if( sscanf(&szBuf[2], "%d %s %d %s", &dIndex, ProcName, &dActFlag, szDumy) == 4)
            {
				/* changed by uamyd 050426: Used struct 'st_SWInit' */
#if 0
                idx = is_registered_block( ProcName );


                if( idx >= 0 )
                {
                    SWInit[idx] = dActFlag;
                }
#endif
				/* added by uamyd 050426: for search duplicated process name */
                for( i = 0; i< g_dProcCnt; i++ ){
                    if( !strcasecmp( st_SWInit[i].szProcName, ProcName ) ){
                        printf(" IGNORE PROCESS[%s], DUPLICATED \n", ProcName );
                        i = -1;
                        break;
                    }
                }
                if( i != -1 ){
					st_SWInit[idx].dActFlag = dActFlag;
					sprintf( st_SWInit[idx].szProcName, "%s", ProcName );
				 	st_SWInit[idx].dIdx = dIndex;
					idx++;
					g_dProcCnt = idx;
				}
            }
        }
		/* added by uamyd 050427: for check count of starting process to over MAX */
        /* value of MAX is MAX_SW_COM in 'fidb_info.h' */
        if( g_dProcCnt > MAX_SW_COM ){
            printf( "OVER MAX_PROCESS_COUNT : %d\n", MAX_SW_COM );
            exit (-1);
        }
    }

    fclose(fa);

    return 0;
}




int GetYorN()
{
    char ch;

    do
    {
        ch = getchar();
    } while(ch != 'Y' && ch != 'y' && ch != 'N' && ch != 'n');

    if(ch == 'Y' || ch == 'y')
        return _YES;
    else
        return _NO;
}




int dGetUserPermission()
{
    FILE        *ptr;
    int         dRet = 0;
    char        szCommand[8];
    char        szBuffer[16];

    sprintf( szCommand, "whoami" );

    if( (ptr = popen( szCommand, "r" )) != NULL )
    {
        while( fgets( szBuffer, 16, ptr ) != NULL )
        {
            szBuffer[strlen(szBuffer)-1] = 0x00;
            if( strcasecmp( szBuffer, "root" ) == 0 )
            {
                dRet = 1;
                break;
            }
        }
    }

    pclose( ptr );

    return dRet;
}
