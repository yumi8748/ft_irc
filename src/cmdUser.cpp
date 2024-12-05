#include "../includes/Irc.hpp"

void	Server::cmdUser(int i, std::vector<std::string> string_array)
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
	int j = 0;
	while (string_array[1][j])
	{
		if (isalnum(string_array[1][j]) == 0)
		{
			msg = "[Error 432] client " + fd_string + " " + string_array[1] + " : Erroneus username\n";
			send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
			return;
		}
		j++;
	}
	if (string_array[1].empty())
	{
		msg = " client " + fd_string + " : No user name provided\n";
        send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
        return;
	}
	this->_clients[i - 1].setUsername(string_array[1]);
	// std::cout << "client user:" << _clients[i - 1].getUsername() << std::endl;
	msg = "Success : Username is saved\n";
	send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
}

