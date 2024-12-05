#include "../includes/Channel.hpp"

// Channel::Channel(const std::string& n, Server& srv): server(srv), name(n), userLimits(100), topic("")  // Server& srv?
// {
Channel::Channel(const std::string& n, Server& srv)
:server(srv), name(n), clients(), invitedClients()
, operators(), Ch_pwd(""), inviteOnly(false), topicRestricted(false)
, userLimits(100), topic(""), modes()
{

// }

// const std::string& Channel::getName() const
// {
//     return (name);
// }

// void Channel::addClient(Client* client)
// {
//     if (clients.size() >= static_cast<std::size_t>(userLimits))
//     {
//         std::cerr << "Channel is full! Cannot add client." << std::endl;
//         return ;
//     }
//     clients.push_back(client);
//     std::cerr << "Client" << client->getNickname() << " joined channel: " << name << std::endl;
// }

// void Channel::removeClient(Client* client)
// {
//     for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
//     {
//         if (*it == client)
//         {
//             clients.erase(it);
//             std::cout << "Client" << client->getNickname() << " left channel: " << name << std::endl;
//             break ;
//         }
//     }
//     std::cerr << "Client not found in the channel!" << std::endl;;
// }

<<<<<<< HEAD
// void Channel::broadcastMessage(std::string msg)
// {
//     for (size_t i = 0; i < clients.size(); ++i)
//     {
//         clients[i]->sendMessage(msg);
//     }
// }
=======
void Channel::broadcastMessage(std::string msg) //server msg
{
    for (size_t i = 0; i < clients.size(); ++i)
    {
        clients[i]->sendMessage(msg);
    }
}
>>>>>>> origin/yu-chen

// void Channel::changeCh_pwd(std::string newCh_pwd)
// {
//     Ch_pwd = newCh_pwd;
//     std::cout << "Channel password updated.\n";
// }

// bool Channel::isOperator(Client* client) const
// {
//     for (std::vector<Client*>::const_iterator it = operators.begin(); it != operators.end(); ++it)
//     {
//         if (*it == client)
//             return true;
//     }
//     return false;
// }

// void Channel::addOperator(Client* client)
// {
//     if (!isOperator(client))
//         operators.push_back(client);
// }

// void Channel::removeOperator(Client* client)
// {
//     for (std::vector<Client*>::iterator it = operators.begin(); it != operators.end(); ++it) {
//         if (*it == client) {
//             operators.erase(it);
//             break;
//         }
//     }
// }
void Channel::addOperator(Client* client)
{
    if (client == NULL)
    {
        std::cerr << "Error: Cannot add a null client as operator." << std::endl;
        return;
    }
    if (!isOperator(client))
    {
        operators.push_back(client);
        std::cout << "Added operator: " << client->getName() << std::endl;
    }
    else
    {
        std::cout << "Client is already an operator: " << client->getName() << std::endl;
    }
}
void Channel::removeOperator(Client* client)
{
    for (std::vector<Client*>::iterator it = operators.begin(); it != operators.end(); ++it) {
        if (*it == client) {
            operators.erase(it);
            break;
        }
    }
}

// void Channel::kickClient(Client* client, std::string &reason)
// {
//     std::vector<Client *>::iterator it = std::find(clients.begin(), clients.end(), client);
//     if (it != clients.end()) {
//         std::string kickMessage = client->getNickname() + " has been kicked from the channel " + name;
        
//         if (!reason.empty()) {
//             kickMessage += " (" + reason + ")";
//         }
//         broadcastMessage(kickMessage); // for other clients
//         clients.erase(it);
//         std::cout << "Client " << client->getNickname() << " has been kicked from channel " << name << "!" << std::endl;
//     } else {
//         std::cerr << "Client not found in channel " << name << "!" << std::endl;
//     }
// }

// void Channel::inviteClient(Client* client)
// {
//     if (std::find(clients.begin(), clients.end(), client) != clients.end())
//     {
//         std::cerr << "Error: You are already in the channel " << name << std::endl;
//         return ;
//     }
//     clients.push_back(client);
//     std::string inviteMessage = client->getNickname() + " has been invited to the channel.";
//     broadcastMessage(inviteMessage);
// }

// void Channel::addInvitedClient(Client* client) {
//     if (std::find(invitedClients.begin(), invitedClients.end(), client) == invitedClients.end()) {
//         invitedClients.push_back(client);
//     }
// }

// void Channel::removeInvitedClient(Client* client) {
//     std::vector<Client*>::iterator it = std::find(invitedClients.begin(), invitedClients.end(), client);
//     if (it != invitedClients.end()) {
//         invitedClients.erase(it);
//     }
// }

// const std::vector<Client*>& Channel::getInvitedClients() const {
//     return invitedClients;
// }

// void Channel::setTopic(const std::string& newTopic)
// {
//     topic = newTopic;
//     std::cout << "Channel topic updated to: " << topic << std::endl;
// }

// std::string Channel::getTopic() const
// {
//     return (topic);
// }

// int stringToInt(const std::string& value) {
//     std::istringstream iss(value);
//     int result;
//     if (!(iss >> result)) {
//         throw std::invalid_argument("Invalid integer value: " + value);
//     }
//     return result;
// }

// void Channel::setMode(const std::string& mode, const std::string& value)
// {
//     //!
//     if (mode == "i")
//     {
//         inviteOnly = (value == "1");
//         std::cout << "Invite-only mode " << (inviteOnly ? "enabled" : "disabled") << " in channel " << name << std::endl;
//     }
//     else if (mode == "t")
//     {
//         topicRestricted = (value == "1");
//         std::cout << "Topic-restricted mode " << (topicRestricted ? "enabled" : "disabled") << " in channel " << name << std::endl;
//     }
//     else if (mode == "k")
//     {
//         Ch_pwd = value;
//         std::cout << "Channel password set to '" << value << "' in channel " << name << std::endl;
//     }
//     else if (mode == "o")
//     {
//         Client* targetClient = server.findClientByNickname(value);
//         if (targetClient) {
//             operators.push_back(targetClient);
//             std::cout << "Client " << value << " granted operator privileges in channel " << name << std::endl;
//         } else {
//             std::cerr << "Error: Client " << value << " not found in channel " << name << std::endl;
//         }
//     }
//     else if (mode == "l")
//     {
//         userLimits = stringToInt(value);
//         std::cout << "User limit set to " << userLimits << " in channel " << name << std::endl;
//     }
//     else
//         std::cerr << "Error: Unknown mode '" << mode << "' in channel " << name << std::endl;
// }
std::string intToString(int num)
{
    if (num == 0) return "0";
    std::string result;
    bool isNegative = (num < 0);
    if (isNegative) num = -num;

    while (num > 0)
    {
        result.insert(result.begin(), '0' + (num % 10));
        num /= 10;
    }

    if (isNegative) result.insert(result.begin(), '-');
    return result;
}

void Channel::setMode(const std::string& modeStr, const std::string& value, Client* client)
{
    if (modeStr.empty())
    {
        client->sendMessage("Error: No mode specified for channel " + name);
        return;
    }

    char operation = modeStr[0]; // '+' or '-'
    if (operation != '+' && operation != '-')
    {
        client->sendMessage("Error: Mode must start with '+' or '-' in channel " + name);
        return;
    }

    // Apply modes one by one
    for (size_t i = 1; i < modeStr.size(); ++i)
    {
        char mode = modeStr[i];
        switch (mode)
        {
            case 'i': // Invite-only
                inviteOnly = (operation == '+');
                client->sendMessage("Invite-only mode " + std::string(inviteOnly ? "enabled" : "disabled") + " in channel " + name);
                break;

            case 't': // Topic restrictions
                topicRestricted = (operation == '+');
                client->sendMessage("Topic-restricted mode " + std::string(topicRestricted ? "enabled" : "disabled") + " in channel " + name);
                break;

            case 'k': // Channel password
                if (operation == '+')
                {
                    Ch_pwd = value;
                    client->sendMessage("Channel password set to '" + value + "' in channel " + name);
                }
                else
                {
                    Ch_pwd.clear();
                    client->sendMessage("Channel password removed in channel " + name);
                }
                break;

            case 'o': // Operator privileges
            {
                Client* targetClient = server.findClientByNickname(value);
                if (!targetClient)
                {
                    client->sendMessage("Error: Client " + value + " not found for mode 'o' in channel " + name);
                    break;
                }

                if (operation == '+')
                {
                    if (!isOperator(client))
                    {
                        client->sendMessage("Error: You are not an operator in this channel.");
                        break;
                    }

                    if (!isOperator(targetClient))
                    {
                        addOperator(targetClient);
                        client->sendMessage("Client " + value + " granted operator privileges in channel " + name);
                    }
                    else
                    {
                        client->sendMessage("Client " + value + " is already an operator in channel " + name);
                    }
                }
                else
                {
                    if (!isOperator(client))
                    {
                        client->sendMessage("Error: You are not an operator in this channel.");
                        break;
                    }

                    removeOperator(targetClient);
                    client->sendMessage("Client " + value + " operator privileges removed in channel " + name);
                }
                break;
            }

            case 'l': // User limit
                if (operation == '+')
                {
                    userLimits = stringToInt(value);
                    client->sendMessage("User limit set to " + intToString(userLimits) + " in channel " + name);
                }
                else
                {
                    userLimits = -1; // Remove limit
                    client->sendMessage("User limit removed in channel " + name);
                }
                break;

            default:
                client->sendMessage("Error: Unknown mode '" + std::string(1, mode) + "' in channel " + name);
                break;
        }
    }
}

// std::string Channel::getMode(const std::string& mode) const
// {
//     std::map<std::string, std::string>::const_iterator it = modes.find(mode);
//     if (it != modes.end())
//         return (it->second);
//     return ("");
// }

// bool Channel::isEmpty() const
// {
//     return clients.empty();
// }

// bool Channel::isClientInChannel(Client* client) const
// {
//     return std::find(clients.begin(), clients.end(), client) != clients.end();
// }

// void Channel::joinChannel(Client* client, const std::string& password)
// {
//     if (std::find(clients.begin(), clients.end(), client) != clients.end()) {
//         client->sendMessage("Error: You are already in the channel " + name);
//         return;
//     }
//     // check invite
//     if (inviteOnly) {
//         if (!client->isInvited(client, this))
//         {
//             client->sendMessage("Error: You need an invitation to join " + name);
//             return;
//         }
//     }
//     // check password
//     if (!Ch_pwd.empty() && Ch_pwd != password) {
//         client->sendMessage("Error: Incorrect password for channel " + name);
//         return;
//     }
//     // check amount of clients
//     if (clients.size() >= static_cast<std::size_t>(userLimits)) {
//         client->sendMessage("Error: Channel is full!");
//         return;
//     }
//     clients.push_back(client);
//     client->addChannel(this);
//     broadcastMessage(client->getNickname() + " has joined the channel " + name);
//     std::cout << "Client " << client->getNickname() << " successfully joined channel " << name << std::endl;
// }
void Channel::joinChannel(Client* client, const std::string& password)
{
    if (std::find(clients.begin(), clients.end(), client) != clients.end()) {
        client->sendMessage("Error: You are already in the channel " + name);
        return;
    }
    // check invite
    if (inviteOnly) {
        if (!client->isInvited(client, this))
        {
            client->sendMessage("Error: You need an invitation to join " + name);
            return;
        }
    }
    // check password
    if (!Ch_pwd.empty() && Ch_pwd != password) {
        client->sendMessage("Error: Incorrect password for channel " + name);
        return;
    }
    // check amount of clients
    if (clients.size() >= static_cast<std::size_t>(userLimits)) {
        client->sendMessage("Error: Channel is full!");
        return;
    }
    //first one in channel
    if (operators.empty()) {
        operators.push_back(client);
        client->sendMessage("You are now an operator in channel " + name);
    }
    clients.push_back(client);
    client->addChannel(this);
    broadcastMessage(client->getNickname() + " has joined the channel " + name);
    std::cout << "Client " << client->getNickname() << " successfully joined channel " << name << std::endl;
}


// void Channel::partChannel(Client* client, const std::string& message)
// {
//     std::vector<Client*>::iterator it = std::find(clients.begin(), clients.end(), client);
//     if (it == clients.end()) {
//         client->sendMessage("Error: You are not in the channel " + name);
//         return;
//     }
//     std::string partMessage = client->getNickname() + " has left the channel " + name;
//     if (!message.empty()) {
//         partMessage += " (" + message + ")";
//     }
//     broadcastMessage(partMessage);
//     clients.erase(it);
//     client->removeChannel(this);

//     std::cout << "Client " << client->getNickname() << " has left channel " << name << std::endl;
// }

// std::vector<Client*> Channel::getClients(void)
// {
// 	return this->clients;
// }
std::vector<Client*> Channel::getClients(void)
{
	return this->clients;
}
