#include "../includes/Irc.hpp"

void	Server::lineParsing(std::string line, int i)
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
	if (line == "CAP LS")
		cmdCap(i);
	else if (line_splitted[0] == "PASS")
		cmdPass(i, line_splitted);
	else if (line_splitted[0] == "USER")
		cmdUser(i, line_splitted);
	else if (line_splitted[0] == "NICK")
		cmdNick(i, line_splitted);
	else if (line_splitted[0] == "QUIT")
		cmdQuit(i, line_splitted);
	else if (line_splitted[0] == "PRIVMSG")
		cmdPrivmsg(i, line_splitted, line);
	else if (line_splitted[0] == "JOIN")
		cmdJoin(i, line_splitted);
	else if (line_splitted[0] == "INVITE")
		cmdInvite(i, line_splitted);
	else if (line_splitted[0] == "PART")
		cmdPart(i, line_splitted);
	else if (line_splitted[0] == "TOPIC")
		cmdTopic(i, line_splitted);
	else if (line_splitted[0] == "MODE")
		cmdMode(i, line_splitted);
	else if (line_splitted[0] == "KICK")
		cmdKick(i, line_splitted);
}

// void	Server::commandParsing(int i, std::string buf)
// {
// 	Client *current = getClient(_fds[i].fd);
// 	std::vector<std::string> string_array;
// 	std::string::size_type pos = 0;
// 	std::string string = current->getBuffer();
// 	std::cout << "Before append: " << string << " | Client " << current->getFd() << std::endl;
// 	string.append(buf);
// 	std::cout << "After append: " << string << std::endl;
// 	pos = string.rfind("\r\n");
//     if (last_pos != std::string::npos && last_pos + 2 == string.length()) {
//         std::string::iterator start = string.begin();
//         std::string::iterator end = string.begin() + last_pos;
//         std::string word;
//         for (std::string::iterator it = start; it != end; ++it) {
//             if (*it == ' ') {
//                 if (!word.empty()) {
//                     string_array.push_back(word);
//                     word.clear();
//                 }
//             }
// 			else
//                 word += *it;
//         }
//         if (!word.empty()) {
//             string_array.push_back(word);
//         }
//         string.clear();
//     } 
// 	else {
//         current.updateBuffer(string);
//         string.clear();
// 		return;
//     }
// }
// 	std::cout<<GREEN "Full message received by " YELLOW "Client "<<current->getFd() <<GREEN ": " RESET << string << std::endl;
// 	if (string_array.empty()) // empty command
// 	{
// 		std::cerr << "Error: Empty command received" << std::endl;
// 		return;
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
// 		if (cmd_array[j] == string_array.front())
// 			break;
// 		j++;
// 	}

// 	if (j >= len)
//     {
//         std::cerr << "Error: Unrecognized command [" << string_array[0] << "]." << std::endl;
//         return;
//     }
	
// 	if (string_array.size() > 1)
//     	std::cout << YELLOW << string_array[1] << RESET << std::endl;
// 	else
// 	{
//     	string_array.push_back("");
// 	}

// 	switch(j)
// 	{
// 		case 0: this->cmdNick(i, string_array); break;
// 		case 1: this->cmdPass(i, string_array); break;
// 		case 2: this->cmdUser(i, string_array); break;
// 		case 3: this->cmdQuit(i, string_array); break;bool isValidChannelName(const std::string& channelName);
//         Channel* findChannelByName(const std::string& channelName);
//         Client* findClientByNickname(const std::string& nickname);
//         void setMode(const std::string& mode, const std::string& value);
// 		case 3: this->cmdQuit(i, string_array); break;
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
// 		std::cout << "/r/n present"<< std::endl;
// Test when ctrl+D (eof), it should accumulate buffer
void	Server::bufferParsing(int i, std::string string)
{
	this->_clients[i - 1].setBuffer(string);
	if (this->_clients[i - 1].getBuffer().find("\r\n") == std::string::npos)
		return;
	
	std::vector<std::string> string_array;
	std::stringstream ss(this->_clients[i - 1].getBuffer());
	std::string line;
	std::string tmp;
 	while(std::getline(ss, line))
	{
		size_t pos = line.find_first_of("\r\n");
		string_array.push_back(line.substr(0,pos));
	}

	for (size_t j =0; j < string_array.size(); j++)
	{
		lineParsing(string_array[j], i);
	}
	this->_clients[i - 1].getBuffer().clear();
}



// YUMI PART --------------------------------------------------------

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

// bool isValidChannelName(const std::string& channelName);
//         Channel* findChannelByName(const std::string& channelName);
//         Client* findClientByNickname(const std::string& nickname);
//         void setMode(const std::string& mode, const std::string& value);


// // YUMI PART --------------------------------------------------------


