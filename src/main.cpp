#include "../includes/Irc.hpp"
#include <poll.h>
#include <signal.h>

int signalisation = 0;

void	check_return_zero(std::string string, int return_value)
{
	if (return_value != 0)
        std::cout << "error : " << string << std::endl;
	else
		std::cout << "Success : " << string <<  std::endl;
}

void	check_return_fd(std::string string, int return_value)
{
	if (return_value == -1)
		std::cout << "error : " << string << "setup failed : " << return_value << std::endl;
	else
		std::cout << "Success : " << string << " fd is : " << return_value << std::endl;
}

void	sigint_not_blocking(int signal)
{
	if (signal == SIGINT)
	{
		std::cout << "OKK"<< std::endl;
		signalisation = 1;
	}
}

int main(void){
	
	// 0. signals
	signal(SIGINT, sigint_not_blocking);
	
	struct pollfd pfds[2];
	
	struct addrinfo hints;
	struct addrinfo *res;

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
	std::cout << std::endl;

	pfds[0].fd = socket_fd;
	pfds[0].events = POLLIN;

	int j = 0;
	while (signalisation == 0)
	{
		std::cout << "poll " << j << std::endl;
		std::cout << "before fds0 : " << pfds[0].revents << std::endl;
		std::cout << "before fds1 : " << pfds[1].revents << std::endl;
		poll(pfds, 2, 5000);
		std::cout << "after fds0 : " << pfds[0].revents << std::endl;
		std::cout << "after fds1 : " << pfds[1].revents << std::endl;

		int i = 0;
		while (i < 2)
		{
			std::cout << "\nenter loop fds " << i << std::endl;
			if (pfds[i].revents & POLLIN)
			{
				std::cout << "there is revents for : "<<i  << std::endl;
				if (pfds[i].fd == socket_fd)
				{
					std::cout << "it is server socker" << std::endl;
					struct sockaddr_storage client_addr;
					socklen_t addr_size;
					int	client_fd;
					addr_size = sizeof client_addr;
					client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &addr_size);
					check_return_fd("client socket", client_fd);
					pfds[1].fd = client_fd;
					pfds[1].events = POLLIN;
				}
				else
				{
					std::cout << "it is client socket" << std::endl;
					int bytes_read;
					char buffer[1024] = {0};
					bytes_read = recv(pfds[1].fd, buffer, sizeof(buffer), 0);
					if (bytes_read == 0) 
					{
						std::cout << "Client socket " << pfds[1].fd << " closed connection." << std::endl;
						signalisation = 1;
					}
					else {
						std::cout << "Client Message: " << buffer;
						const char *msg = "Received.\n";
						int msg_len = strlen(msg);
						int bytes_sent;
						bytes_sent = send(pfds[1].fd, msg, msg_len, 0);
						std::cout << "Server message to client socket " << pfds[1].fd << " : " << msg;
					}
				}
				
			}		
			else
				std::cout << "there no revents for : "<< i  << std::endl;
			i++;
		}
		std::cout << std::endl;
		j++;
	}

     if (pfds[1].fd)
	    close(pfds[1].fd);
    close(pfds[0].fd);
	std::cout << "Closed client and server socket" << std::endl;

	return 0;
}

