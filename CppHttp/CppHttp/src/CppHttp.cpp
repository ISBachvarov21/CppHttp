#include "../include/CppHttp.hpp"

int main() {
	try {
		CppHttp::Net::Router router;
		CppHttp::Net::TcpListener server;
		server.CreateSocket();


		auto onReceive = [&router](CppHttp::Net::Request& req) {
			router.Handle(req);
		};

		server.SetOnReceive(onReceive);

		char* szHostName = new char;
		gethostname(szHostName, 255);
		hostent* host_entry = gethostbyname(szHostName);
		char* szLocalIP = inet_ntoa(*(struct in_addr*)*host_entry->h_addr_list);

		std::cout << "\033[1;34mAddr: " << szLocalIP << "\033[0m\n";
		server.Listen(szLocalIP, 45098, 250);
	}
	catch (std::runtime_error& e) {
		std::cout << "\033[1;31m[-] Error: " << e.what() << "\033[0m\n";
	}
}