*** Settings ***
Test Setup          Setup Webserver
Test Teardown       Stop Webserver
Library             Process
Library             HttpLibrary.HTTP

*** Variables ***
${SERVER_PORT}      8080
${WEBSERVER_BIN}    build/src/Webserver

*** Keywords ***
Setup Webserver
    Start Process   ${WEBSERVER_BIN}
    Create Http Context     host=localhost:${SERVER_PORT}
Stop Webserver
    Terminate All Processes


*** Test Cases ***
HTTP GET request returns expected html-page
    GET     /index.html
    Response Body Should Contain    Hello Web!

HTTP GET request of non-existing file returns 404 Not Found
    Run Keyword And Ignore Error    GET     /IdontExistAtAll.html
    ${status}=      Get Response Status
    Should Start With   ${status}   404

HTTP HEAD request of existing page returns 200 OK
    HEAD     /index.html
    ${status}=      Get Response Status
    Should Start With   ${status}   200