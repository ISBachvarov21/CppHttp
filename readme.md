# CppHttp
- Fast
- Easy to use
- Header-only
- Supports CORS

# Examples

## [Hello, World!](https://github.com/ISBachvarov21/CppHttp/tree/main/CppHttp/CppHttp/examples/hello)
```cpp
#include <CppHttp.hpp>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <tuple>

HttpResponse Hello(CppHttp::Net::Request req) {
	std::string body = "Hello, World!";
	return std::make_tuple(CppHttp::Net::ResponseType::OK, body, std::optional<std::vector<std::string>>(false));
}

int main(int argc, char** argv) {
	CppHttp::Net::Router router;
	CppHttp::Net::TcpListener server;
	server.CreateSocket();

	int requestCount = 0;

	auto onReceive = [&](CppHttp::Net::Request req) {
		router.Handle(req);
		};

	server.SetOnReceive(onReceive);

	router.AddRoute("GET", "/hello", Hello);

	server.Listen("0.0.0.0", 80, std::thread::hardware_concurrency());
}
```
## [Docker container with cmake](https://github.com/ISBachvarov21/CppHttp/tree/main/CppHttp/CppHttp/examples/docker)

Dockerfile
```dockerfile
FROM gcc AS development

WORKDIR /server/

RUN apt-get update

RUN apt-get install -y cmake

COPY src/dependencies .

COPY . .

FROM development AS builder

RUN cmake -S . -B build
RUN cmake --build build

RUN mkdir /app
RUN cp -r build/* /app

FROM builder AS production

EXPOSE 80

CMD [ "stdbuf", "-oL", "./build/docker" ]
```

docker-compose.yml
```yaml
services:
  docker:
    build: .
    command: sh -c "stdbuf -oL ./build/docker"
    ports:
      - "80:80"
```

CmakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.13)
project("CppHttp")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/dependencies/cpphttp/include")

file (GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(docker ${SOURCES})

set_property(TARGET docker PROPERTY CXX_STANDARD 20)
```