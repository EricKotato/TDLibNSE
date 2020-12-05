cmake_minimum_required(VERSION 3.10)

project(TDLibNSE
	VERSION 0.3
	DESCRIPTION "TDLib as native Sciter extension.")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

find_package(Td 1.7.0 REQUIRED)
find_path(SCITER_INCLUDE_DIR NAMES sciter-x.h REQUIRED)

if (UNIX)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
endif()

add_definitions(-DUNICODE -D_UNICODE ${GTK3_CFLAGS_OTHER})

option(USE_STATIC_TDJSON "Link extension with static library instead of dynamic one" OFF)

if (USE_STATIC_TDJSON)
	set(TDLIB_LIBRARY Td::TdJsonStatic)
else()
	set(TDLIB_LIBRARY Td::TdJson)
endif()

add_library(TDLibNSE SHARED tdlib_nse.cpp)
target_include_directories(TDLibNSE	PUBLIC ${SCITER_INCLUDE_DIR} ${GTK3_INCLUDE_DIRS})

if (UNIX)
	target_link_directories(TDLibNSE PUBLIC ${GTK3_LIBRARY_DIRS})
endif()

target_link_libraries(TDLibNSE PRIVATE ${TDLIB_LIBRARY} ${GTK3_LIBRARIES})
