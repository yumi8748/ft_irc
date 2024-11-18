#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <vector>
#include "Channel.hpp"

class Client{
    private:
        std::string nick;
        std::string usr;
        std::vector<Channel*> channel;
        int socket_fd;
        std::string recv_buffer;
    public:
        sendMessage(std::string message);
        addChannel(Channel* channel);
        removeChannel(Channel* channel);
        recv();
        send();
};

#endif