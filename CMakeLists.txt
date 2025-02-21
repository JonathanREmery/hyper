cmake_minimum_required(VERSION 3.20)
project(hyper)

include_directories(include)
add_executable(hyper src/main.c src/server.c src/client.c src/request.c)