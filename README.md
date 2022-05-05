# GTServer

a server made for growtopia private server -> [Growtopia](https://growtopiagame.com/).

## Features
- [x] readable&clean code
- [x] http service provider
- [x] events manager

## How to build the source - starting path (GTServer/)
```shell
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```

## Running the program
- if you're on windows, you've to move dynamic link library files from `GTServer/vendor/mysql/lib`
- move all `.dll` files to `GTServer/build/src/Debug`
- run the program.

## Credits
 - [yhirose - cpp-httplib](https://github.com/yhirose/cpp-httplib)
 - [lsalzman - enet](https://github.com/lsalzman/enet)
 - [fmtlib - fmt](https://github.com/fmtlib/fmt)
 - [SethRobinson - proton](https://github.com/SethRobinson/proton)