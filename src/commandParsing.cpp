#include "../includes/Irc.hpp"

void	Server::commandParsing(int i, std::string buffer)
{
	std::string cmd_array[] = {
		"NICK\n",
		"PASS\n",
		"USER\n",
		"QUIT\n",
		"PRIVMSG\n",
		"JOIN\n",
		"PART\n",
		"KICK\n",
		"INVITE\n",
		"TOPIC\n",
		"MODE\n"
	};
	int len = sizeof(cmd_array) / sizeof(cmd_array[0]);
	int j = 0;
	while (j < len)
	{
		if (cmd_array[j] == buffer)
			break;
		j++;
	}
	if (j < len)
		std::cout << "cmd found" << std::endl;
	else
		std::cout << "other cmd : " << std::endl;
}
