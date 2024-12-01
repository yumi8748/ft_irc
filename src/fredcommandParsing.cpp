#include "../includes/Irc.hpp"

// FRED PART --------------------------------------------------------

void	Server::commandParsing(int i, std::string string)
{
	std::string buffer = string;
	int length = buffer.size();
	buffer.resize(length - 1);
	std::stringstream ss(buffer);
	std::string tmp;
	std::vector<std::string> string_array;

	while (std::getline(ss, tmp, ' '))
	{
		string_array.push_back(tmp);
	}

	std::string cmd_array[] = {
		"/NICK",
		"/PASS",
		"/USER",
		"/QUIT",
		"/PRIVMSG",
		"/JOIN",
		"/PART",
		"/KICK",
		"/INVITE",
		"/TOPIC",
		"/MODE"
	};

	int len = sizeof(cmd_array) / sizeof(cmd_array[0]);
	int j = 0;
	while (j < len)
	{
		if (cmd_array[j] == string_array[0])
			break;
		j++;
	}
	switch(j)
	{
		case 0: this->cmdNick(i, string_array); break;
		case 1: this->cmdPass(i, string_array); break;
		case 2: this->cmdUser(i, string_array); break;
		case 3: this->cmdQuit(i, string_array); break;
		case 4: this->cmdPrivmsg(i, string_array, buffer); break;
		case 5: this->cmdJoin(i, string_array); break;
		case 6: this->cmdPart(i, string_array); break;
		case 7: this->cmdKick(i, string_array); break;
		case 8: this->cmdInvite(i, string_array); break;
		case 9: this->cmdTopic(i, string_array); break;
		case 10: this->cmdMode(i, string_array); break;
    default : break;
	}
}

int		Server::isRegistered(int i)
{
	if (this->_clients[i - 1].getNickname().empty()
		|| this->_clients[i - 1].getUsername().empty()
		|| this->_clients[i - 1].getPasswordIsCorrect() == 0)
	{
		return (0);
	}
	else
	{
		return (1);
	}
}

// YUMI PART --------------------------------------------------------

void	Server::cmdJoin(int i, std::vector<std::string> string_array)
{
	// if (this->isRegistered(i) == 0)
	// {
	// 	std::cout << "Client not registered"  << std::endl;
	// 	return ;
	// }
	for (std::vector<Channel>::iterator it = this->_channels.begin(); it != _channels.end(); it++)
	{
		if ((*it).getName() == string_array[1])
		{
			(*it).addClient(&this->_clients[i - 1]);
			return;
		}
	}
	Channel newchannel(string_array[1].c_str());
	newchannel.addClient(&this->_clients[i - 1]);
	_channels.push_back(newchannel);
}

void	Server::cmdPart(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "Client not registered"  << std::endl;
		return ;
	}
	std::cout << "cmdPart" << " : " << string_array[0] << std::endl;
}

void	Server::cmdKick(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "Client not registered"  << std::endl;
		return ;
	}
	std::cout << "cmdKick" << " : " << string_array[0] << std::endl;
}

void	Server::cmdInvite(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "Client not registered"  << std::endl;
		return ;
	}
	std::cout << "cmdInvite" << " : " << string_array[0] << std::endl;
}

void	Server::cmdTopic(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "Client not registered"  << std::endl;
		return ;
	}
	std::cout << "cmdTopic" << " : " << string_array[0] << std::endl;
}

void	Server::cmdMode(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "Client not registered"  << std::endl;
		return ;
	}
	std::cout << "cmdMode" << " : " << string_array[0] << std::endl;
}


