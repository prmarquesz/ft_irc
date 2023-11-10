#include "catch.hpp"
#include "ServerTestAPI.hpp"

TEST_CASE( "mode should set response 461 ERR_NEEDMOREPARAMS when there are no arguments" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "MODE";

	// Act
	serverTest.testMode(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 461  MODE :Not enough parameters\r\n" );
}

TEST_CASE( "mode should set response 324 RPL_CHANNELMODEIS received only channel" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "MODE";
	command.args.push_back("#channel");

	// Act
	serverTest.testMode(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 324  #channel +t\r\n" );
}

TEST_CASE( "mode should set response 221 RPL_UMODEIS when received only client" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	Command command;
	command.cmd = "MODE";
	command.args.push_back("arrow");

	// Act
	serverTest.testMode(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 221 arrow +iws\r\n" );
}

TEST_CASE( "mode should set response 502 ERR_USERSDONTMATCH when any user trying to view or change the user mode for a user other than themselves." ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	Command command;
	command.cmd = "MODE";
	command.args.push_back("arrows");

	// Act
	serverTest.testMode(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 502 arrow :Cannot change mode for other users\r\n" );
}

TEST_CASE( "mode should set response 403 ERR_NOSUCHCHANNEL when given channel name is invalid" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	Command command;
	command.cmd = "MODE";
	command.args.push_back("#channels");
	command.args.push_back("+i");

	// Act
	serverTest.testMode(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 403 arrow MODE :No such channel\r\n" );
}

TEST_CASE( "mode should set response 482 ERR_CHANOPRIVSNEEDED when you're not channel operator" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client(4, "localhost");
	client.setNickname("arrow");
	Command command;
	command.cmd = "MODE";
	command.args.push_back("#channel");
	command.args.push_back("+i");

	// Act
	serverTest.testMode(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 482 arrow #channel :You're not channel operator\r\n" );
}

TEST_CASE( "mode should set response 421 ERR_UNKNOWNMODE when no received +/-" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client(4, "localhost");
	client.setNickname("arrow");
	Command command;
	command.cmd = "MODE";
	command.args.push_back("#channel");
	command.args.push_back("i");

	// Act
	serverTest.testMode(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 421 arrow MODE :Unknown command\r\n" );
}

TEST_CASE( "mode should set response success to +i" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client(4, "localhost");
	client.setNickname("arrow");
	Command command;
	command.cmd = "MODE";
	command.args.push_back("#channel");
	command.args.push_back("+i");

	// Act
	serverTest.testMode(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":arrow MODE #channel +i\r\n" );
}

TEST_CASE( "mode should set response success to -i" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client(4, "localhost");
	client.setNickname("arrow");
	Command command;
	command.cmd = "MODE";
	command.args.push_back("#channel");
	command.args.push_back("-i");

	// Act
	serverTest.testMode(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":arrow MODE #channel -i\r\n" );
}

TEST_CASE( "mode should set response success to +t" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client(4, "localhost");
	client.setNickname("arrow");
	Command command;
	command.cmd = "MODE";
	command.args.push_back("#channel");
	command.args.push_back("+t");

	// Act
	serverTest.testMode(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":arrow MODE #channel +t\r\n" );
}

TEST_CASE( "mode should set response success to -t" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client(4, "localhost");
	client.setNickname("arrow");
	Command command;
	command.cmd = "MODE";
	command.args.push_back("#channel");
	command.args.push_back("-t");

	// Act
	serverTest.testMode(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":arrow MODE #channel -t\r\n" );
}

TEST_CASE( "mode should set response success to +k" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client(4, "localhost");
	client.setNickname("arrow");
	Command command;
	command.cmd = "MODE";
	command.args.push_back("#channel");
	command.args.push_back("+k");
	command.args.push_back("key");

	// Act
	serverTest.testMode(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":arrow MODE #channel +k key\r\n" );
}

TEST_CASE( "mode should set response success to -k" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client(4, "localhost");
	client.setNickname("arrow");
	Command command;
	command.cmd = "MODE";
	command.args.push_back("#channel");
	command.args.push_back("-k");
	command.args.push_back("key");

	// Act
	serverTest.testMode(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":arrow MODE #channel -k *\r\n" );
}

TEST_CASE( "mode should set response success to +o" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client(4, "localhost");
	client.setNickname("arrow");
	Command command;
	command.cmd = "MODE";
	command.args.push_back("#channel");
	command.args.push_back("+o");
	command.args.push_back("target");

	// Act
	serverTest.testMode(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":arrow MODE #channel +o target\r\n" );
}

TEST_CASE( "mode should set response success to -o" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client(4, "localhost");
	client.setNickname("arrow");
	Client target;
	target.setNickname("target");
	Command command;
	command.cmd = "MODE";
	command.args.push_back("#channel");
	command.args.push_back("-o");
	command.args.push_back("target");

	// Act
	serverTest.testMode(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":arrow MODE #channel -o target\r\n" );
}

TEST_CASE( "mode should set response success to +l" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client(4, "localhost");
	client.setNickname("arrow");
	Client target;
	target.setNickname("target");
	Command command;
	command.cmd = "MODE";
	command.args.push_back("#channel");
	command.args.push_back("+l");
	command.args.push_back("10");

	// Act
	serverTest.testMode(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":arrow MODE #channel +l 10\r\n" );
}

TEST_CASE( "mode should set response success to -l" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client(4, "localhost");
	client.setNickname("arrow");
	Client target;
	target.setNickname("target");
	Command command;
	command.cmd = "MODE";
	command.args.push_back("#channel");
	command.args.push_back("-l");

	// Act
	serverTest.testMode(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":arrow MODE #channel -l \r\n" );
}