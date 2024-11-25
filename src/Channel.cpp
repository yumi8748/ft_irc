#include "../includes/Channel.hpp"

Channel::Channel(const std::string& n): name(n), limits(100), topic("")  // limits set to?
{

}

const std::string& Channel::getName() const
{
    return (name);
}

void Channel::addClient(Client* client)
{
    if (clients.size() >= limits)
    {
        std::cerr << "Channel is full! Cannot add client." << std::endl;
        return ;
    }
    clients.push_back(client);
    std::cerr << "Client" << client->getNickname() << " joined channel: " << name << std::endl;
}

void Channel::removeClient(Client* client)
{
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (*it == client)
        {
            clients.erase(it);
            std::cout << "Client" << client->getNickname() << " left channel: " << name << std::endl;
            break ;
        }
    }
    std::cerr << "Client not found in the channel!" << std::endl;;
}

void Channel::broadcastMessage(std::string msg)
{
    for (size_t i = 0; i < clients.size(); ++i)
    {
        clients[i]->sendMessage(msg);
    }
}

void Channel::changePassword(std::string newPassword)
{
    password = newPassword;
    std::cout << "Channel password updated.\n";
}

bool Channel::isOperator(Client* client) const
{
    for (std::vector<Client*>::const_iterator it = operators.begin(); it != operators.end(); ++it)
    {
        if (*it == client)
            return true;
    }
    return false;
}

void Channel::addOperator(Client* client)
{
    if (!isOperator(client))
        operators.push_back(client);
}

void Channel::removeOperator(Client* client)
{
    for (std::vector<Client*>::iterator it = operators.begin(); it != operators.end(); ++it) {
        if (*it == client) {
            operators.erase(it);
            break;
        }
    }
}

void Channel::kickClient(Client* client)
{
    std::vector<Client *>::iterator it = std::find(clients.begin(), clients.end(), client);
    if (it != clients.end())
    {
        clients.erase(it);
        std::cout << "Client " << client->getNickname() << " has been kicked from channel " << name << "!" << std::endl;
    }
    else
        std::cerr << "Client not found in channel " << name << "!" << std::endl;
}

void Channel::inviteClient(Client* client)
{
    if (std::find(clients.begin(), clients.end(), client) != clients.end())
    {
        std::cerr << "Client " << client->getNickname() << " is already in the channel " << name << std::endl;
        return ;
    }
    clients.push_back(client);
    std::cout << "Client " << client->getNickname() << " has been invited to channel " << name << std::endl;
}

void Channel::setTopic(const std::string& newTopic)
{
    topic = newTopic;
    std::cout << "Channel topic updated to: " << topic << std::endl;
}

std::string Channel::getTopic() const
{
    return (topic);
}


void Channel::setMode(const std::string& mode, const std::string& value)
{
    modes[mode] = value;
    std::cout << "Channel mode " << mode << " set to " << value << " in channel " << name << std::endl;
}

std::string Channel::getMode(const std::string& mode) const
{
    std::map<std::string, std::string>::const_iterator it = modes.find(mode);
    if (it != modes.end())
        return (it->second);
    return ("");
}


