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
	if (nicknameUsed(string_array[1])){
		std::string messfinal = ":localhost 433 " + string_array[1] + " " + string_array[1] + " :Nickname is already in use.\r\n";
		send(this->_fds[i].fd, messfinal.c_str(), messfinal.size(), 0);	
		return;
	}
	else if (this->_clients[i - 1].getIsLogged())
	{
		this->_clients[i - 1].setOldNick(this->_clients[i - 1].getNickname());
		std::string msg = ":"  + this->_clients[i - 1].getOldNick() + "!" + this->_clients[i - 1].getUsername() +  "@localhost NICK " + string_array[1] + "\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		this->_clients[i - 1].setNickname(string_array[1]);
	}
	else
	{
		this->_clients[i - 1].setNickname(string_array[1]);
		this->_clients[i - 1].setOldNick(string_array[1]);
		std::string msg = ":"  + this->_clients[i - 1].getOldNick() + "!" + this->_clients[i - 1].getUsername() +  "@localhost NICK " + string_array[1] + "\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		checkRegistration(i);
	}
	
}

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
