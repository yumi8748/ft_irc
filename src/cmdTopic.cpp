#include "../includes/Irc.hpp"

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
    Channel channel = findChannelByName(channelName);
    if (channel == Channel())
    {
        this->_clients[i - 1].sendMessage(":localhost 403 " + channelName + " :No such channel");
        return;
    }

    if (string_array.size() == 2)
    {
        std::string currentTopic = channel.getTopic();
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

    if (!channel.isOperator(this->_clients[i - 1]))
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

    channel.setTopic(newTopic);
    std::string topicMessage = ":" + this->_clients[i - 1].getNickname() + "!" +
                               this->_clients[i - 1].getUsername() + "@" +
                               this->_clients[i - 1].getHostname() + " TOPIC " + channelName + " :" + newTopic;
    channel.broadcastMessage(topicMessage);

    std::cout << "Topic for channel " << channelName << " updated to: " << newTopic << std::endl;
}
