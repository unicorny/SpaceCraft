PROJECT(SpaceCraft CXX)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../)

SUBDIRS(src)
INCLUDE_DIRECTORIES(include import/include include/render include/sektor)


