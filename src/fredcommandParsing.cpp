#include "../includes/Irc.hpp"

// FRED PART --------------------------------------------------------

void printvector (std::string str)
{
  std::cout << str << std::endl;
}

void	Server::commandParsing(int i, std::string string)
{
	// std::cout << "[" << string << "]" << std::endl;
	std::string buffer = string;
	int length = buffer.size();
	buffer.resize(length - 1);
	// buffer[length - 1] = '\0';
	// std::cout  << "["  << buffer << "]" << std::endl;
	// std::cout << length << std::endl;
	// std::cout << "[" << buffer.size() << "]" << std::endl;

	std::stringstream ss(buffer);
	std::string tmp;
	std::vector<std::string> string_array;

	// std::cout << string_array.size() << std::endl;
	while (std::getline(ss, tmp, ' '))
	{
		string_array.push_back(tmp);
	}
	// std::cout << string_array.size() << std::endl;
	// for_each (string_array.begin(), string_array.end(), printvector);
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
	// if (cmd_array[5] == string_array[0])
	// {
	// 	std::cout << "same" << std::endl;
	// }
	// else
	// {
	// 	std::cout << "[" << string_array[0].size() << "]" << std::endl;
	// 	std::cout << "[" << cmd_array[5].size() << "]" << std::endl;
	// 	std::cout << "different" << std::endl;
	// }

	int len = sizeof(cmd_array) / sizeof(cmd_array[0]);
	int j = 0;
	// std::cout << "[" << string_array[0] << "]" << std::endl;
	while (j < len)
	{
		if (cmd_array[j] == string_array[0])
			break;
		j++;
	}
	// std::cout << j << std::endl;
	switch(j)
	{
		case 0: this->cmdNick(i, string_array); break;
		case 1: this->cmdPass(i, string_array); break;
		case 2: this->cmdUser(i, string_array); break;
		case 3: this->cmdQuit(i, string_array); break;
		case 4: this->cmdPrivmsg(i, string_array); break;
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
	if (this->_clients[i - 1].getNickname().empty() || this->_clients[i - 1].getUsername().empty())
	{
		// std::cout << "reg"   << std::endl;
		return (0);
	}
	else
	{
		// std::cout << "not reg"   << std::endl;
		return (1);
	}
}

void	Server::cmdNick(int i, std::vector<std::string> string_array)
{
	if (string_array.size() != 2)
		throw ErrThrow("Argument error");
	this->_clients[i - 1].setNickname(string_array[1]);
	// std::cout << this->_clients[i - 1].getNickname() << std::endl;
	// std::cout << this->_clients[i - 1].getFd() << std::endl;
}

void	Server::cmdUser(int i, std::vector<std::string> string_array)
{
	if (string_array.size() != 2)
		throw ErrThrow("Argument error");
	this->_clients[i - 1].setUsername(string_array[1]);
	std::cout << this->_clients[i - 1].getUsername() << std::endl;
}

void	Server::cmdPass(int i, std::vector<std::string> string_array)
{
	std::cout << "checkPassword" << " "  << i << string_array[0] << std::endl;
}

void	Server::cmdQuit(int i, std::vector<std::string> string_array)
{
	std::cout << "quitServer" << " "  << i << string_array[0] << std::endl;
}

void	Server::cmdPrivmsg(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "not registered"  << std::endl;
		return ;
	}
	std::cout << "cmdPrivmsg" << " "  << i << string_array[0] << std::endl;
}



// YUMI PART --------------------------------------------------------

void	Server::cmdJoin(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "not registered"  << std::endl;
		return ;
	}
	std::cout << "joinChannel" << " "  << i << string_array[0] << std::endl;
}

void	Server::cmdPart(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "not registered"  << std::endl;
		return ;
	}
	std::cout << "partChannel" << " "  << i << string_array[0] << std::endl;
}

void	Server::cmdKick(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "not registered"  << std::endl;
		return ;
	}
	std::cout << "kickChannel" << " "  << i << string_array[0] << std::endl;
}

void	Server::cmdInvite(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "not registered"  << std::endl;
		return ;
	}
	std::cout << "inviteChannel" << " "  << i << string_array[0] << std::endl;
}

void	Server::cmdTopic(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "not registered"  << std::endl;
		return ;
	}
	std::cout << "topicChannel" << " "  << i << string_array[0] << std::endl;
}

void	Server::cmdMode(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "not registered"  << std::endl;
		return ;
	}
	std::cout << "modeChannel" << " " << i << string_array[0] << std::endl;
}


