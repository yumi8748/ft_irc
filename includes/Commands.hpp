#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include "Macros.hpp"

class Channel;
class Client;

Channel* findChannelByName(const std::vector<Channel*>& channels, const std::string& channelName);
Client* findClientByNickname(const std::vector<Client*>& clients, const std::string& nickname);

void handleKickCmd(Client* operatorClient, const std::string& targetNickname, const std::string& channelName, const std::vector<Channel*>& channels, const std::vector<Client*>& clients);
void handleInviteCmd(Client* operatorClient, const std::string& targetNickname, const std::string& channelName, const std::vector<Channel*>& channels, const std::vector<Client*>& clients);
void handleTopicCmd(Client* operatorClient, const std::string& channelName, const std::string& newTopic, const std::vector<Channel*>& channels);
void handleModeCmd(Client* operatorClient, const std::string& channelName, const std::string& mode, const std::string& value, const std::vector<Channel*>& channels);
void handleJoinCmd(Client* client, const std::string& channelName, const std::string& password, std::vector<Channel*>& channels);
void handlePartCmd(Client* client, const std::string& channelName, const std::string& message, std::vector<Channel*>& channels);


#endif
