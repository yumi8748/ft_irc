#include "../includes/Irc.hpp"

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
	Channel channel = findChannelByName(channelName);
	if (channel == Channel())
	{
		this->_clients[i - 1].sendMessage(":localhost 403 " + channelName + " :No such channel\n");
		return ;
	}
	if (!channel.isClientInChannel(_clients[i - 1]))
	{
		this->_clients[i - 1].sendMessage(":localhost 442 " + channelName + " :You are not on that channel\n");
		return ;
	}
	channel.partChannel(_clients[i], reason); //handle in channel
	if (channel.isEmpty())
	{
		for (std::vector<Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); ) 
		{
    		if ((*it) == channel)
			{
        		it = this->_channels.erase(it);
        		break;
    		}
			else
				++it;
		}
	}
	std::cout << "Client[" << i - 1 << "] parted from channel " << channelName << std::endl;
}
