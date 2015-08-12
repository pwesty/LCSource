#ifndef __RNSOCKET_IO_SERVICE_H__
#define __RNSOCKET_IO_SERVICE_H__

#include "session_base.h"
//#include "MemoryPoolBase.h"
#include "queue_ts.h"

// #include "rnpacket.h"
#include "NetMsg.h"
#include "BPacket.h"


class ListenSession;
class ListenSessionTcp;
class bnf;

class rnSocketIOService : public SessionBase
{
public:
	friend class ListenSession;
	friend class ListenSessionTcp;
	friend class bnf;

//	typedef boost::shared_ptr<rnSocketIOService>	SP;

public:
	rnSocketIOService(boost::asio::io_service& io_service, session_type type)
		: SessionBase(io_service, type)
		, crypt_flag_(false)
	{
	};
	virtual ~rnSocketIOService()	{};

	virtual void							Open(session_handle handle, int packet_type) = 0;
	virtual void							Open(session_handle handle, session_handle listen_handle, int read_wait_time_sec, int write_wait_time_sec ) = 0;

	virtual bool							AsyncConnect( const std::string& host, int port ) = 0;

	virtual session_handle					GetListenSessionHandle() = 0;

//	virtual boost::asio::ip::tcp::socket&	Socket() = 0;

	virtual void							Run() = 0;
	virtual void							Close(std::string err_str) = 0;

	virtual void							ShutdownRead() = 0;
	virtual void							ShutdownWrite() = 0;
	virtual void							ShutdownBoth() = 0;

	virtual std::string&					ip() = 0;
	virtual unsigned int					ipnumber() = 0;

	virtual void							deliver(CNetMsg::SP packet) = 0;
	virtual void							deliver(CNetMsg* pPacket) = 0;

	//////////////////////////////////////////////////////////////////////////
	// only lastchaos - connector(billing)
	virtual void							deliver(CBPacket::SP packet) = 0;
	virtual void							deliver(CBPacket* pPacket) = 0;
	//////////////////////////////////////////////////////////////////////////
	// only lastchaos - connector(billing for TLD)
	virtual void							deliverForTLDBilling(CNetMsg::SP packet) = 0;
	virtual void							deliverForTLDBilling(CNetMsg *pPacket) = 0;
	//////////////////////////////////////////////////////////////////////////

	virtual CNetMsg*						GetMessage() = 0;
	virtual CBPacket*						GetMessageForBilling() = 0;

	virtual bool							isValid() = 0;

	// socket option
	virtual void							ReceiveBufferSize(size_t size) = 0;
	virtual int								ReceiveBufferSize() = 0;
	virtual void							SendBufferSize(size_t size) = 0;
	virtual int								SendBufferSize() = 0;
	virtual void							NoDelayOn() = 0;
	virtual void							NoDelayOff() = 0;
	virtual void							DoNotRouteOn() = 0;
	virtual void							DoNotRouteOff() = 0;
	virtual void							Linger(bool onOff, int linger) = 0;
	virtual void							DebugOn() = 0;
	virtual void							DebugOff() = 0;
	virtual void							KeepAliveOn() = 0;
	virtual void							KeepAliveOff() = 0;
	virtual void							BroadcastOn() = 0;
	virtual void							BroadcastOff() = 0;
	virtual void							NonBlockingIoOn() = 0;
	virtual void							NonBlockingIoOff() = 0;

	virtual int								getWirteQueueCount() = 0;

	virtual void							closeOnExecOn() = 0;
	virtual void							closeOnExecOff() = 0;

	virtual bool							isTimeout() = 0;

	// only lastchaos
	void									setCrypt()
	{
		crypt_flag_ = true;
	}

protected:
	virtual void							_getIp() = 0;

protected:
	bool									crypt_flag_; // only lastchaos
};

#endif
