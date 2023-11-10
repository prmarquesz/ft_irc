#include "catch.hpp"
#include "ServerTestAPI.hpp"

TEST_CASE( "kick should set response 461 ERR_NEEDMOREPARAMS when there are no arguments" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "KICK";

	// Act
	serverTest.testKick(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 461  KICK :Not enough parameters\r\n" );
}

TEST_CASE( "kick should set response 461 ERR_NEEDMOREPARAMS when there are no target" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "KICK";
	command.args.push_back("#mychannel");

	// Act
	serverTest.testKick(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 461  KICK :Not enough parameters\r\n" );
}

TEST_CASE( "kick should set response 403 ERR_NOSUCHCHANNEL when the channel does not exist" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "KICK";
	command.args.push_back("#mychannel");
	command.args.push_back("target");

	// Act
	serverTest.testKick(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 403  #mychannel :No such channel\r\n" );
}

TEST_CASE( "kick should set response 441 ERR_USERNOTINCHANNEL when the target aren't on that channel" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	Command command;
	command.cmd = "KICK";
	command.args.push_back("#channel");
	command.args.push_back("user");

	// Act
	serverTest.testKick(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 441 arrow #channel :They aren't on that channel\r\n" );
}

TEST_CASE( "kick should set response 442 ERR_NOTONCHANNEL when you're not on that channel" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	Command command;
	command.cmd = "KICK";
	command.args.push_back("#channel");
	command.args.push_back("target");

	// Act
	serverTest.testKick(client, command, false);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 442 arrow #channel :You're not on that channel\r\n" );
}

TEST_CASE( "kick should set response 482 ERR_CHANOPRIVSNEEDED when you're not channel operator" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	Command command;
	command.cmd = "KICK";
	command.args.push_back("#channel");
	command.args.push_back("target");

	// Act
	serverTest.testKick(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 482 arrow #channel :You're not channel operator\r\n" );
}

TEST_CASE( "should kick target with success" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	Command command;
	command.cmd = "KICK";
	command.args.push_back("#channel");
	command.args.push_back("target");

	// Act
	serverTest.testKick(client, command, true, true);

	// Assert
	REQUIRE( client.getSendData() == ":arrow KICK #channel target :arrow\r\n" );
}

TEST_CASE( "should kick target with success and reason" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	Command command;
	command.cmd = "KICK";
	command.args.push_back("#channel");
	command.args.push_back("target");
	command.args.push_back(":goodbye");

	// Act
	serverTest.testKick(client, command, true, true);

	// Assert
	REQUIRE( client.getSendData() == ":arrow KICK #channel target :goodbye\r\n" );
}
