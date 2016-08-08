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
	close(epoll_fd);
}

Server::create_and_listen() {
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(SERVER_PORT);
	// create socket to listen
	listen_fd = socket(PF_INET, SOCK_STREAM, 0);
	if(listen_fd < 0) {
			
	}
}

}
