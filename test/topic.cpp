#include "catch.hpp"
#include "ServerTestAPI.hpp"

TEST_CASE( "topic should set response 461 ERR_NEEDMOREPARAMS when there are no arguments" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "TOPIC";

	// Act
	serverTest.testTopic(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 461  TOPIC :Not enough parameters\r\n" );
}

TEST_CASE( "topic should set response 331 RPL_NOTOPIC when no topic is set" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "TOPIC";
	command.args.push_back("#channel");

	// Act
	serverTest.testTopic(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 331  #channel :No topic is set\r\n" );
}

TEST_CASE( "topic should set response 331 RPL_TOPIC when topic is set" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "TOPIC";
	command.args.push_back("#channel");

	// Act
	serverTest.testTopic(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 332  #channel whatever\r\n" );
}

TEST_CASE( "topic should set response 442 ERR_NOTONCHANNEL when you're not on that channel" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	Command command;
	command.cmd = "TOPIC";
	command.args.push_back("#channel");
	command.args.push_back("soccer");

	// Act
	serverTest.testTopic(client, command, false);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 442 arrow #channel :You're not on that channel\r\n" );
}

TEST_CASE( "topic should set response 482 ERR_CHANOPRIVSNEEDED when channel mode is (+t) and you're not channel operator" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	Command command;
	command.cmd = "TOPIC";
	command.args.push_back("#channel");
	command.args.push_back("soccer");

	// Act
	serverTest.testTopic(client, command, true, true);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 482 arrow #channel :You're not channel operator\r\n" );
}

TEST_CASE( "topic should set response 332 RPL_TOPIC when channel mode is (+t) and you're channel operator." ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	Command command;
	command.cmd = "TOPIC";
	command.args.push_back("#channel");
	command.args.push_back("soccer");

	// Act
	serverTest.testTopic(client, command, true, true, true);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 332 arrow #channel soccer\r\n" );
}

TEST_CASE( "topic should set response 332 RPL_TOPIC when channel mode is (-t)" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	Command command;
	command.cmd = "TOPIC";
	command.args.push_back("#channel");
	command.args.push_back("soccer");

	// Act
	serverTest.testTopic(client, command, true, true, false, true);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 332 arrow #channel soccer\r\n" );
}