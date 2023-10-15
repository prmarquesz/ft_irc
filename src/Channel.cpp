#include <Channel.hpp>

Channel::~Channel(void) {}

Channel::Channel(void) {
	host		= "localhost";
	creator		= NULL;
	userLimit	= USER_CHANNEL_LIMIT;
	initialized = false;
	LOGGER		= Logger("Channel");
}

std::string Channel::getName() const { return name; };

std::string Channel::getTopic() const { return topic; }

bool Channel::isInitialized() { return initialized; }

std::map<Client *, unsigned int> &Channel::getClients() { return clients; }

void Channel::setTopic(std::string topic) { this->topic = topic; };

void Channel::addClient(Client &client) { clients.insert(std::make_pair(&client, 0)); }

void Channel::setPassword(std::string password) { this->password = password; }

void Channel::removePassword() { this->password = ""; };

void Channel::removeClient(Client &client) {
	if (creator == &client) {
		creator = NULL;
	}
	clients.erase(&client);
	if (clients.size() > 0) {
		asureOperator();
	}
}

void Channel::broadcast(Client &sender, std::string message, bool toSend) {
	std::map<Client *, unsigned int>::iterator it = clients.begin();

	LOGGER.info("broadcast", "Broadcasting message to all clients in channel " + name);
	for (; it != clients.end(); it++) {
		if (it->first == &sender && toSend) {
			(*it->first).setSendData(message);
		} else if (it->first != &sender) {
			(*it->first).setSendData(message);
		}
	}
}

Client &Channel::getCreator() { return *creator; };

bool Channel::toggleMode(char mode, bool on) {
	std::size_t before = modes.size();

	LOGGER.info("toggleMode", "Toggling mode " + std::string(1, mode) + " to " + std::to_string(on));
	if (on)
		modes.insert(mode);
	else
		modes.erase(mode);
	return !(before == modes.size());
}

std::set<char> &Channel::getMode() { return modes; }

void Channel::initialize(std::string name, std::string password, Client &op) {
	LOGGER.info("initialize", "Initializing channel " + name);
	this->name	   = name;
	this->password = password;
	this->clients.insert(std::make_pair(&op, USER_OPERATOR));
	this->initialized = true;
	this->modes.insert('t');
}

void Channel::initialize(std::string name, Client &op) {
	LOGGER.info("initialize", "Initializing channel " + name);
	this->name = name;
	this->clients.insert(std::make_pair(&op, USER_OPERATOR));
	this->initialized = true;
	this->modes.insert('t');
}

void		 Channel::setUserLimit(unsigned int limit) { userLimit = limit; };

unsigned int Channel::getUserLimit() const { return userLimit; };

bool Channel::setOperator(std::string clientNickname, bool newValue) {
	LOGGER.info("setOperator", "Setting operator status of " + clientNickname + " to " + std::to_string(newValue));
	std::map<Client *, unsigned int>::iterator it =
		getClientByNick(clientNickname);
	if (it != clients.end()) {
		if (newValue)
			it->second |= USER_OPERATOR;
		else
			it->second &= (~USER_OPERATOR);
		return true;
	}
	return (false);
};

void Channel::setSpeaker(std::string clientNickname, bool newValue) {
	LOGGER.info("setSpeaker", "Setting speaker status of " + clientNickname + " to " + std::to_string(newValue));
	std::map<Client *, unsigned int>::iterator it =
		getClientByNick(clientNickname);
	if (it != clients.end()) {
		if (newValue)
			it->second |= USER_SPEAKER;
		else
			it->second &= (~USER_SPEAKER);
	}
};

bool Channel::evalPassword(std::string psw) { return (password == psw); }

std::map<Client *, unsigned int>::iterator Channel::getClientByNick(std::string clientNickname) {
	LOGGER.info("getClientByNick", "Looking for client " + clientNickname);
	std::map<Client *, unsigned int>::iterator it = clients.begin();
	while (it != clients.end()) {
		if (it->first->getNickname() == clientNickname) return (it);
		it++;
	}
	return (it);
};

std::string Channel::getStrModes() {
	std::string modeStr = "";

	if (modes.size() > 0) {
		modeStr += "+";
		std::set<char>::iterator it = modes.begin();
		while (it != modes.end()) {
			modeStr += (*it);
			it++;
		}
		if (modeStr.find('k') != std::string::npos) {
			modeStr.append(" ");
			modeStr.append(this->password);
		}
	}
	return (modeStr);
};

bool Channel::isInvited(std::string nickname) {
	if (invited.find(toIrcUpperCase(nickname)) != invited.end()) {
		return true;
	}
	return false;
};

void Channel::addInvite(std::string nickname) {
	invited.insert(toIrcUpperCase(nickname));
};

void Channel::removeInvited(std::string nickname) {
	invited.erase(toIrcUpperCase(nickname));
};

void Channel::asureOperator() {
	std::map<Client *, unsigned int>::iterator it;
	it = clients.begin();
	while (it != clients.end()) {
		if (it->second & USER_OPERATOR) return;
		it++;
	}
	it		   = clients.begin();
	it->second = it->second | USER_OPERATOR;
};