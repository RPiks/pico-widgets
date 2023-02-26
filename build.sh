#!/bin/bash

### VT Colors.

ECHO_ESCAPE=" -e "

TEXT_FG_BLACK="[30m"
TEXT_FG_RED="[31m"
TEXT_FG_GREEN="[32m"
TEXT_FG_ORANGE="[33m"
TEXT_FG_BLUE="[34m"
TEXT_FG_MAGENTA="[35m"
TEXT_FG_CYAN="[36m"
TEXT_FG_GRAY="[37m"
TEXT_FG_DARK_GRAY="[30;1m"
TEXT_FG_LIGHT_RED="[31;1m"
TEXT_FG_LIGHT_GREEN="[32;1m"
TEXT_FG_YELLOW="[33;1m"
TEXT_FG_VIOLET="[34;1m"
TEXT_FG_LIGHT_MAGENTA="[35;1m"
TEXT_FG_LIGHT_CYAN="[36;1m"
TEXT_FG_WHITE="[37;1m"

TEXT_BG_BLACK="[40m"
TEXT_BG_RED="[41m"
TEXT_BG_GREEN="[42m"
TEXT_BG_YELLOW="[43m"
TEXT_BG_BLUE="[44m"
TEXT_BG_MAGENTA="[45m"
TEXT_BG_CYAN="[46m"
TEXT_BG_GRAY="[47m"

TEXT_NORM="[0m" # Back to normal text

MSG_PREFIX="$TEXT_BG_GREEN$TEXT_FG_YELLOW => $TEXT_NORM"

_banner()
{
    echo
    echo $ECHO_ESCAPE " "$TEXT_BG_GREEN$TEXT_FG_DARK_GRAY"┌──────────────"$TEXT_FG_LIGHT_GREEN"┐"$TEXT_NORM
    echo $ECHO_ESCAPE " "$TEXT_BG_GREEN$TEXT_FG_DARK_GRAY"│ "$TEXT_FG_GRAY"PICO-WIDGETS "$TEXT_FG_LIGHT_GREEN"│"$TEXT_NORM
    echo $ECHO_ESCAPE " "$TEXT_BG_GREEN$TEXT_FG_DARK_GRAY"└"$TEXT_FG_LIGHT_GREEN"──────────────┘"$TEXT_NORM
    echo
}

_failed()
{
    echo
    echo $ECHO_ESCAPE$MSG_PREFIX$TEXT_BG_RED$TEXT_FG_YELLOW" Failed :( "$TEXT_NORM
    echo
}

_completed()
{
    echo
    echo $ECHO_ESCAPE$MSG_PREFIX$TEXT_BG_CYAN$TEXT_FG_WHITE" Completed successfully :) "$TEXT_NORM
    echo
}

_ex()
{
    echo $@
    if ! $@
    then
        _failed
        exit 1
    fi
}

############################################

_banner

export SC_OS=`uname -s | tr '[:upper:]' '[:lower:]'`

echo $@ | grep -- 'release' > /dev/null 2>&1
if [ "$?" -eq "0" ]; then
    CMAKE_BUILD_TYPE=Release
    BUILD_PARAMS=$BUILD_PARAMS' release'
else
    CMAKE_BUILD_TYPE=Debug
    BUILD_PARAMS=$BUILD_PARAMS' debug'
fi

echo $@ | grep -- 'verbose' > /dev/null 2>&1
if [ "$?" -eq "0" ]; then
    CMAKE_VERBOSE_MAKEFILE=TRUE
    BUILD_PARAMS=$BUILD_PARAMS' verbose'
else
    CMAKE_VERBOSE_MAKEFILE=FALSE
    BUILD_PARAMS=$BUILD_PARAMS' silent'
fi

echo $@ | grep -- 'single' > /dev/null 2>&1
if [ "$?" -eq "0" ]; then
    SC_BUILD_THREADS=1
    BUILD_PARAMS=$BUILD_PARAMS' single'
else if [ "$SC_OS" == "darwin" ]; then
    SC_BUILD_THREADS=`/usr/sbin/system_profiler -detailLevel full SPHardwareDataType | awk '/Total Number of Cores/ {print $5};'`
else
    SC_BUILD_THREADS=`cat /proc/cpuinfo | grep ^processor | wc -l`
fi

    BUILD_PARAMS=$BUILD_PARAMS' multi-make'
fi

SC_BUILD_THREADS=$((SC_BUILD_THREADS / 2 + 1))

echo $ECHO_ESCAPE$MSG_PREFIX$TEXT_BG_BLUE$TEXT_FG_WHITE" BUILD MODE:$BUILD_PARAMS "$TEXT_NORM
echo $ECHO_ESCAPE$MSG_PREFIX$TEXT_BG_BLUE$TEXT_FG_WHITE" MAKE JOBS : $SC_BUILD_THREADS "$TEXT_NORM
echo

cd build
_ex cmake -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE -DCMAKE_VERBOSE_MAKEFILE=$CMAKE_VERBOSE_MAKEFILE ..
_ex make -j $SC_BUILD_THREADS
# _ex sudo make install -j $SC_BUILD_THREADS

_completed


#cd build
#cmake ..
#make

