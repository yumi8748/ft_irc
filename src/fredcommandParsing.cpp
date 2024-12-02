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
		case 3: this->cmdQuit(i, string_array); break;bool isValidChannelName(const std::string& channelName);
        Channel* findChannelByName(const std::string& channelName);
        Client* findClientByNickname(const std::string& nickname);
        void setMode(const std::string& mode, const std::string& value);
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

void	Server::cmdPass(int i, std::vector<std::string> string_array)
{
	std::cout << "cmdPass" << " : "  << i << string_array[0] << std::endl;
}

void	Server::cmdQuit(int i, std::vector<std::string> string_array)
{
	std::cout << "cmdQuit" << " : "  << i << string_array[0] << std::endl;
}

void	Server::cmdPrivmsg(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "Client not registered"  << std::endl;
		return ;
	}
	std::cout << "cmdPrivmsg" << " : " << string_array[0] << std::endl;
}
bool isValidChannelName(const std::string& channelName);
        Channel* findChannelByName(const std::string& channelName);
        Client* findClientByNickname(const std::string& nickname);
        void setMode(const std::string& mode, const std::string& value);


// YUMI PART --------------------------------------------------------

Channel* Server::findChannelByName(const std::string& channelName)
{
    for (size_t j = 0; j < this->_channels.size(); j++)
    {
        if (this->_channels[j]->getName() == channelName)
            return (this->_channels[j]);
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

void	Server::cmdJoin(int i, std::vector<std::string> string_array) // parameter: 1channel, pwd
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "Client not registered"  << std::endl;
		return ;
	}
	std::cout << "cmdJoin" << " : " << string_array[0] << std::endl;
	if (string_array.size() < 2)
	{
		this->_clients[i - 1].sendMessage("Error: JOIN command requires at least 1 argument: <channel> [password]");
        return;
	}
	std::string channelName = string_array[1]; //for now only 1 channel as parameter allowed
	std::string pwd = (string_array.size() > 2) ? string_array[2] : "";
	if (!isValidChannelName(channelName))
	{
		this->_clients[i - 1].sendMessage("Error: Invalid channel name. Channel names must start with '#' and no space allowed.");
		return ;
	}
	Channel * channel = findChannelByName(channelName);
	if (!channel)
	{
		channel = new Channel(channelName, *this);
		channel->addOperator(&this->_clients[i - 1]);
		this->_channels.push_back(channel);
	}
	channel->joinChannel(&this->_clients[i - 1], pwd); //handle in channel

}

void	Server::cmdPart(int i, std::vector<std::string> string_array) //parameter: 1channel, reason
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "Client not registered"  << std::endl;
		return ;
	}
	std::cout << "cmdPart" << " : " << string_array[0] << std::endl;
	if (string_array.size() < 2) {
        this->_clients[i].sendMessage("Error: PART command requires a channel name.");
        return;
    }
	std::string &channelName = string_array[1]; //for now only 1 channel as parameter allowed
	std::string reason = string_array.size() > 2 ? string_array[2] : "";
	if (!isValidChannelName(channelName))
	{
		this->_clients[i].sendMessage("Error: Invalid channel name. Channel names must start with '#' and no space allowed.");
		return ;
	}
	Channel *channel = findChannelByName(channelName);
	if (!channel)
	{
		this->_clients[i].sendMessage("Error: Channel " + channelName + " does not exist.");
		return ;
	}
	if (!channel->isClientInChannel(&_clients[i]))
	{
		this->_clients[i].sendMessage("Error: You are not in the channel " + channelName);
		return ;
	}
	channel->partChannel(&_clients[i], reason); //handle in channel
	if (channel->isEmpty())
	{
		for (std::vector<Channel*>::iterator it = this->_channels.begin(); it != this->_channels.end(); ) 
		{
    		if (*it == channel)
			{
        		this->_channels.erase(it);
        		break;
    		}
			else
				++it;
		}
		delete channel;
	}
	std::cout << "Client[" << i << "] parted from channel " << channelName << std::endl;
}

//