Channel* Server::findChannelByName(const std::string& channelName)
{
    for (size_t j = 0; j < this->_channels.size(); j++)
    {
        if (this->_channels[j].getName() == channelName)
            return (&this->_channels[j]);
    }
    return (NULL);
}
Client* Server::findClientByNickname(const std::string& nickname)
{
    for (size_t j = 0; j < this->_clients.size(); j++)
	{
        if (this->_clients[j].getNickname() == nickname)
            return (&this->_clients[j]);
    }
    return NULL;
}

bool Server::isValidChannelName(const std::string& channelName)
{
    // start with # and no space
    return !channelName.empty() && channelName[0] == '#' && channelName.find(' ') == std::string::npos;
}

// void	Server::cmdJoin(int i, std::vector<std::string> string_array) // parameter: 1channel, pwd
// {
//     if (this->isRegistered(i) == 0)
//     {
//         std::cout << "Client not registered" << std::endl;
//         return;
//     }

//     std::cout << "cmdJoin: " << string_array[0] << std::endl;

//     if (string_array.size() < 2)
//     {
//         this->_clients[i - 1].sendMessage("[Error 461] JOIN : Not enough parameters\n");
//         return;
//     }

//     std::string channelName = string_array[1];
//     std::string pwd = (string_array.size() > 2) ? string_array[2] : ""; // 預設密碼為空

//     if (!isValidChannelName(channelName))
//     {
//         this->_clients[i - 1].sendMessage("[Error 403] " + channelName + " : Invalid channel name\n");
//         return;
//     }

//     Channel *channel = findChannelByName(channelName);
//     if (!channel)
//     {
//         channel = new Channel(channelName);
//         channel->addOperator(&this->_clients[i - 1]);
//         _channels.push_back(*channel); //
//     }
//     channel->joinChannel(&this->_clients[i - 1], pwd);
// }

void	Server::cmdPart(int i, std::vector<std::string> string_array) //parameter: 1channel, reason
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "Client not registered"  << std::endl;
		return ;
	}
	if (string_array.size() < 2) {
        this->_clients[i - 1].sendMessage(":localhost 461 PART :Not enough parameters\n");
        return;
    }
	std::string &channelName = string_array[1]; //for now only 1 channel as parameter allowed
	std::string reason = string_array.size() > 2 ? string_array[2] : "";
	if (!isValidChannelName(channelName))
	{
		this->_clients[i - 1].sendMessage(":localhost 403 " + channelName + " :No such channel\n");
		return ;
	}
	Channel *channel = findChannelByName(channelName);
	if (!channel)
	{
		this->_clients[i - 1].sendMessage(":localhost 403 " + channelName + " :No such channel\n");
		return ;
	}
	if (!channel->isClientInChannel(&_clients[i - 1]))
	{
		this->_clients[i - 1].sendMessage(":localhost 442 " + channelName + " :You are not on that channel\n");
		return ;
	}
	channel->partChannel(&_clients[i], reason); //handle in channel
	if (channel->isEmpty())
	{
		for (std::vector<Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); ) 
		{
    		if (&(*it) == channel)
			{
        		it = this->_channels.erase(it);
        		break;
    		}
			else
				++it;
		}
		delete channel;
	}
	std::cout << "Client[" << i - 1 << "] parted from channel " << channelName << std::endl;
}

//

void	Server::cmdKick(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		this->_clients[i - 1].sendMessage(":localhost 451 * :You have not registered");
		return ;
	}
	std::cout << "cmdKick" << " : " << string_array[0] << std::endl;
	if (string_array.size() < 3)
	{
		this->_clients[i - 1].sendMessage(":localhost 461 KICK :Not enough parameters");
		return ;
	}
	std::string channelName = string_array[1];
    std::string targetNickname = string_array[2];
	std::string reason = string_array.size() > 3 ? string_array[3] : "No reason provided.";
	Channel *channel = findChannelByName(channelName);
	if (!channel)
	{
		this->_clients[i - 1].sendMessage(":localhost 403 " + channelName + " :No such channel");
		return ;
	}
	Client *operatorClient = &_clients[i - 1];
	if (!channel->isOperator(operatorClient))
	{
		operatorClient->sendMessage(":localhost 482 " + channelName + " :You're not a channel operator");
        return ;
	}
	Client *targetClient = findClientByNickname(targetNickname);
	if (!targetClient)
	{
		operatorClient->sendMessage(":localhost 401 " + targetNickname + " :No such nick/channel");
        return ;
	}
	if (!channel->isClientInChannel(targetClient))
	{
		operatorClient->sendMessage(":localhost 441 " + targetNickname + " " + channelName + " :They aren't on that channel");
        return ;
	}//for irssi
	std::string kickMessage = ":" + operatorClient->getNickname() + "!" + operatorClient->getUsername() + "@" + operatorClient->getHostname() +
							  " KICK " + channelName + " " + targetNickname + " :" + (reason.empty() ? "No reason" : reason);
	targetClient->sendMessage(kickMessage);

	channel->broadcastMessage(kickMessage, targetClient);

	channel->kickClient(targetClient);

	std::cout << "Client " << targetNickname << " has been kicked from channel " << channelName
			  << " by operator " << operatorClient->getNickname() << ". Reason: " << (reason.empty() ? "No reason" : reason) << std::endl;
	// channel->kickClient(targetClient, reason);
    // channel->broadcastMessage(targetNickname + " has been kicked from the channel " + channelName + " for: " + reason);
    // std::cout << "Client " << targetNickname << " has been kicked from channel " << channelName << " by operator " 
    //           << operatorClient->getNickname() << ". Reason: " << reason << std::endl;
}

