# CppHttp
- Fast
- Easy to use
- Header-only
- Supports CORS

# Examples

### Hello, World!
```cpp
#include "../include/CppHttp.hpp"
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <tuple>

using returnType = std::tuple<CppHttp::Net::ResponseType, std::string, std::optional<std::vector<std::string>>>;

returnType Hello(CppHttp::Net::Request req) {
	std::string body = "Hello, World!";
	return std::make_tuple(CppHttp::Net::ResponseType::OK, body, std::nullopt);
}

int main(int argc, char** argv) {
	CppHttp::Net::Router router;
	CppHttp::Net::TcpListener server;
	server.CreateSocket();

	auto onReceive = [&](CppHttp::Net::Request req) {
		router.Handle(req);
	};

	server.SetOnReceive(onReceive);

	router.AddRoute("GET", "/hello", Hello);

	server.Listen("0.0.0.0", 80, std::thread::hardware_concurrency());
}

```