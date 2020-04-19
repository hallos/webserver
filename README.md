### Travis CI status
[![Build Status](https://travis-ci.org/hallos/webserver.svg?branch=crosscompatible)](https://travis-ci.org/hallos/webserver)

### Appveyor status
[![Build status](https://ci.appveyor.com/api/projects/status/l7my4r7wh10f0coh?svg=true)](https://ci.appveyor.com/project/hallos/webserver)


# webserver
Simple crossplatfom console-based webserver written in C++

Compiled with 
* ~~MinGW64 GCC 4.7.3 on Windows~~ This is not validated yet
* GCC 8.4.0 on Ubuntu

## Build
The project is built with CMake.  
```
$: mkdir build && cd build  
$: cmake ..  
$: make  
```

### Run

example usage:
```
$: Webserver -p 8080 -d /home/bertil/www/
```
  
Use the --help argument to get additional info about the CLI.