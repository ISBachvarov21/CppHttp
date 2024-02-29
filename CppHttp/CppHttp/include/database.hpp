#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <regex>
#include <sstream>
#include <map>
#include <pqxx/pqxx>

namespace Core {
	namespace Database {
		class Database {
		private:
			pqxx::connection* m_conn;

		public:
			Database(std::string_view const& connString = "dbname=postgres user=proektna password=kT$Me#sb3WpuNeJ host=10gr-2nd-proj.postgres.database.azure.com port=5432 sslmode=require") {
				this->m_conn = new pqxx::connection(connString.data());
				std::cout << "\033[1;32m[+] Connected to database\033[0m\n";
			}

			~Database() {
				this->m_conn->close();

				free(this->m_conn);
			}

			/// <summary>
			///		Execute a query
			/// </summary>
			/// <param name="std::string query"></param>
			/// <param name="bool commit"></param>
			/// <returns>
			///		pqxx::result result of query operation on database
			/// </returns>
			pqxx::result Query(std::string_view const& query) {
				pqxx::work work(*this->m_conn);

				auto res = work.exec(query);

				work.commit();

				return res;
			}
		};

		#ifndef db
			#define db
			static Database database;
		#endif
	}
}
