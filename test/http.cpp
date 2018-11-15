#include "catch.hpp"
#include "http.h"

TEST_CASE("isHTTP returns true when interpreter is constructed with a valid HTTP response")
{
	std::string http = "HTTP/1.0 200 OK\nDate: Fri, 31 Dec 1999 23:59:59 GMT\nContent-Type: text/html\nContent-Length: 1354<html><body><h1>Happy New Millennium!</h1>(more file contents)</body></html>";
	httpInterpreter interpreter(http);
	bool isHttp = interpreter.isHTTP();

    REQUIRE(isHttp);
}

TEST_CASE("isHTTP returns false when interpreter is constructed with a valid HTTP response")
{
	std::string http = "FTP/4.0 OK\nDate: Fri, 31 Dec 1999 23:59:59 GMT\nContent-Type: text/html\n";
	httpInterpreter interpreter(http);
	bool isHttp = interpreter.isHTTP();
	
	REQUIRE_FALSE(isHttp);
}

TEST_CASE("interpretRequest returns filename of requested file of HTTP request")
{
	std::string httpRequest = "GET / HTTP/1.1\nHost: localhost:8090\nConnection: keep-alive\nCache-Control: max-age=0\nUpgrade-Insecure-Requests: 1\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\nAccept-Encoding: gzip, deflate, br\nAccept-Language: sv-SE,sv;q=0.9,en-US;q=0.8,en;q=0.7,nb;q=0.6";
	SECTION("index.html is returned on a HTTP GET /")
	{
		httpInterpreter interpreter(httpRequest);
		std::string requestedFile;
		interpreter.interpretRequest(requestedFile);

		REQUIRE(requestedFile == "index.html");
	}
}