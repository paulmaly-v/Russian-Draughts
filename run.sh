#!/bin/bash

if [ -f $PWD/.build/SOURCE ]; then
  ./.build/SOURCE
else
  mkdir .build
  cd .build
  cmake ..
  cmake --build .
  ./SOURCE
  cd ..
fi