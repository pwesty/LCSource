#ifndef __EVENT_SESSION_H__
#define __EVENT_SESSION_H__

#include <boost/any.hpp>
#include "session_base.h"
#include "MemoryPoolBase.h"
#include "queue_ts.h"

class EventSession : public SessionBase, public MemoryPoolBaseWithMutex<EventSession>
{
public:
	typedef std::pair<int, boost::any> type_t;

public:
	explicit EventSession(boost::asio::io_service& io_service);
	~EventSession(void);

	void push(type_t);
	type_t pop();

private:
	queue_ts<type_t>	queue_;
};

#endif
