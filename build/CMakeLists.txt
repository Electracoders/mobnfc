cmake_minimum_required(VERSION 2.8)
project(nfc)

include(${CMAKE_CURRENT_SOURCE_DIR}/../main/CMakeLists.txt)
platform()

set(RZ_ROOT "../")
message(status "** RZ_ROOT Include: ${RZ_ROOT}")
#add_definitions(-DBOOST_ALL_NO_LIB)  # tell the compiler to define this boost macro

IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
	message("Debug build.")
ELSE()
	message("Release build.")
ENDIF()

SET(RZ_MAIN
	${RZ_ROOT}/main/main.cpp
	${RZ_ROOT}/main/Inotify.cpp
	${RZ_ROOT}/main/main.h
	${RZ_ROOT}/main/Inotify.h
)

set(RZ_MYLIB "${RZ_ROOT}/MyLib")
INCLUDE(
	${RZ_ROOT}/MyLib/MyLib.cmake
)

FIND_PACKAGE (Threads REQUIRED )

add_executable(nfc 
	${RZ_MAIN} 
	${RZ_THREADAPI}
	${RZ_FILESYSTEMAPI}
	${RZ_PROCESSAPI}
)
TARGET_LINK_LIBRARIES ( nfc ${CMAKE_THREAD_LIBS_INIT} )

