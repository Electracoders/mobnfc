 #bin/bash!

CURR=${PWD}
echo "CURR : ${CURR}"

qmake
make
#LD_LIBRARY_PATH=../libndef ./snep-encode -z test.txt "en-US" > ndef1