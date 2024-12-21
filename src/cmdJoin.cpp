#include "../includes/Irc.hpp"

int		Server::cmdJoinErrors(int i, std::vector<std::string> string_array)
{
	std::string msg;
	std::string target = this->_clients[i - 1].getNickname();
	if (this->_clients[i - 1].getIsRegistered() == 0)
	{
		msg = ":localhost 451 " + target + " :You have not registered\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return (1);
	}
	if (string_array.size() < 2)
    {
		std::string msg = ":localhost 461 " + target + " JOIN :Not enough parameters\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
        return (1);
    }
	return (0);
}

int		Server::cmdJoinErrorsInvalidName(int i, std::string channelName)
{
	if (isValidChannelName(channelName) == 0)
    {
        this->_clients[i - 1].sendMessage("Error : " + channelName + " : Invalid channel name\r\n");
        return (1);
    }
	return (0);
}

int		Channel::cmdJoinErrorsExistingChannel(Client &client, const std::string& password)
{
	std::string msg;
	std::string clientNick = client.getNickname();
	if (inviteOnly == true && client.isInvited(client, *this) == false)
    {
        msg = ":localhost 473 " + clientNick + " " + name + " :Cannot join channel (+i)\r\n";
        send(client.getFd(), msg.c_str(), msg.length(), 0);
        return (1);
    }
    if (!Ch_pwd.empty() && Ch_pwd != password)
    {
        msg = ":localhost 475 " + clientNick + " " + name + " :Cannot join channel (+k)\r\n";
        send(client.getFd(), msg.c_str(), msg.length(), 0);
		return (1);
    }
    if (userLimits > 0 && clients.size() >= static_cast<std::size_t>(userLimits))
    {
		msg = ":localhost 471 " + clientNick + " " + name + " :Cannot join channel (+l)\r\n";
        send(client.getFd(), msg.c_str(), msg.length(), 0);
	    return (1);
    }
	return (0);
}

void	Server::cmdJoin(int i, std::vector<std::string> string_array)
{
	if (cmdJoinErrors(i, string_array) == 1)
		return;
    std::string channelName = string_array[1];
    std::string pwd = (string_array.size() > 2) ? string_array[2] : "";
	if (cmdJoinErrorsInvalidName(i, channelName) == 1)
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
    if (channel == NULL)
    {
        this->_channels.push_back(Channel(channelName));
        channel = &_channels.back();
        channel->addClient(this->_clients[i - 1]);
        channel->addOperator(this->_clients[i - 1]);
		channel->cmdJoinSend(this->_clients[i - 1]);
    }
    else
        channel->cmdJoinExistingChannel(this->_clients[i - 1], pwd);
}

void Channel::cmdJoinExistingChannel(Client &client, const std::string& password)
{
	if (std::find(clients.begin(), clients.end(), client) != clients.end())
    {
		cmdJoinSend(client);
		return;
    }
	else
	{
		if (cmdJoinErrorsExistingChannel(client, password) == 1)
			return;
		clients.push_back(client);
		client.addChannel(*this);
		cmdJoinSend(client);
	}
	
}

void Channel::cmdJoinSend(Client &client)
{
	std::string msg;
	std::string sender = client.getNickname() + "!" + client.getUsername() + "@localhost";
	std::string clientNick = client.getNickname();
	for (std::size_t k = 0; k < clients.size(); ++k)
    {
		msg = ":" + sender + " JOIN " + name + "\r\n";
        send(clients[k].getFd(), msg.c_str(), msg.length(), 0);
    }
	if (getTopic().size() != 0)
	{
		msg = ":localhost 332 " + clientNick + " " + name + " " + getTopic() + "\r\n";
		send(client.getFd(), msg.c_str(), msg.length(), 0);
	}
	msg = ":localhost 353 " + sender + " = " + name + " :";
    for (std::size_t j = 0; j < clients.size(); ++j)
    {
        if (j > 0)
            msg += " ";
		if (isOperator(clients[j]))
			msg += "@";
        msg += clients[j].getNickname();
    }
    msg += "\r\n";
    send(client.getFd(), msg.c_str(), msg.length(), 0);
    msg = ":localhost 366 " + sender + " " + name + " :End of NAMES list\r\n";
    send(client.getFd(), msg.c_str(), msg.length(), 0);
}
