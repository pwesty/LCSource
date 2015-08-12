#ifndef __RNSOCKET_IO_SERVICE_TCP_H__
#define __RNSOCKET_IO_SERVICE_TCP_H__

#include <boost/atomic.hpp>
#include <boost/array.hpp>

#include "rnsocketioservice.h"
#include "session_base.h"
#include "MemoryPoolBase.h"
#include "queue_ts.h"

#include "rnpacket.h"

class ListenSessionTcp;
class bnf;

class rnSocketIOServiceTcp : public rnSocketIOService, public MemoryPoolBaseWithMutex<rnSocketIOServiceTcp>
{
public:
	friend class ListenSessionTcp;
	friend class bnf;

//	typedef boost::shared_ptr<rnSocketIOServiceTcp>	SP;

public:
	rnSocketIOServiceTcp(boost::asio::io_service& io_service);
	virtual ~rnSocketIOServiceTcp();

	virtual void							Open(session_handle handle, int packet_type);
	virtual void							Open(session_handle handle, session_handle listen_handle, int read_wait_time_sec, int write_wait_time_sec );

	virtual bool							AsyncConnect( const std::string& host, int port );

	virtual session_handle					GetListenSessionHandle()
	{
		return listen_session_handle_;
	}

	virtual void							Run();
	virtual void							Close(std::string err_str);

	virtual void							ShutdownRead();
	virtual void							ShutdownWrite();
	virtual void							ShutdownBoth();

	virtual std::string&					ip();
	virtual unsigned int					ipnumber();

	virtual void							deliver(CNetMsg::SP packet);
	virtual void							deliver(CNetMsg *pPacket)
	{
		deliver(CNetMsg::SP(pPacket));
	}

	//////////////////////////////////////////////////////////////////////////
	// only lastchaos - connector(billing)
	virtual void							deliver(CBPacket::SP packet);
	virtual void							deliver(CBPacket* pPacket)
	{
		deliver(CBPacket::SP(pPacket));
	}
	//////////////////////////////////////////////////////////////////////////
	// only lastchaos - connector(billing for TLD)
	virtual void							deliverForTLDBilling(CNetMsg::SP packet);
	virtual void							deliverForTLDBilling(CNetMsg *pPacket)
	{
		deliverForTLDBilling(CNetMsg::SP(pPacket));
	}
	//////////////////////////////////////////////////////////////////////////

	virtual CNetMsg*						GetMessage()
	{
		return read_queue_.front_pop();
	}
	virtual CBPacket*						GetMessageForBilling()
	{
		return read_queue_for_billing_.front_pop();
	}

	virtual bool							isValid()
	{
		return (handle_ != INVALID_SESSION_HANDLE && socket_.is_open());
	}

	// socket option
	virtual void							ReceiveBufferSize(size_t size);
	virtual int								ReceiveBufferSize();
	virtual void							SendBufferSize(size_t size);
	virtual int								SendBufferSize();
	virtual void							NoDelayOn();
	virtual void							NoDelayOff();
	virtual void							DoNotRouteOn();
	virtual void							DoNotRouteOff();
	virtual void							Linger(bool onOff, int linger);
	virtual void							DebugOn();
	virtual void							DebugOff();
	virtual void							KeepAliveOn();
	virtual void							KeepAliveOff();
	virtual void							BroadcastOn();
	virtual void							BroadcastOff();
	virtual void							NonBlockingIoOn();
	virtual void							NonBlockingIoOff();

	virtual int								getWirteQueueCount()
	{
		return write_queue_.size();
	}

	virtual void							closeOnExecOn();
	virtual void							closeOnExecOff();

	virtual bool							isTimeout()
	{
		return timeout_;
	}

	void									setCheckSequence()
	{
		check_seq_flag_ = true;
	}

	bool									isCallCloseFunction()
	{
		return is_call_close_function_;
	}

protected:
	virtual void							_getIp();

	boost::asio::ip::tcp::socket&			Socket()
	{
		return socket_;
	}

	void							Init();
	void							SetReadWaitTimer();
	void							SetWriteWaitTimer();

	void							HandleAsyncConnect( const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoint_iterator );
	void							HandleReadHeader(const boost::system::error_code& error);
	void							HandleReadBody(const boost::system::error_code& error);
	void							HandleWrite(const boost::system::error_code& error, std::size_t bytes_transferred);
	void							ReadWaitTimerClose(const boost::system::error_code& error);
	void							WriteWaitTimerClose(const boost::system::error_code& error);
	void							__close();
	void							__deliver(CNetMsg::SP packet);
	void							__deliver_for_cbpacket(CBPacket::SP packet);
	void							__deliver__for_tld_billing(CNetMsg::SP packet);

	//////////////////////////////////////////////////////////////////////////
	// only lastchaos - connector(billing)
	void							HandleReadHeaderForBilling(const boost::system::error_code& error);
	void							HandleReadBodyForBilling(const boost::system::error_code& error);
	void							HandleWriteForBilling(const boost::system::error_code& error, std::size_t bytes_transferred);
	//////////////////////////////////////////////////////////////////////////
	// only lastchaos - connector(billing for TLD)
	void							HandleReadHeaderForTLDBilling(const boost::system::error_code& error);
	void							HandleReadBodyForTLDForBilling(const boost::system::error_code& error);
	void							HandleWriteForTLDBilling(const boost::system::error_code& error, std::size_t bytes_transferred);
	//////////////////////////////////////////////////////////////////////////

	bool							isInvalidAction();

	int								getUniqueId()		{ return unique_id_; }

private:
	boost::asio::ip::tcp::socket	socket_;
	boost::asio::deadline_timer		read_wait_timer_;
	int								read_wait_time_sec_;
	boost::asio::deadline_timer		write_wait_timer_;
	int								write_wait_time_sec_;
	std::string						ip_;
	unsigned int					ipnumber_;

	session_handle					listen_session_handle_;

	MsgHeader						packet_header_;
	CNetMsg*						now_packet_;
	queue_ts<CNetMsg*>				read_queue_;
	std::deque<CNetMsg::SP>			write_queue_;

	//////////////////////////////////////////////////////////////////////////
	// only lastchaos - connector(billing)
	BPacketHeader					packet_header_for_billing_;
	CBPacket*						now_packet_for_billing_;
	queue_ts<CBPacket*>				read_queue_for_billing_;
	std::deque<CBPacket::SP>		write_queue_for_billing_;
	//////////////////////////////////////////////////////////////////////////

	bool							write_error_;

	bool							timeout_;

	bool							close_flag_;
	bool							close_completed_;

	bool							is_call_close_function_;		// close 계열의 함수를 호출하면 true

	unsigned int					seq_; // only lastchaos
	bool							check_seq_flag_;
	int								packet_type_; // only lastchaos-connect, 0 - in game, 1 - billing

	int								unique_id_;			// 각 세션이 연결될때마다 부여받는 고유 번호

	boost::asio::strand				boost_strand_;
};

inline bool	rnSocketIOServiceTcp::isInvalidAction()
{
	return (socket_.is_open() == false || io_service_.stopped());
}

#endif
