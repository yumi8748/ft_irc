#include "../includes/Irc.hpp"

int Server::nicknameUsed(std::string nickname)
{
	for (size_t j = 0; j < this->_clients.size(); j++)
	{
		// std::cout << "ICI" << nickname << this->_clients[j].getNickname() << std::endl;
		if (this->_clients[j].getNickname() == nickname)
		{
			// std::cout << "SAME" << nickname << this->_clients[j].getNickname() << std::endl;
			return 1;
		}
	}
	return 0;
}


void	Server::checkRegistration(int i)
{
	if (!this->_clients[i - 1].getNickname().empty()
	&& !this->_clients[i - 1].getUsername().empty()
	&& this->_clients[i - 1].getPasswordIsCorrect()
	&& !this->_clients[i - 1].getIsLogged())
	{
		this->_clients[i - 1].setIsLogged();
		std::string messfinal = ":localhost 001 " + this->_clients[i - 1].getNickname() + " :Welcome to the Internet Relay Network :" + this->_clients[i - 1].getNickname() + "!ftanon@localhost\r\n";
		send(this->_fds[i].fd, messfinal.c_str(), messfinal.size(), 0);	
	}
}

void	Server::cmdNick(int i, std::vector<std::string> string_array)
{
	if (nicknameUsed(string_array[1])){
		std::string messfinal = ":localhost 433 " + string_array[1] + " " + string_array[1] + " :Nickname is already in use.\r\n";
		send(this->_fds[i].fd, messfinal.c_str(), messfinal.size(), 0);	
		return;
	}
	else if (this->_clients[i - 1].getIsLogged())
	{
		this->_clients[i - 1].setOldNick(this->_clients[i - 1].getNickname());
		// std::string msg = ":"  + this->_clients[i - 1].getOldNick() +  " NICK " + string_array[1] + "\r\n";
		std::string msg = ":"  + this->_clients[i - 1].getOldNick() + "!" + this->_clients[i - 1].getUsername() +  "@localhost NICK " + string_array[1] + "\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		this->_clients[i - 1].setNickname(string_array[1]);
	}
	else
	{
		this->_clients[i - 1].setNickname(string_array[1]);
		this->_clients[i - 1].setOldNick(string_array[1]);
		// std::string msg = ":"  + this->_clients[i - 1].getOldNick() +  " NICK " + string_array[1] + "\r\n";
		std::string msg = ":"  + this->_clients[i - 1].getOldNick() + "!" + this->_clients[i - 1].getUsername() +  "@localhost NICK " + string_array[1] + "\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		checkRegistration(i);
	}
	
}

void	Server::cmdPass(int i, std::vector<std::string> string_array)
{
	if (string_array[1] == _pwd)
	{
		this->_clients[i - 1].setPasswordIsCorrect();
		std::string msg = "Success : Password is correct\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		checkRegistration(i);
	}	
}

void	Server::cmdUser(int i, std::vector<std::string> string_array)
{
	this->_clients[i - 1].setUsername(string_array[1]);
	std::string msg = "Success : "  + this->_clients[i - 1].getUsername() +  " Username is saved\r\n";
	send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
	checkRegistration(i);
}

