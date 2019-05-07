#! /bin/bash
##
##- Script to run CMake prior to building Hades.

##- Uncomment this line to debug.
# set -x

##- Determine the name of the build directory and the location of the project's
##  CMakeLists.txt file (which should be in the same directory as this script).
##
IFS="/" read -ra ARR <<< `pwd`
build_dir=${ARR[-1]}

##- Compose a basic CMake command
##
project_dir=`dirname $(readlink -f "$0")`
target="-G 'CodeBlocks - Unix Makefiles'"
cmake_cmd="cmake $project_dir $target"

##- Define some build types to be appended to the basic command.
##
release="-DCMAKE_BUILD_TYPE=Release"
debug="-DCMAKE_BUILD_TYPE=Debug"

##- Define the actual command to be run based on the name of the build directory.
##
if [ ${build_dir} = "build-debug-gcc" ]
then
    cmd="CC=`which gcc` CXX=`which g++` $cmake_cmd $debug"

elif [ ${build_dir} = "build-release-gcc" ]
then
    cmd="CC=`which gcc` CXX=`which g++` $cmake_cmd $release"

elif [ ${build_dir} = "build-debug-clang" ]
then
    cmd="CC=`which clang` CXX=`which clang++` $cmake_cmd $debug"

elif [ ${build_dir} = "build-release-clang" ]
then
    cmd="CC=`which clang` CXX=`which clang++` $cmake_cmd $release"

elif [ ${build_dir} = "cmake-build-debug" ]
then
    cmd="CC=`which gcc` CXX=`which g++` $cmake_cmd $debug"

elif [ ${build_dir} = "cmake-build-release" ]
then
    cmd="CC=`which gcc` CXX=`which g++` $cmake_cmd $release"
fi

##- Run it!
##
echo $cmd
echo ""
eval $cmd

