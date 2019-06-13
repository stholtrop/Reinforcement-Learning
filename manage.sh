#!/bin/bash

# Constants
BUILD_COMMAND="g++ -Wall -pipe -O2 -g --std=c++17 -lm  -I./src"
BUILD_COMMAND_RELEASE="g++ -Wall -pipe -O3 --std=c++17 -lm  -I./src"

function tests {
    mkdir -p ./unit_tests/build
    NAME="./unit_tests/src/test_${1}.cpp"
    # Check existence
    if test ! -f $NAME
    then
        echo "Non existent test"
        exit
    fi
    BASENAME=$(basename $NAME)
    OUTPUT="./unit_tests/build/${BASENAME%.*}"
    # Build
    COMMAND="${BUILD_COMMAND} ${NAME} -o ${OUTPUT}"
    echo $COMMAND
    eval $COMMAND
    # Running
    if [ $2 = "run" ]
    then
        echo $OUTPUT
        eval $OUTPUT
    fi
}

function main {
    mkdir -p build
    OUTPUT=${2:-./build/main}
    COMMAND="${BUILD_COMMAND} ./src/main.cpp -o ${OUTPUT}"
    echo $COMMAND
    eval $COMMAND
    if [ $1 = "run" ]
    then
        echo ./build/main
        ./build/main
    fi
}

function debug {
    if [ $1 = "main" ]
    then
        # Build main
        main "build"
        # Start debugger
        gdb ./build/main
        exit
    fi
    if [ $1 = "test" ]
    then
        # Build test
        tests $2 "build"
        # Start debugger
        gdb $OUTPUT
        exit
    fi
}

function release {
    BUILD_COMMAND=$BUILD_COMMAND_RELEASE
    main "build" "./build/release"
}

case $1 in
test)
    tests $2 "run";;
main)
    main "run";;
debug)
    case $2 in
    test)
        debug "test" $3;;
    main)
        debug "main";;
    esac
    ;;
release)
    release ;;
help)
    echo "\
Usage for running: ./manage.sh [test | main] {name of test (optional)}
Example command: ./manage.sh test neural
Usage for debugging: ./manage.sh debug [test | main] {name of test (optional)}
Example command: ./message.sh debug test neural
Usage for release: ./manage.sh release" ;;
*)
    echo "Not a command" ;;
esac