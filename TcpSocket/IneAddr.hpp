#pragma once

#include<string>
#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

class IneAddr
{
public:
    IneAddr(sockaddr_in &process)
    :_address(process),
     _len(sizeof(process))
    {
        _ip = inet_ntoa(process.sin_addr);
        _port = ntohs(process.sin_port);
    }

    IneAddr(std::string ip, uint16_t port)
    :_ip(ip),
     _port(port)
    {
        _address.sin_family = AF_INET;
        _address.sin_addr.s_addr = inet_addr(ip.c_str());
        _address.sin_port = htons(port);
        _len = sizeof(_address);
    }

    IneAddr(){}

    bool operator == (const IneAddr &addr)
    {
        return (addr._ip == this->_ip) && (addr._port == this->_port);
    }

    std::string ToString()
    {
        return "[" + _ip + " " + std::to_string(_port) + "]";
    }

    sockaddr_in * GetNetAddress()
    {
        return &_address;
    }

    uint16_t GetLen()
    {
        return _len;
    }

    ~IneAddr(){}


private:
    struct sockaddr_in _address;
    socklen_t _len;
    std::string _ip;
    uint16_t _port;

};