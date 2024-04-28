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
## [Docker container with cmake](https://github.com/ISBachvarov21/CppHttp/tree/main/CppHttp/CppHttp/examples/docker)

Dockerfile
```dockerfile
FROM alpine:3.19.1

RUN apk update
RUN apk add coreutils
RUN apk add build-base
RUN apk add cmake

WORKDIR /server

COPY . .

RUN cmake -S . -B build
RUN cmake --build build

# copy build files to /app
RUN mkdir /app
RUN cp -r build/* /app

EXPOSE 80

CMD [ "stdbuf", "-oL", "./build/CppHttp" ]
```

docker-compose.yml
```yaml
version: '3.9'

services:
  cpphttp:
    build: .
    command: sh -c "stdbuf -oL ./build/CppHttp"
    ports:
      - "80:80"
      - "8080:8080"
```

CmakeLists.txt
```cmake
include_directories("./src/dependencies/CppHttp/include")

add_executable (CppHttp "src/main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CppHttp PROPERTY CXX_STANDARD 20)
endif()
```