 ///
 /// @file    EventfdThread.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2019-06-14 11:21:27
 ///
 
#ifndef __WD_EVENTFDTHREAD_H__
#define __WD_EVENTFDTHREAD_H__

#include "Eventfd.h"
#include "Thread.h"


             
namespace wd
{
class EventfdThread
{
	using EventfdCallback = function<void()>;
public:
	EventfdThread(EventfdCallback && cb)
	: _eventfd(move(cb))
	, _thread(bind(&Eventfd::start, &_eventfd))
	{}

	void start()
	{
		_thread.start();
	}

	void stop()
	{
		_eventfd.stop();
		_thread.join();
	}

	void wakeup()
	{
		_eventfd.wakeup();
	}

private:
	Eventfd _eventfd;
	Thread _thread;
};
}//end of namespace wd

#endif 
 
