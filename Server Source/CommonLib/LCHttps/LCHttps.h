#pragma once

#ifndef __LC_HTTPS_CONNECT__
#define __LC_HTTPS_CONNECT__

#include <iostream>
#include <curl/curl.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
using namespace std;

#define MAX_CERTS 20
#define FINGERPRINT_FILENAME ".fp"

class CLCHttps
{
private:
	CURL*		m_curl;

public:
	enum { E_INIT_CURL=-1, E_OK=0 , E_CONNECT_URL = 1, E_MATCH_FINGERPRINT=2 };

	CLCHttps()
	{
		m_curl = curl_easy_init();
	}
	virtual ~CLCHttps()
	{
		curl_easy_cleanup(m_curl);
	}

public:
	/*
	param:
	*	url : 인증서버 주소
	*	fingerprint : CA인증서를 X509로 변환하여 base64로 디코딩한후 sha1 으로 암호화한 값 ( 독일에서 넘겨준 값 )
	return:
	*	E_CONNECT_URL - 연결은됨
	*	E_MATCH_FINGERPRINT - 암호까지 일치함
	*/
	int Connect( const char* url, const char* fingerprint );

private:
	static int	verify_callback(int preverify_ok, X509_STORE_CTX *x509_ctx);
	static CURLcode	sslctxfun(CURL *curl, SSL_CTX *sslctx, void *parm);
	CURLcode	go ( CURL *curl, const char* url );
};
#endif
