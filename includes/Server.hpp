#ifndef SERVER_HPP
#define SERVER_HPP
#include "Macros.hpp"

// prototype for compilation
class Client{
    public:
        Client(){};
        ~Client(){};
        int getFd(){return 1;};
};

class Server{
    public:
        Server(){_fd = -1; _port = 6667;};
        ~Server(){};

        // INITS:
        void initServer();
        void initSockets();

        // FD:
        void closeServer();
        void closeClients(int);
        void sigHandler(int);

        // CLIENT:
        void acceptClient();
        void receiveData(int);

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
