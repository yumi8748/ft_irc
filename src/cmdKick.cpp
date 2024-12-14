#include "../includes/Irc.hpp"

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
	Channel channel = findChannelByName(channelName);
	if (channel == Channel())
	{
		this->_clients[i - 1].sendMessage(":localhost 403 " + channelName + " :No such channel");
		return ;
	}
	Client operatorClient = _clients[i - 1];
	if (!channel.isOperator(operatorClient))
	{
		operatorClient.sendMessage(":localhost 482 " + channelName + " :You're not a channel operator");
        return ;
	}
	Client targetClient = findClientByNickname(targetNickname);
	if (targetClient == Client())
	{
		operatorClient.sendMessage(":localhost 401 " + targetNickname + " :No such nick/channel");
        return ;
	}
	if (!channel.isClientInChannel(targetClient))
	{
		operatorClient.sendMessage(":localhost 441 " + targetNickname + " " + channelName + " :They aren't on that channel");
        return ;
	}//for irssi
	std::string kickMessage = ":" + operatorClient.getNickname() + "!" + operatorClient.getUsername() + "@" + operatorClient.getHostname() +
							  " KICK " + channelName + " " + targetNickname + " :" + (reason.empty() ? "No reason" : reason);
	targetClient.sendMessage(kickMessage);

	channel.broadcastMessage(kickMessage, targetClient);

	channel.kickClient(targetClient);

	std::cout << "Client " << targetNickname << " has been kicked from channel " << channelName
			  << " by operator " << operatorClient.getNickname() << ". Reason: " << (reason.empty() ? "No reason" : reason) << std::endl;
}
