#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Channel.hpp"
#include "Commands.hpp"
#include "Server.hpp"
#include "Macros.hpp"

class Channel;

class Client{
    private:
        std::string nick;
        std::string usr;
        std::vector<Channel*> channels;
        std::vector<Channel*> joinedChannels; // 用戶加入的頻道
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
        bool isInvited(Client* client, Channel* channel);
};

#endif
