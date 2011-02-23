cmake_minimum_required(VERSION 2.6)

project(ssort)

#set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++0x")

#add_definitions("-Wall -g")
add_definitions("-Wall -O3 -DNDEBUG -funroll-loops -march=native")
#add_definitions("-Wall -O3 -DNDEBUG -pg")

set(sources main.cpp burst.cpp sqs.cpp insertion.cpp msd_radixsort.cpp mergesort.cpp)
set(tsources test.cpp burst.cpp sqs.cpp insertion.cpp)


add_executable(ssort ${sources})
#add_executable(test ${tsources})
