#include "../includes/Irc.hpp"

void	Server::cmdPass(int i, std::vector<std::string> string_array)
{
	if (string_array.size() < 2)
	{
		std::string msg = ":localhost 461 " + this->_clients[i - 1].getNickname() + " PASS :Not enough parameters\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}
	else if (this->_clients[i - 1].getNickname().empty() == 0 && this->_clients[i - 1].getUsername().empty() == 0)
	{
		std::string msg = ":localhost 462 " + this->_clients[i - 1].getNickname() + " :You may not reregister\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}
	else if (string_array[1] != this->_pwd)
	{
		std::string msg = ":localhost 464 " + this->_clients[i - 1].getNickname() + " :Password incorrect\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}	
	else
	{
		this->_clients[i - 1].setPasswordIsCorrect();
		std::string msg = "Success : Password is correct\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
	}	
}
