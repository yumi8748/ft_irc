#include "../includes/Irc.hpp"

int		Server::cmdInviteErrors(int i, std::vector<std::string> string_array)
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
		std::string msg = ":localhost 461 " + target + " JOIN :Not enough parameters\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
        return (1);
    }
	return (0);
}

int		Server::cmdInviteErrorsChannel(int i, std::string channelName)
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

int		Server::cmdInviteErrorsNickname(int i, std::string nickInvited)
{
	std::string msg;
	std::string target = this->_clients[i - 1].getNickname();
	Client* clientInvited = NULL;
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->getNickname() == nickInvited)
        {
            clientInvited = &(*it);
            break;
        }
    }
    if (clientInvited == NULL)
    {
		msg = ":localhost 401 " + target + " " + nickInvited + ": No such nick/channel\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
        return (1);
    }
	return (0);
}

int		Channel::cmdInviteErrorsOnChannel(Client &client, Client &clientInvited)
{
	std::string msg;
	std::string target = client.getNickname();
	std::string nickInvited = clientInvited.getNickname();
	std::string channeName = this->getName();
	if (std::find(clients.begin(), clients.end(), clientInvited) != clients.end())
    {
		msg = ":localhost 443 " + target + " " + nickInvited + " " + channeName + " :is already on channel\r\n";
		send(client.getFd(), msg.c_str(), msg.length(), 0);
        return (1);
    }
	return (0);
}

void	Server::cmdInvite(int i, std::vector<std::string> string_array)
{
	if (cmdInviteErrors(i, string_array) == 1)
		return;
    std::string nickInvited = string_array[1];
    std::string channelName = string_array[2];
	Channel* channel = NULL;
	if (cmdInviteErrorsChannel(i, channelName) == 1)
		return;
	if (cmdInviteErrorsNickname(i, nickInvited) == 1)
		return;
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->getName() == channelName)
        {
            channel = &(*it);
            break;
        }
    }
	Client* clientInvited = NULL;
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->getNickname() == nickInvited)
        {
            clientInvited = &(*it);
            break;
        }
    }
    channel->inviteClient(_clients[i - 1], *clientInvited);
}

void Channel::inviteClient(Client &client, Client &clientInvited)
{
	if (cmdInviteErrorsOnChannel(client, clientInvited) == 1)
		return;
	std::string msg;
	std::string target = client.getNickname();
	std::string nickInvited = clientInvited.getNickname();
	std::string channeName = this->getName();
	std::string sender = client.getNickname() + "!" + client.getUsername() + "@localhost";
    clients.push_back(clientInvited);
    clientInvited.addChannel(*this);
	addInvitedClient(clientInvited);
    msg = ":localhost 341 " + target + " " + nickInvited + " " + channeName + "\r\n";
	send(client.getFd(), msg.c_str(), msg.length(), 0);
	msg = ":" + sender + " INVITE " + nickInvited + " " + channeName + "\r\n";
	send(clientInvited.getFd(), msg.c_str(), msg.length(), 0);

}

void Channel::addInvitedClient(const Client &client)
{
    if (std::find(invitedClients.begin(), invitedClients.end(), client) == invitedClients.end()) {
        invitedClients.push_back(client);
    }
}
