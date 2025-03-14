#include "../includes/Irc.hpp"

int		Server::cmdPartErrors(int i, std::vector<std::string> string_array)
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
		msg = ":localhost 461 " + target + " PART :Not enough parameters\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
        return (1);
    }
	return (0);
}

int		Server::cmdPartErrorsChannel(int i, std::string channelName)
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
	return (0);
}

void	Server::cmdPart(int i, std::vector<std::string> string_array)
{
	if (cmdPartErrors(i, string_array) == 1)
		return;
	std::string reason = string_array.size() > 2 ? string_array[2] : "No reason provided";
	std::string channelName;

	std::vector<std::string> param_splitted;
	std::stringstream ss(string_array[1]);
	std::string tmp;
	char del = ',';
 	while(std::getline(ss, tmp, del))
	{
		param_splitted.push_back(tmp);
	}
	for (size_t j = 0; j < param_splitted.size(); ++j)
    {
		channelName = param_splitted[j];
		if (cmdPartErrorsChannel(i, channelName) == 1)
			continue;
		Channel* channel = NULL;
		for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		{
			if (it->getName() == channelName)
			{
				channel = &(*it);
				break;
			}
		}
		channel->partChannel(_clients[i - 1], reason, channelName);
		if (channel->isEmpty())
		{
			for (std::vector<Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); ) 
			{
				if ((*it) == *channel)
				{
					it = this->_channels.erase(it);
					break;
				}
				else
					++it;
			}
		}
	}
}

void Channel::partChannel(Client& client, const std::string& reason, const std::string& channelName)
{
	std::string msg;
	std::string sender = client.getNickname() + "!" + client.getUsername() + "@localhost";
	for (std::size_t k = 0; k < clients.size(); ++k)
    {
		msg = ":" + sender + " PART " + channelName + " :" + reason + "\r\n";
        send(clients[k].getFd(), msg.c_str(), msg.length(), 0);
    }
	std::vector<Client>::iterator it = std::find(clients.begin(), clients.end(), client);
    if (it != clients.end())
    {
        client.removeChannel(*this);
        clients.erase(it);
		removeInvitedClient(client);
		removeOperator(client);
    }
}

void Channel::removeInvitedClient(const Client &client)
{
    std::vector<Client>::iterator it = std::find(invitedClients.begin(), invitedClients.end(), client);
    if (it != invitedClients.end()) {
        invitedClients.erase(it);
    }
}
