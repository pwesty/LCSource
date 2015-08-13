#ifndef WIN32

#include "LCHttps.h"
#include "logsystem.h"

CURLcode CLCHttps::sslctxfun(CURL *curl, SSL_CTX *sslctx, void *parm)
{
	SSL_CTX_set_verify(sslctx, SSL_VERIFY_PEER, &CLCHttps::verify_callback );
	return CURLE_OK;
}

int CLCHttps::verify_callback(int preverify_ok, X509_STORE_CTX *x509_ctx)
{
	X509*	cert = X509_STORE_CTX_get_current_cert(x509_ctx);
	if( cert->valid == 0 || cert->ex_xkusage == 0 )
		return 1;

	int j;
	unsigned int n;
	unsigned char md[EVP_MAX_MD_SIZE];

	const EVP_MD *digest=EVP_sha1();
	FILE* fFinger = fopen(FINGERPRINT_FILENAME,"w+");
	BIO* STDout = BIO_new_fp(fFinger,BIO_NOCLOSE);
	if ( X509_digest( cert, digest,md,&n))
	{
		for (j=0; j<(int)n; j++)
		{
			BIO_printf(STDout,"%02X%c", md[j], (j+1 == (int)n)?'\n':':');
		}
	}
	fclose( fFinger);
	return 1;
}

CURLcode CLCHttps::go(CURL *curl, const char* url)
{
	CURLcode code;
	// code = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);
	curl_easy_setopt(m_curl, CURLOPT_URL, url);
	curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 10 ) ;
	curl_easy_setopt(m_curl, CURLOPT_HEADER, false ) ;
	curl_easy_setopt(m_curl, CURLOPT_NOBODY, true ) ;
	/* fetch the page even if verifying the certificates fails */
	curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(m_curl, CURLOPT_SSL_CTX_FUNCTION, &CLCHttps::sslctxfun);
	code = curl_easy_perform(m_curl);
	return code;
}

int CLCHttps::Connect( const char* _url, const char* fingerprint )
{
	if( !m_curl )
		return E_INIT_CURL;

	CURLcode code = go( m_curl, _url);
	int		 result = 0;
	if( code == CURLE_OK )
	{
		curl_easy_getinfo( m_curl, CURLINFO_HTTP_CODE, &result );
		if ( result == 200 )
		{
			FILE* pFingerFile;
			char  strFinger[1024];
			pFingerFile = fopen( FINGERPRINT_FILENAME , "r" );
			fgets( strFinger, 1024, pFingerFile );
			fclose(pFingerFile);
			unlink(FINGERPRINT_FILENAME);

			if( strncmp( strFinger, fingerprint, strlen(fingerprint)) == 0 ) // fingerprint 와 같으면
			{
				result = E_MATCH_FINGERPRINT;
			}
			else
			{
				LOG_ERROR("Deffernt fingerprint. i(%s) you(%s)", fingerprint, strFinger);
				result = E_NOT_MATCH_FINGERPRINT;
			}
		}
		else
		{
			LOG_ERROR("Curl GetInfo Error(Connect) HTTP errorCode : %d", result);			
		}
	}
	else
	{
		LOG_ERROR("libcurl error(%s)", code);
		return E_CONNECT_URL;
	}
	return result;
}

#endif
