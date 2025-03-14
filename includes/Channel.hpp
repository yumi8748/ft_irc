#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include "Macros.hpp"
#include "Server.hpp"

class Client;
class Server;

class Channel{
    private:
        // Server& server;
        std::string name;
        // std::vector<Client*> clients;
        // std::vector<Client*> invitedClients;
        // std::vector<Client*> operators;
        std::vector<Client> clients;
        std::vector<Client> invitedClients;
        std::vector<Client> operators;
        std::string Ch_pwd;
        bool inviteOnly;
        bool topicRestricted;
        int userLimits; // -1 for none
        std::string topic;
        std::map<std::string, std::string> modes;
    public:
        Channel(){};
        ~Channel(){};
		Channel(const std::string& n);
        bool operator==(const Channel& other) const
        {
            return this->name == other.name;
        }
        bool operator!=(const Channel& other) const
        {
            return !(*this == other);
        }
        // Channel(const Channel &src);
        // Channel &operator=(const Channel &src);
        // Channel(const std::string& n, Server& srv);
        const std::string& getName() const;
        void addClient(const Client &client);
        void removeClient(const Client &client);
        void broadcastMessage(const std::string& message);
        void broadcastMessage(const std::string &message, const Client &excludeClient);
        void changeCh_pwd(std::string newCh_pwd);
        //client join&leave channel(s)
        void cmdJoinExistingChannel(Client &client, const std::string& password);
		int  cmdJoinErrorsExistingChannel(Client &client, const std::string& password);
		int  cmdInviteErrorsOnChannel(Client &client, Client &targetclient);
		int  cmdKickErrorsNotOnChannel(Client &client, Client &targetclient);
        int		cmdModeErrorsOperator(int i, std::string nickOperator);
        void partChannel(Client &client, const std::string& reason, const std::string& channelname);
		void cmdJoinSend(Client &client);
        //channel operators commands:
        bool isOperator(const Client &client) const;
        void addOperator(const Client &client);
        void removeOperator(const Client &client);
        void kickClient(Client &client, const std::string& reason, const std::string& channelName, Client &clientKicked);
        void inviteClient(Client &client, Client &targetclient);
        // void inviteClient(Client &client);
        void addInvitedClient(const Client &client);
        void removeInvitedClient(const Client &client);
        bool isClientInChannel(const Client &client) const;
        bool isEmpty() const;
        const std::vector<Client>& getInvitedClients() const;
        void setTopic(const std::string& newTopic);
        std::string getTopic() const;
        void setMode(const std::string& modeStr, std::vector<std::string> string_array, const Client &client);
        std::string getMode(const std::string& mode) const;
        bool getTopicRestricted() const;
	void quitClient(Client &clientKicked, const std::string& reason);

		std::vector<Client> getClients(void);
};


#endif
