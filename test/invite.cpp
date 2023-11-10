#include "catch.hpp"
#include "ServerTestAPI.hpp"

TEST_CASE( "invite should set response 461 ERR_NEEDMOREPARAMS when there are no arguments" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "INVITE";

	// Act
	serverTest.testInvite(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 461  INVITE :Not enough parameters\r\n" );
}

TEST_CASE( "invite should set response 461 ERR_NEEDMOREPARAMS when there are no channel arg" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "INVITE";
	command.args.push_back("target");

	// Act
	serverTest.testKick(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 461  KICK :Not enough parameters\r\n" );
}

TEST_CASE( "invite should set response 401 ERR_NOSUCHNICK when no such channel" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "INVITE";
	command.args.push_back("target");
	command.args.push_back("#mychannel");

	// Act
	serverTest.testInvite(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 401  #mychannel :No such nick/channel\r\n" );
}

TEST_CASE( "invite should set response 401 ERR_NOSUCHNICK when no such nick" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "INVITE";
	command.args.push_back("user");
	command.args.push_back("#channel");

	// Act
	serverTest.testInvite(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 401  user :No such nick/channel\r\n" );
}

TEST_CASE( "invite should set response 442 ERR_NOTONCHANNEL when you're not on that channel" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	Command command;
	command.cmd = "INVITE";
	command.args.push_back("target");
	command.args.push_back("#channel");

	// Act
	serverTest.testInvite(client, command, false);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 442 arrow arrow :You're not on that channel\r\n" );
}

TEST_CASE( "invite should set response 482 ERR_CHANOPRIVSNEEDED when you're not channel operator" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	Command command;
	command.cmd = "INVITE";
	command.args.push_back("target");
	command.args.push_back("#channel");

	// Act
	serverTest.testInvite(client, command, true, false);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 482 arrow #channel :You're not channel operator\r\n" );
}

TEST_CASE( "invite should set response 341 RPL_INVITING when message was successful and is being passed onto the end client." ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	Command command;
	command.cmd = "INVITE";
	command.args.push_back("target");
	command.args.push_back("#channel");

	// Act
	serverTest.testInvite(client, command, true, true);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 341 arrow #channel arrow\r\n" );
}
