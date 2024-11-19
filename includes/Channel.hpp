#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "Client.hpp"

class Channel{
    private:
        std::string name;
        std::vector<Client*> clients;
        std::string password;
        unsigned int limits;
        std::string topic;
        std::map<std::string, std::string> modes;
    public:
        // Channel();
        // ~Channel();
        // Channel(const Channel &src);
        // Channel &operator=(const Channel &src);
        Channel(const std::string& n);
        void addClient(Client* client);
        void removeClient(Client* client);
        void broadcastMessage(std::string message);
        void setTopic(std::string newTopic);
        void changePassword(std::string newPassword);

};

#endif