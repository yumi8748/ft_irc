#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include "Client.hpp"
#include "Channel.hpp"

Channel* findChannelByName(const std::vector<Channel*>& channels, const std::string& channelName);
Client* findClientByNickname(const std::vector<Client*>& clients, const std::string& nickname);

void handleKickCmd(Client* operatorClient, const std::string& targetNickname, const std::string& channelName);
void handleInviteCmd(Client* operatorClient, const std::string& targetNickname, const std::string& channelName);
void handleTopicCmd(Client* operatorClient, const std::string& channelName, const std::string& newTopic);
void handleModeCmd(Client* operatorClient, const std::string& channelName, const std::string& mode, const std::string& value);

#endif