void	Server::cmdKick(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "Client not registered"  << std::endl;
		return ;
	}
	std::cout << "cmdKick" << " : " << string_array[0] << std::endl;
	if (string_array.size() < 3)
	{
		this->_clients[i - 1].sendMessage("Error: KICK command requires at least 2 arguments: <channel> <nickname>");
		return ;
	}
	std::string channelName = string_array[1];
    std::string targetNickname = string_array[2];
	std::string reason = string_array.size() > 3 ? string_array[3] : "No reason provided.";
	Channel *channel = findChannelByName(channelName);
	if (!channel)
	{
		this->_clients[i - 1].sendMessage("Error: Channel " + channelName + " not found.");
		return ;
	}
	Client *operatorClient = &_clients[i - 1];
	if (!channel->isOperator(operatorClient))
	{
		operatorClient->sendMessage("Error: You are not an operator in the channel " + channelName + ".");
        return ;
	}
	Client *targetClient = findClientByNickname(targetNickname);
	if (!targetClient)
	{
		operatorClient->sendMessage("Error: Client with nickname " + targetNickname + " not found.");
        return ;
	}
	if (!channel->isClientInChannel(targetClient))
	{
		operatorClient->sendMessage("Error: Client " + targetNickname + " is not in the channel " + channelName + ".");
        return ;
	}
	channel->kickClient(targetClient, reason);
    channel->broadcastMessage(targetNickname + " has been kicked from the channel " + channelName + " for: " + reason);
    std::cout << "Client " << targetNickname << " has been kicked from channel " << channelName << " by operator " 
              << operatorClient->getNickname() << ". Reason: " << reason << std::endl;
}

void	Server::cmdInvite(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "Client not registered"  << std::endl;
		return ;
	}
	std::cout << "cmdInvite" << " : " << string_array[0] << std::endl;

    if (string_array.size() < 3)
    {
        this->_clients[i].sendMessage("Error: INVITE command requires at least two arguments: <nickname> <channel>");
        return;
    }

    std::string targetNickname = string_array[1];
    std::string channelName = string_array[2];

    Channel* channel = findChannelByName(channelName);
    if (!channel)
    {
        this->_clients[i].sendMessage("Error: Channel " + channelName + " not found.");
        return;
    }

    if (!channel->isOperator(&this->_clients[i]))
    {
        this->_clients[i].sendMessage("Error: You are not an operator in this channel.");
        return;
    }

    Client* targetClient = findClientByNickname(targetNickname);
    if (!targetClient)
    {
        this->_clients[i].sendMessage("Error: Target client " + targetNickname + " not found.");
        return;
    }
    channel->inviteClient(targetClient);
}

void	Server::cmdTopic(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "Client not registered" << std::endl;
		return ;
	}
	std::cout << "cmdTopic" << " : " << string_array[0] << std::endl;

    if (string_array.size() < 2)
    {
        this->_clients[i].sendMessage("Error: TOPIC command requires a channel name.");
        return;
    }

	std::string channelName = string_array[1];
    std::string newTopic = (string_array.size() > 2) ? string_array[2] : ""; // parameter with new_topic

	Channel* channel = findChannelByName(channelName);
	if (!channel)
	{
		this->_clients[i].sendMessage("Error: Channel " + channelName + " not found.");
		return ;
	}

	if (!channel->isOperator(&this->_clients[i]))
	{
		this->_clients[i].sendMessage("Error: You are not an operator in this channel.");
		return ;
	}
    channel->setTopic(newTopic);
	this->_clients[i].sendMessage("Topic updated successfully.");
}

void	Server::cmdMode(int i, std::vector<std::string> string_array)
{
    if (this->isRegistered(i) == 0)
    {
        std::cout << "Client not registered" << std::endl;
        return;
    }
    std::cout << "cmdMode" << " : " << string_array[0] << std::endl;
    if (string_array.size() < 3)
    {
        this->_clients[i].sendMessage("Error: MODE command requires a channel name, mode, and value.");
        return;
    }
    std::string channelName = string_array[1];
    std::string mode = string_array[2];
    std::string extra_cmd = (string_array.size() > 3) ? string_array[3] : "";

    Channel* channel = findChannelByName(channelName);
    if (!channel)
    {
        this->_clients[i].sendMessage("Error: Channel " + channelName + " not found.");
        return;
    }
    if (!channel->isOperator(&this->_clients[i]))
    {
        this->_clients[i].sendMessage("Error: You are not an operator in this channel.");
        return;
    }
    channel->setMode(mode, extra_cmd);
    this->_clients[i].sendMessage("Mode updated successfully.");
}


