#include "../includes/Commands.hpp"
#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"


Channel* findChannelByName(const std::vector<Channel*>& channels, const std::string& channelName) {
    for (std::vector<Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it) {
        if ((*it)->getName() == channelName) {
            return *it;
        }
    }
    return NULL;
}

Client* findClientByNickname(const std::vector<Client*>& clients, const std::string& nickname) {
    for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        if ((*it)->getNickname() == nickname)
            return *it;
    }
    return NULL;
}

void handleKickCmd(Client* operatorClient, const std::string& targetNickname, const std::string& channelName,
        const std::vector<Channel*>& channels, const std::vector<Client*>& clients)
{
    Channel* channel = findChannelByName(channels, channelName);
    if (!channel) {
        operatorClient->sendMessage("Error: Channel not found.");
        return;
    }
    if (!channel->isOperator(operatorClient)) {
        operatorClient->sendMessage("Error: You are not an operator in this channel.");
        return;
    }
    Client* targetClient = findClientByNickname(clients, targetNickname);
    if (!targetClient) {
        operatorClient->sendMessage("Error: Target client not found.");
        return;
    }
    channel->kickClient(targetClient);
    channel->broadcastMessage(targetNickname + " has been kicked from the channel.");
}

void handleInviteCmd(Client* operatorClient, const std::string& targetNickname, const std::string& channelName,
                    const std::vector<Channel*>& channels, const std::vector<Client*>& clients)
{
Channel* channel = findChannelByName(channels, channelName);
    if (!channel) {
        operatorClient->sendMessage("Error: Channel not found.");
        return;
    }
    if (!channel->isOperator(operatorClient)) {
        operatorClient->sendMessage("Error: You are not an operator in this channel.");
        return;
    }
    Client* targetClient = findClientByNickname(clients, targetNickname);
    if (!targetClient) {
        operatorClient->sendMessage("Error: Target client not found.");
        return;
    }
    channel->inviteClient(targetClient);
}


void handleTopicCmd(Client* operatorClient, const std::string& channelName, const std::string& newTopic,
                    const std::vector<Channel*>& channels)
{
    Channel* channel = findChannelByName(channels, channelName);
    if (!channel) {
        operatorClient->sendMessage("Error: Channel not found.");
        return;
    }
    if (!channel->isOperator(operatorClient)) {
        operatorClient->sendMessage("Error: You are not an operator in this channel.");
        return;
    }
    channel->setTopic(newTopic);
    operatorClient->sendMessage("Topic updated successfully.");
}

void handleModeCmd(Client* operatorClient, const std::string& channelName, const std::string& mode, const std::string& value,
                    const std::vector<Channel*>& channels)
{
    Channel* channel = findChannelByName(channels, channelName);
    if (!channel) {
        operatorClient->sendMessage("Error: Channel not found.");
        return;
    }
    if (!channel->isOperator(operatorClient)) {
        operatorClient->sendMessage("Error: You are not an operator in this channel.");
        return;
    }
    channel->setMode(mode, value);
    operatorClient->sendMessage("Mode updated successfully.");
}
