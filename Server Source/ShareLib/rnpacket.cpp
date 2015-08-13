#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>

#include "rnpacket.h"
#include "logsystem.h"

#include <boost/pool/singleton_pool.hpp>

#include <boost/pool/singleton_pool.hpp>
#include <boost/bind.hpp>
#include <boost/tokenizer.hpp>

typedef boost::tokenizer<boost::char_separator<char> > stokenizer;
static boost::char_separator<char> sep(" \n", NULL, boost::drop_empty_tokens);

//////////////////////////////////////////////////////////////////////////
#define DEFINE_POOL_OBJECT(x)		struct _tag_##x {}; typedef boost::singleton_pool<_tag_##x, x> g_buffer_pool_##x;

DEFINE_POOL_OBJECT(128);
DEFINE_POOL_OBJECT(192);
DEFINE_POOL_OBJECT(256);
DEFINE_POOL_OBJECT(384);
DEFINE_POOL_OBJECT(512);
DEFINE_POOL_OBJECT(786);
DEFINE_POOL_OBJECT(1024);
DEFINE_POOL_OBJECT(1536);
DEFINE_POOL_OBJECT(2048);
DEFINE_POOL_OBJECT(3072);
DEFINE_POOL_OBJECT(4096);
DEFINE_POOL_OBJECT(6144);
DEFINE_POOL_OBJECT(MAX_PACKET_MESSAGE);

#define MALLOC_BUFFER( x )	do { \
	buffer_ = (tBYTE*)g_buffer_pool_##x::malloc(); \
} while(0)

#define FREE_BUFFER( buffer, x )	g_buffer_pool_##x::free( buffer );

static tINT *packet_size_index = NULL;

void rnPacket::init(void)
{
	tINT base_index[] =
	{
		128,
		192,
		256,
		384,
		512,
		786,
		1024,
		1536,
		2048,
		3072,
		4096,
		6144,
		MAX_PACKET_MESSAGE,
	};

	packet_size_index = new tINT[MAX_PACKET_MESSAGE + 1];
	tINT index = 0;
	for (int i = 0; i <= MAX_PACKET_MESSAGE; ++i)
	{
		packet_size_index[i] = index;

		if (i >= base_index[index])
		{
			++index;
		}
	}
}

void rnPacket::mallocBuffer( tINT size )
{
	size += PACKET_CHECK_SUM_SIZE;
	init_alloc_size_ = size;

	if( size <= MAX_PACKET_MESSAGE )
	{
		tINT index = packet_size_index[size];
		switch( index )
		{
		case 0:
			MALLOC_BUFFER(128);
			break;
		case 1:
			MALLOC_BUFFER(192);
			break;
		case 2:
			MALLOC_BUFFER(256);
			break;
		case 3:
			MALLOC_BUFFER(384);
			break;
		case 4:
			MALLOC_BUFFER(512);
			break;
		case 5:
			MALLOC_BUFFER(786);
			break;
		case 6:
			MALLOC_BUFFER(1024);
			break;
		case 7:
			MALLOC_BUFFER(1536);
			break;
		case 8:
			MALLOC_BUFFER(2048);
			break;
		case 9:
			MALLOC_BUFFER(3072);
			break;
		case 10:
			MALLOC_BUFFER(4096);
			break;
		case 11:
			MALLOC_BUFFER(6144);
			break;
		default:
			MALLOC_BUFFER(MAX_PACKET_MESSAGE);
			break;
		}

		pool_index_ = index;
	}
	else
	{
		// TO DO : 에러 처리할것
		LOG_ERROR("invalid packet size[%d]", size);
		return;
	}

	tINT test_offset = init_alloc_size_ - PACKET_CHECK_SUM_SIZE;

	buffer_[test_offset] = 0xff;
	buffer_[test_offset + 1] = 0xff;
	buffer_[test_offset + 2] = 0xff;
}

rnPacket::rnPacket()
{
	tUINT size = MAX_PACKET_MESSAGE - PACKET_CHECK_SUM_SIZE;
	mallocBuffer(size);
	next_store_pos_ = 0;
}

rnPacket::rnPacket( std::string& msg )
{
	tUINT size = msg.length();
	mallocBuffer(size);
	memcpy(buffer_, msg.c_str(), msg.length());
	next_store_pos_ = size;
}

rnPacket::rnPacket( const char* msg, const int length )
{
	tUINT size = length;
	mallocBuffer(size);
	memcpy(buffer_, msg, length);
	next_store_pos_ = length;
}

rnPacket::~rnPacket()
{
	tINT test_offset = init_alloc_size_ - PACKET_CHECK_SUM_SIZE;

	if( buffer_[test_offset] != 0xff ||
			buffer_[test_offset + 1] != 0xff ||
			buffer_[test_offset + 2] != 0xff )
	{
		LOG_ERROR( "rnPacket::check error" );
		return;
	}

	switch( pool_index_ )
	{
	case 0:
		FREE_BUFFER(buffer_, 128);
		break;
	case 1:
		FREE_BUFFER(buffer_, 192);
		break;
	case 2:
		FREE_BUFFER(buffer_, 256);
		break;
	case 3:
		FREE_BUFFER(buffer_, 384);
		break;
	case 4:
		FREE_BUFFER(buffer_, 512);
		break;
	case 5:
		FREE_BUFFER(buffer_, 786);
		break;
	case 6:
		FREE_BUFFER(buffer_, 1024);
		break;
	case 7:
		FREE_BUFFER(buffer_, 1536);
		break;
	case 8:
		FREE_BUFFER(buffer_, 2048);
		break;
	case 9:
		FREE_BUFFER(buffer_, 3072);
		break;
	case 10:
		FREE_BUFFER(buffer_, 4096);
		break;
	case 11:
		FREE_BUFFER(buffer_, 6144);
		break;
	default:
		FREE_BUFFER(buffer_, MAX_PACKET_MESSAGE);
		break;
	}
}

bool rnPacket::addString( std::string& msg )
{
	if (next_store_pos_ + msg.length() >= MAX_PACKET_MESSAGE)
		return false;

	tBYTE* p = buffer_ + next_store_pos_;
	memcpy(p, msg.c_str(), msg.length());
	next_store_pos_ += msg.length();
	p[next_store_pos_] = '\0';

	return true;
}

bool rnPacket::addString( const tBYTE* msg, tUINT length )
{
	if (next_store_pos_ + length >= MAX_PACKET_MESSAGE)
		return false;

	tBYTE* p = buffer_ + next_store_pos_;
	memcpy(p, msg, length);
	next_store_pos_ += length;
	p[next_store_pos_] = '\0';

	return true;
}

void rnPacket::calc()
{
	std::string str((char *)buffer_);
	stokenizer token(str, sep);

	vec_.assign(token.begin(), token.end());
}