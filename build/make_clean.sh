#!/bin/bash

#Add file that want to remove on this script execution
array=("CMakeFiles/" "CMakeCache.txt" "Makefile" "nfc" "cmake_install.cmake" ".cproject" ".project")

i=0
while [ $i != ${#array[@]} ]; do
	if [ -e  ${array[$i]} ]; then
		rm -r ${array[$i]}
		echo "removed ${array[$i]}"
	fi
	i=`expr $i + 1`
done
