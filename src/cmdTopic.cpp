#include "../includes/Irc.hpp"

void Server::cmdTopic(int i, std::vector<std::string> string_array)
{
    if (this->isRegistered(i) == 0)
    {
        this->_clients[i - 1].sendMessage(":localhost 451 * :You have not registered\r\n");
        return;
    }

    if (string_array.size() < 2)
    {
        this->_clients[i - 1].sendMessage(":localhost 461 TOPIC :Not enough parameters\r\n");
        return;
    }

    std::string channelName = string_array[1];
    // Channel channel = findChannelByName(channelName);
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
        this->_clients[i - 1].sendMessage(":localhost 403 " + channelName + " :No such channel\r\n");
        return;
    }

    if (string_array.size() == 2)
    {
        // std::string currentTopic = channel.getTopic();
        if (channel->getTopic().empty())
        {
            this->_clients[i - 1].sendMessage(":localhost 331 " + channelName + " :No topic is set\r\n");
        }
        else
        {
            this->_clients[i - 1].sendMessage(":localhost 332 " + channelName + " :" + channel->getTopic() + "\r\n");
        }
        return;
    }

    if (channel->getTopicRestricted() == true && !channel->isOperator(this->_clients[i - 1]))
    {
        this->_clients[i - 1].sendMessage(":localhost 482 " + channelName + " :You're not a channel operator" + "\r\n");
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

void Channel::setTopic(const std::string& newTopic)
{
    topic = newTopic;
    std::cout << "Channel topic updated to: " << topic << std::endl;
}

std::string Channel::getTopic() const
{
    return (topic);
}