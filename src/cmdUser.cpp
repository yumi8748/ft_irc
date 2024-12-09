#include "../includes/Irc.hpp"

void	Server::cmdUser(int i, std::vector<std::string> string_array)
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
		msg = ":localhost 461 " + this->_clients[i - 1].getNickname() + " USER :Not enough parameters\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}
	else if (this->_clients[i - 1].getIsRegistered())
	{
		msg = ":localhost 462 " + this->_clients[i - 1].getNickname() + " :You may not reregister\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}

	if (checkStringFormat(string_array[1]))
	{
		msg = "Error : Wrong username format\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}

	this->_clients[i - 1].setUsername(string_array[1]);
	msg = "Success : "  + this->_clients[i - 1].getUsername() +  " username is saved\r\n";
	send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
	checkRegistration(i);
}
