#include "../includes/Irc.hpp"

int		Server::cmdPassErrors(int i, std::vector<std::string> string_array)
{
	std::string msg;
	std::string target = this->_clients[i - 1].getNickname();
	if (string_array.size() < 2)
	{
		msg = ":localhost 461 " + target + " PASS :Not enough parameters\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return (1);
	}
	else if (this->_clients[i - 1].getNickname().empty() == 0 && this->_clients[i - 1].getUsername().empty() == 0)
	{
		msg = ":localhost 462 " + target + " :Unauthorized command (already registered)\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return (1);
	}
	else if (string_array[1] != this->_pwd)
	{
		msg = ":localhost 464 " + target + " :Password incorrect\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return (1);
	}	
	return (0);
}

void	Server::cmdPass(int i, std::vector<std::string> string_array)
{
	if (cmdPassErrors(i, string_array) == 1)
		return;
	else
	{
		this->_clients[i - 1].setPasswordIsCorrect();
		std::string msg = "Success : Password is correct\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
	}	
}
