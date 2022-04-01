/*
 * TestClient.cpp
 *
 *  Created on: 31 de mar. de 2022
 *      Adapted from: https://www.codeproject.com/
 *      			  https://www.boost.org/doc/libs/1_71_0/
 */
#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;


int main() {
	boost::asio::io_service io_service;

	//socket creation
	tcp::socket socket(io_service);

	//connection
	socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 60079 ));

	// request/message from client
	const string msg = "Hello from the biggest Client ever! So good to see you!\n";
	boost::system::error_code error;
	boost::asio::write( socket, boost::asio::buffer(msg), error );

	if ( !error )
	{
	cout << "Client sent hello message: Hello from the other side!" << endl;
	}
	else
	{
		cout << "send failed: " << error.message() << endl;
	}

	// getting response from server
	boost::asio::streambuf receive_buffer;
	boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);

	if ( error && error != boost::asio::error::eof )
	{
		cout << "receive failed: " << error.message() << endl;
	}
	else
	{
		const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
		cout << data << endl;
	}

	return 0;
}
