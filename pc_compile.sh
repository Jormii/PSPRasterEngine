#!/bin/bash

###
# MODIFY THESE
###
MAIN_FILE="./main.cpp"
OUTPUT_FILENAME="./Engine"

COMPILE_FLAGS="-Wall -O2"
HEADER_DIR="./include/"
SOURCE_DIR="./src/"

###
# Compile engine
###
TYPES_DIR="types/"
ENGINE_DIR="engine/"

# Include args
INCLUDE_PATHS=""
for DIR in $TYPES_DIR $ENGINE_DIR;
do
    INCLUDE_PATHS+=" -I $HEADER_DIR$DIR"
done

# Compilation
for DIR in $TYPES_DIR $ENGINE_DIR;
do
    SOURCE_PATH=$SOURCE_DIR$DIR
    SOURCE_FILES=$(find $SOURCE_PATH -name \*.cpp)
    for SOURCE_FILE in $SOURCE_FILES;
    do
        echo g++ -c $SOURCE_FILE $COMPILE_FLAGS $INCLUDE_PATHS
        g++ -c $SOURCE_FILE $COMPILE_FLAGS $INCLUDE_PATHS
    done
    echo
done

# Linking
echo Linking...
OBJECT_FILES=$(find . -name \*.o)
echo $OBJECT_FILES
g++ -o $OUTPUT_FILENAME $MAIN_FILE $OBJECT_FILES $COMPILE_FLAGS $INCLUDE_PATHS