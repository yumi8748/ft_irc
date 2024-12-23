#include "../includes/Irc.hpp"

int		Server::cmdKickErrors(int i, std::vector<std::string> string_array)
{
	std::string msg;
	std::string target = this->_clients[i - 1].getNickname();
	if (this->_clients[i - 1].getIsRegistered() == 0)
	{
		msg = ":localhost 451 " + target + " :You have not registered\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return (1);
	}
	if (string_array.size() < 3)
    {
		msg = ":localhost 461 " + target + " KICK :Not enough parameters\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
        return (1);
    }
	return (0);
}

int		Server::cmdKickErrorsChannel(int i, std::string channelName)
{
	std::string msg;
	std::string target = this->_clients[i - 1].getNickname();
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
		msg = ":localhost 403 " + target + " " + channelName + " :No such channel\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return (1);
	}
	if (!channel->isClientInChannel(_clients[i - 1]))
	{
		msg = ":localhost 442 " + target + " " + channelName + " :You aren't on that channel\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return (1);
	}
	if (!channel->isOperator(this->_clients[i - 1]))
    {
		msg = ":localhost 482 " + target + " " + channelName + " :You're not channel operator\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return (1);
    }
	return (0);
}

int		Server::cmdKickErrorsNickname(int i, std::string nickKicked)
{
	std::string msg;
	std::string target = this->_clients[i - 1].getNickname();
	Client* clientKicked = NULL;
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->getNickname() == nickKicked)
        {
            clientKicked = &(*it);
            break;
        }
    }
    if (clientKicked == NULL)
    {
		msg = ":localhost 401 " + target + " " + nickKicked + ": No such nick/channel\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
        return (1);
    }
	return (0);
}

int		Channel::cmdKickErrorsNotOnChannel(Client &client, Client &clientKicked)
{
	std::string msg;
	std::string target = client.getNickname();
	std::string nickKicked = clientKicked.getNickname();
	std::string channeName = this->getName();
	if (std::find(clients.begin(), clients.end(), clientKicked) == clients.end())
    {
		msg = ":localhost 441 " + target + " " + nickKicked + " " + channeName + " :They aren't on that channel\r\n";
		send(client.getFd(), msg.c_str(), msg.length(), 0);
        return (1);
    }
	return (0);
}

void	Server::cmdKick(int i, std::vector<std::string> string_array)
{
	if (cmdKickErrors(i, string_array) == 1)
		return;
	std::string channelName = string_array[1];
    std::string nickKicked = string_array[2];
	std::string reason = (string_array.size() > 3 && string_array[3] != ":") ? string_array[3] : this->_clients[i - 1].getNickname();
	if (cmdKickErrorsChannel(i, channelName) == 1)
		return;
	if (cmdKickErrorsNickname(i, nickKicked) == 1)
		return;
	
	Channel* channel = NULL;
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->getName() == channelName)
        {
            channel = &(*it);
            break;
        }
    }
	Client* clientKicked = NULL;
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->getNickname() == nickKicked)
        {
            clientKicked = &(*it);
            break;
        }
    }
	if (channel->cmdKickErrorsNotOnChannel(_clients[i - 1], *clientKicked) == 1)
		return;
	channel->kickClient(_clients[i -1],  reason, channelName, *clientKicked);
}

void Channel::kickClient(Client &client, const std::string& reason, const std::string& channelName, Client &clientKicked)
{
	std::string msg;
	std::string sender = client.getNickname() + "!" + client.getUsername() + "@localhost";
	std::string senderKicked = clientKicked.getNickname() + "!" + clientKicked.getUsername() + "@localhost";
	std::string nickKicked = clientKicked.getNickname();
	msg = ":" + senderKicked + " PART " + channelName + " :" + reason + "\r\n";
	send(clientKicked.getFd(), msg.c_str(), msg.length(), 0);
	for (std::size_t k = 0; k < clients.size(); ++k)
    {
		msg = ":" + sender + " KICK " + channelName + " " + nickKicked + " " + reason + "\r\n";
        send(clients[k].getFd(), msg.c_str(), msg.length(), 0);
    }
	std::vector<Client >::iterator it = std::find(clients.begin(), clients.end(), clientKicked);
	if (it != clients.end())
	{
		clients.erase(it);
		client.removeChannel(*this);
		removeInvitedClient(client);
	}
}

