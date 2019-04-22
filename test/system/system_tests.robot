*** Settings ***
Suite Setup         Start Webserver
Suite Teardown      Stop Webserver
Library             Process
Library             HttpLibrary.HTTP

*** Variables ***
${SERVER_PORT}      8090
${WEBSERVER_BIN}    build/src/Webserver

*** Keywords ***
Start Webserver
    Start Process   ${WEBSERVER_BIN}
    Create Http Context     host=localhost:${SERVER_PORT}
Stop Webserver
    Terminate All Processes


*** Test Cases ***
HTTP Get request returns expected html-page
    GET     /index.html
    Response Body Should Contain    Hello Web!