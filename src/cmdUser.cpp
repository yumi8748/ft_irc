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
	else if (this->_clients[i - 1].getIsLogged())
	{
		msg = ":localhost 462 " + this->_clients[i - 1].getNickname() + " :You may not reregister\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}
	int j = 0;
	while (string_array[1][j])
	{
		if (isalnum(string_array[1][j]) == 0)
		{
			msg = "Error : Wrong username format\r\n";
			send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
			return;
		}
		j++;
	}
	this->_clients[i - 1].setUsername(string_array[1]);
	msg = "Success : "  + this->_clients[i - 1].getUsername() +  " username is saved\r\n";
	send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
	checkRegistration(i);
}

// void	Server::cmdUser(int i, std::vector<std::string> string_array)
// {
// 	std::ostringstream str1;
// 	str1 << this->_clients[i - 1].getFd();
// 	std::string fd_string = str1.str();
// 	std::string msg;
// 	if (string_array.size() != 2)
// 	{
// 		msg = "[Error 461] client " + fd_string + " : Wrong number of parameters\n";
// 		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
// 		return;
// 	}
// 	else if (this->_clients[i - 1].getNickname().empty() == 0 && this->_clients[i - 1].getUsername().empty() == 0)
// 	{
// 		msg = "[Error 462] client " + fd_string + " : You may not reregister\n";
// 		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
// 		return;
// 	}
// 	int j = 0;
// 	while (string_array[1][j])
// 	{
// 		if (isalnum(string_array[1][j]) == 0)
// 		{
// 			msg = "[Error 432] client " + fd_string + " " + string_array[1] + " : Erroneus username\n";
// 			send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
// 			return;
// 		}
// 		j++;
// 	}
// 	this->_clients[i - 1].setUsername(string_array[1]);
// 	msg = "Success : Username is saved\n";
// 	send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
// }

