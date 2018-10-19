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