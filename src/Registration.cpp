#include "../includes/Irc.hpp"

void	Server::checkRegistration(int i)
{
	std::string target = this->_clients[i - 1].getNickname();
	std::string sender = this->_clients[i - 1].getNickname() + "!" + this->_clients[i - 1].getUsername() + "@localhost";
	std::string messfinal;

	if (this->_clients[i - 1].getNickname() != "*"
	&& !this->_clients[i - 1].getUsername().empty()
	&& this->_clients[i - 1].getPasswordIsCorrect()
	&& !this->_clients[i - 1].getIsRegistered())
	{
		this->_clients[i - 1].setIsRegistered();
		messfinal = ":localhost 001 " + target + " Welcome to the Internet Relay Network " + sender +  "\r\n";
		send(this->_fds[i].fd, messfinal.c_str(), messfinal.size(), 0);	
	}
}

int	Server::checkStringFormat(std::string str)
{
	int j = 0;
	while (str[j])
	{
		if (isalnum(str[j]) == 0 && str[j] != '_' && str[j] != '-')
			return 1;
		j++;
	}
	return 0;
}

int		Server::isRegistered(int i)
{
	if (this->_clients[i - 1].getNickname().empty()
		|| this->_clients[i - 1].getUsername().empty()
		|| this->_clients[i - 1].getPasswordIsCorrect() == 0)
	{
		return (0);
	}
	else
	{
		return (1);
	}
}
