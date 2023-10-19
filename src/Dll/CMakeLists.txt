cmake_minimum_required(VERSION 3.5)

project(Dll)

set(SOURCES
    Dll.cpp
)

add_library(MyDll SHARED ${SOURCES})