void	Server::cmdPrivmsg(int i, std::vector<std::string> string_array, std::string buffer)
{
	// if (this->isRegistered(i) == 0)
	// {
	// 	std::cout << "Client not registered"  << std::endl;
	// 	return ;
	// }
	std::ostringstream str1;
	str1 << this->_clients[i - 1].getFd();
	std::string fd_string = str1.str();
	std::string msg;
	// if (string_array.size() < 3)
	// {
	// 	msg = "[Error 461] client " + fd_string + " : Wrong number of parameters\n";
	// 	send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
	// 	return;
	// }


	std::string message;
	// if (string_array[1][0] == '#')
	// {
		int pos = buffer.find("#");
		int pos2 = buffer.find(" ", pos) + 1;
		message = buffer.substr(pos2);

	// 	int k = 0;
	// 	while (k < static_cast<int>(getChannels().size()))
	// 	{
	// 		if (getChannels()[k]->getName() == string_array[1])
	// 			break;
	// 		k++;
	// 	}
	// 	if (k == static_cast<int>(getChannels().size()))
	// 	{
	// 		msg = "[Error 401] client " + fd_string + " " + string_array[1] + " : No such channel\n";
	// 		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
	// 		return;
	// 	}

		for (std::vector<Channel >::iterator it = this->_channels.begin(); it != _channels.end(); it++)
		{
			if ((*it).getName() == string_array[1])
			{
				// int k = 0;
				// -------------------
				// std::cout << "[" << this->_clients[i - 1].getNickname() << "]" << std::endl;
				// -------------------
				// while (k < static_cast<int>((*it).getClients().size()))
				// {
				// 	if ((*it).getClients()[k]->getNickname() == this->_clients[i - 1].getNickname())
				// 		break;
				// 	k++;
				// }
				// -------------------
				// std::cout << k << " vs " << (*it).getClients().size() << std::endl;
				// -------------------
				// if (k == static_cast<int>((*it).getClients().size()))
				// {
				// 	msg = "[Error 404] client " + fd_string + " " + string_array[1] + " : Cannot send to channel\n";
				// 	send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
				// 	return;
				// }

				std::cout << "CHANNEL FOUND" << std::endl;
				std::cout << (*it).getClients().size() << std::endl;
				
				int j = 0;
				while (j < static_cast<int>((*it).getClients().size()))
				{
					// std::string msg = "[" + this->_clients[i - 1].getNickname() + "] : " + message + "\n";
					std::cout << this->_clients[i - 1].getNickname() + " VS " +  (*it).getClients()[j]->getNickname() << std::endl;
					if (this->_clients[i - 1].getNickname() == (*it).getClients()[j]->getNickname())
					{
						std::cout << "SKIP" << std::endl;
						j++;
					}
					else{
					std::cout << "NOT SKIP" << std::endl;
					std::string msg = ":" + this->_clients[i - 1].getNickname() + "!" + this->_clients[i - 1].getUsername() + "@localhost PRIVMSG " + string_array[1] + " " + message + "\r\n";
					std::cout << msg << std::endl;
					send((*it).getClients()[j]->getFd(), msg.c_str(), msg.length(), 0);
					j++;
					}
					
				}
				break;
			}
		}
	// }
	// else
	// {
		// int pos = buffer.find(string_array[1]);
		// int pos2 = buffer.find(" ", pos) + 1;
		// message = buffer.substr(pos2);

		// // -------------------
		// // int k = 0;
		// // while (k < static_cast<int>(getClients().size()))
		// // {
		// // 	if (getClients()[k].getNickname() == string_array[1])
		// // 		break;
		// // 	k++;
		// // }
		// // if (k == static_cast<int>(getClients().size()))
		// // {
		// // 	msg = "[Error 401] client " + fd_string + " " + string_array[1] + " : No such nickname\n";
		// // 	send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		// // 	return;
		// // }
		// // -------------------

		// for (std::vector<Client>::iterator it = this->_clients.begin(); it != _clients.end(); it++)
		// {
		// 	if ((*it).getNickname() == string_array[1])
		// 	{
		// 		std::string msg = "" + this->_clients[i - 1].getNickname() + " " + message + "\r\n";
				
		// 		// std::string msg = ":"  + this->_clients[i - 1].getNickname() + "!" + this->_clients[i - 1].getUsername() +  "@localhost PRIVMSG " + string_array[1] + " " + message + "\r\n";
		// 		// std::string msg = ":" + this->_clients[i - 1].getNickname() + " PRIVMSG " + string_array[1] + " " + message + "\r\n";
		// 		// std::cout << msg << std::endl;
		// 		send((*it).getFd(), msg.c_str(), msg.length(), 0);
		// 		// send((*it).getFd(), string_array[2].c_str(), string_array[2].length(), 0);
		// 	}
		// }
	// }
}

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
			int k = 0;
			while (k < static_cast<int>((*it).getClients().size()))
			{
				std::string msg = ":" + this->_clients[i - 1].getNickname() + " JOIN " + string_array[1] + "\r\n";
				std::cout  << msg << std::endl;
				send(getClients()[k].getFd(), msg.c_str(), msg.length(), 0);

				std::string msg2 = ":localhost 353 " + this->_clients[i - 1].getUsername() + " = " + string_array[1] + " :@" ;
				int j = 0;
				while (j < static_cast<int>((*it).getClients().size()))
				{
					if (msg2[msg2.size()] != '@')
						msg2 = msg2 + " ";
					msg2 = msg2 + (*it).getClients()[j]->getNickname();
					j++;
				}
				msg2 = msg2 + "\r\n"; 
				std::cout  << msg2 << std::endl;
				send(getClients()[k].getFd(), msg2.c_str(), msg2.length(), 0);

				std::string msg3 = ":localhost 366 " + this->_clients[i - 1].getUsername() + " " + string_array[1] + " :End of /NAMES list.\r\n";
				std::cout  << msg3 << std::endl;
				send(getClients()[k].getFd(), msg3.c_str(), msg3.length(), 0);

				k++;
			}

			return;
		}
	}
	Channel newchannel(string_array[1].c_str());
	newchannel.addClient(&this->_clients[i - 1]);
	_channels.push_back(newchannel);

	std::string msg = ":" + this->_clients[i - 1].getNickname() + " JOIN " + string_array[1] + "\r\n";
	std::cout  << msg << std::endl;
	send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);

	std::string msg2 = ":localhost 353 " + this->_clients[i - 1].getUsername() + " = " + string_array[1] + " :@" + this->_clients[i - 1].getNickname() + "\r\n";
	std::cout  << msg2 << std::endl;
	send(this->_clients[i - 1].getFd(), msg2.c_str(), msg2.length(), 0);

	std::string msg3 = ":localhost 366 " + this->_clients[i - 1].getUsername() + " " + string_array[1] + " :End of /NAMES list.\r\n";
	std::cout  << msg3 << std::endl;
	send(this->_clients[i - 1].getFd(), msg3.c_str(), msg3.length(), 0);
}

