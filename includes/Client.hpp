#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <vector>
#include "Channel.hpp"
#include "Irc.hpp"

class Client{
    private:
        std::string nick;
        std::string usr;
        std::vector<Channel*> channels;
        int client_fd;
        std::string recv_buf;
    public:
        Client(int fd);
        void setNickname(const std::string& nickname);
        void setUsername(const std::string& username);
        const std::string& getNickname() const;
        void sendMessage(const std::string &message);
        void addChannel(Channel* ch);
        void removeChannel(Channel* channel);
        void Recv();
        void Send();
        int getFd() const;
};

#endif