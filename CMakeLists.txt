cmake_minimum_required(VERSION 2.6)
project(aura)
list(APPEND CMAKE_CXX_FLAGS "-std=c++0x")
add_subdirectory(libaura)
add_subdirectory(plugins)
add_subdirectory(live)
#add_subdirectory(filter)

