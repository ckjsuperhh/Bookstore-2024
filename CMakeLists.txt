cmake_minimum_required(VERSION 3.28)
project(5Bookstore)

set(CMAKE_CXX_STANDARD 26)

add_executable(5Bookstore main.cpp
        user.h
        DataBase.h
        book.h
        log.h
        cmd.h)
