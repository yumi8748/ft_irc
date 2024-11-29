#include "../includes/Irc.hpp"

// FRED PART --------------------------------------------------------

void printvector (std::string str)
{
  std::cout << str << std::endl;
}

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

void	Server::cmdNick(int i, std::vector<std::string> string_array)
{
	if (string_array.size() != 2)
		throw ErrThrow("Argument error");
	this->_clients[i - 1].setNickname(string_array[1]);
	std::cout << "Nickname saved" << std::endl;
}

void	Server::cmdUser(int i, std::vector<std::string> string_array)
{
	if (string_array.size() != 2)
		throw ErrThrow("Argument error");
	this->_clients[i - 1].setUsername(string_array[1]);
	std::cout << "Username saved" << std::endl;
}

void	Client::setPasswordIsCorrect(void)
{
	this->_passwordIsCorrect = 1;
}

int	Client::getPasswordIsCorrect(void)
{
	return(this->_passwordIsCorrect);
}

void	Server::cmdPass(int i, std::vector<std::string> string_array)
{
	if (string_array[1] == this->_pwd)
	{
		this->_clients[i - 1].setPasswordIsCorrect();
		std::cout << "Password is correct" << std::endl;
	}
	else
	{
		std::cout << "Incorrect password" << std::endl;
	}
	// std::cout << "cmdPass" << " : "  << i << string_array[0] << std::endl;
}

void	Server::cmdQuit(int i, std::vector<std::string> string_array)
{
	std::cout << "cmdQuit" << " : "  << i << string_array[0] << std::endl;
	int fd = _fds[i].fd;
	CloseClients(fd);
    close(fd);
	std::cout<<PURPLE<<"Client["<< fd <<"]"<<RED<<" has disconnected"<<RESET<<std::endl;
}

std::vector<Client> _clients;

void	Server::cmdPrivmsg(int i, std::vector<std::string> string_array)
{
	// if (this->isRegistered(i) == 0)
	// {
	// 	std::cout << "Client not registered"  << std::endl;
	// 	return ;
	// }
	std::vector<Channel>::iterator it;
	if (string_array[1][0] == '#')
	{
		for (it = this->_channels.begin(); it != _channels.end(); it++)
		{
			// std::cout << "enter" << std::endl;
			// std::cout << (*it).getName() << std::endl;
			if ((*it).getName() == string_array[1])
			{
				// (*it).addClient(&this->_clients[i - 1]);

				std::cout << "found channel" << std::endl;
				// exists = 1;
				// for (ito = (*it).getClients().begin(); ito != (*it).getClients().end(); ito++)
				// {
				// 	std::cout << (*ito)->getUsername() << std::endl;
				// }
				int j = 0;
				while (j < static_cast<int>((*it).getClients().size()))
				{
					std::string msg = "[" + this->_clients[i - 1].getNickname() + "] : " + string_array[2] + "\n";
					send((*it).getClients()[j]->getFd(), msg.c_str(), msg.length(), 0);
					j++;
				}
				break;
			}
		}
	}
	else
	{
		for (std::vector<Client>::iterator it = this->_clients.begin(); it != _clients.end(); it++)
		{
			if ((*it).getNickname() == string_array[1])
			{
				std::string msg = "[" + this->_clients[i - 1].getNickname() + "] :";
				std::cout << msg << std::endl;
				send((*it).getFd(), msg.c_str(), msg.length(), 0);
				send((*it).getFd(), string_array[2].c_str(), string_array[2].length(), 0);
			}
		}
	}
	
}



// YUMI PART --------------------------------------------------------

std::vector<Client*> Channel::getClients(void)
{
	return this->clients;
}

std::string Client::getName(void)
{
	return this->nick;
}

void	Server::cmdJoin(int i, std::vector<std::string> string_array)
{
	// if (this->isRegistered(i) == 0)
	// {
	// 	std::cout << "Client not registered"  << std::endl;
	// 	return ;
	// }
	// Channel* channel = findChannelByName(channels, channelName);
    // if (!channel) {
    //     channel = new Channel(channelName);
    //     channel->addOperator(client); //channel creator becomes operator automatically
    //     channels.push_back(channel);
    // }
    // channel->joinChannel(client, password);

	int done = 0;

	std::cout << this->_channels.size() << std::endl;
	if (this->_channels.size() == 0)
	{
		Channel newchannel(string_array[1].c_str());
		newchannel.addClient(&this->_clients[i - 1]);
		_channels.push_back(newchannel);
		std::cout << "new channel" << std::endl;
		done = 1;
	}

	if (done == 0)
	{
		int exists = 0;
		for (std::vector<Channel>::iterator it = this->_channels.begin(); it != _channels.end(); it++)
		{
			// std::cout << "enter" << std::endl;
			if ((*it).getName() == string_array[1])
			{
				(*it).addClient(&this->_clients[i - 1]);
				std::cout << "existing channel" << std::endl;
				exists = 1;
				break;
			}
		}
		if (exists == 0)
		{
			Channel newchannel(string_array[1].c_str());
			newchannel.addClient(&this->_clients[i - 1]);
			_channels.push_back(newchannel);
			std::cout << "new channel" << std::endl;
		}
	}
	

	std::cout << "\ncheck channel" << std::endl;
	for (std::vector<Channel>::iterator it = this->_channels.begin(); it != _channels.end(); it++)
	{
		std::cout << (*it).getName() << std::endl;

		std::cout << (*it).getClients().size() << std::endl;
		i = 0;
		while (i < static_cast<int>((*it).getClients().size()))
		{
			// std::cout << (*it).getClients()[i].getFd()<<std::endl;
			std::cout << (*it).getClients()[i]->getFd() << std::endl;
			i++;
		}
		// for (std::vector<Client *>::iterator ito = (*it)->getClients().begin(); ito != (*it)->getClients().end(); ito++)
		// {
			// std::cout << (*ito)->getName() << std::endl;
		// }

	}
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


