cmake_minimum_required(VERSION 2.6)

project(Voronoi)

find_package(SDL REQUIRED)
find_package(GLU REQUIRED)
find_package(PkgConfig REQUIRED)

link_directories(
${SDL_LIBRARY_DIRS}
${GLU_LIBRARY_DIRS})

set(SDL_LIBS
${SDL_LIBRARY}
SDLmain)

add_subdirectory(src)
