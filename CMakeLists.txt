cmake_minimum_required(VERSION 3.12)
project(XML C)

set(CMAKE_C_STANDARD 99)

add_executable(XML main.c baseNode.c buffer.c stack.c tag.c xmlAttribute.c)