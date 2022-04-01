/**
* O intuito do software é receber dados proveniente da rede e salvar estes dados
* em arquivos localmente, ou seja, será um servidor que receberá dados via
* stream (socket) e salvará os dados em arquivos de até X bytes.
**/

#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "ServerUtils.h"
#include "CognyteServer.h"

using boost::asio::ip::tcp;
//using namespace boost::asio;
//using ip::tcp;


// Reads data from socket buffer
std::string read_(tcp::socket& socket)
{
	boost::asio::streambuf buffer;
	boost::asio::read_until( socket, buffer, "\n");
	std::string data = boost::asio::buffer_cast<const char*>(buffer.data());
	return data;
}

// Sends data via socket buffer
void send_(tcp::socket& socket, const std::string& data)
{
	const std::string msg = data + "\n";
	boost::asio::write(socket, boost::asio::buffer(data));
}

// Super loop
int main(int argc, char**argv)
{
    std::cout << "/* COGNYTE SERVER */" << std::endl;

    Logger        logger;
    ServerUtils   serverUtils;
    std::ifstream confFile;
    std::string   nameOfConfFile;

    std::string isAsync;		// 0 - Synchronous server
    							// 1 - Assynchronous server

/******************************Application Setup*******************************/
    // Receiving config file name from user when software is first executed
    if (argc <= 2)
    {
        std::cout << "Enter config file and server mode" << std::endl;
        std::cout << "Config file: ";
        std::getline(std::cin >> std::ws, nameOfConfFile);
        std::cout << "Mode (0 - Synchronous mode; 1 - Assynchronous mode): ";
        std::getline(std::cin >> std::ws, isAsync);
    }
    else
    {
        nameOfConfFile = argv[1];
        isAsync = argv[2];
    }

    // Reading config file
    confFile.open(nameOfConfFile, std::ios::in);
    if (!confFile)
    {
        std::cout << nameOfConfFile << " is not a valid file!" << std::endl;
        return 1;
    }

	std::cout << "Reading settings..." << std::endl;
	serverUtils.processConfFile(confFile);

	std::cout << "Closing file..." << std::endl;
	confFile.close();


	/****************************Server Activation*****************************/
	// Running in Synchronous mode
	if (std::stoi(isAsync) == 0)
	{
		std::cout << "\n***Running synchronous server***" << std::endl;
		// Starting server
		std::string receivedData;
		boost::asio::io_service io_service;

		// Listening for new connection
		tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(),
				serverUtils.getConPort()));

		// Creating socket
		tcp::socket socket_(io_service);

		// Waiting for connection and reading received message
		acceptor_.accept(socket_);
		receivedData = read_(socket_);

		serverUtils.saveReceivedData(serverUtils.getFilePrefix(), receivedData);
		send_(socket_, "Hello from the other side!");

	}
	// Running in Assynchronous mode
	else
	{
		std::cout << "\n***Running assynchronous server***" << std::endl;
		try
		{
			boost::asio::io_context io_context;
			tcp_server server(io_context);
			io_context.run();
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	return 0;
}
