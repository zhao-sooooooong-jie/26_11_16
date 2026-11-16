#ifndef __ECHOSERVER_HPP
#define __ECHOSERVER_HPP
#include<iostream>
#include<cstdint>
#include<strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include"logger.h""

const static int default_fd = -1;
const static int default_port = -1;

enum
{
    SUCCESS = 0,
    SOCK_ERR,
    BIND_ERR,
    LISTEN_ERR,
    USAGE_ERR,
};

class TcpServer
{
public:
    TcpServer(uint16_t port = default_port)
    :_sockfd(default_fd),
     _port(port)
    {}
    ~TcpServer(){}

    void Init()
    {
        //1
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(_sockfd < 0)
        {
            LOG_FATAL("socket error");
            exit(SOCK_ERR);
        }
        LOG_INFO("socket success");
        

        //2
        struct sockaddr_in local;
        bzero(&local, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_port = htons(_port);

        //
        int n = bind(_sockfd, (sockaddr *)&local, sizeof(local));
        if(n < 0)
        {
            LOG_FATAL("bind error");
            exit(BIND_ERR);
        }
        LOG_INFO("bind success");

        n = listen(_sockfd, _backlog);
        if(n < 0)
        {
            LOG_FATAL("listen error");
            exit(LISTEN_ERR);
        }
        LOG_INFO("listen success");

    }

    void Loop(){}

private:
    int _sockfd;
    int _backlog = 16;
    uint16_t _port;
};





#endif