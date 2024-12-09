#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Channel.hpp"
#include "Commands.hpp"
#include "Server.hpp"
#include "Macros.hpp"

class Channel;

class Client{
    private:
        std::string nick;
		std::string oldnick;
        std::string usr;
        std::string _buffer; // take the message until \r\n
        std::string buffer; // take the message until \r\n
        std::vector<Channel*> channels;
        std::vector<Channel*> joinedChannels; // 用戶加入的頻道
        int client_fd;
        std::string recv_buf;

		int _passwordIsCorrect;
        int _isRegistered;
    public:
        Client(){};
        ~Client(){};
        Client(int fd);
        void setFd(int fd);
        void setNickname(const std::string& nickname);
        void setUsername(const std::string& username);
        void setIsLogged(void);
        int getIsLogged(void);
		void updateBuffer(const std::string);
		std::string getBuffer();
		void clearBuffer();
		void setOldNick(const std::string& username);
        void setIsRegistered(void);
        int getIsRegistered(void);
        const std::string& getNickname() const;
        const std::string& getUsername() const;
		const std::string& getOldNick() const;
        void sendMessage(const std::string &message);
        void addChannel(Channel* ch);
        void removeChannel(Channel* channel);
        void Send();
        int getFd() const;
        bool isInvited(Client* client, Channel* channel);
        
		void setPasswordIsCorrect(void);
		int getPasswordIsCorrect(void);
		void setBuffer(std::string);
		std::string &	getBuffer(void);
        // void Recv();

		std::string getName(void);
};

#endif
