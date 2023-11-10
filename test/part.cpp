#include "catch.hpp"
#include "ServerTestAPI.hpp"

TEST_CASE( "part should set response 461 ERR_NEEDMOREPARAMS when there are no arguments" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "PART";

	// Act
	serverTest.testJoin(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 461  JOIN :Not enough parameters\r\n" );
}

TEST_CASE( "part should set response 403 ERR_NOSUCHCHANNEL when no such channel" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "PART";
	command.args.push_back("#channel");

	// Act
	serverTest.testPart(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 403  #channel :No such channel\r\n" );
}

TEST_CASE( "part should set response 442 ERR_NOTONCHANNEL when not on channel" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "PART";
	command.args.push_back("#channel");

	// Act
	serverTest.testPart(client, command, true, false);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 442  #channel :You're not on that channel\r\n" );
}

TEST_CASE( "part should work with success" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	client.setUsername("arrowverse");
	client.setHostname("localhost");
	Command command;
	command.cmd = "PART";
	command.args.push_back("#channel");

	// Act
	std::string response = serverTest.testPart(client, command, true, true, true);

	// Assert
	REQUIRE( response == ":arrow PART #channel :arrow\r\n" );
}
