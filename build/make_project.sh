#!/bin/bash

# Project generation steps...
# ./make_project.sh CDT 	= for eclipse file generated
# ./make_project.sh CDT ARM = for eclipse file generated with Cross Compile for arm
# ./make_project.sh ARM 	= Only Cross Compile for arm
# make to compile project

# Comment it for Release version build
#DEBUG="DEBUG"

FIRST_PARA="$1"
SECOND_PARA="$2"
COMMAND="cmake"

#$@ for printing all argument
#$# for numbers of passed argument
echo "$# Arguments : $@"

if [ "$DEBUG" != "DEBUG" ]; then
	DEBUG="RELEASE"
fi

COMMAND+=" -D CMAKE_BUILD_TYPE=$DEBUG -lpthread . "
if [[ $@ == *"CDT"* ]]; then
	echo "With Eclipse project."
	COMMAND+="-G \"Eclipse CDT4 - Unix Makefiles\""
fi

if [[ $@ == *"ARM"* ]]; then
	echo "ARM define for Cross Compile."
	COMMAND+=" -DCMAKE_TOOLCHAIN_FILE=rpi_toolchain.cmake "
fi
echo "Execute Command : $COMMAND"

eval $COMMAND
echo "Done...!!"
