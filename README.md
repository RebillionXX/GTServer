# GTServer

a server made for growtopia private server -> [Growtopia](https://growtopiagame.com/).

[server development discord](https://discord.gg/5BRwQVwvRq).
## Features
- [x] readable&clean code
- [x] inbuilt HTTPS service
- [x] MySQL Connection
- [x] Working VariantList
- [ ] Load Balancer and Server Gateway for sub-servers
- [ ] EventQueue and LoginQueue
- [x] Guest account support
- [ ] Login/Register
- [ ] Caching for holding world offers
- [x] Events Handler
- [x] Player Movements

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
- [OpenSSL](https://github.com/openssl/openssl)
- [fmt](https://github.com/fmtlib/fmt)
- [enet](https://github.com/lsalzman/enet)

## You'll need conan build installed via python
## How to clone the source
```
$ git clone https://github.com/RebillionXX/GTServer.git
$ cd GTServer
$ git submodule update --init --recursive
```

## How to build the source - starting path (GTServer/)
```
$ build.bat
```

## Running the program
- copy all files `GTServer/build/src/debug` to `GTServer/bin`
- run the program.

## Credits
 - [yhirose - cpp-httplib](https://github.com/yhirose/cpp-httplib)
 - [lsalzman - enet](https://github.com/lsalzman/enet)
 - [fmtlib - fmt](https://github.com/fmtlib/fmt)
 - [SethRobinson - proton](https://github.com/SethRobinson/proton)
