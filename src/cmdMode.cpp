#include "../includes/Irc.hpp"

void Server::cmdMode(int i, std::vector<std::string> string_array)
{
    if (this->isRegistered(i) == 0)
    {
        this->_clients[i - 1].sendMessage(":localhost 451 * :You have not registered\r\n");
        return;
    }

    if (string_array.size() < 2)
    {
        this->_clients[i - 1].sendMessage(":localhost 461 MODE :Not enough parameters\r\n");
        return;
    }

    std::string channelName = string_array[1];
    std::string mode = (string_array.size() > 2) ? string_array[2] : "";
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

    if (channel == NULL)
    {
        this->_clients[i - 1].sendMessage(":localhost 403 " + channelName + " :No such channel" + "\r\n");
        return;
    }

    if (mode.empty())
    {
        // Return current channel modes
        this->_clients[i - 1].sendMessage(":localhost 324 " + this->_clients[i - 1].getNickname() +
                                          " " + channelName + " " + channel->getMode("") + "\r\n"); //! check needed
        return;
    }

    if (!channel->isOperator(this->_clients[i - 1]))
    {
        this->_clients[i - 1].sendMessage(":localhost 482 " + channelName + " :You're not a channel operator" + "\r\n");
        return;
    }

    channel->setMode(mode, extra_cmd, _clients[i - 1]);
    this->_clients[i - 1].sendMessage("Mode updated successfully."); // keep?
}
