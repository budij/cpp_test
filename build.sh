#!/bin/bash

# Remove the existing build directory and create one again
buildDir=build
if [ -d "$buildDir" ]; then
   if [ -L "$buildDir" ]; then
    rm "$buildDir"
  else
    rm -rf "$buildDir"
  fi
fi

mkdir $buildDir
cd $buildDir

echo Creating the project using cmake for unix makefile
echo Executing cmake .. -G "Unix Makefiles"
cmake .. -G "Unix Makefiles"

echo Building the project
make clean
make all

echo Running tests
make test

cd ..