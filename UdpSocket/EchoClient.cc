#include<iostream>
#include<string>
#include<cstring>
#include<cstdlib>
#include<cstdint>
#include<memory>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

const static void Usage(std::string proc)
{
    std::cerr << "Usage:\n\t";
    std::cerr << proc << "local ip local prot";
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }
    std::string server_ip = argv[1];
    uint16_t server_port = std::stoi(argv[2]);

    //1.创建socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "socket error" << std::endl;
        exit(2);
    }

    //2.
    struct sockaddr_in server;
    memset(&server, 0, sizeof(0));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_ip.c_str());
    server.sin_port = htons(server_port);
    

    while(true)
    {
        //1.获取用户输入
        std::string message;
        std::cout << "Please Enter:";
        std::getline(std::cin, message);

        //2.先传后读
        ssize_t n = sendto(sockfd, message.c_str(), message.size(), 0, (struct sockaddr *)&server, sizeof(server));
        if(n > 0)
        {
            char inbuffer[1024] = {0};
            struct sockaddr_in temp;
            socklen_t len = sizeof(temp);
            ssize_t m = recvfrom(sockfd, &inbuffer, sizeof(inbuffer)-1, 0, (struct sockaddr *)&temp, &len);
            if(m > 0)
            {
                inbuffer[m] = 0;
                std::cout << inbuffer << std::endl;
            }
        }
        else
        {
            exit(3);
        }
    }



    return 0;
};