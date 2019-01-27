#include "catch.hpp"
#include "HTTP.h"

TEST_CASE("isValidHTTP returns true when interpreter is constructed with a valid HTTP response")
{
	std::string http = "HTTP/1.0 200 OK\r\nDate: Fri, 31 Dec 1999 23:59:59 GMT\r\nContent-Type: text/html\r\nContent-Length: 1354<html><body><h1>Happy New Millennium!</h1>(more file contents)</body></html>";

    REQUIRE(HTTP::isValidHTTP(http));
}

TEST_CASE("isValidHTTP returns false when interpreter is constructed with a valid HTTP response")
{
	std::string http = "FTP/4.0 OK\r\nDate: Fri, 31 Dec 1999 23:59:59 GMT\r\nContent-Type: text/html\r\n";
	
	REQUIRE_FALSE(HTTP::isValidHTTP(http));
}

TEST_CASE("interpretGETRequest returns filename of requested file of HTTP request")
{
	SECTION("index.html is returned on a HTTP GET /")
	{
		std::string httpRequest = "GET / HTTP/1.1\r\nHost: localhost:8090\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: sv-SE,sv;q=0.9,en-US;q=0.8,en;q=0.7,nb;q=0.6";
		std::string requestedFile = HTTP::interpretGETRequest(httpRequest);

		REQUIRE(requestedFile == "index.html");
	}

	SECTION("movies.html is returned when requested in HTTP GET")
	{
		std::string httpRequest = "GET movies.html HTTP/1.1\r\nHost: localhost:8090\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: sv-SE,sv;q=0.9,en-US;q=0.8,en;q=0.7,nb;q=0.6";
		std::string requestedFile = HTTP::interpretGETRequest(httpRequest);

		REQUIRE(requestedFile == "movies.html");	
	}
}