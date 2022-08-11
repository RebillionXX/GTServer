# GTServer

a server made for growtopia private server -> [Growtopia](https://growtopiagame.com/).

## Features
- [x] readable&clean code
- [x] http service provider
- [x] proper events manager
- [x] MySQL connection

## Todo
- load more server requirement components (e.g. splicing data, items element, items buff/mods and many more)
- account registration and logon queue
- world database manager, world event manager and more single thread for handling **WorldNPC**, **WorldEvent** and more
- worlds randomization for generating world offer and more
- structure for **WorldTile**, **WorldObject** and **World**
- **World Data**&**Tile Data** serialization to data (every **single byte** might be the same with **RealGT**
- many more but lets finish these first **^^**

## Requirement
To building this source, you would need to install these libraries:
- [Boost](https://www.boost.org/) (You'll need x64 for this)

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
