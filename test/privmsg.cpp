#include "catch.hpp"
#include "ServerTestAPI.hpp"

TEST_CASE( "privmsg should set response 411 ERR_NEEDMOREPARAMS when no args" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "PRIVMSG";

	// Act
	serverTest.testPrivmsg(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 461  PRIVMSG :Not enough parameters\r\n" );
}

TEST_CASE( "privmsg should set response 411 ERR_NORECIPIENT when no recipient" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "PRIVMSG";
	command.args.push_back("#channel");

	// Act
	serverTest.testPrivmsg(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 411  :No recipient given (PRIVMSG)\r\n" );
}

TEST_CASE( "privmsg should set response 412 ERR_NOTEXTTOSEND when no text to send" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "PRIVMSG";
	command.args.push_back("target");

	// Act
	serverTest.testPrivmsg(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 412  :No text to send\r\n" );
}

TEST_CASE( "privmsg should set response 401 ERR_NOSUCHNICK when no such channel" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "PRIVMSG";
	command.args.push_back("target");
	command.args.push_back("text");

	// Act
	serverTest.testPrivmsg(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 401  target :No such nick/channel\r\n" );
}

TEST_CASE( "privmsg should set response 442 ERR_NOTONCHANNEL when not on channel" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "PRIVMSG";
	command.args.push_back("#channel");
	command.args.push_back("text");

	// Act
	serverTest.testPrivmsg(client, command, false);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 442  #channel :You're not on that channel\r\n" );
}

TEST_CASE( "privmsg should set response 401 ERR_NOSUCHNICK when no such nick" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "PRIVMSG";
	command.args.push_back("user");
	command.args.push_back("text");

	// Act
	serverTest.testPrivmsg(client, command, false);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 401  user :No such nick/channel\r\n" );
}

TEST_CASE( "privmsg should set response to channel" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	client.setUsername("arrowverse");
	client.setHostname("localhost");
	Command command;
	command.cmd = "PRIVMSG";
	command.args.push_back("#channel");
	command.args.push_back(":text");

	// Act
	std::string response = serverTest.testPrivmsg(client, command);

	// Assert
	REQUIRE( response == ":arrow!arrowverse@localhost PRIVMSG #channel :text\r\n" );
}

TEST_CASE( "privmsg should set response to user" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	client.setUsername("arrowverse");
	client.setHostname("localhost");
	Command command;
	command.cmd = "PRIVMSG";
	command.args.push_back("oper");
	command.args.push_back(":text");

	// Act
	std::string response = serverTest.testPrivmsg(client, command, false, true);

	// Assert
	REQUIRE( response == ":arrow!arrowverse@localhost PRIVMSG oper :text\r\n" );
}