void	Server::parseLine(std::string line, int i)
{
	std::vector<std::string> line_splitted;

	std::stringstream ss(line);
	std::string tmp;
	char del = ' ';

 	while(std::getline(ss, tmp, del))
	{
		line_splitted.push_back(tmp);
	}
	std::cout << line_splitted.size() << std::endl;
	if (line_splitted[1] == "LS")
	{
		std::string msg = "CAP * LS\r\n";
		send(this->_fds[i].fd, msg.c_str(), msg.length(), 0);
	}
	else if (line_splitted[0] == "NICK")
		cmdNick(i, line_splitted);
	else if (line_splitted[0] == "PASS")
		cmdPass(i, line_splitted);
	else if (line_splitted[0] == "USER")
		cmdUser(i, line_splitted);
	else if (line_splitted[0] == "PRIVMSG")
		cmdPrivmsg(i, line_splitted, line);
	else if (line_splitted[0] == "JOIN")
		cmdJoin(i, line_splitted);
}

void	Server::commandParsing(int i, std::string string)
{
	// std::cout << "[" << string << " " << i << "]" << std::endl;
	this->_clients[i - 1].setBuffer(string);
	std::cout << "[" << this->_clients[i - 1].getBuffer() << "]" << std::endl;
	if (this->_clients[i - 1].getBuffer().find("\r\n") == std::string::npos)
	{
		// test when ctrl+D (eof), it should accumulate buffer
		std::cout << "/r/n not present"<< std::endl;
		return;
	}
	else
		std::cout << "/r/n present"<< std::endl;
	
	std::vector<std::string> string_array;
	std::stringstream ss(this->_clients[i - 1].getBuffer());
	std::string line;
	std::string tmp;

 	while(std::getline(ss, line))
	{
		size_t pos = line.find_first_of("\r\n");
		string_array.push_back(line.substr(0,pos));
	}

	// std::vector<std::string>::iterator it;
	// for (it = string_array.begin(); it != string_array.end(); it++)
	// {
	// 	std::cout << *it  << std::endl;
	// }

	for (size_t j =0; j < string_array.size(); j++)
	{
		parseLine(string_array[j], i);
	}
	this->_clients[i - 1].getBuffer().clear();
	std::cout << "clear" <<  this->_clients[i - 1].getBuffer() <<std::endl;
	// std::vector<std::string> string_array;
	// std::string str;
	// size_t pos = str.find_first_of("\r\n");
	
	// while (pos != std::string::npos)
	// {
	// 	string_array.push_back(str.substr(0,pos));
	// 	str.erase(0, pos + )
	// }
}




