#include "../includes/Irc.hpp"

int Server::nicknameUsed(std::string nickname)
{
	for (size_t j = 0; j < this->_clients.size(); j++)
	{
		if (this->_clients[j].getNickname() == nickname)
			return 1;
	}
	return 0;
}

void	Server::cmdNick(int i, std::vector<std::string> string_array)
{
	std::string msg;
	if (this->_clients[i - 1].getPasswordIsCorrect() == 0)
	{
		msg = "Error : Need to enter correct password first\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}
	else if (string_array.size() < 2)
	{
		msg = ":localhost 431 " + this->_clients[i - 1].getNickname() + " :No nickname given\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}
	else if (nicknameUsed(string_array[1])){
		msg = ":localhost 433 " + string_array[1] + " " + string_array[1] + " :Nickname is already in use\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}
	
	if (checkStringFormat(string_array[1]))
	{
		msg = ":localhost 432 " + string_array[1] + " " + string_array[1] + " :Erroneus nickname\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}

	if (this->_clients[i - 1].getIsRegistered())
	{
		this->_clients[i - 1].setOldNick(this->_clients[i - 1].getNickname());
		this->_clients[i - 1].setNickname(string_array[1]);
		msg = ":"  + this->_clients[i - 1].getOldNick() + "!" + this->_clients[i - 1].getUsername() +  "@localhost NICK " + string_array[1] + "\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
	}
	else
	{
		this->_clients[i - 1].setNickname(string_array[1]);
		this->_clients[i - 1].setOldNick(string_array[1]);
		msg = ":"  + this->_clients[i - 1].getOldNick() + "!" + this->_clients[i - 1].getUsername() +  "@localhost NICK " + string_array[1] + "\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		checkRegistration(i);
	}
	
}
