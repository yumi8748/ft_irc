#include "../includes/Irc.hpp"

void handleClientCommand(Client* client, const std::string& cmd, const std::vector<std::string>& args, 
                         const std::vector<Channel*>& channels, const std::vector<Client*>& clients)
{
    if (cmd == "KICK" && args.size() >= 2) {
        handleKickCmd(client, args[0], args[1], channels, clients);
    } else if (cmd == "INVITE" && args.size() >= 2) {
        handleInviteCmd(client, args[0], args[1], channels, clients);
    } else if (cmd == "TOPIC" && args.size() >= 2) {
        handleTopicCmd(client, args[0], args[1], channels);
    } else if (cmd == "MODE" && args.size() >= 3) {
        handleModeCmd(client, args[0], args[1], args[2], channels);
    } else {
        client->sendMessage("Error: Invalid command or arguments.");
    }
}

int close_and_ret(std::string msg, int srv, int cli){
	if (srv > 0)
		close(srv);
	if (cli > 0)
		close(cli);
	std::cout << YELLOW << msg << RED << " error" << RESET << std::endl;
	return 1;
}

// void parseCommand(const std::string& input, std::string& cmd, std::vector<std::string>& args) {
//     std::istringstream stream(input);
//     stream >> cmd; // get cmds
//     std::string arg;
//     while (stream >> arg) {
// 		if (isValidCmd(args)) // get parameters
//         	args.push_back(arg);
// 		std::cou
//     }
// }



int main(int ac, char **av){
	if (ac != 3)
		return 1; // handle return msg
	Server server;
	try {
    signal(SIGINT, Server::SigHandler);
    signal(SIGQUIT, Server::SigHandler);
		server.InitServer();
	}
	catch (ErrThrow &e){
		server.CloseServer();
		std::cout<<BRED<<"Server error: "<<YELLOW<<e.what()<<RESET<<std::endl;
	}
	(void)av;
	(void)ac;
	return 0;
}