// // FRED PART --------------------------------------------------------

// void printvector (std::string str)
// {
//   std::cout << str << std::endl;
// }

// void	Server::commandParsing(int i, std::string string)
// {
// 	size_t pos = string.find("\\");
// 	if (pos != std::string::npos)
// 		string = string.substr(0, pos);
// 	std::stringstream ss(string);
// 	std::string tmp;
// 	std::vector<std::string> string_array;

// 	while (std::getline(ss, tmp, ' '))
// 	{
// 		string_array.push_back(tmp);
// 	}

// 	std::string cmd_array[] = {
// 		"NICK",
// 		"PASS",
// 		"USER",
// 		"QUIT",
// 		"PRIVMSG",
// 		"JOIN",
// 		"PART",
// 		"KICK",
// 		"INVITE",
// 		"TOPIC",
// 		"MODE"
// 	};

// 	int len = sizeof(cmd_array) / sizeof(cmd_array[0]);
// 	int j = 0;
// 	while (j < len)
// 	{
// 		if (cmd_array[j] == string_array[0])
// 			break;
// 		j++;
// 	}
// 	std::cout << YELLOW << string_array[1] << RESET << std::endl;
// 	switch(j)
// 	{
// 		case 0: this->cmdNick(i, string_array); break;
// 		case 1: this->cmdPass(i, string_array); break;
// 		case 2: this->cmdUser(i, string_array); break;
// 		case 3: this->cmdQuit(i, string_array); break;bool isValidChannelName(const std::string& channelName);
//         Channel* findChannelByName(const std::string& channelName);
//         Client* findClientByNickname(const std::string& nickname);
//         void setMode(const std::string& mode, const std::string& value);
// 		case 4: this->cmdPrivmsg(i, string_array, string); break;
// 		case 5: this->cmdJoin(i, string_array); break;
// 		case 6: this->cmdPart(i, string_array); break;
// 		case 7: this->cmdKick(i, string_array); break;
// 		case 8: this->cmdInvite(i, string_array); break;
// 		case 9: this->cmdTopic(i, string_array); break;
// 		case 10: this->cmdMode(i, string_array); break;
//     default : break;
// 	}
// }

// int		Server::isRegistered(int i)
// {
// 	if (this->_clients[i - 1].getNickname().empty()
// 		|| this->_clients[i - 1].getUsername().empty()
// 		|| this->_clients[i - 1].getPasswordIsCorrect() == 0)
// 	{
// 		return (0);
// 	}
// 	else
// 	{
// 		return (1);
// 	}
// }

// bool isValidChannelName(const std::string& channelName);
//         Channel* findChannelByName(const std::string& channelName);
//         Client* findClientByNickname(const std::string& nickname);
//         void setMode(const std::string& mode, const std::string& value);


// // YUMI PART --------------------------------------------------------

// Channel* Server::findChannelByName(const std::string& channelName)
// {
//     for (size_t j = 0; j < this->_channels.size(); j++)
//     {
//         if (this->_channels[j]->getName() == channelName)
//             return (this->_channels[j]);
//     }
//     return (NULL);
// }


// Client* Server::findClientByNickname(const std::string& nickname)
// {
//     for (size_t j = 0; j < this->_clients.size(); j++)
// 	{
//         if (this->_clients[j].getNickname() == nickname)
//             return (&this->_clients[j]);
//     }
//     return NULL;
// }

// bool Server::isValidChannelName(const std::string& channelName)
// {
//     // start with # and no space
//     return !channelName.empty() && channelName[0] == '#' && channelName.find(' ') == std::string::npos;
// }

