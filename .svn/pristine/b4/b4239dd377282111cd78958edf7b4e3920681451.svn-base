#include <procfs.h>

#include <fcntl.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <sys/procfs.h>

#include <con_def.h>
#include <con_extern.h>

#include <tifb.h>
#include <tifb_api.h>
#include <tifb_extern.h>

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
	struct dirent *direntp;
	psinfo_t	p;
	/* chagned by uamyd 050425 : PROC_NAME_LEN -> PROC_DIR_LEN */
	char   		pname[PROC_DIR_LEN];


	if( (dirp = opendir(PROC_PATH)) == (DIR*)NULL )
	{
		fprintf(stderr, "\n\tCan't Access Porcess Directory (%s)\n", PROC_PATH);
		exit(0);
	}


	while( (direntp = readdir(dirp)) != NULL )
	{

		if( !strcmp(direntp->d_name, PARENT_PATH) ||
			!strcmp(direntp->d_name, HOME_PATH) ) continue;


        if( !atoi(direntp->d_name) )
        {
            continue;
        }


		sprintf(pname, "%s/%s/psinfo", PROC_PATH, direntp->d_name );

		if( (fd = open(pname, O_RDONLY)) < 0 )
		{
			close(fd);
			continue;
		}

        if( read( fd, (char *)&p, sizeof(psinfo_t)) != sizeof(psinfo_t))
        {
            fprintf(stderr, "\n\t %s File Read Error(%s)\n",
                    pname, strerror(errno));
            close(fd);
            continue;
        }

        close(fd);
        if( !strcasecmp(name, p.pr_psargs) )
        {
            closedir(dirp);
            pid_stime = p.pr_start.tv_sec;
            return p.pr_pid;
        }

	}

	closedir(dirp);

	return -1;
}



#if 0
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
#endif


int GetMcInitInfo()
{
    FILE *fa;
    char    szBuf[1024];
    char    ProcName[64];
    char    szDumy[64];
    int     dActFlag;
	int		dIndex;
    int     i, idx;

	/* changed by uamyd 050425: CURR_SW_COM -> MAX_SW_COM */
    for(i=0; i<MAX_SW_COM; i++ )
    {
		/* changed by uamyd 050425: SWInit[i] -> st_SWInit[i].dActFlag */
        st_SWInit[i].dActFlag = 0;
		st_SWInit[i].eflg = 0;
		st_SWInit[i].dIdx = 0;
		SWPid[i] = 0;
    }

    fa = fopen(MCInit_FILE, "r");

    if( fa == NULL )
    {

        printf( "%s FILE OPEN FAIL (%s)\n", MCInit_FILE, strerror(errno)) ;
        return -1;
    }


	idx = 0; /* added by uamyd 050425: count initialize for while count */

    while(fgets(szBuf, 1024, fa) != NULL)
    {
        if(szBuf[0] != '#')
        {
            printf("GetMcInitInfo fun, [%s] File [%d] row format error\n", MCInit_FILE, idx+1);
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
				/* CHECK DUPLICATED PROCESS */
				for( i = 0; i< g_dProcCnt; i++ ){
					if( !strcasecmp( st_SWInit[i].szProcName, ProcName ) ){
						sprintf( errbuf, "DUPLICATED , IGNORE PROCESS[%s]\n", ProcName );
						PrintOut(TIFB_FAIL, errbuf );
						i = -1;
						break;
					}
				}
				if( i != -1 ){
					sprintf( st_SWInit[idx].szProcName, "%s", ProcName );
					st_SWInit[idx].dActFlag = dActFlag;
					st_SWInit[idx].dIdx = dIndex;
					idx++;
					g_dProcCnt = idx;
				
				}
            }
        }
		/* added by uamyd 050427: for check count of starting process to over MAX */
		/* value of MAX is MAX_SW_COM in 'fidb_info.h' */
		if( g_dProcCnt > MAX_SW_COM ){
			sprintf( errbuf, "OVER MAX_PROCESS_COUNT : %d\n", MAX_SW_COM );
			PrintOut( TIFB_FAIL, errbuf );
			exit(-1);
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
