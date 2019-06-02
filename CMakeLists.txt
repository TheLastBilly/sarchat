cmake_minimum_required(VERSION 3.0)
project ("sarchat")

add_compile_options(-std=c++11)

file(GLOB SOURCES
    libdirecpp/include/*.hpp
    libdirecpp/src/*.cpp
    src/include/*.hpp
    src/*.cpp
)

set(EXECUTABLE_OUTPUT_PATH "bin")

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_INSTALL_PREFIX "/usr/local/bin")

SET(CURSES_USE_NCURSES TRUE)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable( sarchat ${SOURCES} )
target_link_libraries( sarchat ${CURSES_LIBRARIES})
install(TARGETS sarchat DESTINATION ${CMAKE_INSTALL_PREFIX})
