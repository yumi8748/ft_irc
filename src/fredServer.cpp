#include "../includes/Irc.hpp"

int signalisation = 0;

Server::Server(void)
{
	// std::cout << "Server Constructor" << std::endl;
}

Server::~Server(void)
{
	// std::cout << "Server Destructor" << std::endl;
}
Server::Server(Server const & src)
{
	std::cout << "Server Copy constructor" << std::endl;
	*this = src;
}

Server & Server::operator=(Server const & rhs)
{
	std::cout << "Server Copy assignment operator" << std::endl;
	(void)rhs;
	return *this;
}

std::vector<struct pollfd> Server::getfds(void)
{
	return this->pfds;
}

void	check_return_zero(std::string string, int return_value)
{
	if (return_value != 0)
        std::cout << "error : " << string;
	// else
		// std::cout << " | Success : " << string;
}

void	check_return_fd(std::string string, int return_value)
{
	if (return_value == -1)
		std::cout << "error : " << string << "setup failed : " << return_value;
	// else
		// std::cout << " | Success : " << string << " fd is : " << return_value;
}

void	Server::createServer(void)
{

	struct addrinfo hints;
	struct addrinfo *res;
	struct pollfd NewPoll;

	// Server Socket
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;        // IPv4 ou IPv6, indifférent
    hints.ai_socktype = SOCK_STREAM;    // Connexion TCP
    hints.ai_flags = AI_PASSIVE;		// Remplit l'IP automatiquement
	check_return_zero("getaddrinfo", getaddrinfo(NULL, "6667", &hints, &res));
	int socket_fd;
	socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	check_return_fd("server socket", socket_fd);
	check_return_zero("bind", bind(socket_fd, res->ai_addr, res->ai_addrlen));
	check_return_zero("listen", listen(socket_fd, 5));
	// std::cout << std::endl;

	NewPoll.fd = socket_fd;
	NewPoll.events = POLLIN;
	NewPoll.revents = 0;
	(this->pfds).push_back(NewPoll);
}



void	Server::acceptNewClients(void)
{
	Client client;
	struct pollfd NewPoll;
	// std::cout << "[server socket]";
	struct sockaddr_storage client_addr;
	socklen_t addr_size;
	int	client_fd;
	addr_size = sizeof client_addr;
	client_fd = accept(this->pfds[0].fd, (struct sockaddr *)&client_addr, &addr_size);
	check_return_fd("client socket", client_fd);
	NewPoll.fd = client_fd;
	NewPoll.events = POLLIN;
	NewPoll.revents = 0;
	this->pfds.push_back(NewPoll);
	client.setFd(client_fd);
	this->_clients.push_back(client);
}

void	Server::receiveExistingClients(int i)
{
	// std::cout << "[client socket]";
	int bytes_read;
	char buffer[1024] = {0};
	bytes_read = recv(this->pfds[i].fd, buffer, sizeof(buffer), 0);
	// std::cout << "ici : " << buffer[bytes_read - 1] << std::endl;
	std::string str(buffer);
	if (bytes_read == 0) 
	{
		std::cout << "Client socket " << this->pfds[i].fd << " closed connection." << std::endl;
		signalisation = 1;
	}
	else {
		buffer[bytes_read - 1] = '\0';
		this->commandParsing(i, buffer);
		// std::cout << " | Client Message: " << buffer;
		// const char *msg = "Received.\n";
		// int msg_len = strlen(msg);
		// int bytes_sent;
		// bytes_sent = send(this->pfds[i].fd, msg, msg_len, 0);
		// std::cout << "Server message to client socket " << this->pfds[i].fd << " : " << msg;
	}
}

void myfunction (struct pollfd i)
{
  std::cout << i.revents << " ";
}

void	Server::serverListen(void)
{

	int j = 0;
	while (signalisation == 0)
	{
		// std::cout << "\npoll " << j ;
		// std::cout << " | before : " ;
		// for_each (this->pfds.begin(), this->pfds.end(), myfunction);
		poll(&this->pfds[0], static_cast<int>(this->pfds.size()), 5000);
		// std::cout << " | after : " ;
		// for_each (this->pfds.begin(), this->pfds.end(), myfunction);

		int i = 0;
		while (i < static_cast<int>(this->pfds.size()))
		{
			// std::cout << "\nloop fds " << i << " : ";
			if (this->pfds[i].revents & POLLIN)
			{
				// std::cout << "yes";
				if (this->pfds[i].fd == this->pfds[0].fd)
					acceptNewClients();
				else
					receiveExistingClients(i);
				
			}		
			// else
				// std::cout << "no";
			i++;
		}
		// std::cout << std::endl;
		j++;
	}
}

void	Client::setFd(int i)
{
	this->fd = i;
}

int		Client::getFd(void)
{
	return (this->fd);
}
