//////////////////////////////////////////////////////////////////////////
// File Name: server.h
// Author: SongLee
// E-mail: lisong.shine@qq.com 
// Created Time: Mon 08 Aug 2016 03:12:18 PM UTC
// Personal Blog: songlee24.github.io
//////////////////////////////////////////////////////////////////////////

#ifndef __REACTOR_SERVER_H__
#define __REACTOR_SERVER_H__

#include <sys/socket.h>  // socket
#include <sys/epoll.h>   // epoll
#include <arpa/inet.h>
#include <unistd.h>      // close
#include <fcntl.h>       // fcntl
#include <errno.h>
#include <cstring>
#include <iostream>
#include "common.h"

#define BUFFER_SIZE 1024
#define EVENT_SIZE 100 
#define SERVER_PORT 15000
#define LISTEN_QUEUE 20

namespace reactor {

class Server {
private:
	struct sockaddr_in server_addr;
	socklen_t server_addr_len;

	int listen_fd;   // 监听fd
	int epfd;    // epoll fd
	struct epoll_event events[EVENT_SIZE];  // epoll_wait返回的就绪事件
public:
	Server();
	~Server();
	void create_and_listen();
	void Accept();
	void Recv(int);
	void start();
};

}

#endif // __REACTOR_SERVER_H__
