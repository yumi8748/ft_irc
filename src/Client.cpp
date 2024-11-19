#include "../includes/Client.hpp"

Client::Client(int fd): socket_fd(fd)
{
    nick = "";
    usr = "";
    recv_buf = "";
}

void Client::setNickname(const std::string& nickname)
{
    nick = nickname;
}

void Client::setUsername(const std::string& username)
{
    usr = username;
}

const std::string& Client::getNickname() const
{
    return (nick);
}

void Client::sendMessage(std::string &message)
{
    if (send(socket_fd, message.c_str(), message.length(), 0) == -1)
        perror("send");
}

void Client::addChannel(Channel* ch)
{
    channel.push_back(ch);
}

void Client::removeChannel(Channel* ch)
{
    for (std::vector<Channel *>::iterator it = channel.begin(); it != channel.end(); ++it)
    {
        if (*it == ch)
        {
            channel.erase(it);
            break ;
        }
    }
}

void Client::Recv()
{
    char buf[1024];
    int bytes_received = recv(socket_fd, buf, sizeof(buf) - 1, 0);
    if (bytes_received > 0)
    {
        buf[bytes_received] = '\0';
        recv_buf += buf;
    }
    else if (bytes_received == 0)
    {
        close(socket_fd);
        socket_fd = -1;
    }
    else
        perror("recv");
}

void Client::Send()
{
    if (!recv_buf.empty())
    {
        int bytes_sent = send(socket_fd, recv_buf.c_str(), recv_buf.length(), 0);
        if (bytes_sent > 0)
            recv_buf.erase(bytes_sent); //Erases part of the string
        else
            perror("send");
    }
}
