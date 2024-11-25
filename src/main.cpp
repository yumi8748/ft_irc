#include "../includes/Macros.hpp"

int close_and_ret(std::string msg, int srv, int cli){
	if (srv > 0)
		close(srv);
	if (cli > 0)
		close(cli);
	std::cout << YELLOW << msg << RED << " error" << RESET << std::endl;
	return 1;
}

int main(int ac, char **av){
	// initial parsings
	/*if (ac != 3)
		return 1; // ? ./ircserv <port> <pwd>
*/
	// setting up socket
	(void)av;
	(void)ac;
	return 0;
}

