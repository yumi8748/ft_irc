#ifndef IRC_HPP
#define IRC_HPP
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#include <poll.h>
#include <signal.h>
#include <vector>
#include <algorithm>
#include <map>

// Recommended useful macros
#define user_id(nickname, username) (":" + nickname + "!" + username + "@localhost")
#define RPL_INVITE(user_id, invited, channel) (user_id + " INVITE " + invited + " #" + channel + "\r\n")
#define ERR_INVALIDMODEPARAM(client, channel, mode, password) ("696 " + client + " #" + channel + " " + mode + " " + password + " : password must only contained alphabetic character\r\n")

// Parsing / debugging / testing macros
#define HERE(event) std::cout<<YELLOW<<event<<GREEN<<" success"<<RESET<<std::endl;
#define NEWLINE std::cout<<std::endl;

//Base colors
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"

//Bold colors
#define BRED "\033[1;31m"
#define BGREEN "\033[1;32m"
#define BYELLOW "\033[1;33m"
#define BBLUE "\033[1;34m"
#define BPURPLE "\033[1;35m"

#define RESET "\033[0m"

class Server {


	public:

	Server(void);
	~Server(void);
	Server(Server const & src);
	Server & operator=(Server const & rhs);


	std::vector<struct pollfd> getfds(void);
	void	createServer(void);
	void	serverListen(void);
	void	acceptNewClients(void);
	void	receiveExistingClients(int i);

	void	commandParsing(int i, std::string buffer);

	private:

	std::vector<struct pollfd> 	pfds;

};

void	check_return_zero(std::string string, int return_value);
void	check_return_fd(std::string string, int return_value);

extern int signalisation;

#endif /*IRC_HPP*/
