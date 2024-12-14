#include "../includes/Irc.hpp"

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

    Channel channel = findChannelByName(channelName);
    if (channel == Channel())
    {
        // this->_clients[i - 1].sendMessage(":localhost 403 " + channelName + " :No such channel");
        return;
    }

    if (mode.empty())
    {
        // Return current channel modes
        this->_clients[i - 1].sendMessage(":localhost 324 " + this->_clients[i - 1].getNickname() +
                                          " " + channelName + " " + channel.getMode("")); //! check needed
        return;
    }

    if (!channel.isOperator(this->_clients[i - 1]))
    {
        this->_clients[i - 1].sendMessage(":localhost 482 " + channelName + " :You're not a channel operator");
        return;
    }

    channel.setMode(mode, extra_cmd, _clients[i - 1]);
    this->_clients[i - 1].sendMessage("Mode updated successfully."); // keep?
}
