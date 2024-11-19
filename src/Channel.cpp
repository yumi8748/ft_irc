#include "../includes/Channel.hpp"

Channel::Channel(const std::string& n): name(n), limits(100), topic("")  // limits set to?
{

}

void Channel::addClient(Client* client)
{
    if (clients.size() >= limits)
    {
        std::cerr << "Channel is full! Cannot add client." << std::endl;
        return ;
    }
    clients.push_back(client);
    std::cerr << "Client added to channel: " << name << std::endl;
}

void Channel::removeClient(Client* client)
{
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (*it == client)
        {
            clients.erase(it);
            std::cout << "Client removed from channel: " << name << std::endl;
            break ;
        }
    }
    std::cerr << "Client not found in the channel!" << std::endl;;
}

void Channel::broadcastMessage(std::string message)
{
    //continue here
}

void Channel::setTopic(std::string newTopic)
{

}

void Channel::changePassword(std::string newPassword)
{

}
