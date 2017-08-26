cmake_minimum_required(VERSION 3.6)
project(parser)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Parser.cpp Parser.h Ptr.cpp Ptr.h File.cpp File.h  Slicer.cpp Slicer.h Packman2.cpp Packman2.h) #RawPackman.cpp RawPackman.h Packman.cpp Packman.h)
add_executable(parser ${SOURCE_FILES})

#TODO: Implement Slicer
#TODO: finalise parser (better algorithm in project report)
#TODO: Expand DNS resolver (to dynamically find esp8266)
#TODO: resolve UDP recovery which will loop if a packet is lost

#only packman2 is used, please ignore the others and poor naming convention
#it will be fixed.