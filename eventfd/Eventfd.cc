 ///
 /// @file    Eventfd.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2019-06-14 10:16:54
 ///
 
#include "Eventfd.h"

#include <unistd.h>
#include <poll.h>
#include <sys/eventfd.h>

#include <iostream>
using namespace std;
             
 
namespace wd
{


void Eventfd::start()
{
	_isStarted = true;

	struct pollfd pfd;
	pfd.fd = _fd;
	pfd.events = POLLIN;

	while(_isStarted) {
		int nready = ::poll(&pfd, 1, 5000);
		if(nready == -1 && errno == EINTR)
			continue;
		else if(nready == -1) {
			perror(">> poll");
			return;
		} else if(nready == 0) {
			printf(">> poll timeout!\n");	
		} else {
			if(pfd.revents & POLLIN) {
				handleRead();//处理读事件
				//如果没有处理该事件，会一直触发
				if(_cb) 
					_cb();//执行任务
			}
		}
	}
}

void Eventfd::stop()
{
	if(_isStarted) {
		_isStarted = false;
	}
}

void Eventfd::wakeup()
{
	uint64_t one = 1;
	int ret = ::write(_fd, &one, sizeof(one));
	if(ret != sizeof(one)) {
		perror(">> write");
	}
}

int Eventfd::createEventfd()
{
	int fd = ::eventfd(10, 0);
	if(fd == -1) {
		perror(">> eventfd");
	}
	return fd;
}


void Eventfd::handleRead()
{
	uint64_t howmany;
	int ret = ::read(_fd, &howmany, sizeof(howmany));
	if(ret != sizeof(howmany)) {
		perror(">> read");
	}
}
}//end of namespace wd
