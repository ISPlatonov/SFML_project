# SFML_project

Multiplatform minigame that demonstrates SFML library potential.

![](https://img.shields.io/github/workflow/status/ISPlatonov/SFML_project/CI)
![](https://img.shields.io/tokei/lines/github/ISPlatonov/SFML_project)
![](https://img.shields.io/github/languages/code-size/ISPlatonov/SFML_project)

## Configuring

In file `textures/constants.txt` you can set up the game settings: server ip, ports, pixel size, VSYNC, framerate limit, etc.

## Installation

### Linux

- install dependencies:

```bash
sudo apt-get update && sudo apt-get install libxrandr-dev libxcursor-dev libudev-dev libopenal-dev libflac-dev libvorbis-dev libgl1-mesa-dev libegl1-mesa-dev libdrm-dev libgbm-dev
```

- intall SFML (if it's not yet):

```bash
git clone https://github.com/SFML/SFML.git && cd SFML && git checkout 2.6.x && cd ..
cmake -S SFML -B SFML/build -DCMAKE_INSTALL_PREFIX=SFML/install -DBUILD_SHARED_LIBS=FALSE
cmake --build SFML/build --config Release --target install
```

- install the game:

```bash
cmake -B build -DCMAKE_INSTALL_PREFIX=install -DSFML_DIR=SFML/install/lib/cmake/SFML
cmake --build build --config Release --target install
```

## Running

`bin` directory will consist all binaries and textures:

- `SFML_project` - game client executable

    You can use `w`, `a`, `s` and `d` buttons for moving across the map.

- `server` - game server executable
- `textures` - dir with all textures and configuration files

### About server and multiplayer

Server sends all the information about terrain, objects around and player's inventory to each client.

At the moment, player can't connect to the global server defined in settings file without being in the same local network. So, player has 2 options:

- message me in Telegram and ask OpenVPN configuration file for globally deployed server
- run `server` on a dedicated device in the same local network you want to play in

## Architecture

```mermaid
flowchart LR
    main{{main}} --> Controls
    server{{server}} --> UdpManager
    subgraph Multiplayer class
        UdpManager --> PDP(Player Data Pool)
        UdpManager --> ODP(Object Data Pool)
        UdpManager --> UdpSocket
        PDP -.- Transportable[[Transportable]]
        ODP -.- Transportable
        UdpManager -- for server only --> PerlinNoise
        PerlinNoise --> ODP
    end
    subgraph Controls class
        Controls --> user{{user}}
        Controls --> Window(Window)
        Controls --> Events
        Controls --> OM(Object Map)
        Controls --> PP(Player Pool)
        Controls --> UdpManager(UdpManager)
    end
    subgraph Object class
        Object
    end
    subgraph Actor class
        User -.- Actor[[Actor]]
        Bot -.- Actor
        Player -.- Actor
    end
    ODP --> Object
    OM --> Object
    user --> User
    PDP --> Player
    PP --> Player
```
