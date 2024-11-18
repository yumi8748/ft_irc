#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <iostream>
#include <vector>
#include "Client.hpp"

class Channel{
    private:
        std::string name;
        std::vector<Client*> client;
        std::string password;
        int limits;
        std::string topic;
    public:
        // Channel();
        // ~Channel();
        // Channel(const Channel &src);
        // Channel &operator=(const Channel &src);
        addClient(Client* client);
        removeClient(Client* client);
        broadcastMessage(std::string message);
        setTopic(std::string newTopic);
        changePassword(std::string newPassword);


};

#endif