#include "catch.hpp"
#include "ServerTestAPI.hpp"

TEST_CASE( "nick should set response 431 ERR_NONICKNAMEGIVEN when a nickname parameter expected for a command and isn't found" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "NICK";

	// Act
	serverTest.testNick(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 431  :No nickname given\r\n" );
}

TEST_CASE( "nick should set response 432 ERR_ERRONEUSNICKNAME when is empty" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "NICK";
	command.args.push_back("");

	// Act
	serverTest.testNick(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 432   :Erroneus nickname\r\n" );
}

TEST_CASE( "nick should set response 432 ERR_ERRONEUSNICKNAME when contains num at first position" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "NICK";
	command.args.push_back("1user");

	// Act
	serverTest.testNick(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 432  1user :Erroneus nickname\r\n" );
}

TEST_CASE( "nick should set response 432 ERR_ERRONEUSNICKNAME when contains invalid chars" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "NICK";
	command.args.push_back("user@");

	// Act
	serverTest.testNick(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 432  user@ :Erroneus nickname\r\n" );
}

TEST_CASE( "nick should set response 433 ERR_NICKNAMEINUSE when in use" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "NICK";
	command.args.push_back("nickInUse");

	// Act
	serverTest.testNick(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 433  nickInUse :Nickname is already in use\r\n" );
}

TEST_CASE( "nick should set nickname" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "NICK";
	command.args.push_back("arrow");

	// Act
	serverTest.testNick(client, command);

	// Assert
	REQUIRE( client.getRegistration() & NICK_FLAG );
}
