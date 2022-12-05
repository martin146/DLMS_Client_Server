#pragma warning( disable : 6031)
#pragma warning( disable : 26812)

#include <iostream>

#include "ServerDirector/ServerDirector.h"
#include <Exceptions/ConfigException.h>
#include <Exceptions/InitException.h>
#include <Exceptions/HandleException.h>
#include <Logger.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <tchar.h>
#endif

int main(int argc, char* argv[])
{
    ServerDirector serverDirector;
    if (argc != 2) {
        throw RuntimeException("Invalid number of parameter. Please enter one parameter which represents path to config file.");
    }

    try
	{
		serverDirector.CreateServer(argv[1]);
	} catch (RuntimeException &e) {
        Logger::LogEvent(Logger::ERROR, e.what());
        return -1;
    }

	serverDirector.PrintConfig();
	Logger::LogEvent(Logger::INFO, "Server Running...");

    try {
        serverDirector.StartServer();
    } catch (RuntimeException &e) {
        Logger::LogEvent(Logger::ERROR, "Error in starting server: " + std::string(e.what()));
    }
    return 0;
}
