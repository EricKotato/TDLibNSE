cmake_minimum_required(VERSION 3.10)

project(TDLibNSE
	VERSION 0.1.1
	DESCRIPTION "TDLib as native Sciter extension.")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

add_definitions(-DUNICODE -D_UNICODE)

find_package(Td 1.7.0 REQUIRED)
find_path(SCITER_INCLUDE_DIR NAMES sciter-x.h REQUIRED)

add_library(TDLibNSE SHARED tdlib_nse.cpp)
target_include_directories(TDLibNSE	PUBLIC ${SCITER_INCLUDE_DIR})

target_link_libraries(TDLibNSE PRIVATE Td::TdJson)
