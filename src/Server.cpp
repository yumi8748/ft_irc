#include "../includes/Server.hpp"

int Server::_sig = 0;

void Server::initServer(){
    initSockets();
    // check it worked inside or here?
    std::cout<<PURPLE<<"Server["<<_fd<<"]"<<GREEN<<" connected successfully"<<RESET<<std::endl;
    // accept?
    // settup poll loop, revents, POLLIN check for _fds.fd == _fd
    // while (Server::_sig == 0)
}

void Server::initSockets(){
    // CREATING A SOCKET
    struct sockaddr_in addr;
    struct pollfd pollNew;
    memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = _port;
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0)
        // error
    HERE("socket")
    // int option = 1;
    // if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)));
        // error
    // if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
        // error 
    if (bind(_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        // error
    HERE("bind")
    if (listen(_fd, SOMAXCONN) < 0)
        // error
    HERE("listen")

    // ADDING TO THE POLL VECTOR
    pollNew.fd = _fd;
    pollNew.events = POLLIN;
    pollNew.revents = 0;
    _fds.push_back(pollNew);
    
}

void Server::closeServer(){
    for (size_t i = 0; i < _clients.size(); i++){
        std::cout<<PURPLE<<"Client["<<_clients[i].getFd()<<"]"<<RED<<" has disconnected"<<RESET<<std::endl;
        close(_clients[i].getFd());
    }
    if (_fd > 0){
        std::cout<<PURPLE<<"Server["<<_fd<<"]"<<RED<<" has disconnected"<<RESET<<std::endl;
        close(_fd);
    }
}

void Server::closeClients(int fd){
    for (size_t i = 0; i < _fds.size(); i++){
        if (_fds[i].fd == fd){_fds.erase(_fds.begin() + i); break;}}
    for (size_t i = 0; i < _clients.size(); i++){
        if (_clients[i].getFd() == fd){_clients.erase(_clients.begin() + i); break;}}
}

void Server::sigHandler(int signum){
    (void)signum;
    // _sig = 1;
}

void Server::acceptClient(){

}

void Server::receiveData(int fd){
    (void)fd;
}
