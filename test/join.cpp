#include "catch.hpp"
#include "ServerTestAPI.hpp"

TEST_CASE( "join should set response 461 ERR_NEEDMOREPARAMS when there are no arguments" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "JOIN";

	// Act
	serverTest.testJoin(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 461  JOIN :Not enough parameters\r\n" );
}

TEST_CASE( "join should set response 403 ERR_NOSUCHCHANNEL when the channel length is 0" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "JOIN";
	command.args.push_back("");

	// Act
	serverTest.testJoin(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 403   :No such channel\r\n" );
}

TEST_CASE( "join should set response 403 ERR_NOSUCHCHANNEL when the channel does not have prefix" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "JOIN";
	command.args.push_back("channel");

	// Act
	serverTest.testJoin(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 403  channel :No such channel\r\n" );
}

TEST_CASE( "join should set response 461 ERR_NEEDMOREPARAMS when reveived invalid passwd" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "JOIN";
	command.args.push_back("#channel");
	command.args.push_back("passwd,");

	// Act
	serverTest.testJoin(client, command);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 461  JOIN :Not enough parameters\r\n" );
}

TEST_CASE( "join should set response success when channel does not exist" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	client.setUsername("flash");
	client.setHostname("localhost");
	Command command;
	command.cmd = "JOIN";
	command.args.push_back("#mychannel");
	std::string reply = std::string(":arrow!flash@localhost JOIN :#mychannel\r\n");
	reply.append(":localhost 331 arrow #mychannel :No topic is set\r\n");
	reply.append(":localhost 353 arrow = #mychannel :!arrow @arrow \r\n");
	reply.append(":localhost 366 arrow #mychannel :End of NAMES list\r\n");

	// Act
	serverTest.testJoin(client, command);

	// Assert
	REQUIRE( client.getSendData() == reply);
}

TEST_CASE( "join should set response success when channel already exist without modes or passwd" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	client.setNickname("arrow");
	client.setUsername("flash");
	client.setHostname("localhost");
	Command command;
	command.cmd = "JOIN";
	command.args.push_back("#channel");
	std::string reply = std::string(":arrow!flash@localhost JOIN :#channel\r\n");
	reply.append(":localhost 331 arrow #channel :No topic is set\r\n");
	reply.append(":localhost 353 arrow = #channel :!oper target @oper arrow \r\n");
	reply.append(":localhost 366 arrow #channel :End of NAMES list\r\n");

	// Act
	serverTest.testJoin(client, command);

	// Assert
	REQUIRE( client.getSendData() == reply);
}

TEST_CASE( "join should set response 471 ERR_CHANNELISFULL when channel is full" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "JOIN";
	command.args.push_back("#channel");
	command.args.push_back("passwd");

	// Act
	serverTest.testJoin(client, command, true);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 471  #channel :Cannot join channel (+l)\r\n" );
}

TEST_CASE( "join should set response 473 ERR_INVITEONLYCHAN when channel is invite only" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "JOIN";
	command.args.push_back("#channel");
	command.args.push_back("passwd");

	// Act
	serverTest.testJoin(client, command, false, true);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 473  #channel :Cannot join channel (+i)\r\n" );
}

TEST_CASE( "join should set response 475 ERR_BADCHANNELKEY when invalid passwd" ) {
	// Arrange
	ServerTestAPI serverTest;
	Client client;
	Command command;
	command.cmd = "JOIN";
	command.args.push_back("#channel");
	command.args.push_back("pass");

	// Act
	serverTest.testJoin(client, command, false, false, true);

	// Assert
	REQUIRE( client.getSendData() == ":localhost 475  #channel :Cannot join channel (+k)\r\n" );
}
