#include "../includes/Irc.hpp"

void	Client::setNickname(std::string name)
{
	this->nick = name;
}

std::string	Client::getNickname(void)
{
	return this->nick;
}

void	Server::setNickname(int i, std::string buffer)
{
	std::cout << "setnickname" << " "  << i << buffer << std::endl;
	// std::cout << this->_clients[i - 1].getFd() << std::endl;
	this->_clients[i - 1].setNickname("fred");
	std::cout << this->_clients[i - 1].getNickname() << std::endl;
}

void	Server::setUsername(int i, std::string buffer)
{
	std::cout << "setusername" << " "  << i << buffer << std::endl;
}

void	Server::checkPassword(int i, std::string buffer)
{
	std::cout << "checkPassword" << " "  << i << buffer << std::endl;
}

void	Server::quitServer(int i, std::string buffer)
{
	std::cout << "quitServer" << " "  << i << buffer << std::endl;
}

void	Server::privateMessage(int i, std::string buffer)
{
	std::cout << "privateMessage" << " "  << i << buffer << std::endl;
}

void	Server::joinChannel(int i, std::string buffer)
{
	std::cout << "joinChannel" << " "  << i << buffer << std::endl;
}

void	Server::partChannel(int i, std::string buffer)
{
	std::cout << "partChannel" << " "  << i << buffer << std::endl;
}

void	Server::kickChannel(int i, std::string buffer)
{
	std::cout << "kickChannel" << " "  << i << buffer << std::endl;
}

void	Server::inviteChannel(int i, std::string buffer)
{
	std::cout << "inviteChannel" << " "  << i << buffer << std::endl;
}

void	Server::topicChannel(int i, std::string buffer)
{
	std::cout << "topicChannel" << " "  << i << buffer << std::endl;
}

void	Server::modeChannel(int i, std::string buffer)
{
	std::cout << "modeChannel" << " " << i << buffer << std::endl;
}

void	Server::commandParsing(int i, std::string buffer)
{
	std::string cmd_array[] = {
		"/NICK\n",
		"/PASS\n",
		"/USER\n",
		"/QUIT\n",
		"/PRIVMSG\n",
		"/JOIN\n",
		"/PART\n",
		"/KICK\n",
		"/INVITE\n",
		"/TOPIC\n",
		"/MODE\n"
	};
	int len = sizeof(cmd_array) / sizeof(cmd_array[0]);
	int j = 0;
	while (j < len)
	{
		if (cmd_array[j] == buffer)
			break;
		j++;
	}
	switch(j)
	{
		case 0: this->setNickname(i, buffer); break;
		case 1: this->setUsername(i, buffer); break;
		case 2: this->checkPassword(i, buffer); break;
		case 3: this->quitServer(i, buffer); break;
		case 4: this->privateMessage(i, buffer); break;
		case 5: this->joinChannel(i, buffer); break;
		case 6: this->partChannel(i, buffer); break;
		case 7: this->kickChannel(i, buffer); break;
		case 8: this->inviteChannel(i, buffer); break;
		case 9: this->topicChannel(i, buffer); break;
		case 10: this->modeChannel(i, buffer); break;
		default : std::cout << "not a command" << std::endl;
	}
}
