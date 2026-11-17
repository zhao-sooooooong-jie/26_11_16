#ifndef __ECHOSERVER_HPP
#define __ECHOSERVER_HPP
#include <iostream>
#include <cstdint>
#include <strings.h>
#include <unistd.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "logger.h"
#include "IneAddr.hpp"

const static int default_fd = -1;
const static int default_port = -1;
const static int gbacklog = 16;

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
        : _listensockfd(default_fd),
          _port(port)
    {
    }
    ~TcpServer() {}

    void Init()
    {
        // 1
        _listensockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_listensockfd < 0)
        {
            LOG_FATAL("listensockfd error");
            exit(SOCK_ERR);
        }
        LOG_INFO("listensockfd success");

        // 2
        struct sockaddr_in local;
        bzero(&local, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_port = htons(_port);

        //
        int n = bind(_listensockfd, (sockaddr *)&local, sizeof(local));
        if (n < 0)
        {
            LOG_FATAL("bind error");
            exit(BIND_ERR);
        }
        LOG_INFO("bind success");

        n = listen(_listensockfd, gbacklog);
        if (n < 0)
        {
            LOG_FATAL("listen error");
            exit(LISTEN_ERR);
        }
        LOG_INFO("listen success");
    }

    void serverIO(int sockfd, IneAddr address)
    {
        while (true)
        {
            char inbuffer[1024] = {0};
            ssize_t n = read(sockfd, inbuffer, sizeof(inbuffer) - 1);
            if (n > 0)
            {
                inbuffer[n] = 0;
                std::string echo_string = "Server Say#";
                echo_string += inbuffer;
                n = write(sockfd, echo_string.c_str(), echo_string.size());
            }
            else if (n < 0)
            {
                LOG_ERROR("read error");
            }
            else
            {
                LOG_INFO("client quit");
            }
        }
    }

    void Loop()
    {
        while (true)
        {
            struct sockaddr_in clientaddr;
            socklen_t clientlen = sizeof(clientaddr);

            const int sockfd = accept(_listensockfd, (sockaddr *)&clientaddr, &clientlen);
            IneAddr clientaddress(clientaddr);
            if (sockfd > 0)
            {
                serverIO(sockfd, clientaddress);
            }
            else if (sockfd == 0)
            {
                LOG_DEBUG(clientaddress.ToString() << "#quit");
            }
            else
            {
                LOG_WARN(clientaddress.ToString() << "#accept error");
            }
        }
    }

private:
    int _listensockfd;
    uint16_t _port;
};

#endif