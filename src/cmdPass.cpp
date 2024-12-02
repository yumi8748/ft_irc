#include "../includes/Irc.hpp"

void	Client::setPasswordIsCorrect(void)
{
	this->_passwordIsCorrect = 1;
}

int	Client::getPasswordIsCorrect(void)
{
	return(this->_passwordIsCorrect);
}

void	Server::cmdPass(int i, std::vector<std::string> string_array)
{
	std::ostringstream str1;
	str1 << this->_clients[i - 1].getFd();
	std::string fd_string = str1.str();
	std::string msg;
	if (string_array.size() != 2)
	{
		msg = "[Error 461] client " + fd_string + " : Wrong number of parameters\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}
	else if (this->_clients[i - 1].getNickname().empty() == 0 && this->_clients[i - 1].getUsername().empty() == 0)
	{
		msg = "[Error 462] client " + fd_string + " : You may not reregister\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}
	else if (string_array[1] != this->_pwd)
	{
		msg = "[Error 464] client " + fd_string + " : Password incorrect\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}	
	std::cout << string_array[1] << std::endl;
	this->_clients[i - 1].setPasswordIsCorrect();
	msg = "Success : Password is correct\n";
	send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
}
