#include <string>
#include <exception>

#include <Server.hpp>
#include <Logger.hpp>

int main(int argc, char *argv[]) {
    std::string passwd;
    int         port;

    try {
        if (argc != 3) throw std::invalid_argument("Invalid number of arguments");
        port = std::atoi(argv[1]);
        passwd = argv[2];
        Server server(passwd, port);
        server.startServer();
        Logger().debug("main", "chegou aqui sem seg fault 1");
    } catch (const std::exception& e) {
        Logger().error("main", e.what());
        return EXIT_FAILURE; 
    } 
    Logger().debug("main", "chegou aqui sem seg fault 2");
    return EXIT_SUCCESS;
}