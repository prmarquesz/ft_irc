#include "catch.hpp"
#include "ServerTestAPI.hpp"

TEST_CASE( "pass should set response 461 ERR_NEEDMOREPARAMS when there are no arguments" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "PASS";

	// Act
	serverTest.testPass(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 461  PASS :Not enough parameters\r\n" );
}

TEST_CASE( "pass should set response 464 ERR_PASSWDMISMATCH when invalid passwd" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "PASS";
	command.args.push_back("pass");

	// Act
	serverTest.testPass(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 464  :Password incorrect\r\n" );
}

TEST_CASE( "pass should set response 462 ERR_ALREADYREGISTRED when client already registred" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "PASS";
	command.args.push_back("passwd");

	// Act
	serverTest.testPass(client, command, true, true);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 462  :You may not reregister\r\n" );
}


TEST_CASE( "pass should authenticate" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "PASS";
	command.args.push_back("passwd");

	// Act
	serverTest.testPass(client, command, true);

	// Assert
	REQUIRE( client.getRegistration() & PASS_FLAG );
}
