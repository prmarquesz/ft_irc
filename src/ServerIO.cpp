#include <Server.hpp>

std::string Server::receiveDataFromSocket(int fd) {
	char	buffer[BUFFER_SIZE];
	ssize_t bytesReceived;
	bool	keepReading = true;
	std::string bufferStr = "";

	LOGGER.info("receiveDataFromSocket", "Receiving data from fd " + std::to_string(fd));
	while (keepReading) {
		std::memset(buffer, 0, BUFFER_SIZE);
		bytesReceived = recv(fd, buffer, BUFFER_SIZE, 0);
		if (bytesReceived < 0) {
			unexpectedDisconnectHandling(fd);
			keepReading = false;
		} else {
			bufferStr.append(buffer);
			if (bytesReceived < BUFFER_SIZE) {
				keepReading = false;
			}
		}
	}
	return bufferStr;
}

void Server::sendDataThroughSocket(Client &client) {
	int	bytesSent;

	LOGGER.info("sendDataThroughSocket", "Sending data to fd " + std::to_string(client.getFd()));
	if (client.getSendData().size()) {
		bytesSent = send(client.getFd(), client.getSendData().c_str(), client.getSendData().size(), 0);
		if (bytesSent < 0) {
			LOGGER.error("Failed", "");
		} else {
			LOGGER.info("Sent", client.getSendData());
			client.resetSendData(bytesSent);
		}
	}
}