cmake_minimum_required(VERSION 3.6)
project(FTSFC)

set(CMAKE_CXX_STANDARD 11)

# Boost
set(BOOST_INC_PATH /usr/local/Cellar/boost/1.63.0/include)
set(BOOST_LIB_PATH /usr/local/Cellar/boost/1.63.0/lib)
include_directories(${BOOST_INC_PATH})
find_library(BOOST_SER_LIB NAMES libboost_serialization.a PATHS ${BOOST_LIB_PATH})
find_library(BOOST_IOS_LIB NAMES libboost_iostreams.a PATHS ${BOOST_LIB_PATH})

# LIBZ
set(LIBZ_INC_PATH /usr/local/include)
set(LIBZ_LIB_PATH /usr/local/lib)
include_directories(${LIBZ_INC_PATH})
find_library(LIBZ_LIB NAMES libz.a PATHS ${LIBZ_LIB_PATH})

# click
set(CLICK_LIB_PATH /usr/local/lib)
find_library(CLICK_LIB NAMES libclick.a PATHS ${CLICK_LIB_PATH})

set(SOURCE_FILES
        FTTypes.cc
        FTTypes.hh
#        FTAppenderElement.cc
#        FTAppenderElement.hh
#        FTStateElement.cc
#        FTStateElement.hh
#        FTBufferElement.cc
#        FTBufferElement.hh
#        CounterMB.cc
#        CounterMB.hh
        main.cpp FTFilterElement.cc FTPassElement.cc FTPassElement.hh Test.cpp Test.h)

add_executable(FTSFC ${SOURCE_FILES})
target_link_libraries(FTSFC ${CLICK_LIB})
target_link_libraries(FTSFC ${LIBZ_LIB})
target_link_libraries(FTSFC ${BOOST_SER_LIB})
target_link_libraries(FTSFC ${BOOST_IOS_LIB})