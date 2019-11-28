cmake_minimum_required(VERSION 2.6)
project(OpenQM)

# Project settings
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O2 ") 
# SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ")


# Uncomment to enable compiler warning
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Weffc++") 

# Enable test
enable_testing()
add_subdirectory(test)

# Source
set(LNS_SRCS 
${PROJECT_SOURCE_DIR}/implicant.h
${PROJECT_SOURCE_DIR}/implicant.cpp
${PROJECT_SOURCE_DIR}/qm.h
${PROJECT_SOURCE_DIR}/qm.cpp
${PROJECT_SOURCE_DIR}/main.cpp
)


set(BINQM_SRCS 
${PROJECT_SOURCE_DIR}/implicant.h
${PROJECT_SOURCE_DIR}/implicant.cpp
${PROJECT_SOURCE_DIR}/qm.h
${PROJECT_SOURCE_DIR}/qm.cpp
${PROJECT_SOURCE_DIR}/binqm.cpp
)

set(PRUNE_SRCS 
${PROJECT_SOURCE_DIR}/implicant.h
${PROJECT_SOURCE_DIR}/implicant.cpp
${PROJECT_SOURCE_DIR}/qm.h
${PROJECT_SOURCE_DIR}/qm.cpp
${PROJECT_SOURCE_DIR}/qmprune.cpp
)




add_executable(OpenQM ${LNS_SRCS})

add_executable(BinQM ${BINQM_SRCS})

add_executable(PruneQM ${PRUNE_SRCS})
