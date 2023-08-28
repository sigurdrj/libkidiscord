#ifndef LIBKIDISCORD_HPP
#define LIBKIDISCORD_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Infos.hpp>
#include <curlpp/Options.hpp>

#include "json.hpp"

using std::string;
using json = nlohmann::json;

namespace libkidiscord{
	class Client{
		string baseURL = "https://discord.com/api/v9";
		string useragent = "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:99.0) Gecko/20100101 Firefox/99.0";
		long httpTimeout = 3; // Seconds
		string authToken;

		std::pair <string, long> perform_http_request(const string method, const string url, const string bodyContent = ""){
			curlpp::Easy client;

			std::stringstream responseStream;

			client.setOpt(new curlpp::options::Url(url));
			client.setOpt(new curlpp::options::UserAgent(useragent));
			client.setOpt(new curlpp::options::WriteStream(&responseStream));
			client.setOpt(new curlpp::options::Timeout(httpTimeout));
			client.setOpt(new curlpp::options::CustomRequest(method));

			client.setOpt(new curlpp::options::HttpHeader({"Authorization: " + authToken, "Content-Type: application/json"}));

			client.setOpt(new curlpp::options::PostFields(bodyContent));
			client.setOpt(new curlpp::options::PostFieldSize(bodyContent.size()));

			try{
				client.perform();
			} catch(curlpp::LibcurlRuntimeError &e){
				return std::make_pair("", 499);
			}

			return std::make_pair(responseStream.str(), curlpp::infos::ResponseCode::get(client));
		}

		public:

		void set_base_url(const string &newBaseURL){baseURL = newBaseURL;}
		string get_base_url(){return baseURL;}

		void set_auth_token(const string &newAuthToken){authToken = newAuthToken;}
		string get_auth_token(){return authToken;}

		long send_message(const string &message, const unsigned long long &channelID, const bool tts = false){
			json jsonContent;
			jsonContent["content"] = message;
			jsonContent["nonce"] = nullptr;
			jsonContent["tts2"] = tts;
			const string url = baseURL + "/channels/" + std::to_string(channelID) + "/messages";

			return perform_http_request("POST", url, jsonContent.dump()).second;
		}

		std::pair <json, long> get_messages(const unsigned long long &channelID, const unsigned long long limit=50){
			const string url = baseURL + "/channels/" + std::to_string(channelID) + "/messages?limit=" + std::to_string(limit);

			const std::pair <string, long> response = perform_http_request("GET", url);

			if (response == std::pair<string, long>("", 499))
				return std::make_pair(""_json, 499);

			return std::make_pair(json::parse(response.first), response.second);
		}

		void print_msgs(const json &msgs){
			for (json msg : msgs){
				std::cout << msg["content"] << '\n';
			}
		}
	};
}

#endif // LIBKIDISCORD_HPP
