#ifdef  _MSC_VER

#ifndef CIRCLE_WINDOWS
#define CIRCLE_WINDOWS
#endif // CIRCLE_WINDOWS

#define pid_t int
#define RETSIGTYPE void
#define socklen_t int
#define STDC_HEADERS 1
#define ssize_t int
#define HAVE_INET_ADDR 1
#define HAVE_ASSERT_H 1
#define HAVE_ERRNO_H 1
#define HAVE_FCNTL_H 1
#define HAVE_LIMITS_H 1
#define HAVE_STRING_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
#undef NEED_ACCEPT_PROTO
#undef NEED_ATOI_PROTO
#undef NEED_ATOL_PROTO
#undef NEED_BIND_PROTO
#undef NEED_BZERO_PROTO
#undef NEED_CHDIR_PROTO
#undef NEED_CLOSE_PROTO
#undef NEED_CRYPT_PROTO
#undef NEED_FCLOSE_PROTO
#undef NEED_FCNTL_PROTO
#undef NEED_FFLUSH_PROTO
#undef NEED_FPRINTF_PROTO
#undef NEED_FPUTC_PROTO
#undef NEED_FPUTS_PROTO
#undef NEED_FREAD_PROTO
#undef NEED_FSCANF_PROTO
#undef NEED_FSEEK_PROTO
#undef NEED_FWRITE_PROTO
#undef NEED_GETPEERNAME_PROTO
#undef NEED_GETPID_PROTO
#undef NEED_GETRLIMIT_PROTO
#undef NEED_GETSOCKNAME_PROTO
#undef NEED_GETTIMEOFDAY_PROTO
#undef NEED_HTONL_PROTO
#undef NEED_HTONS_PROTO
#undef NEED_INET_ADDR_PROTO
#undef NEED_INET_ATON_PROTO
#undef NEED_INET_NTOA_PROTO
#undef NEED_LISTEN_PROTO
#undef NEED_NTOHL_PROTO
#undef NEED_PERROR_PROTO
#undef NEED_PRINTF_PROTO
#undef NEED_QSORT_PROTO
#undef NEED_READ_PROTO
#undef NEED_REWIND_PROTO
#undef NEED_SELECT_PROTO
#undef NEED_SETITIMER_PROTO
#undef NEED_SETRLIMIT_PROTO
#undef NEED_SETSOCKOPT_PROTO
#undef NEED_SOCKET_PROTO
#undef NEED_SPRINTF_PROTO
#undef NEED_SSCANF_PROTO
#undef NEED_SYSTEM_PROTO
#undef NEED_TIME_PROTO
#undef NEED_UNLINK_PROTO
#undef NEED_WRITE_PROTO
#define HAVE_STRUCT_IN_ADDR 1
#undef HAVE_UNSAFE_CRYPT

#define LC_THREAD_T				DWORD
#define LC_CURRENT_THREAD_ID	(GetCurrentThreadId())

#else // _MSC_VER

#define CIRCLE_UNIX 1
#ifndef TRUE
#define TRUE true
#endif

#ifndef FALSE
#define FALSE false
#endif

#define HAVE_SYS_WAIT_H 1
#define RETSIGTYPE void
#define STDC_HEADERS 1
#define TIME_WITH_SYS_TIME 1
#define CIRCLE_CRYPT 1
#define HAVE_STRUCT_IN_ADDR 1
#define HAVE_INET_ADDR 1
#define HAVE_INET_ATON 1
#define HAVE_ARPA_INET_H 1
#define HAVE_ARPA_TELNET_H 1
#define HAVE_ASSERT_H 1
#define HAVE_CRYPT_H 1
#define HAVE_ERRNO_H 1
#define HAVE_FCNTL_H 1
#define HAVE_LIMITS_H 1
#define HAVE_MEMORY_H 1
#define HAVE_NETDB_H 1
#define HAVE_NETINET_IN_H 1
#define HAVE_SIGNAL_H 1
#define HAVE_STRING_H 1
#define HAVE_STRINGS_H 1
#define HAVE_SYS_FCNTL_H 1
#define HAVE_SYS_RESOURCE_H 1
#define HAVE_SYS_SELECT_H 1
#define HAVE_SYS_SOCKET_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_SYS_UIO_H 1
#define HAVE_UNISTD_H 1

#define LC_THREAD_T				pthread_t
#define LC_CURRENT_THREAD_ID	(pthread_self())

#endif
