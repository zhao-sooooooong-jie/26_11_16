#ifndef __ECHOSERVER_HPP
#define __ECHOSERVER_HPP

#include<iostream>
#include<cstdlib>
#include<string>
#include <cstdint>
#include<strings.h>
#include<unistd.h>
#include<functional>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"logger.h"

const static int default_fd = -1;
const static int default_port = -1;

using callback_t = std::function<std::string(std::string)>;

enum
{
    SUCCESS = 0,
    SOCKET_ERR,
    USAGE_ERR,
    BIND_ERR,
    USAGE_ERR,
};

class UdpServer{
public:
    UdpServer(callback_t cb, uint16_t port = default_port)
    :_sockfd(default_fd),
     _cb(cb),
     _port(port)
    {}
    ~UdpServer(){
        close(_sockfd);
    }
    void Init()
    {
        //1.创建socket,本质：打开网卡
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(_sockfd < 0)
        {
            LOG_FATAL("creat socket error");
            exit(SOCKET_ERR);
        }
        LOG_INFO("create socket success, sockfd");

        //2.完善信息
        struct sockaddr_in local;
        bzero(&local, sizeof(local));
        local.sin_family = AF_INET;
        //local.sin_addr.s_addr = inet_addr(_ip.c_str());
        local.sin_addr.s_addr = INADDR_ANY; //
        local.sin_port = htons(_port);//h->n

        //3.bind socket 信息
        int n = bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
        if(n < 0)
        {
            LOG_FATAL("bind suruct error");
            exit(BIND_ERR);
        }
        LOG_DEBUG("bind socket success, "<< ",port:" << _port);
    }
    void Loop(){

        char inbuffer[1024];
        while(true)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            ssize_t n = recvfrom(_sockfd, &inbuffer, sizeof(inbuffer)-1 , 0, (struct sockaddr *)&peer, &len);
            if(n > 0){
                 inbuffer[n] = 0;
                /*LOG_DEBUG("clint say#" << inbuffer);
                std::string echo_string = "server echo# ";
                echo_string += inbuffer; */

                std::string result = _cb(inbuffer);

                std::string client_ip = inet_ntoa(peer.sin_addr);
                uint16_t client_port = peer.sin_port;
                std::string client_address = "[" + client_ip + ":" + std::to_string(client_port) +"]#" + result;
                sendto(_sockfd, client_address.c_str(), client_address.size(), 0, (struct sockaddr *)&peer, len);

            }
            else{
                LOG_ERROR("recvfrom error");
            }
        }
    }


private:
    int _sockfd;
    uint16_t _port;
    callback_t _cb;//用回调的方式

};

#endif