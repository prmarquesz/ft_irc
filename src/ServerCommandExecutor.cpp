#include <Server.hpp>

void Server::executeCommands(Client &client, std::vector<Command> &commands) {
	LOGGER.info("executeCommands", "Executing commands");
	std::string nick = "(unidentified)";
	std::vector<Command>::iterator it = commands.begin();
	for (; it < commands.end(); it++) {
		if (client.getNickname().size() > 0) {
			nick = client.getNickname();
		}
		std::ostringstream logMessage;
		logMessage << "Client " << nick << " on fd " << client.getFd() << " sent: " << (*it).cmd ;
		LOGGER.info("executeCommands", logMessage.str());
		executeCommand(client, (*it));
	}
}

void Server::executeCommand(Client &client, Command &command) {
	if (command.cmd == "NICK") { 		// dont update on front
		nick(client, command);
	} else if (command.cmd == "PASS") { // ok cmd
		pass(client, command);
	} else if (command.cmd == "USER") { // ok cmd
		user(client, command);
	} else if (command.cmd == "OPER") {
		oper(client, command);
	} else if (command.cmd == "JOIN") { // ok
		join(client, command);
	} else if (command.cmd == "QUIT") { // ok
		quit(client, command);
	} else if (command.cmd == "PING") {
		ping(client, command);
	} else if (command.cmd == "MODE") {
		mode(client, command);
	} else if (command.cmd == "PRIVMSG") {
		privmsg(client, command);
	} else if (command.cmd == "TOPIC") { // ok
		topic(client, command);
	} else if (command.cmd == "NOTICE") {
		notice(client, command);
	} else if (command.cmd == "PART") {
		part(client, command);
	} else if (command.cmd == "WHO") { // segfault
		who(client, command);
	} else if (command.cmd == "WHOIS") {
		whois(client, command);
	} else if (command.cmd == "KICK") { // segfault
		kick(client, command);
	} else if (command.cmd == "INVITE") { // ok
		invite(client, command);
	} else {
		client.setSendData(unknowncommand(client, command.cmd));
	}

	if (client.getRegistration() == (PASS_FLAG | USER_FLAG | NICK_FLAG)
		&& !client.getWelcome()) {
		client.setWelcome(true);
		client.setSendData(welcome(client));
		client.setSendData(motd(client));
	}
}