#include "../includes/Server.hpp"

// NEXT
// accept new clients
// receive data from clients

int Server::_sig = 0;

void Server::initServer(){
  initSockets();
  if (_fd > 0)
    std::cout<<PURPLE<<"Server["<<_fd<<"]"<<GREEN<<" connected successfully"<<RESET<<std::endl;
  else
    return;
  while (_sig == 0){
  if ((poll(&_fds[0], _fds.size(), -1) == -1) && _sig == 0)
    perror("poll");
  for (size_t i = 0; i < _fds.size(); i++){
    if (_fds[i] & POLLIN) // comparing bitmasks, POLLIN == 0001, if it is true, there is data to read
      acceptClient(); // ACCEPT CLIENT HERE? CHECK IF CLIENT? UNSURE
    //else
      // WHAT ELSE?
    }
  }
}

void Server::closeMessage(std::string errMsg){
  if (_fd > 0)
    close(_fd);
  _fd = -1;
  perror(RED + "ERROR\n" + YELLOW + errMsg + RESET);
  return;
}

void Server::initSockets(){
  // CREATING A SOCKET
  struct sockaddr_in addr;
  struct pollfd pollNew;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(_port);
  _fd = socket(AF_INET, SOCK_STREAM, 0);
  if (_fd < 0){
    perror("Socket error");
    return;}
    
  // SOCKET OPTIONS AND BIND
  int enable = 1;
  if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)); // speeds up restart times
    return closeMessage("setsockopt"); 
  if (bind(_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) // bind to address and port in addr
    return closeMessage("bind");
  if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0) // fd operations are returned instantly
    return closeMessage("fcntl");
  if (listen(_fd, SOMAXCONN) < 0)
    return closeMessage("listen");

  // ADDING TO THE POLL VECTOR
  pollNew.fd = _fd;
  pollNew.events = POLLIN;
  pollNew.revents = 0;
  _fds.push_back(pollNew); // add to the end of the vector
    
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
  // _sig = 1; This would stop the server from the while loop
}

void Server::acceptClient(){
  // CREATING A CLIENT SOCKET (ACCEPT)
  Client client;
  struct sockaddr_in addr;
  struct pollfd newPoll;
  socklen_t len = sizeof(addr);
  int lisFd = accept(_fd, (sockaddr *)&addr, &len);
  if (lisFd < 0)
    perror("listen");

  // ADDING TO THE POLL VECTOR
  newPoll.fd = lisFd;
  newPoll.events = POLLIN;
  newPoll.revents = 0;
  _fds.push_back(pollNew);
  // client.do_stuff?
  // _clients.push_back(client);
  // ADD TO CLIENT VECTOR?
}

void Server::receiveData(int fd){
  char buf[1024] = {0};
  ssize_t recData = recv(fd, buf, sizeof(buf) - 1, 0);
  if (recData < 0){
    closeClients(fd);
    close(fd);
    // maybe put close in closeClients?
  }
  else{
    buf[recData] = 0;
    // parse?
    std::cout << PURPLE << "Client["<<fd<<"]: "<< RESET << buf << std::endl; 
    // process?
  }
}

