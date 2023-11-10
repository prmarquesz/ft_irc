#include "catch.hpp"
#include "ServerTestAPI.hpp"

TEST_CASE( "user should set response 461 ERR_NONICKNAMEGIVEN when no args" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "USER";

	// Act
	serverTest.testUser(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 461  USER :Not enough parameters\r\n" );
}

TEST_CASE( "nick should set response 462 ERR_ALREADYREGISTRED when already registred" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "USER";
	command.args.push_back("");
	command.args.push_back("");
	command.args.push_back("");
	command.args.push_back("");

	// Act
	serverTest.testUser(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 462  :You may not reregister\r\n" );
}


TEST_CASE( "nick should registry" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "USER";
	command.args.push_back("whatever");
	command.args.push_back("0");
	command.args.push_back("*");
	command.args.push_back(":something");

	// Act
	serverTest.testUser(client, command);

	// Assert
	REQUIRE( client.getRegistration() & USER_FLAG );
}
