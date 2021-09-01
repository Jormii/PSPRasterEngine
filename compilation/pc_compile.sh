#!/bin/bash

###
# MODIFY THESE
###
MAIN_FILE="../main.cpp"
OUTPUT_FILENAME="../tmp/Engine"

COMPILE_FLAGS="-Wall -O2"
HEADER_DIR="../include/"
SOURCE_DIR="../src/"

###
# Remove existent .o
###
OBJECT_FILES_TO_RMV=$(find . -name \*.o)
for OBJ_FILE in $OBJECT_FILES_TO_RMV; do
    rm $OBJ_FILE
done

###
# Compile engine
###
TYPES_DIR="types/"
ENGINE_DIR="engine/"

# Include args
INCLUDE_PATHS=""
for DIR in $TYPES_DIR $ENGINE_DIR; do
    INCLUDE_PATHS+=" -I $HEADER_DIR$DIR"
done

# Compile types
TYPES_FILES=("vec2f" "vec2i" "vec3f" "vec3i" "vec4f" "mat4f" "rgba" "mesh")
for TYPE_SRC in ${TYPES_FILES[@]}; do
    echo g++ -c $SOURCE_DIR$TYPES_DIR$TYPE_SRC".cpp" $COMPILE_FLAGS $INCLUDE_PATHS
    g++ -c $SOURCE_DIR$TYPES_DIR$TYPE_SRC".cpp" $COMPILE_FLAGS $INCLUDE_PATHS
done

echo ""

ENGINE_FILES=("utils" "draw_matrices" "edge" "fragment" "rasterization" "engine")
for ENGINE_SRC in ${ENGINE_FILES[@]}; do
    echo g++ -c $SOURCE_DIR$ENGINE_DIR$ENGINE_SRC".cpp" $COMPILE_FLAGS $INCLUDE_PATHS
    g++ -c $SOURCE_DIR$ENGINE_DIR$ENGINE_SRC".cpp" $COMPILE_FLAGS $INCLUDE_PATHS
done

# Linking
echo
echo Linking...
OBJECT_FILES=$(find . -name \*.o)
echo $OBJECT_FILES
g++ -o $OUTPUT_FILENAME $MAIN_FILE $OBJECT_FILES $COMPILE_FLAGS $INCLUDE_PATHS