// void	Server::cmdJoin(int i, std::vector<std::string> string_array) // parameter: 1channel, pwd
// {
// 	if (this->isRegistered(i) == 0)
// 	{
// 		std::cout << "Client not registered"  << std::endl;
// 		return ;
// 	}
// 	std::cout << "cmdJoin" << " : " << string_array[0] << std::endl;
// 	if (string_array.size() < 2)
// 	{
// 		this->_clients[i - 1].sendMessage("Error: JOIN command requires at least 1 argument: <channel> [password]");
//         return;
// 	}
// 	std::string channelName = string_array[1]; //for now only 1 channel as parameter allowed
// 	std::string pwd = (string_array.size() > 2) ? string_array[2] : "";
// 	if (!isValidChannelName(channelName))
// 	{
// 		this->_clients[i - 1].sendMessage("Error: Invalid channel name. Channel names must start with '#' and no space allowed.");
// 		return ;
// 	}
// 	Channel * channel = findChannelByName(channelName);
// 	if (!channel)
// 	{
// 		channel = new Channel(channelName, *this);
// 		channel->addOperator(&this->_clients[i - 1]);
// 		this->_channels.push_back(channel);
// 	}
// 	channel->joinChannel(&this->_clients[i - 1], pwd); //handle in channel

// }

// void	Server::cmdPart(int i, std::vector<std::string> string_array) //parameter: 1channel, reason
// {
// 	if (this->isRegistered(i) == 0)
// 	{
// 		std::cout << "Client not registered"  << std::endl;
// 		return ;
// 	}
// 	std::cout << "cmdPart" << " : " << string_array[0] << std::endl;
// 	if (string_array.size() < 2) {
//         this->_clients[i].sendMessage("Error: PART command requires a channel name.");
//         return;
//     }
// 	std::string &channelName = string_array[1]; //for now only 1 channel as parameter allowed
// 	std::string reason = string_array.size() > 2 ? string_array[2] : "";
// 	if (!isValidChannelName(channelName))
// 	{
// 		this->_clients[i].sendMessage("Error: Invalid channel name. Channel names must start with '#' and no space allowed.");
// 		return ;
// 	}
// 	Channel *channel = findChannelByName(channelName);
// 	if (!channel)
// 	{
// 		this->_clients[i].sendMessage("Error: Channel " + channelName + " does not exist.");
// 		return ;
// 	}
// 	if (!channel->isClientInChannel(&_clients[i]))
// 	{
// 		this->_clients[i].sendMessage("Error: You are not in the channel " + channelName);
// 		return ;
// 	}
// 	channel->partChannel(&_clients[i], reason); //handle in channel
// 	if (channel->isEmpty())
// 	{
// 		for (std::vector<Channel*>::iterator it = this->_channels.begin(); it != this->_channels.end(); ) 
// 		{
//     		if (*it == channel)
// 			{
//         		this->_channels.erase(it);
//         		break;
//     		}
// 			else
// 				++it;
// 		}
// 		delete channel;
// 	}
// 	std::cout << "Client[" << i << "] parted from channel " << channelName << std::endl;
// }

// //

// void	Server::cmdKick(int i, std::vector<std::string> string_array)
// {
// 	if (this->isRegistered(i) == 0)
// 	{
// 		std::cout << "Client not registered"  << std::endl;
// 		return ;
// 	}
// 	std::cout << "cmdKick" << " : " << string_array[0] << std::endl;
// 	if (string_array.size() < 3)
// 	{
// 		this->_clients[i - 1].sendMessage("Error: KICK command requires at least 2 arguments: <channel> <nickname>");
// 		return ;
// 	}
// 	std::string channelName = string_array[1];
//     std::string targetNickname = string_array[2];
// 	std::string reason = string_array.size() > 3 ? string_array[3] : "No reason provided.";
// 	Channel *channel = findChannelByName(channelName);
// 	if (!channel)
// 	{
// 		this->_clients[i - 1].sendMessage("Error: Channel " + channelName + " not found.");
// 		return ;
// 	}
// 	Client *operatorClient = &_clients[i - 1];
// 	if (!channel->isOperator(operatorClient))
// 	{
// 		operatorClient->sendMessage("Error: You are not an operator in the channel " + channelName + ".");
//         return ;
// 	}
// 	Client *targetClient = findClientByNickname(targetNickname);
// 	if (!targetClient)
// 	{
// 		operatorClient->sendMessage("Error: Client with nickname " + targetNickname + " not found.");
//         return ;
// 	}
// 	if (!channel->isClientInChannel(targetClient))
// 	{
// 		operatorClient->sendMessage("Error: Client " + targetNickname + " is not in the channel " + channelName + ".");
//         return ;
// 	}
// 	channel->kickClient(targetClient, reason);
//     channel->broadcastMessage(targetNickname + " has been kicked from the channel " + channelName + " for: " + reason);
//     std::cout << "Client " << targetNickname << " has been kicked from channel " << channelName << " by operator " 
//               << operatorClient->getNickname() << ". Reason: " << reason << std::endl;
// }

