#pragma once
#include <Server.hpp>
#include <Client.hpp>
#include <Channel.hpp>

class ServerTestAPI {
	public:
		void testPass(Client &client, Command &command, bool setPasswd = true, bool setRegistration = false) {
			Server server("test", 4242);
			if (setPasswd)
				server.passwd = "passwd";
			if (setRegistration)
				client.setRegistration(PASS_FLAG);
			server.pass(client, command);
		}

		void testNick(Client &client, Command &command, bool setNick = false) {
			Server server("test", 4242);
			Client target;
			target.setNickname("nickInUse");
			if (setNick)
				server.clients.insert(std::pair<int, Client&>(4, target));
			server.nick(client, command);
		}

		void testUser(Client &client, Command &command, bool setRegistration = false) {
			Server server("test", 4242);
			Client target;
			target.setNickname("nickInUse");
			if (setRegistration)
				client.setRegistration(USER_FLAG);
			server.user(client, command);
		}

		std::string testPrivmsg(Client &client, Command &command, bool addClientTochannel = true, bool addClientToServer = true) {
			Server server("test", 4242);
			Client oper;
			oper.setNickname("oper");
			oper.setUsername("super");
			oper.setHostname("localhost");
			server.channels[toIrcUpperCase("#channel")] = Channel();
			server.channels[toIrcUpperCase("#channel")].initialize("#channel", oper);
			if (addClientTochannel)
				server.channels[toIrcUpperCase("#channel")].addClient(client);
			if (addClientToServer)
				server.clients.insert(std::pair<int, Client&>(4, oper));
			server.privmsg(client, command);
			return server.clients.at(4).getSendData();
		}

		void testJoin(Client &client, Command &command, bool modeMaxL = false, bool modeMaxI = false, bool setPasswd = false) {
			Server server("test", 4242);
			server.clients.insert(std::pair<int, Client&>(4, client));
			Client oper;
			oper.setNickname("oper");
			oper.setUsername("super");
			oper.setHostname("localhost");
			Client target;
			target.setNickname("target");
			server.channels[toIrcUpperCase("#channel")] = Channel();
			server.channels[toIrcUpperCase("#channel")].initialize("#channel", oper);
			server.channels[toIrcUpperCase("#channel")].addClient(target);
			if (modeMaxL) {
				server.channels[toIrcUpperCase("#channel")].toggleMode('l', true);
				server.channels[toIrcUpperCase("#channel")].setUserLimit(2);
			}
			if (modeMaxI)
				server.channels[toIrcUpperCase("#channel")].toggleMode('i', true);
			if (setPasswd)
				server.channels[toIrcUpperCase("#channel")].setPassword("passwd");
			server.join(client, command);
		}

		void testKick(Client &client, Command &command, bool addArrow = true, bool setOper = false) {
			Client oper;
			Server server("test", 4242);
			server.channels[toIrcUpperCase("#channel")] = Channel();
			server.channels[toIrcUpperCase("#channel")].initialize("#channel", oper);
			if (addArrow)
				server.channels[toIrcUpperCase("#channel")].addClient(client);
			if (setOper)
				server.channels[toIrcUpperCase("#channel")].setOperator(client.getNickname(), true);
			Client target;
			target.setNickname("target");
			server.channels[toIrcUpperCase("#channel")].addClient(target);
			server.kick(client, command);
		}

		void testInvite(Client &client, Command &command, bool addArrow = true, bool setOper = false) {
			Client oper;
			Server server("test", 4242);
			server.channels[toIrcUpperCase("#channel")] = Channel();
			server.channels[toIrcUpperCase("#channel")].toggleMode('i', true);
			server.channels[toIrcUpperCase("#channel")].initialize("#channel", oper);
			if (addArrow)
				server.channels[toIrcUpperCase("#channel")].addClient(client);
			if (setOper)
				server.channels[toIrcUpperCase("#channel")].setOperator(client.getNickname(), true);
			Client target;
			target.setNickname("target");
			server.clients.insert(std::pair<int, Client&>(5, target));
			server.channels[toIrcUpperCase("#channel")].addClient(target);
			server.invite(client, command);
		}

		void testTopic(Client &client, Command &command, bool setTopic = false, bool addArrow = false, bool setOper = false, bool modeMinusT = false) {
			Client oper;
			Server server("test", 4242);
			server.channels[toIrcUpperCase("#channel")] = Channel();
			server.channels[toIrcUpperCase("#channel")].initialize("#channel", oper);
			if (setTopic)
				server.channels[toIrcUpperCase("#channel")].setTopic("whatever");
			if (addArrow)
				server.channels[toIrcUpperCase("#channel")].addClient(client);
			if (setOper)
				server.channels[toIrcUpperCase("#channel")].setOperator(client.getNickname(), true);
			if (modeMinusT)
				server.channels[toIrcUpperCase("#channel")].toggleMode('t', false);
			server.topic(client, command);
		}

		void testMode(Client &client, Command &command, bool setOper = false) {
			Server server("test", 4242);
			server.clients.insert(std::pair<int, Client&>(4, client));
			Client oper;
			Client target;
			target.setNickname("target");
			server.channels[toIrcUpperCase("#channel")] = Channel();
			server.channels[toIrcUpperCase("#channel")].initialize("#channel", oper);
			server.channels[toIrcUpperCase("#channel")].addClient(client);
			server.channels[toIrcUpperCase("#channel")].addClient(target);
			if (setOper)
				server.channels[toIrcUpperCase("#channel")].setOperator(client.getNickname(), true);
			server.mode(client, command);
		}

		std::string testPart(Client &client, Command &command, bool createChannel = false, bool initiChannel = false, bool addCLientToChannel = false) {
			Server server("test", 4242);
			Client oper;
			oper.setNickname("oper");
			oper.setUsername("super");
			oper.setHostname("localhost");
			if (createChannel) {
				server.channels[toIrcUpperCase("#channel")] = Channel();
				if (initiChannel)
					server.channels[toIrcUpperCase("#channel")].initialize("#channel", oper);
				client.addChannel(server.channels[toIrcUpperCase("#channel")]);
				if (addCLientToChannel)
					server.channels[toIrcUpperCase("#channel")].addClient(client);
			}
			server.part(client, command);
			return oper.getSendData();
		}

		std::string testQuit(Client &client, Command &command, bool createChannel = false, bool initiChannel = false, bool addCLientToChannel = false) {
			Server server("test", 4242);
			Client oper;
			oper.setNickname("oper");
			oper.setUsername("super");
			oper.setHostname("localhost");
			if (createChannel) {
				server.channels[toIrcUpperCase("#channel")] = Channel();
				if (initiChannel)
					server.channels[toIrcUpperCase("#channel")].initialize("#channel", oper);
				client.addChannel(server.channels[toIrcUpperCase("#channel")]);
				if (addCLientToChannel)
					server.channels[toIrcUpperCase("#channel")].addClient(client);
			}
			server.quit(client, command);
			return oper.getSendData();
		}
};
