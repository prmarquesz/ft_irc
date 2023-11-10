#include "catch.hpp"
#include "ServerTestAPI.hpp"


TEST_CASE( "quit should work with success without message" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	client.setUsername("arrowverse");
	client.setHostname("localhost");
	Command command;
	command.cmd = "QUIT";

	// Act
	std::string response = serverTest.testQuit(client, command, true, true, true);

	// Assert
	REQUIRE( response == ":arrow QUIT :Gone to have lunch\r\n" );
}

TEST_CASE( "quit should work with success with message" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	client.setUsername("arrowverse");
	client.setHostname("localhost");
	Command command;
	command.cmd = "QUIT";
	command.args.push_back("Goodbye");

	// Act
	std::string response = serverTest.testQuit(client, command, true, true, true);

	// Assert
	REQUIRE( response == ":arrow QUIT Goodbye\r\n" );
}
