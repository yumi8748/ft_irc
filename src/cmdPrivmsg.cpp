#include "../includes/Irc.hpp"

int		Server::cmdPrivmsgErrors(int i, std::vector<std::string> string_array)
{
	std::string msg;
	std::string target = this->_clients[i - 1].getNickname();
	if (this->_clients[i - 1].getIsRegistered() == 0)
	{
		msg = ":localhost 451 " + target + " :You have not registered\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return (1);
	}
	else if (string_array.size() < 2)
	{
		msg = ":localhost 411 " + target + " :No recipient given PRIVMSG\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return (1);
	}
	else if (string_array.size() < 3)
	{
		msg = ":localhost 412 " + target + " :No text to send\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return (1);
	}
	return (0);
}

int		Server::cmdPrivmsgErrorsNoSuchChannel(int i, std::vector<std::string> string_array)
{
	std::string msg;
	std::string target = this->_clients[i - 1].getNickname();
	int k = 0;
	while (k < static_cast<int>(getChannels().size()))
	{
		if (getChannels()[k].getName() == string_array[1])
			break;
		k++;
	}
	if (k == static_cast<int>(getChannels().size()))
	{
		msg = ":localhost 401 " + target + " " + string_array[1] + " : No such nick/channel\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return (1);
	}
	return (0);
}

int		Server::cmdPrivmsgErrorsNoSuchNick(int i, std::vector<std::string> string_array)
{
	std::string msg;
	std::string target = this->_clients[i - 1].getNickname();
	int k = 0;
	while (k < static_cast<int>(getClients().size()))
	{
		if (getClients()[k].getNickname() == string_array[1])
			break;
		k++;
	}
	if (k == static_cast<int>(getClients().size()))
	{
		msg = ":localhost 401 " + target + " " + string_array[1] + ": No such nick/channel\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return (1);
	}
	return (0);
}

int		Server::cmdPrivmsgErrorsNotInChannel(int i, std::vector<std::string> string_array)
{
	std::string msg;
	std::string target = this->_clients[i - 1].getNickname();
	std::string channelName = string_array[1];
	for (std::vector<Channel >::iterator it = this->_channels.begin(); it != _channels.end(); it++)
	{
		if ((*it).getName() == string_array[1])
		{
			int k = 0;
			while (k < static_cast<int>((*it).getClients().size()))
			{
				if ((*it).getClients()[k].getNickname() == this->_clients[i - 1].getNickname())
					break;
				k++;
			}
			if (k == static_cast<int>((*it).getClients().size()))
			{
				msg = ":localhost 404 " + target + " " + channelName + " :Cannot send to channel\r\n";
				send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
				return (1);
			}
		}
	}
	return (0);
}

void Server::channelMsg(int i, std::vector<std::string> string_array, std::string buffer)
{
	std::string msg;
	std::string message;
	std::string sender = this->_clients[i - 1].getNickname() + "!" + this->_clients[i - 1].getUsername() + "@localhost";
	int pos = buffer.find("#");
	int pos2 = buffer.find(" ", pos) + 1;
	message = buffer.substr(pos2);

	if (cmdPrivmsgErrorsNoSuchChannel(i, string_array) == 1)
		return;
	if (cmdPrivmsgErrorsNotInChannel(i, string_array) == 1)
		return;
	for (std::vector<Channel >::iterator it = this->_channels.begin(); it != _channels.end(); it++)
	{
		if ((*it).getName() == string_array[1])
		{
			int j = 0;
			while (j < static_cast<int>((*it).getClients().size()))
			{
				if (this->_clients[i - 1].getNickname() != (*it).getClients()[j].getNickname())
				{
					msg = ":" + sender + " PRIVMSG " + string_array[1] + " " + message + "\r\n";
					send((*it).getClients()[j].getFd(), msg.c_str(), msg.length(), 0);
				}
				j++;
			}
		}
	}
}

void Server::userMsg(int i, std::vector<std::string> string_array, std::string buffer)
{
	std::string msg;
	std::string message;
	int pos = buffer.find(string_array[1]);
	int pos2 = buffer.find(" ", pos) + 1;
	message = buffer.substr(pos2);
	if (cmdPrivmsgErrorsNoSuchNick(i, string_array) == 1)
		return;
	for (std::vector<Client>::iterator it = this->_clients.begin(); it != _clients.end(); it++)
	{
		if ((*it).getNickname() == string_array[1])
		{
			msg = this->_clients[i - 1].getNickname() + " " + message + "\r\n";
			send((*it).getFd(), msg.c_str(), msg.length(), 0);
		}
	}
}

void	Server::cmdPrivmsg(int i, std::vector<std::string> string_array, std::string buffer)
{
	if (cmdPrivmsgErrors(i, string_array) == 1)
		return;
	if (string_array[1][0] == '#')
		channelMsg(i, string_array, buffer);
	else
		userMsg(i, string_array, buffer);
}

// msg = ":"  + this->_clients[i - 1].getNickname() + "!" + this->_clients[i - 1].getUsername() +  "@localhost PRIVMSG " + string_array[1] + " " + message + "\r\n";
// msg = ":" + this->_clients[i - 1].getNickname() + " PRIVMSG " + string_array[1] + " " + message + "\r\n";