void	Server::cmdInvite(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		this->_clients[i - 1].sendMessage(":localhost 451 * :You have not registered");
		return ;
	}

    if (string_array.size() < 3)
    {
        this->_clients[i - 1].sendMessage(":localhost 461 INVITE :Not enough parameters");
        return;
    }

    std::string targetNickname = string_array[1];
    std::string channelName = string_array[2];

    Channel* channel = findChannelByName(channelName);
    if (!channel)
    {
        this->_clients[i - 1].sendMessage(":localhost 403 " + channelName + " :No such channel");
        return;
    }

    if (!channel->isOperator(&this->_clients[i - 1]))
    {
        this->_clients[i - 1].sendMessage(":localhost 482 " + channelName + " :You're not a channel operator");
        return;
    }

    Client* targetClient = findClientByNickname(targetNickname);
    if (!targetClient)
    {
        this->_clients[i - 1].sendMessage(":localhost 401 " + targetNickname + " :No such nick/channel");
        return;
    }
    channel->inviteClient(targetClient);
    this->_clients[i - 1].sendMessage(":localhost 341 " + this->_clients[i - 1].getNickname() + " " + targetNickname + " " + channelName);
    targetClient->sendMessage(":localhost :You have been invited to join " + channelName + " by " + this->_clients[i - 1].getNickname());
}

void Server::cmdTopic(int i, std::vector<std::string> string_array)
{
    if (this->isRegistered(i) == 0)
    {
        this->_clients[i - 1].sendMessage(":localhost 451 * :You have not registered");
        return;
    }

    if (string_array.size() < 2)
    {
        this->_clients[i - 1].sendMessage(":localhost 461 TOPIC :Not enough parameters");
        return;
    }

    std::string channelName = string_array[1];
    Channel* channel = findChannelByName(channelName);
    if (!channel)
    {
        this->_clients[i - 1].sendMessage(":localhost 403 " + channelName + " :No such channel");
        return;
    }

    if (string_array.size() == 2)
    {
        std::string currentTopic = channel->getTopic();
        if (currentTopic.empty())
        {
            this->_clients[i - 1].sendMessage(":localhost 331 " + channelName + " :No topic is set");
        }
        else
        {
            this->_clients[i - 1].sendMessage(":localhost 332 " + channelName + " :" + currentTopic);
        }
        return;
    }

    if (!channel->isOperator(&this->_clients[i - 1]))
    {
        this->_clients[i - 1].sendMessage(":localhost 482 " + channelName + " :You're not a channel operator");
        return;
    }

    std::string newTopic;
    for (size_t j = 2; j < string_array.size(); ++j)
    {
        newTopic += string_array[j] + " ";
    }
    newTopic = newTopic.substr(0, newTopic.size() - 1);

    channel->setTopic(newTopic);
    std::string topicMessage = ":" + this->_clients[i - 1].getNickname() + "!" +
                               this->_clients[i - 1].getUsername() + "@" +
                               this->_clients[i - 1].getHostname() + " TOPIC " + channelName + " :" + newTopic;
    channel->broadcastMessage(topicMessage);

    std::cout << "Topic for channel " << channelName << " updated to: " << newTopic << std::endl;
}


void Server::cmdMode(int i, std::vector<std::string> string_array)
{
    if (this->isRegistered(i) == 0)
    {
        this->_clients[i - 1].sendMessage(":localhost 451 * :You have not registered");
        return;
    }

    if (string_array.size() < 2)
    {
        this->_clients[i - 1].sendMessage(":localhost 461 MODE :Not enough parameters");
        return;
    }

    std::string channelName = string_array[1];
    std::string mode = (string_array.size() > 2) ? string_array[2] : "";
    std::string extra_cmd = (string_array.size() > 3) ? string_array[3] : "";

    Channel* channel = findChannelByName(channelName);
    if (!channel)
    {
        this->_clients[i - 1].sendMessage(":localhost 403 " + channelName + " :No such channel");
        return;
    }

    if (mode.empty())
    {
        // Return current channel modes
        this->_clients[i - 1].sendMessage(":localhost 324 " + this->_clients[i - 1].getNickname() +
                                          " " + channelName + " " + channel->getMode("")); //! check needed
        return;
    }

    if (!channel->isOperator(&this->_clients[i - 1]))
    {
        this->_clients[i - 1].sendMessage(":localhost 482 " + channelName + " :You're not a channel operator");
        return;
    }

    channel->setMode(mode, extra_cmd, &_clients[i - 1]);
    this->_clients[i - 1].sendMessage("Mode updated successfully."); // keep?
}




