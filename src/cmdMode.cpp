#include "../includes/Irc.hpp"

std::string intToString(int num)
{
    if (num == 0) return "0";
    std::string result;
    bool isNegative = (num < 0);
    if (isNegative) num = -num;

    while (num > 0)
    {
        result.insert(result.begin(), '0' + (num % 10));
        num /= 10;
    }

    if (isNegative) result.insert(result.begin(), '-');
    return result;
}

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
    std::string extra_cmd = (string_array.size() > 3) ? string_array[3] : "";
    Channel* channel = NULL;
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->getName() == channelName)
        {
            channel = &(*it);
            break;
        }
    }
    channel->setMode(mode, extra_cmd, curr);
}



void Channel::setMode(const std::string& modeStr, const std::string& value, const Client& client)
{
	std::string cliname = client.getNickname() + " ";
    char operation = modeStr[0]; // '+' or '-'
    // Apply modes one by one
    for (size_t i = 1; i < modeStr.size(); ++i)
    {
        char mode = modeStr[i];
        std::string modeMessage;  // Standard IRC MODE message
        std::string feedback;    // Intuitive message for the client
		std::string usrId = USER_ID(client.getNickname(), client.getUsername());
        switch (mode)
        {
            case 'i':
                inviteOnly = (operation == '+');
                modeMessage = usrId + " MODE " + name + " " + operation + "i";
                break;

            case 't':
                topicRestricted = (operation == '+');
                modeMessage = usrId + " MODE " + name + " " + operation + "t";
                break;

            case 'k':
                if (operation == '+')
                {
                    Ch_pwd = value;
               		modeMessage = usrId + " MODE " + name + " " + operation + "k";
                }
                else
                {
                    Ch_pwd.clear();
               		modeMessage = usrId + " MODE " + name + " " + operation + "k";
                }
                break;

            case 'o':
            {
                Client* targetClient = NULL;
				for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
				{
					if (it->getNickname() == value)
					{
						targetClient = &(*it);
						break;
					}
				}

				if (!targetClient)
                {
                    client.sendMessage(":localhost 401 " + cliname + value + " :No such nick/channel\r\n");
                    continue;
                }

                if (operation == '+')
                {
                    if (!isOperator(*targetClient))
                    {
                        addOperator(*targetClient);
               			modeMessage = usrId + " MODE " + name + " " + operation + "o " + value;
                        feedback = "You have granted operator privileges to " + value + "." + "\r\n";

                        // Notify targetClient
                        targetClient->sendMessage(":localhost NOTICE " + targetClient->getNickname() + 
                            " :You have been granted operator privileges in channel " + name + "." + "\r\n");
                    }
                    else
                    {
                        client.sendMessage(":localhost 324 " + cliname + " " + name + " " + value + " " + " :is already an operator\r\n");
                        continue;
                    }
                }
                else
                {
                    if (isOperator(*targetClient))
                    {
                        removeOperator(*targetClient);
               		modeMessage = usrId + " MODE " + name + " " + operation + "o " + value;
                        feedback = "You have removed operator privileges from " + value + "." + "\r\n";

                        // Notify targetClient
                        targetClient->sendMessage(":localhost NOTICE " + targetClient->getNickname() + 
                            " :Your operator privileges in channel " + name + " have been revoked." + "\r\n");
                    }
                    else
                    {
                        client.sendMessage(":localhost 324 " + cliname + " " + name + " " + value + " " + " :is not an operator and cannot be removed" + "\r\n"); //not yet
                        continue;
                    }
                }
                break;
            }

            case 'l': // User limit
                if (operation == '+')
                {
                    userLimits = std::atoi(value.c_str());
					modeMessage = usrId + " MODE " + name + " " + operation + "l " + intToString(userLimits);
                    feedback = "You have set the channel user limit to " + intToString(userLimits) + "." + "\r\n";
                }
                else
                {
                    userLimits = -1; // Remove limit
					modeMessage = usrId + " MODE " + name + " " + operation + "l";
                    feedback = "You have removed the user limit for the channel\r\n.";
                }
                break;

            default:
                client.sendMessage(":localhost 472 " + cliname + std::string(1, mode) + " :is unknown mode char to me" + "\r\n");
                continue;
        }

        // Send standard IRC mode message
        client.sendMessage(modeMessage + "\r\n");
		std::cout << YELLOW << modeMessage << "\n" << feedback << RESET << std::endl;
        // broadcastMessage(modeMessage, client);

        // Send intuitive feedback to the client
        client.sendMessage(":localhost NOTICE " + cliname + " :" + feedback + "\r\n");
    }
}

std::string Channel::getMode(const std::string& mode) const
{
    std::map<std::string, std::string>::const_iterator it = modes.find(mode);
    if (it != modes.end())
        return (it->second);
    return ("");
}
