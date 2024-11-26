#ifndef SERVER_HPP
#define SERVER_HPP
#include "Macros.hpp"
#include "Client.hpp"

class Server{
    public:
        Server(){_fd = -1; _port = 6667;};
        ~Server(){};

        // INITS:
        void InitServer();
        void InitSockets();

        // FD:
        void CloseServer();
        void CloseClients(int);
        static void SigHandler(int);

        // CLIENT:
        void AcceptClient();
        void ReceiveData(int, int);
	    
        // GTRS/STRS
        std::vector<struct pollfd> getfds(void);

        // METHODS:
        void CloseMessage(std::string);
        void	commandParsing(int i, std::string buffer);
	    void	setNickname(int i, std::string buffer);
	    void	setUsername(int i, std::string buffer);
	    void	checkPassword(int i, std::string buffer);
	    void	quitServer(int i, std::string buffer);
	    void	privateMessage(int i, std::string buffer);
	    void	joinChannel(int i, std::string buffer);
	    void	partChannel(int i, std::string buffer);
	    void	kickChannel(int i, std::string buffer);
	    void	inviteChannel(int i, std::string buffer);
	    void	topicChannel(int i, std::string buffer);
	    void	modeChannel(int i, std::string buffer);

    private:
        // VARIABLES:
        int _port;
        int _fd;
        static int _sig;

        // VECTORS:
        std::vector<struct pollfd> _fds;
        std::vector<Client> _clients;
};

#endif /*SERVER_HPP*/
