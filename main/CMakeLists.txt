function(platform)
    message(STATUS "Platform ")

IF(UNIX AND NOT WIN32)
    message(STATUS "UNIX")
	FIND_PROGRAM( GCOV_PATH gcov )
	FIND_PROGRAM( LCOV_PATH lcov )
	FIND_PROGRAM( GENHTML_PATH genhtml )
	FIND_PROGRAM( GCOVR_PATH gcovr PATHS ${CMAKE_SOURCE_DIR}/tests)

#For release build
	SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -fPIC")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -fPIC")

#For debug build
	SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -Wall -fPIC ${GCC_COVERAGE_COMPILE_FLAGS}")
	SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -Wall -W -fPIC ${GCC_COVERAGE_COMPILE_FLAGS}")
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
ENDIF()

IF(MSVC)
    message(STATUS "WINDOWS")
	# Use secure functions by defaualt and suppress warnings about
	#"deprecated" functions
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ")
	set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ")
ENDIF(MSVC)

endfunction()
