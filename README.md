# GTServer

a server made for growtopia private server -> [Growtopia](https://growtopiagame.com/).

## Features
- [x] readable&clean code
- [x] http service provider
- [x] events manager
- [x] variantlist & gameupdatepackets handler
- [x] MySQL connection

## Requirement
To building this source, you would need to install these libraries:
- [Boost](https://www.boost.org/)

## How to build the source - starting path (GTServer/)
```shell
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```

## Running the program
- copy a executable file (`server.exe`) from `GTServer/build/src/debug` to `GTServer/bin`
- run the program.

## Credits
 - [yhirose - cpp-httplib](https://github.com/yhirose/cpp-httplib)
 - [lsalzman - enet](https://github.com/lsalzman/enet)
 - [fmtlib - fmt](https://github.com/fmtlib/fmt)
 - [SethRobinson - proton](https://github.com/SethRobinson/proton)
