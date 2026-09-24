#!/bin/bash

echo "Building..."

cmake -B build -DCMAKE_BUILD_TYPE=Release

echo "Making build..."

cmake --build build -j1

echo "The end!"