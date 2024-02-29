#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <regex>

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "ctre.hpp"

namespace CppHttp {
	namespace Utils {
		std::vector<std::string> Split(std::string const& str, char delimiter) {
			std::vector<std::string> split;
			std::stringstream ss(str);
			std::string word;
			while (std::getline(ss, word, delimiter)) {
				if (!word.empty()) {
					split.push_back(word);
				}
			}
			return split;
		}

		// Extract route from request
		std::string GetRoute(std::string const& req) {
			std::vector<std::string> split = CppHttp::Utils::Split(req, ' ');
			std::vector<std::string> split2 = CppHttp::Utils::Split(split[1], '?');

			return split2[0];
		}

		// Extract method from request
		std::string GetMethod(const std::string& req) {
			std::vector<std::string> split = CppHttp::Utils::Split(req, ' ');
			return split[0];
		}

		// Extract parameters from request
		// Returns a map of parameters
		std::unordered_map<std::string, std::string> GetParameters(const std::string& req) {
			std::vector<std::string> split = CppHttp::Utils::Split(req, ' ');
			std::string route = split[1];
			std::regex argsRegex("\\?(.*)");
			std::smatch argsMatch;
			std::regex_search(route, argsMatch, argsRegex);
			std::string parametersString = argsMatch[1];
			std::vector<std::string> parameters = CppHttp::Utils::Split(parametersString, '&');

			std::unordered_map<std::string, std::string> parametersMap;
			for (std::string parameter : parameters) {
				std::stringstream parameterStream(parameter);
				std::string key;
				std::string value;
				std::getline(parameterStream, key, '=');
				std::getline(parameterStream, value, '=');
				parametersMap[key] = value;
			}

			return parametersMap;
		}

		// Extract headers from request
		// Returns a map of headers
		std::unordered_map<std::string, std::string> GetHeaders(std::string& req) {
			int bodyStartChar = req.size();

			if (req.find("\r\n\r\n") != std::string::npos) {
				bodyStartChar = req.find("\r\n\r\n");
			}
			else if (req.find("\n\n") != std::string::npos) {
				bodyStartChar = req.find("\n\n");
			}

			// remove body
			req = req.substr(0, bodyStartChar);

			auto split = CppHttp::Utils::Split(req, '\n');
			std::unordered_map<std::string, std::string> headers;

			for (int i = 0; i < split.size(); ++i) {
				auto headerMatch = ctre::match<"(.*?): (.*)">(split[i]);
				auto matched = headerMatch.matched();
				if (matched.to_string() != "") {
					std::string header = matched.get<1>().to_string();
					std::string value = matched.get<2>().to_string();

					headers[header] = value;
				}
			}

			return headers;
		}

		// Extract body from request
		std::string GetBody(const std::string& req) {
			std::string body = "";

			int bodyStartChar = -1;

			if (req.find("\r\n\r\n") != std::string::npos) {
				bodyStartChar = req.find("\r\n\r\n");
			}
			else if (req.find("\n\n") != std::string::npos) {
				bodyStartChar = req.find("\n\n");
			}

			if (bodyStartChar == -1) {
				return "";
			}

			return req.substr(bodyStartChar + 4);
		}

		// Extract header from request
		std::string GetHeader(std::string const& req, std::string const& header) {
			std::vector<std::string> split = CppHttp::Utils::Split(req, '\n');
			std::string headerLine = "";

			for (int i = 0; i < split.size(); ++i) {
				if (split[i].find(header) != std::string::npos) {
					headerLine = split[i];
					break;
				}
			}

			if (headerLine == "") {
				return "";
			}

			std::regex headerRegex(header + ": (.*)");
			std::smatch headerMatch;
			std::regex_search(headerLine, headerMatch, headerRegex);
			std::string headerStr = headerMatch[1];

			return headerStr;
		}

		// Extract protocol from request
		std::string GetProtocol(std::string const& req) {
			std::vector<std::string> split = CppHttp::Utils::Split(req, ' ');
			return split[2];
		}

		// Extract protocol version from request
		std::string GetProtocolVersion(std::string const& req) {
			std::vector<std::string> split = CppHttp::Utils::Split(req, ' ');
			return split[3];
		}
	}

	namespace Net {

		struct RequestInformation {
			RequestInformation() = default;
			
			RequestInformation(std::string& req, SOCKET& sender) :
				sender(sender),
				original(req),
				route(CppHttp::Utils::GetRoute(original)),
				method(CppHttp::Utils::GetMethod(original)),
				parameters(CppHttp::Utils::GetParameters(original)),
				headers(CppHttp::Utils::GetHeaders(original)),
				body(CppHttp::Utils::GetBody(original))
			{}
			
			SOCKET sender = INVALID_SOCKET;
			std::string original;
			std::string route;
			std::string method;
			std::unordered_map<std::string, std::string> parameters;
			std::unordered_map<std::string, std::string> headers;
			std::string body;
		};
		
		class Request {
		public:
			Request() {
				this->m_info = {};
			}

			Request(std::string& req, SOCKET& sender) {
				this->m_info = {
					req,
					sender
				};
			}
			RequestInformation m_info;
		};

		#ifndef REQOPOVERLOAD
			#define REQOPOVERLOAD
			inline std::ostream& operator<<(std::ostream& os, const CppHttp::Net::Request& req) {
				return os << req.m_info.original;
			}
		#endif
	}
}