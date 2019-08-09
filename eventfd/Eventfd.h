 ///
 /// @file    Eventfd.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2019-06-14 10:12:57
 ///
 
#ifndef __WD_EVENTFD_H__
#define __WD_EVENTFD_H__


#include <functional>
#include <iostream>
using namespace std;


namespace wd
{

class Eventfd
{
	using EventfdCallback = function<void()>;
public:
	Eventfd(EventfdCallback && cb)
	: _fd(createEventfd())
	, _cb(move(cb))
	, _isStarted(false)
	{
		cout << "eventfd = " << _fd << endl;
	}

	void start();
	void stop();
	void wakeup();

private:
	int createEventfd();
	void handleRead();

private:
	int _fd;
	EventfdCallback _cb;
	bool _isStarted;
};

}
             
 

#endif
