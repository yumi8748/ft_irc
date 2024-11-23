#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "Client.hpp"
#include <algorithm>

class Channel{
    private:
        std::string name;
        std::vector<Client*> clients;
        std::vector<Client*> operators;
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
        const std::string& getName() const;
        void addClient(Client* client);
        void removeClient(Client* client);
        void broadcastMessage(std::string message);
        void changePassword(std::string newPassword);
        //channel operators commands:
        bool isOperator(Client* client) const;
        void addOperator(Client* client);
        void removeOperator(Client* client);
        void kickClient(Client* client);
        void inviteClient(Client* client);
        void setTopic(const std::string& newTopic);
        std::string getTopic() const;
        void setMode(const std::string& mode, const std::string& value);
        std::string getMode(const std::string& mode) const;
};


#endif