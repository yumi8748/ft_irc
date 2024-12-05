// #include "../includes/Irc.hpp"

// void	Server::cmdNick(int i, std::vector<std::string> string_array)
// {
// 	std::ostringstream str1;
// 	str1 << this->_clients[i - 1].getFd();
// 	std::string fd_string = str1.str();
// 	std::string msg;
// 	if (string_array.size() != 2)
// 	{
// 		msg = "[Error 431] client " + fd_string + " : Wrong number of parameters\n";
// 		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
// 		return;
// 	}
// 	for (std::vector<Client>::iterator it = this->_clients.begin(); it != _clients.end(); it++)
// 	{
// 		if ((*it).getNickname() == string_array[1])
// 		{
// 			msg = "[Error 433] client " + fd_string + " " + string_array[1] + " : Nickname is already in use\n";
// 			send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
// 			return;
// 		}
// 	}
// 	int j = 0;
// 	while (string_array[1][j])
// 	{
// 		if (isalnum(string_array[1][j]) == 0)
// 		{
// 			msg = "[Error 432] client " + fd_string + " " + string_array[1] + " : Erroneus nickname\n";
// 			send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
// 			return;
// 		}
// 		j++;
// 	}
// 	this->_clients[i - 1].setNickname(string_array[1]);
// 	msg = "Success : Nickname is saved\n";
// 	send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
// }
#include "../includes/Irc.hpp"

void	Server::cmdNick(int i, std::vector<std::string> string_array)
{
	std::ostringstream str1;
	str1 << this->_clients[i - 1].getFd();
	std::string fd_string = str1.str();
	std::string msg;
	if (string_array[1].empty())
	{
		msg = "client " + fd_string + " : No nick name provided\n";
        send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
        return;
	}
	if (string_array.size() != 2)
	{
		msg = "[Error 431] client " + fd_string + " : Wrong number of parameters\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}
	for (std::vector<Client>::iterator it = this->_clients.begin(); it != _clients.end(); it++)
	{
		if ((*it).getNickname() == string_array[1])
		{
			msg = "[Error 433] client " + fd_string + " " + string_array[1] + " : Nickname is already in use\n";
			send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
			return;
		}
	}
	int j = 0;
	while (string_array[1][j])
	{
		if (isalnum(string_array[1][j]) == 0)
		{
			msg = "[Error 432] client " + fd_string + " " + string_array[1] + " : Erroneus nickname\n";
			send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
			return;
		}
		j++;
	}
	this->_clients[i - 1].setNickname(string_array[1]);
	// std::cout << "client user:" << _clients[i - 1].getNickname() << std::endl;
	msg = "Success : Nickname is saved\n";
	send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
}
