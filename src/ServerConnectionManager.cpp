#include <Server.hpp>

void Server::addNewClient(void) {
	LOGGER.info("addNewClient", "Adding new client...");
	if (pollFds.size() >= MAX_CLIENTS) {
		LOGGER.error("addNewClient", "The maximum number of clients connected to the server has been reached. The connection will be rejected");
		return;
	}
	struct sockaddr_in	clientAddr;
	int	addrlen = sizeof(clientAddr);
	LOGGER.info("addNewClient", "Accepting connection...");
	int fd = accept(socket, (struct sockaddr *)&clientAddr, (socklen_t *)&addrlen);
	if (fd < 0) {
		LOGGER.error("addNewClient", "Failed to accept connection");
		return;
	}
	Client client(fd, inet_ntoa(clientAddr.sin_addr));
	clients.insert(std::pair<int, Client>(fd, client));
	pollfd pollFd = {fd, POLLIN | POLLOUT | POLLERR | POLLHUP | POLLNVAL, 0};
	pollFds.push_back(pollFd);
	LOGGER.info("addNewClient", "New connection established with " + client.getHostname() + " on fd " + std::to_string(fd));
}

void Server::unexpectedDisconnectHandling(int fd) {
	Client		   		&client = clients.at(fd);
	std::stringstream	ss;

	LOGGER.info("unexpectedDisconnectHandling", "Handling unexpected disconnection...");
	if (client.getRegistration() == (NICK_FLAG | USER_FLAG | PASS_FLAG)) {
		ss << ":" << client.getNickname();
		ss << "!" << client.getUsername();
		ss << "@" << client.getHostname();
		ss << " QUIT: Client exited unexpectedly";
		ss << "\r\n";

		broadcastMessage(fd, ss.str());
	}
	client.setToDisconnect(true);
}

void Server::ejectClient(int clientFd, int reason) {
	std::vector<pollfd>::iterator it = pollFds.begin();

	LOGGER.info("ejectClient", "Ejecting client on fd " + std::to_string(clientFd));
	for (; it < pollFds.end(); it++) {
		if ((*it).fd == clientFd) {
			close(clientFd);
			std::memset(&(*it), 0, sizeof(pollfd));
			pollFds.erase(it);
			break;
		}
	}
	clients.erase(clientFd);

	switch (reason) {
		case LOSTCONNECTION:
			LOGGER.info("ejectClient", "Client connection lost. (fd : " + std::to_string(clientFd) + ")");
			break;
		case QUITED:
			LOGGER.info("ejectClient", "Client left. (fd : " + std::to_string(clientFd) + ")");
			break;
		case KICKED:
			LOGGER.info("ejectClient", "Client kicked. (fd : " + std::to_string(clientFd) + ")");
			break;
		default:
			LOGGER.info("ejectClient", "Client successfully ejected. (fd : " + std::to_string(clientFd) + ")");
	}
}

void Server::removeChannel(std::string name) { channels.erase(name); }