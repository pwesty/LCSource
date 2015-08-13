#include "bnf.h"
#include "event_session.h"

EventSession::EventSession( boost::asio::io_service& io_service )
	: SessionBase(io_service, SessionBase::EVENT_SESSION)
{
	bnf::instance()->event_session_seq_.pop(handle_);
}

EventSession::~EventSession( void )
{
	bnf::instance()->event_session_seq_.push(handle_);
}

void EventSession::push( type_t data)
{
	if (io_service_.stopped())
		return;
	
	queue_.push(data);

	bnf::instance()->PutSessionEvent(SessionEvent::ON_EVENT, this);
}

EventSession::type_t EventSession::pop()
{
	return queue_.front_pop();
}