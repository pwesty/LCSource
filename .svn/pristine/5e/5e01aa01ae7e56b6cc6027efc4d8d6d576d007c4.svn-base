#ifndef __SEQ_MANAGER_H__
#define __SEQ_MANAGER_H__

#include <assert.h>
#include <boost/thread/recursive_mutex.hpp>

#define SEQ_MANAGER_GROW_INDEX_COUNT			(32)

template<class _Ty>
class seq_manager
{
public:
	seq_manager() : total_count_(0)
	{
	}

	~seq_manager()
	{
	}

	void init(_Ty _startno, _Ty _endno)
	{
		assert(_startno < _endno);

		for (_Ty i = _startno; i <= _endno; ++i)
		{
			tbuf_.push_back(i);
		}

		total_count_ = tbuf_.size();
		endno_ = _endno;
	}

	// return value가 false이면 자동 증가된 상태
	bool pop(_Ty &data)
	{
		bool flag = true;

		if (tbuf_.empty())
		{
			for (int i = 0; i < SEQ_MANAGER_GROW_INDEX_COUNT; ++i)
			{
				tbuf_.push_back(++endno_);
			}

			total_count_ += SEQ_MANAGER_GROW_INDEX_COUNT;
			flag = false;
		}

		data = tbuf_.front();
		tbuf_.pop_front();

		return flag;
	}

	bool push(_Ty index)
	{
		if (tbuf_.size() >= total_count_)
		{
			return false;
		}

		tbuf_.push_back(index);
		return true;
	}

private:
	volatile size_t			total_count_;
	std::deque<_Ty>			tbuf_;
	_Ty						endno_;
};

template<class _Ty>
class seq_manager_ts : public seq_manager<_Ty>
{
public:
	seq_manager_ts() {}
	~seq_manager_ts() {}

	bool pop(_Ty &data)
	{
		boost::recursive_mutex::scoped_lock lock(mutex);
		return seq_manager<_Ty>::pop(data);
	}

	bool push(_Ty index)
	{
		boost::recursive_mutex::scoped_lock lock(mutex);
		return seq_manager<_Ty>::push(index);
	}

private:
	boost::recursive_mutex mutex;
};

#endif