// void	Server::cmdInvite(int i, std::vector<std::string> string_array)
// {
// 	if (this->isRegistered(i) == 0)
// 	{
// 		std::cout << "Client not registered"  << std::endl;
// 		return ;
// 	}
// 	std::cout << "cmdInvite" << " : " << string_array[0] << std::endl;

//     if (string_array.size() < 3)
//     {
//         this->_clients[i].sendMessage("Error: INVITE command requires at least two arguments: <nickname> <channel>");
//         return;
//     }

//     std::string targetNickname = string_array[1];
//     std::string channelName = string_array[2];

//     Channel* channel = findChannelByName(channelName);
//     if (!channel)
//     {
//         this->_clients[i].sendMessage("Error: Channel " + channelName + " not found.");
//         return;
//     }

//     if (!channel->isOperator(&this->_clients[i]))
//     {
//         this->_clients[i].sendMessage("Error: You are not an operator in this channel.");
//         return;
//     }

//     Client* targetClient = findClientByNickname(targetNickname);
//     if (!targetClient)
//     {
//         this->_clients[i].sendMessage("Error: Target client " + targetNickname + " not found.");
//         return;
//     }
//     channel->inviteClient(targetClient);
// }

// void	Server::cmdTopic(int i, std::vector<std::string> string_array)
// {
// 	if (this->isRegistered(i) == 0)
// 	{
// 		std::cout << "Client not registered" << std::endl;
// 		return ;
// 	}
// 	std::cout << "cmdTopic" << " : " << string_array[0] << std::endl;

//     if (string_array.size() < 2)
//     {
//         this->_clients[i].sendMessage("Error: TOPIC command requires a channel name.");
//         return;
//     }

// 	std::string channelName = string_array[1];
//     std::string newTopic = (string_array.size() > 2) ? string_array[2] : ""; // parameter with new_topic

// 	Channel* channel = findChannelByName(channelName);
// 	if (!channel)
// 	{
// 		this->_clients[i].sendMessage("Error: Channel " + channelName + " not found.");
// 		return ;
// 	}

// 	if (!channel->isOperator(&this->_clients[i]))
// 	{
// 		this->_clients[i].sendMessage("Error: You are not an operator in this channel.");
// 		return ;
// 	}
//     channel->setTopic(newTopic);
// 	this->_clients[i].sendMessage("Topic updated successfully.");
// }

// void	Server::cmdMode(int i, std::vector<std::string> string_array)
// {
//     if (this->isRegistered(i) == 0)
//     {
//         std::cout << "Client not registered" << std::endl;
//         return;
//     }
//     std::cout << "cmdMode" << " : " << string_array[0] << std::endl;
//     if (string_array.size() < 3)
//     {
//         this->_clients[i].sendMessage("Error: MODE command requires a channel name, mode, and value.");
//         return;
//     }
//     std::string channelName = string_array[1];
//     std::string mode = string_array[2];
//     std::string extra_cmd = (string_array.size() > 3) ? string_array[3] : "";

//     Channel* channel = findChannelByName(channelName);
//     if (!channel)
//     {
//         this->_clients[i].sendMessage("Error: Channel " + channelName + " not found.");
//         return;
//     }
//     if (!channel->isOperator(&this->_clients[i]))
//     {
//         this->_clients[i].sendMessage("Error: You are not an operator in this channel.");
//         return;
//     }
//     channel->setMode(mode, extra_cmd);
//     this->_clients[i].sendMessage("Mode updated successfully.");
// }


