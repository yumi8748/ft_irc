#include "../includes/Irc.hpp"

int		Server::cmdTopicErrors(int i, std::vector<std::string> string_array)
{
	std::string msg;
	std::string target = this->_clients[i - 1].getNickname();
	if (this->_clients[i - 1].getIsRegistered() == 0)
	{
		msg = ":localhost 451 " + target + " :You have not registered\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return (1);
	}
	if (string_array.size() < 2)
    {
		msg = ":localhost 461 " + target + " TOPIC :Not enough parameters\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
        return (1);
    }
	return (0);
}

int		Server::cmdTopicErrorsChannel(int i, std::string channelName)
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

void Server::cmdTopic(int i, std::vector<std::string> string_array)
{
	if (cmdTopicErrors(i, string_array) == 1)
		return;
    std::string channelName = string_array[1];
    std::string msg;
	std::string target = this->_clients[i - 1].getNickname();
	std::string sender = this->_clients[i - 1].getNickname() + "!" + this->_clients[i - 1].getUsername() + "@localhost";
	Channel* channel = NULL;
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->getName() == channelName)
        {
            channel = &(*it);
            break;
        }
    }
    if (string_array.size() == 2)
    {
        if (channel->getTopic().empty())
        {
			msg = ":localhost 331 " + target + " " + channelName + " :No topic is set\r\n";
			send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
        }
        else
        {
			msg = ":localhost 332 " + target + " " + channelName + " :" + channel->getTopic() + "\r\n";
			send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
        }
        return;
    }
    std::string newTopic;
    for (size_t j = 2; j < string_array.size(); ++j)
    {
        newTopic += string_array[j] + " ";
    }
    newTopic = newTopic.substr(0, newTopic.size() - 1);
    channel->setTopic(newTopic);
	for (std::size_t k = 0; k < _clients.size(); ++k)
    {
		msg = ":" + sender + " TOPIC " + channelName + " :" + newTopic + "\r\n";
        send(_clients[k].getFd(), msg.c_str(), msg.length(), 0);
    }
}

void Channel::setTopic(const std::string& newTopic)
{
    topic = newTopic;
    std::cout << "Channel topic updated to: " << topic << std::endl;
}

std::string Channel::getTopic() const
{
    return (topic);
}
