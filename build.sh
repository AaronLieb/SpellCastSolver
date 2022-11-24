#! /usr/bin/bash
clang++ -std=c++20 spellcast.cc -g -o sc.out && echo "Starting Test" && ./sc.out < given.txt