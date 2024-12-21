#include "../includes/Irc.hpp"

int		Server::cmdUserErrors(int i, std::vector<std::string> string_array)
{
	std::string msg;
	std::string target = this->_clients[i - 1].getNickname();
	if (this->_clients[i - 1].getPasswordIsCorrect() == 0)
	{
		msg = "Error : Need to enter correct password first\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return (1);
	}
	else if (string_array.size() < 2)
	{
		msg = ":localhost 461 " + target + " USER :Not enough parameters\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return (1);
	}
	else if (this->_clients[i - 1].getIsRegistered())
	{
		msg = ":localhost 462 " + target + " :Unauthorized command (already registered)\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return (1);
	}
	if (checkStringFormat(string_array[1]))
	{
		msg = "Error : Wrong username format\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return (1);
	}
	return (0);
}

void	Server::cmdUser(int i, std::vector<std::string> string_array)
{
	std::string msg;
	if (cmdUserErrors(i, string_array) == 1)
		return;
	this->_clients[i - 1].setUsername(string_array[1]);
	msg = "Success : "  + this->_clients[i - 1].getUsername() +  " username is saved\r\n";
	send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
	checkRegistration(i);
}

