cmake_minimum_required(VERSION 3.21)
project(SafeAndMirrorsProblem)

set(CMAKE_CXX_STANDARD 20)

add_executable(SafeAndMirrorsProblem src/main.cpp src/Safe.cpp headers/Safe.h src/Mirror.cpp headers/Mirror.h src/SafeBreaker.cpp headers/SafeBreaker.h src/Api.cpp headers/Api.h)
