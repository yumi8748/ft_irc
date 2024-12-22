#include "../includes/Irc.hpp"

int		Server::cmdModeErrors(int i, std::vector<std::string> string_array, std::string modeStr)
{
	std::string msg;
	std::string target = this->_clients[i - 1].getNickname();
	char operation = modeStr[0];
	if (this->_clients[i - 1].getIsRegistered() == 0)
	{
		msg = ":localhost 451 " + target + " :You have not registered\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return (1);
	}
	if (string_array.size() < 2)
    {
		msg = ":localhost 461 " + target + " MODE :Not enough parameters\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
        return (1);
    }
	if (operation != '+' && operation != '-')
    {
		msg = ":localhost 501 " + target + " :Unknown MODE flag\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
        return (1);
    }
	return (0);
}

int		Server::cmdModeErrorsChannel(int i, std::string channelName)
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
	if (!channel->isOperator(this->_clients[i - 1]))
    {
		msg = ":localhost 482 " + target + " " + channelName + " :You're not channel operator\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return (1);
    }
	return (0);
}

void Server::cmdMode(int i, std::vector<std::string> string_array)
{
    std::string mode = (string_array.size() > 2) ? string_array[2] : "";
	if (cmdModeErrors(i, string_array, mode) == 1)
		return;
	std::string channelName = string_array[1];
	if (cmdModeErrorsChannel(i, channelName) == 1)
		return;
	Client curr = _clients[i - 1];
    Channel* channel = NULL;
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->getName() == channelName)
        {
            channel = &(*it);
            break;
        }
    }
    channel->setMode(mode, string_array, curr);
}

void Channel::setMode(const std::string& modeStr, std::vector<std::string> string_array, const Client& client)
{
	std::string cliname = client.getNickname() + " ";
    std::string modeMessage;
    std::string usrId = USER_ID(client.getNickname(), client.getUsername());
    int arrayMax = string_array.size();
    char operation = modeStr[0]; // '+' or '-'
    int arrayIndex = 3;

    for (size_t i = 1; i < modeStr.size(); ++i)
    {
        if (modeStr[i] == '-' || modeStr[i] == '+')
        {
            operation = modeStr[i];
            i++;
            if (i == modeStr.size())
                break;
        }
        char mode = modeStr[i];
        if (mode == 'i')
        {
            inviteOnly = (operation == '+');
            modeMessage = usrId + " MODE " + name + " " + operation + "i" + "\r\n";
        }
        else if (mode == 't')
        {
            topicRestricted = (operation == '+');
            modeMessage = usrId + " MODE " + name + " " + operation + "t" + "\r\n";
        }
        else if (mode == 'k' && arrayIndex < arrayMax)
        {
            if (operation == '+')
                Ch_pwd = string_array[arrayIndex];
            else if (operation == '-')
                Ch_pwd.clear();
            arrayIndex++;
            modeMessage = usrId + " MODE " + name + " " + operation + "k" + "\r\n";
        }
        else if (mode == 'l' && operation == '+' && arrayIndex < arrayMax)
        {
            userLimits = std::atoi(string_array[arrayIndex].c_str());
            arrayIndex++;
			modeMessage = usrId + " MODE " + name + " " + operation + "l " + "\r\n";
        }
        else if (mode == 'l' && operation == '-')
        {
            userLimits = -1;
			modeMessage = usrId + " MODE " + name + " " + operation + "l " + "\r\n";
        }
        else if (mode == 'o' && arrayIndex < arrayMax)
        {
            Client* targetClient = NULL;
            for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
            {
                if (it->getNickname() == string_array[arrayIndex])
                {
                    targetClient = &(*it);
                    break;
                }
            }
            if (!targetClient)
            {
                modeMessage = ":localhost 401 " + cliname + " " + string_array[arrayIndex] + " :No such nick/channel\r\n";
            }
            else if (operation == '+' && !isOperator(*targetClient))
            {
                addOperator(*targetClient);
                modeMessage = usrId + " MODE " + name + " " + operation + "o " + string_array[arrayIndex] + "\r\n";
            }
            else if (operation == '-' && isOperator(*targetClient))
            {
                removeOperator(*targetClient);
               	modeMessage = usrId + " MODE " + name + " " + operation + "o " + string_array[arrayIndex] + "\r\n";
            }
            arrayIndex++;
        }
        for (std::size_t k = 0; k < clients.size(); ++k)
        {
            send(clients[k].getFd(), modeMessage.c_str(), modeMessage.length(), 0);
        }
    }
}

std::string Channel::getMode(const std::string& mode) const
{
    std::map<std::string, std::string>::const_iterator it = modes.find(mode);
    if (it != modes.end())
        return (it->second);
    return ("");
}
