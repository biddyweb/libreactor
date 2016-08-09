//////////////////////////////////////////////////////////////////////////
// File Name: server.cc
// Author: SongLee
// E-mail: lisong.shine@qq.com 
// Created Time: Mon 08 Aug 2016 03:13:01 PM UTC
// Personal Blog: songlee24.github.io
//////////////////////////////////////////////////////////////////////////

#include "server.h"

namespace reactor {

Server::Server() {
	create_and_listen();
}

Server::~Server() {
	close(listen_fd);
	close(epfd);
}

void Server::create_and_listen() {
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(SERVER_PORT);
	// 1. create socket to listen 
	listen_fd = socket(PF_INET, SOCK_STREAM, 0);
	if(listen_fd < 0) {
		LOG(ERROR, "Create socket failed.");			
	} else {
		LOG(INFO, "Create socket successfully.");
	}
	//int FLAGS = fcntl(listen_fd, F_GETFL, 0);
	//fcntl(listen_fd, F_SETFL, FLAGS | O_NONBLOCK); // non-blocking
	int on = 1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	// 2. bind socket to server address
	int res = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));	
	if(res == -1) {
		LOG(ERROR, "Server bind failed.");
	} else {
		LOG(INFO, "Server bind successfully.");
	}
	// 3. listen incoming connection
	res = listen(listen_fd, LISTEN_QUEUE);
	if(res == -1) {
		LOG(ERROR, "Server listen failed.");
	} else {
		LOG(INFO, "Server listen successfully.");
	}
}

void Server::start() {
	epfd = epoll_create(1);  // 创建epoll fd
	
	struct epoll_event event;
	event.data.fd = listen_fd;
	event.events = EPOLLIN;
	epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &event);  // 注册listen_fd

	while(1) {
		int nums = epoll_wait(epfd, events, EVENT_SIZE, -1);
		if (nums == -1) {
			LOG(ERROR, "epoll_wait error.");
			break;
		}
		for(int i=0; i<nums; ++i) {   // 遍历所有就绪事件
			int fd = events[i].data.fd;
			if((fd == listen_fd) && (events[i].events & EPOLLIN))
				Accept();   // 有新的客户端请求
			else if(events[i].events & EPOLLIN)
				Recv(fd);   // 读数据
			else
				;
		}
	}	
}

void Server::Accept() {
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	
	int new_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
	if(new_fd < 0) {
		LOG(ERROR, "Server accept failed.");
	} else {
		LOG(INFO, "Server accept %s, connection established.", inet_ntoa(client_addr.sin_addr));	
	}

	// 在epfd中注册新建立的连接
	struct epoll_event event;
	event.data.fd = new_fd;
	event.events = EPOLLIN;

	epoll_ctl(epfd, EPOLL_CTL_ADD, new_fd, &event);
}

void Server::Recv(int fd) {
	bool close_conn = false;  // 标记当前连接是否断开
	
	PackageHead head;         // 先接收包头
	recv(fd, &head, sizeof(head), 0);
	
	char* buffer = new char[head.length];
	bzero(buffer, head.length);	

	int total = 0;
	while(total < head.length) {
		int len = recv(fd, buffer + total, head.length - total, 0);
		if(len < 0) {
			LOG(ERROR, "recv() error.");
			close_conn = true;
			break;
		}
		total = total + len;
	}
	
	if(total == head.length) {   // 将收到的消息原样发回给客户端
		int ret1 = send(fd, &head, sizeof(head), 0);
		int ret2 = send(fd, buffer, head.length, 0);
		if(ret1 < 0 || ret2 < 0) {
            LOG(ERROR, "send() error.");
			close_conn = true;
        }
	}

	delete [] buffer; 

	if(close_conn) {  // 当前这个连接有问题，关闭它
		close(fd);
		struct epoll_event event;
		event.data.fd = fd;
		event.events = EPOLLIN;
		epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &event);  // 注销
	}
}

}
