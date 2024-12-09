#include "../includes/Irc.hpp"

void	Server::cmdCap(int i)
{
	std::string msg = "CAP * LS\r\n";
	send(this->_fds[i].fd, msg.c_str(), msg.length(), 0);
}
