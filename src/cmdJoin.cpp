#include "../includes/Irc.hpp"

// void	Server::cmdJoin(int i, std::vector<std::string> string_array)
// {
// 	// if (this->isRegistered(i) == 0)
// 	// {
// 	// 	std::cout << "Client not registered"  << std::endl;
// 	// 	return ;
// 	// }
// 	for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
// 	{
// 		if ((*it).getName() == string_array[1])
// 		{
// 			(*it).addClient(&this->_clients[i - 1]);
// 			int k = 0;
// 			while (k < static_cast<int>((*it).getClients().size()))
// 			{
// 				std::string msg = ":" + this->_clients[i - 1].getNickname() + " JOIN " + string_array[1] + "\r\n";
// 				std::cout  << msg << std::endl;
// 				send(getClients()[k].getFd(), msg.c_str(), msg.length(), 0);

// 				std::string msg2 = ":localhost 353 " + this->_clients[i - 1].getNickname() + " = " + string_array[1] + " :@" ;
// 				int j = 0;
// 				while (j < static_cast<int>((*it).getClients().size()))
// 				{
// 					if (msg2[msg2.size()] != '@')
// 						msg2 = msg2 + " ";
// 					msg2 = msg2 + (*it).getClients()[j]->getNickname();
// 					j++;
// 				}
// 				msg2 = msg2 + "\r\n"; 
// 				std::cout  << msg2 << std::endl;
// 				send(getClients()[k].getFd(), msg2.c_str(), msg2.length(), 0);

// 				std::string msg3 = ":localhost 366 " + this->_clients[i - 1].getNickname() + " " + string_array[1] + " :End of /NAMES list.\r\n";
// 				std::cout  << msg3 << std::endl;
// 				send(getClients()[k].getFd(), msg3.c_str(), msg3.length(), 0);

// 				k++;
// 			}

// 			return;
// 		}
// 	}
// 	Channel newchannel(string_array[1].c_str());
// 	newchannel.addClient(&this->_clients[i - 1]);
// 	_channels.push_back(newchannel);

// 	std::string msg = ":" + this->_clients[i - 1].getNickname() + " JOIN " + string_array[1] + "\r\n";
// 	std::cout  << msg << std::endl;
// 	send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);

// 	std::string msg2 = ":localhost 353 " + this->_clients[i - 1].getNickname() + " = " + string_array[1] + " :@" + this->_clients[i - 1].getNickname() + "\r\n";
// 	std::cout  << msg2 << std::endl;
// 	send(this->_clients[i - 1].getFd(), msg2.c_str(), msg2.length(), 0);

// 	std::string msg3 = ":localhost 366 " + this->_clients[i - 1].getNickname() + " " + string_array[1] + " :End of /NAMES list\r\n";
// 	std::cout  << msg3 << std::endl;
// 	send(this->_clients[i - 1].getFd(), msg3.c_str(), msg3.length(), 0);
// }
