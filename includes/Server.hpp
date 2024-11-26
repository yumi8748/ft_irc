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
        void SigHandler(int);

        // CLIENT:
        void AcceptClient();
        void ReceiveData(int);

        // METHODS:
        void CloseMessage(std::string);

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
