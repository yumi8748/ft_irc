#include "../includes/Irc.hpp"

void	Server::cmdInvite(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		this->_clients[i - 1].sendMessage(":localhost 451 " + _clients[i - 1].getNickname() +  " :You have not registered\r\n");
		return ;
	}
    for (std::string::size_type i = 0; i < string_array.size(); i++)
        std::cout << YELLOW << string_array[i] << std::endl;
    std::cout << RESET;
    // If the user doesn't specify the channel, IRSSI will append the current channel the user is in
    // to the INVITE command 
    // e.g. /INVITE yu-chen (from user in channel #a) --> string_array ==  INVITE yu-chen #a
    // CONDITIONAL JUMPS IN CMDINVITE
    if (string_array.size() < 3)
    {
        this->_clients[i - 1].sendMessage(":localhost 461 " + _clients[i - 1].getNickname() + " INVITE :Not enough parameters\r\n");
        return;
    }
    std::cout << "out\n";
    std::string targetNickname = string_array[1];
    std::string channelName = string_array[2];

    Channel* channel = NULL;
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->getName() == channelName)
        {
            channel = &(*it);
            break;
        }
    }

    if (channel == NULL)
    {
        this->_clients[i - 1].sendMessage(":localhost 403 " + _clients[i - 1].getNickname() + channelName + " :No such channel\r\n");
        return;
    }

    // Channel channel = findChannelByName(channelName);
    // if (channel == Channel())
    // {
    //     this->_clients[i - 1].sendMessage(":localhost 403 " + channelName + " :No such channel");
    //     return;
    // }

    std::cout << "Checking if client " << this->_clients[i - 1].getName() << " is an operator in channel " << channelName << std::endl;
    // std::cout << "Client address in cmdInvite: " << &this->_clients[i - 1] << std::endl;
    if (!channel->isOperator(this->_clients[i - 1]))
    {
        this->_clients[i - 1].sendMessage(":localhost 482 " + _clients[i - 1].getNickname() + " " + channelName + " :You're not a channel operator\r\n");
        return;
    }

    Client* targetClient = NULL;

    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->getNickname() == targetNickname)
        {
            targetClient = &(*it);
            break;
        }
    }

    if (targetClient == NULL)
    {
        this->_clients[i - 1].sendMessage(":localhost 401 " + _clients[i - 1].getNickname() + " " + targetNickname + " :No such nick/channel\r\n");
        return;
    }
    if (std::find(_clients.begin(), _clients.end(), _clients[i - 1]) != _clients.end())
    {
        this->_clients[i - 1].sendMessage(":localhost 443 " + _clients[i - 1].getNickname() + " " + targetClient->getNickname() + " " + channel->getName() + " :is already on channel");
        return ;
    } 
    channel->inviteClient(*targetClient);
    this->_clients[i - 1].sendMessage(":localhost 341 " + this->_clients[i - 1].getNickname() + " " + targetNickname + " " + channelName + "\r\n");
    targetClient->sendMessage(":localhost :You have been invited to join " + channelName + " by " + this->_clients[i - 1].getNickname() + "\r\n");
}

// void Channel::inviteClient(Client &client, Client &targetclient)
void Channel::inviteClient(Client &client)
{
    // if (std::find(clients.begin(), clients.end(), client) != clients.end())
    // {
    //     std::cerr << ":localhost 443 " << client.getNickname() << " " << targetclient.getNickname() << " " << this->getName() << " :is already on channel"<< std::endl;
    //     return ;
    // }
    // clients.push_back(client);
    // client.addChannel(*this);
	addInvitedClient(client);
    std::cout << "Client " << client.getNickname() << " has been invited to the channel " << name << "." << std::endl;
}

void Channel::addInvitedClient(const Client &client) {
    if (std::find(invitedClients.begin(), invitedClients.end(), client) == invitedClients.end()) {
        invitedClients.push_back(client);
    }
}

void Channel::removeInvitedClient(const Client &client) {
    std::vector<Client>::iterator it = std::find(invitedClients.begin(), invitedClients.end(), client);
    if (it != invitedClients.end()) {
        invitedClients.erase(it);
    }
}

const std::vector<Client>& Channel::getInvitedClients() const {
    return invitedClients;
}
