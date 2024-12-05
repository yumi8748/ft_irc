#ifndef SERVER_HPP
#define SERVER_HPP
#include "Macros.hpp"
#include "Client.hpp"

class Channel;
class Client;

class Server{
    public:
        Server(){_fd = -1;};
        ~Server(){};

        // INITS:
        void InitServer(int, char*);
        void InitSockets();

        // FD:
        void CloseServer();
        void CloseClients(int);
        static void SigHandler(int);

        // CLIENT:
        void AcceptClient();
        void ReceiveData(int, int);
	    
        // GTRS/STRS
        std::vector<Channel *> getChannels();
        std::vector<Client> getClients();
        std::vector<struct pollfd> getfds(void);

        // METHODS:
        void 	CloseMessage(std::string);
        void	commandParsing(int i, std::string buffer);
	    void	cmdNick(int i, std::vector<std::string> string_array);
	    void	cmdUser(int i, std::vector<std::string> string_array);
	    void	cmdPass(int i, std::vector<std::string> string_array);
	    void	cmdQuit(int i, std::vector<std::string> string_array);
	    void	cmdPrivmsg(int, std::vector<std::string>, std::string);
	    void	cmdJoin(int i, std::vector<std::string> string_array);
	    void	cmdPart(int i, std::vector<std::string> string_array);
	    void	cmdKick(int i, std::vector<std::string> string_array);
	    void	cmdInvite(int i, std::vector<std::string> string_array);
	    void	cmdTopic(int i, std::vector<std::string> string_array);
	    void	cmdMode(int i, std::vector<std::string> string_array);
		int		isRegistered(int i);
        void	parseLine(std::string line, int i);
		void	checkRegistration(int i);
		int		nicknameUsed(std::string nickname);
        //YUMI
        bool isValidChannelName(const std::string& channelName);
        Channel* findChannelByName(const std::string& channelName);
        Client* findClientByNickname(const std::string& nickname);
        void setMode(const std::string& mode, const std::string& value);

    private:
        // VARIABLES:
        int _port;
        int _fd;
        std::string _pwd;
        static int _sig;

        // VECTORS:
        std::vector<struct pollfd> _fds;
        std::vector<Client> _clients;
        std::vector<Channel *> _channels;
};

#endif /*SERVER_HPP*/
