cmake_minimum_required(VERSION 4.0)
project(OceanDepth C)

set(CMAKE_C_STANDARD 11)

include_directories(Code/combat)
include_directories(Code/globals)
include_directories(Code/input)
include_directories(Code/player)
include_directories(Code/render)
include_directories(Code/types)
include_directories(Code/utils)
include_directories(Code/world)

add_executable(OceanDepth
        Code/combat/combat.c
        Code/combat/combat.h
        Code/creature/creature.c
        Code/creature/creature.h
        Code/globals/globals.c
        Code/globals/globals.h
        Code/input/input.c
        Code/input/input.h
        Code/inventaire/inventaire.c
        Code/inventaire/inventaire.h
        Code/marchand/marchand.c
        Code/marchand/marchand.h
        Code/player/player.c
        Code/player/player.h
        Code/render/render.c
        Code/render/render.h
        Code/save/save.c
        Code/save/save.h
        Code/types/types.h
        Code/utils/utils.c
        Code/utils/utils.h
        Code/world/world.c
        Code/world/world.h
        Code/main.c
        artTest/artTest.c
        Code/world/map_lt.h
        Code/world/map_lt.c
        Code/world/deco.c
        Code/world/deco.h
)
