#include"EchoServer.hpp"
#include"Dict.hpp"
#include<memory>

static void Usage(const std::string process)
{
    std::cerr << "Usage:\n\t";
    std::cerr << process << "local port" << std::endl;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }

    Dict dict;

    uint16_t server_port = std::stoi(argv[1]);
    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>([&dict](std::string word)->std::string{
        return dict.Translate(word);
    },server_port);
    usvr->Init();
    usvr->Loop(); 
    return 0;
}