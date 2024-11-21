#include "../includes/Irc.hpp"

void	sigint_not_blocking(int signal)
{
	if (signal == SIGINT)
	{
		std::cout << "OKK"<< std::endl;
		signalisation = 1;
	}
}

int main(void){
	
	Server serverObject;

	signal(SIGINT, sigint_not_blocking);
	
	serverObject.createServer();
	serverObject.serverListen();

    if (serverObject.getfds().size() > 1)
	{	
		int i = 0;
		while (i < static_cast<int>(serverObject.getfds().size()))
		{
			close(serverObject.getfds()[i].fd);
			i++;
		}
	}
	std::cout << "Closed client and server socket" << std::endl;

	return 0;
}

